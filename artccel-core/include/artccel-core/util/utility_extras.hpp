#ifndef ARTCCEL_CORE_UTIL_UTILITY_EXTRAS_HPP
#define ARTCCEL_CORE_UTIL_UTILITY_EXTRAS_HPP
#pragma once

#include <concepts>    // import std::invocable
#include <cstddef>     // import std::size_t
#include <functional>  // import std::invoke
#include <type_traits> // import std::invoke_result_t, std::is_nothrow_invocable_v, std::is_nothrow_move_constructible_v, std::is_reference_v
#include <utility>     // import std::index_sequence, std::index_sequence_for

namespace artccel::core::util {
template <typename> constexpr inline auto dependent_false_v{false};

template <typename T>
constexpr auto possible_ref_as_pointer(T &&value) noexcept {
  if constexpr (std::is_reference_v<T>) {
    // lvalues (T = t&), xvalues (T = t&&)
    return &value;
  } else {
    // prvalues (T = t), others
    return value;
  }
}

template <typename F, template <typename...> typename Tuple, typename... Args>
requires std::invocable<F, Args...>
constexpr auto forward_apply(F &&func, Tuple<Args...> &&t_args) noexcept(
    std::is_nothrow_invocable_v<F, Args...> &&
        std::is_nothrow_move_constructible_v<std::invoke_result_t<F, Args...>>)
    -> decltype(auto) {
  using TArgs = Tuple<Args...>;
  return
      []<std::size_t... I>(
          F && func, TArgs && t_args,
          [[maybe_unused]] std::index_sequence<
              I...> /*unused*/) mutable noexcept(std::
                                                     is_nothrow_invocable_v<
                                                         F, Args...> &&
                                                 std::
                                                     is_nothrow_move_constructible_v<
                                                         std::invoke_result_t<
                                                             F, Args...>>)
          ->decltype(auto) {
    return std::invoke(
        std::forward<F>(func),
        std::forward<Args>(std::get<I>(std::forward<TArgs>(t_args)))...);
  }
  (std::forward<F>(func), std::forward<TArgs>(t_args),
   std::index_sequence_for<Args...>{});
}
} // namespace artccel::core::util

#endif
