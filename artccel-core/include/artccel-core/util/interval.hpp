#pragma once

#include <assert.h>
#include <concepts>
#include <inttypes.h>
#include <utility>

namespace artccel::core {
template <std::totally_ordered Type>
requires std::default_initializable<Type>
constexpr Type unbounded{};

enum class bound : uint8_t {
  open,
  closed,
  unbounded,
};

template <bound Bound, std::totally_ordered auto Left,
          std::totally_ordered auto Right>
requires std::totally_ordered_with<decltype(Left), decltype(Right)>
consteval bool bound_less_than() {
  return (Bound == bound::open && Left < Right) ||
         (Bound == bound::closed && Left <= Right) || Bound == bound::unbounded;
}

inline bool bound_less_than(bound bound, std::totally_ordered auto const &left,
                            std::totally_ordered auto const &right) requires
    std::totally_ordered_with<decltype(left), decltype(right)> {
  return (bound == bound::open && left < right) ||
         (bound == bound::closed && left <= right) || bound == bound::unbounded;
}

template <std::totally_ordered Type, bound LeftBound, Type Left, Type Right,
          bound RightBound>
requires std::movable<Type>
struct interval {
  interval(Type const &value) requires std::copyable<Type>
      : interval{Type{value}} {}
  interval(Type &&value) : interval{std::move(value), false} {
    assert(("left >(=) value", bound_less_than(LeftBound, Left, this->value)));
    assert(
        ("value >(=) right", bound_less_than(RightBound, this->value, Right)));
  }
  template <Type Value> static auto check() {
    static_assert(bound_less_than<LeftBound, Left, Value>(), "left >(=) value");
    static_assert(bound_less_than<RightBound, Value, Right>(),
                  "value >(=) right");
    return interval{Value, false};
  }
  inline operator Type() { return value; }
  inline operator Type const() const { return value; }

private:
  Type value;
  interval(Type &&value, bool) : value{value} {}
};

template <std::totally_ordered Type>
requires std::constructible_from<Type, decltype(0)>
using nonnegative_interval =
    interval<Type, bound::closed, Type{0}, unbounded<Type>, bound::unbounded>;
template <std::totally_ordered Type>
requires std::constructible_from<Type, decltype(0)>
using nonpositive_interval =
    interval<Type, bound::unbounded, unbounded<Type>, Type{0}, bound::closed>;
template <std::totally_ordered Type>
requires std::constructible_from<Type, decltype(0)>
using positive_interval =
    interval<Type, bound::open, Type{0}, unbounded<Type>, bound::unbounded>;
template <std::totally_ordered Type>
requires std::constructible_from<Type, decltype(0)>
using negative_interval =
    interval<Type, bound::unbounded, unbounded<Type>, Type{0}, bound::open>;
} // namespace artccel::core
