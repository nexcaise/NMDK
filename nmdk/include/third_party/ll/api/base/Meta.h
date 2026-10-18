
#pragma once

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#ifndef LL_MAY_CONSTEXPR
#define LL_MAY_CONSTEXPR constexpr
#endif

namespace ll::meta {

template <class F, class... Args> struct Elide {
    using result_type = std::invoke_result_t<F, Args...>;

    F &&fn;
    std::tuple<Args &&...> args;

    constexpr operator result_type() && { return std::apply(std::forward<F>(fn), std::move(args)); }
};

template <class F, class... Args> [[nodiscard]] constexpr Elide<F, Args...> elide(F &&fn, Args &&...args) {
    return {std::forward<F>(fn), std::forward_as_tuple(std::forward<Args>(args)...)};
}

} // namespace ll::meta
