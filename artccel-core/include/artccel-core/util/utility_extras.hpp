#ifndef ARTCCEL_CORE_UTIL_UTILITY_EXTRAS_HPP
#define ARTCCEL_CORE_UTIL_UTILITY_EXTRAS_HPP
#pragma once

#include <concepts>   // import std::invocable
#include <cstddef>    // import std::size_t
#include <functional> // import std::invoke
#include <utility>    // import std::index_sequence, std::index_sequence_for

namespace artccel::core::util {
template <typename F, template <typename...> typename Tuple, typename... Args>
requires std::invocable<F, Args...>
constexpr auto forward_apply(F &&func, Tuple<Args...> &&t_args)
    -> decltype(auto) {
  using TArgs = Tuple<Args...>;
  return [ func = std::forward<F>(func),
           t_args = std::forward<TArgs>(t_args) ]<std::size_t... I>(
      [[maybe_unused]] std::index_sequence<I...> /*unused*/) mutable {
    return std::invoke(
        std::forward<F>(func),
        std::forward<Args>(std::get<I>(std::forward<TArgs>(t_args)))...);
  }
  (std::index_sequence_for<Args...>{});
}
} // namespace artccel::core::util

#endif