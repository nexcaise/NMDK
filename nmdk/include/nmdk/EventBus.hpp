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

class MCAPI EventBus {
  public:
    using ListenerId = std::uint64_t;

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
