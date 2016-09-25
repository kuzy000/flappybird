#pragma once

#include "Engine/Math/Vec4.h"

#include <type_traits>

namespace Flappy {

template <typename T>
class RGBA : public Vec4<T> {
public:
	using Base = Vec4<T>;
	
	RGBA() = default;
	using Base::Base;
	
	explicit RGBA(Vec4<T> arg)
		: Base {std::move(arg)} {}
	
#define FLAPPY_RGBA_DECLARE_ALIAS(NAME, NUMBER) \
	const T& NAME() const { \
		return (*this)[NUMBER]; \
	} \
	T& NAME() { \
		return (*this)[NUMBER]; \
	}
	
	FLAPPY_RGBA_DECLARE_ALIAS(r, 0)
	FLAPPY_RGBA_DECLARE_ALIAS(g, 1)
	FLAPPY_RGBA_DECLARE_ALIAS(b, 2)
	FLAPPY_RGBA_DECLARE_ALIAS(a, 3)
	
#undef FLAPPY_RGBA_DECLARE_ALIAS
private:
	using Base::x;
	using Base::y;
	using Base::z;
	using Base::w;
};

template <typename T>
std::ostream& operator << (std::ostream& os, const RGBA<T>& value) {
	os << "RGBA " << static_cast<Vec4<T>>(value);
	return os;
}

using RGBAf = RGBA<float>;
using RGBAd = RGBA<double>;

static_assert(std::is_pod<RGBAf>::value, "RGBAf should be a POD");
static_assert(std::is_pod<RGBAd>::value, "RGBAd should be a POD");

}
