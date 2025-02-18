#pragma once

#include <cmath>
#include <cstdint>
#include <cstring>

#include "p3a_macros.hpp"
#include "p3a_constants.hpp"

#include <Kokkos_Core.hpp>

namespace p3a {

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr
auto square(T const& a)
{
  return a * a;
}

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr
auto cube(T const& a)
{
  return a * a * a;
}

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr
T average(T const& a, T const& b)
{
  return (a + b) / 2;
}

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr T const&
condition(bool a, T const& b, T const& c)
{
  return a ? b : c;
}

using Kokkos::min;
using Kokkos::max;
using Kokkos::clamp;
using Kokkos::abs;
using Kokkos::sqrt;
using Kokkos::cbrt;
using Kokkos::sin;
using Kokkos::cos;
using Kokkos::tan;
using Kokkos::asin;
using Kokkos::acos;
using Kokkos::exp;
using Kokkos::pow;
using Kokkos::log;
using Kokkos::hypot;

template <class Head, class... Tail>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr auto
recursive_maximum(Head const& head, Tail... tail)
{
  return max(head, recursive_maximum(std::forward<Tail>(tail)...));
}

// only call recursive_maximum for three or more arguments
template <class T, class... Tail>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr auto
maximum(T const& a, T const& b, T const& c, Tail... tail)
{
  return recursive_maximum(a, b, c, std::forward<Tail>(tail)...);
}

template <class Head>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE constexpr
Head const& recursive_maximum(Head const& head)
{
  return head;
}

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE inline constexpr
T ceildiv(T a, T b) {
  return (a / b) + ((a % b) ? 1 : 0);
}

template <class A, class B>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE inline constexpr
A linear_interpolation(A a, A b, B t) {
  return a + t * (b - a);
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST_DEVICE inline
T load(T const* ptr, int offset)
{
  return ptr[offset];
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST_DEVICE inline
T load_scalar(T const* ptr, int offset)
{
  return ptr[offset];
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST_DEVICE inline
void store(T const& value, T* ptr, int offset)
{
  ptr[offset] = value;
}

P3A_ALWAYS_INLINE P3A_HOST_DEVICE inline constexpr double sign(double x)
{
  return (x < 0.0) ? -1.0 : 1.0;
}

template <class T>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE inline
T cotangent(T const& a)
{
  return 1.0 / p3a::tan(a);
}

template <typename T>
P3A_HOST_DEVICE P3A_ALWAYS_INLINE inline
void swap(T& t1, T& t2) {
    T temp(std::move(t1));
    t1 = std::move(t2);
    t2 = std::move(temp);
}

// In the algrebra of rotations one often comes across functions that
// take undefined (0/0) values at some points. Close to such points
// these functions must be evaluated using their asymptotic
// expansions; otherwise the computer may produce wildly erroneous
// results or a floating point exception. To avoid unreachable code
// everywhere such functions are used, we introduce here functions to
// the same effect.
//
// Function form: sin(x) / x
// X: 0
// Asymptotics: 1.0 (-x^2/6)
// First radius: (6 * EPS)^(.5)
// Second radius: (120 * EPS)^(.25)
template <class T>
[[nodiscard]] P3A_HOST_DEVICE inline
T sin_x_over_x(T const& x)
{
  auto const y = abs(x);
  auto constexpr epsilon = epsilon_value<T>();
  auto const e2 = sqrt(epsilon);
  auto const e4 = sqrt(e2);
  if (y > e4) {
    return sin(y) / y;
  } else if (y > e2) {
    return T(1.0) - (y * y) / T(6.0);
  } else {
    return T(1.0);
  }
}

// this is std::bit_cast in C++20
template <class To, class From>
[[nodiscard]] P3A_ALWAYS_INLINE P3A_HOST_DEVICE inline
To bit_cast(From const& src)
{
  To dst;
  memcpy(&dst, &src, sizeof(To));
  return dst;
}

template <class Value, class Tolerance>
[[nodiscard]] P3A_HOST_DEVICE P3A_ALWAYS_INLINE inline
auto are_close(Value const& a, Value const& b, Tolerance const& tolerance)
{
  using p3a::abs;
  auto const difference = abs(b - a);
  auto const scale = abs(a) + abs(b);
  return difference <= tolerance * p3a::max(scale, Value(1));
}

}
