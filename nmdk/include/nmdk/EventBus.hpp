#pragma once

#include "nmdk/api/Macro.h"

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string_view>
#include <type_traits>
#include <utility>

namespace nmdk {

namespace detail {

template <typename T> constexpr std::string_view eventKey() noexcept {
#if defined(__clang__) || defined(__GNUC__)
    return __PRETTY_FUNCTION__;
#else
    return __FUNCSIG__;
#endif
}

} // namespace detail

/**
 * Event bus.
 *
 * Hot path (publish) design:
 *  - Each event type resolves its Channel ONCE (cached in a function-local static),
 *    so publish never hashes/allocates strings again.
 *  - If a channel has no listeners, publish is a single relaxed atomic load.
 *  - Listeners are stored as an immutable snapshot (copy-on-write). publish copies
 *    only one shared_ptr, never the callbacks themselves, and is safe against
 *    subscribe/unsubscribe from inside a callback.
 *  - Exceptions thrown by listeners are swallowed so they can't unwind through
 *    game code (which would crash the process).
 */
class MCAPI EventBus {
  public:
    using ListenerId = std::uint64_t;

    /// Opaque per-event-type state. `active` is readable inline for the fast path.
    struct Channel {
        std::atomic<std::size_t> active{0};
    };

    EventBus();
    ~EventBus();

    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;
    EventBus(EventBus &&) noexcept;
    EventBus &operator=(EventBus &&) noexcept;

    static EventBus &getInstance() noexcept;

    template <typename Event, typename Callback> ListenerId subscribe(Callback &&callback, int priority = 0) {
        using Fn = std::decay_t<Callback>;
        static_assert(std::is_invocable_v<Fn &, Event &>,
                      "EventBus callback must be callable as callback(Event&) or callback(const Event&)");
        static_assert(std::is_copy_constructible_v<Fn>, "EventBus callback must be copy-constructible");

        return subscribeImpl(
            channelOf<Event>(),
            [fn = Fn(std::forward<Callback>(callback))](void *value) mutable {
                std::invoke(fn, *static_cast<Event *>(value));
            },
            priority);
    }

    bool unsubscribe(ListenerId id) noexcept;

    void clear() noexcept;

    /// Cheap check; use it in hooks to skip building the event when nobody listens.
    template <typename Event> bool hasListeners() const noexcept {
        return channelOf<Event>()->active.load(std::memory_order_relaxed) != 0;
    }

    template <typename Event> std::size_t publish(Event &event) noexcept {
        Channel *ch = channelOf<Event>();
        if (ch->active.load(std::memory_order_relaxed) == 0)
            return 0;
        return dispatch(ch, &event);
    }

    template <typename Event> std::size_t listenerCount() const noexcept {
        return channelOf<Event>()->active.load(std::memory_order_relaxed);
    }

  private:
    struct Impl;

    // Channels are never destroyed while the bus lives, so the pointer is stable
    // and can be cached. The cache is only valid for the global bus; any other
    // instance falls back to a (locked) map lookup.
    template <typename Event> Channel *channelOf() const {
        if (this == &getInstance()) {
            static Channel *cached = findChannel(detail::eventKey<Event>());
            return cached;
        }
        return findChannel(detail::eventKey<Event>());
    }

    Channel *findChannel(std::string_view key) const;
    ListenerId subscribeImpl(Channel *channel, std::function<void(void *)> callback, int priority);
    static std::size_t dispatch(Channel *channel, void *event) noexcept;

    Impl *mImpl{};
};

} // namespace nmdk
