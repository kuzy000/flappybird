#pragma once

#include <string>
#include <utility>

#include "Engine/Math/Vec2.h"

namespace Flappy {

template <typename T>
class Size2 : public Vec2<T> {
public:
	using Base = Vec2<T>;
	
	Size2()
		: Base {Base::zero()} {}
		
	Size2(Vec2<T> value) 
		: Base {std::move(value)} {}
		
	template <typename Other>
	explicit operator Size2<Other>() const {
		return Size2<Other> {static_cast<Vec2<Other>>(*this)};
	}
	
	using Base::Base;
	
	const Base& center() const {
		return *this / 2.;
	}
	
	const T& width() const {
		return this->x();
	}
	
	T& width() {
		return this->x();
	}
	
	const T& height() const {
		return this->y();
	}
	
	T& height() {
		return this->y();
	}
	
};

template <typename T>
std::ostream& operator << (std::ostream& os, const Size2<T>& value) {
	os << "Size2" << " " << static_cast<Vec2<T>>(value);
	return os;
}

using Size2f = Size2<float>;
using Size2u = Size2<std::uint_fast32_t>;

}
