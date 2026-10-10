#pragma once

#include "nmdk/api/Macro.h"

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
            detail::eventKey<Event>(),
            [fn = Fn(std::forward<Callback>(callback))](void *value) mutable {
                std::invoke(fn, *static_cast<Event *>(value));
            },
            priority);
    }

    bool unsubscribe(ListenerId id) noexcept;

    void clear() noexcept;

    template <typename Event> std::size_t publish(Event &event) {
        return publishImpl(detail::eventKey<Event>(), &event);
    }

    template <typename Event> std::size_t listenerCount() const noexcept {
        return listenerCountImpl(detail::eventKey<Event>());
    }

  private:
    struct Impl;

    ListenerId subscribeImpl(std::string_view key, std::function<void(void *)> callback, int priority);
    std::size_t publishImpl(std::string_view key, void *event);
    std::size_t listenerCountImpl(std::string_view key) const noexcept;

    Impl *mImpl{};
};

} // namespace nmdk
