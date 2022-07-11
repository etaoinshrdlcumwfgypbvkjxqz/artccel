#ifndef ARTCCEL_CORE_GEOMETRY_GEOMETRY_IMPL_HPP
#define ARTCCEL_CORE_GEOMETRY_GEOMETRY_IMPL_HPP
#pragma once

#include "geometry.hpp" // interface

#include "../util/interval.hpp"  // import util::Nonnegative_interval
#include "../util/semantics.hpp" // import util::Observer_ptr
#include <array>                 // import std::array
#include <concepts>              // import std::convertible_to
#include <cstdint>               // import std::int_fast8_t
#include <gsl/gsl>               // import gsl::owner
#include <type_traits>           // import std::is_nothrow_copy_constructible_v
#include <typeinfo>              // import std::type_info
#include <utility>               // import std::swap

namespace artccel::core::geometry::impl {
template <util::Nonnegative_interval<std::int_fast8_t> Dim>
class Geometry_impl : public virtual Geometry {
public:
  // NOLINTNEXTLINE(fuchsia-statically-constructed-objects): constexpr ctor
  constexpr static auto dimension_{Dim};
  auto dimension [[nodiscard]] () const
      -> util::Nonnegative_interval<std::int_fast8_t> final {
    return Dim;
  }
  ~Geometry_impl() noexcept override = default;
  auto clone [[nodiscard]] () const -> gsl::owner<Geometry *> {
    return clone_impl();
  }
  virtual auto clone_impl [[nodiscard]] () const
      -> gsl::owner<Geometry_impl *> = 0;

protected:
#pragma warning(suppress : 4589)
  using Geometry::Geometry;
  void swap(Geometry_impl &other [[maybe_unused]]) noexcept { using std::swap; }
  Geometry_impl(Geometry_impl const &other [[maybe_unused]]) noexcept {}
  auto operator=(Geometry_impl const &right [[maybe_unused]]) noexcept(
      noexcept(Geometry_impl{right}.swap(*this), *this)) -> Geometry_impl & {
    Geometry_impl{right}.swap(*this);
    return *this;
  }
  Geometry_impl(Geometry_impl &&) noexcept = default;
  auto operator=(Geometry_impl &&right [[maybe_unused]]) noexcept
      -> Geometry_impl & {
    Geometry_impl{std::move(right)}.swap(*this);
    return *this;
  }
#pragma warning(suppress : 4820)
};

template <util::Nonnegative_interval<std::int_fast8_t> Dim>
// NOLINTNEXTLINE(fuchsia-multiple-inheritance)
class Primitive_impl : public virtual Primitive, public Geometry_impl<Dim> {
public:
  using Primitive_impl::Geometry_impl::dimension_;
  ~Primitive_impl() noexcept override = default;
  auto clone [[nodiscard]] () const -> gsl::owner<Primitive *> {
    return clone_impl();
  }
  auto clone_impl [[nodiscard]] () const
      -> gsl::owner<Primitive_impl *> override = 0;

protected:
#pragma warning(suppress : 4589)
  using Primitive_impl::Geometry_impl::Geometry_impl;
  void swap(Primitive_impl &other) noexcept {
    using std::swap;
    Primitive_impl::Geometry_impl::swap(other);
  }
  Primitive_impl(Primitive_impl const &other) noexcept(
      std::is_nothrow_copy_constructible_v<
          typename Primitive_impl::Geometry_impl>)
      : Primitive_impl::Geometry_impl{other} {}
  auto operator=(Primitive_impl const &right) noexcept(
      noexcept(Primitive_impl{right}.swap(*this), *this)) -> Primitive_impl & {
    Primitive_impl{right}.swap(*this);
    return *this;
  }
  Primitive_impl(Primitive_impl &&) noexcept = default;
  auto operator=(Primitive_impl &&right) noexcept -> Primitive_impl & {
    Primitive_impl{std::move(right)}.swap(*this);
    return *this;
  }
#pragma warning(suppress : 4250 4820)
};

template <util::Nonnegative_interval<std::int_fast8_t> Dim>
// NOLINTNEXTLINE(fuchsia-multiple-inheritance)
class Point_impl : public virtual Point, public Primitive_impl<Dim> {
public:
  using Point_impl::Primitive_impl::dimension_;

private:
  std::array<compute::Compute_out<double>, Dim> position_{};

public:
  Point_impl() noexcept = default;
  template <
      std::convertible_to<typename decltype(position_)::value_type>... Position>
  requires(sizeof...(Position) == Dim) explicit Point_impl(Position... position)
      : position_{{std::move(position)...}} {}
  ~Point_impl() noexcept override = default;
  auto clone [[nodiscard]] () const -> gsl::owner<Point *> {
    return clone_impl();
  }
  auto clone_impl [[nodiscard]] () const -> gsl::owner<Point_impl *> override {
    return new Point_impl{*this};
  }

protected:
  auto try_get_quality [[nodiscard]] (std::type_info const &quality_type
                                      [[maybe_unused]]) noexcept
      -> util::Observer_ptr<Quality> override {
    return nullptr;
  }
  auto try_get_quality [[nodiscard]] (std::type_info const &quality_type
                                      [[maybe_unused]]) const noexcept
      -> util::Observer_ptr<Quality const> override {
    return nullptr;
  }

  void swap(Point_impl &other) noexcept {
    using std::swap;
    Point_impl::Primitive_impl::swap(other);
    swap(position_, other.position_);
  }
  Point_impl(Point_impl const &other) noexcept(
      std::is_nothrow_copy_constructible_v<typename Point_impl::Primitive_impl>
          &&noexcept(decltype(position_){other.position_}))
      : Point_impl::Primitive_impl{other}, position_{other.position_} {}
  auto operator=(Point_impl const &right) noexcept(
      noexcept(Point_impl{right}.swap(*this), *this)) -> Point_impl & {
    Point_impl{right}.swap(*this);
    return *this;
  }
  Point_impl(Point_impl &&other) noexcept
      : Point_impl::Primitive_impl{std::move(other)}, position_{std::move(
                                                          other.position_)} {}
  auto operator=(Point_impl &&right) noexcept -> Point_impl & {
    Point_impl{std::move(right)}.swap(*this);
    return *this;
  }
#pragma warning(suppress : 4250 4820)
};
} // namespace artccel::core::geometry::impl

#endif