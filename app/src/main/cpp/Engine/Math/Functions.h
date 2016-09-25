#pragma once

#include <cmath>
#include <tuple>
#include <limits>
#include <algorithm>
#include <type_traits>

#include "Engine/Util/Assert.h"

namespace Flappy { 

template <typename T>
bool equal(T a, T b) {
	const float max = std::max(std::max(1.0f, std::fabs(a)), std::fabs(b));
	return std::fabs(b - a) <= std::numeric_limits<T>::epsilon() * max;
}

template <typename T>
bool equal(T a, T b, T threshold) {
	return std::fabs(b - a) <= threshold;
}

template <typename T, typename S>
auto mix(T a, T b, S value) -> typename std::enable_if<std::is_fundamental<T>::value, T>::type {
	FLAPPY_ASSERT(value >= S {0} && value <= S {1}, "value of value should be between 0 and 1");
	return a * (S {1} - value) + b * value;
}

template <typename T, typename S>
constexpr auto mix(T&& a, T&& b, S&& value) -> decltype(a.mix(b, value)) {
	return a.mix(std::forward<T>(b), std::forward<S>(value));
}

template <typename T>
constexpr T clamp(T&& value, T&& min, T&& max) {
	FLAPPY_ASSERT(min <= max);
	return std::min(std::forward<T>(max), std::max(std::forward<T>(min), std::forward<T>(value)));
}

template <typename T>
constexpr T step(T&& edge, T&& value) {
	return std::forward<T>(value) < std::forward<T>(edge) ? T {0} : T {1};
}

template <typename T>
constexpr T smoothstep(T&& edge0, T&& edge1, T&& value) {
	FLAPPY_ASSERT(edge0 < edge1); 
	
	const auto t = clamp((value - edge0) / (edge1 - edge0), T {0}, T {1});
	return t * t * (T {3} - T {2} * t);
}

}
