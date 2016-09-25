#pragma once

#include "Engine/Math/Vec3.h"

#include <type_traits>

namespace Flappy {

template <typename T>
class RGB : public Vec3<T> {
public:
	using Base = Vec3<T>;
	
	RGB() = default;
	using Base::Base;
	
	explicit RGB(Vec3<T> arg)
		: Base {std::move(arg)} {}
	
#define FLAPPY_RGB_DECLARE_ALIAS(NAME, NUMBER) \
	const T& NAME() const { \
		return (*this)[NUMBER]; \
	} \
	T& NAME() { \
		return (*this)[NUMBER]; \
	}
	
	FLAPPY_RGB_DECLARE_ALIAS(r, 0)
	FLAPPY_RGB_DECLARE_ALIAS(g, 1)
	FLAPPY_RGB_DECLARE_ALIAS(b, 2)
	
#undef FLAPPY_RGB_DECLARE_ALIAS
private:
	using Base::x;
	using Base::y;
	using Base::z;
};

template <typename T>
std::ostream& operator << (std::ostream& os, const RGB<T>& value) {
	os << "RGB " << static_cast<Vec3<T>>(value);
	return os;
}

using RGBf = RGB<float>;
using RGBd = RGB<double>;

static_assert(std::is_pod<RGBf>::value, "RGBf should be a POD");
static_assert(std::is_pod<RGBd>::value, "RGBd should be a POD");

}
