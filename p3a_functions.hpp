#pragma once

#include <cmath>

#include "p3a_macros.hpp"
#include "p3a_constants.hpp"

namespace p3a {

template <class T>
P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr
bool compare(T a, T b)
{
   return std::abs(a-b) <= T(2.0)*epsilon_value<T>();
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr
auto square(T const& a)
{
  return a * a;
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr
auto cube(T const& a)
{
  return a * a * a;
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr
T average(T const& a, T const& b)
{
  return (a + b) / 2;
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double absolute_value(double a)
{
  return std::abs(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double ceiling(double a)
{
  return std::ceil(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double square_root(double a)
{
  return std::sqrt(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double natural_exponential(double a)
{
  return std::exp(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double natural_logarithm(double a)
{
  return std::log(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double exponentiate(double a, double b)
{
  return std::pow(a, b);
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr T const&
condition(bool a, T const& b, T const& c)
{
  return a ? b : c;
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr auto
minimum(T const& a, T const& b)
{
  return condition(b < a, b, a);
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE constexpr auto
maximum(T const& a, T const& b)
{
  return condition(a < b, b, a);
}

template <class T>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline constexpr
T ceildiv(T a, T b) {
  return (a / b) + ((a % b) ? 1 : 0);
}

template <class A, class B>
[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline constexpr
A linear_interpolation(A a, A b, B t) {
  return a + t * (b - a);
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST P3A_DEVICE inline
T load(T const* ptr, int offset)
{
  return ptr[offset];
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST P3A_DEVICE inline
T load_scalar(T const* ptr, int offset)
{
  return ptr[offset];
}

template <class T>
P3A_ALWAYS_INLINE P3A_HOST P3A_DEVICE inline
void store(T const& value, T* ptr, int offset)
{
  ptr[offset] = value;
}

P3A_ALWAYS_INLINE P3A_HOST P3A_DEVICE inline constexpr double sign(double x)
{
  return (x < 0.0) ? -1.0 : 1.0;
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double arcsin(double a)
{
  return std::asin(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double arccos(double a)
{
  return std::acos(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double sin(double a)
{
  return std::sin(a);
}

[[nodiscard]] P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
double cos(double a)
{
  return std::cos(a);
}

template <typename T>
P3A_HOST P3A_DEVICE P3A_ALWAYS_INLINE inline
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
[[nodiscard]] P3A_HOST P3A_DEVICE inline
T sin_x_over_x(T const& x) {
  auto const y = absolute_value(x);
  auto constexpr epsilon = epsilon_value<T>();
  auto const e2 = square_root(epsilon);
  auto const e4 = square_root(e2);
  if (y > e4) {
    return sin(y) / y;
  } else if (y > e2) {
    return T(1.0) - (y * y) / T(6.0);
  } else {
    return T(1.0);
  }
}

}
