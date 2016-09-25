#pragma once

#include <array>
#include <cstddef>
#include <cmath>

#include "Engine/Util/Assert.h"
#include "Engine/Math/Functions.h"

namespace Flappy {

template <typename T>
class Vec2 {
public:
	constexpr Vec2() = default;
	
	constexpr Vec2(T x, T y)
		: data {{x, y}} {}
	
	template <typename Other>
	explicit operator Vec2<Other>() const {
		Vec2<Other> result;
		
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = static_cast<Other>(this->data[i]);
		}
		
		return result;
	}
	
	static Vec2 zero() {
		Vec2 result;
		result.data.fill(T {0});
		
		return result;
	}
	
	static Vec2 fill(T value) {
		Vec2 result;
		result.data.fill(value);
		
		return result;
	}
	
	static Vec2 unit() {
		Vec2 result;
		result.data.fill(T {1. / std::sqrt(2.)});
		
		return result;
	}
	
	T dot(Vec2 other) const {
		T result = 0;
		for(std::size_t i = 0; i < data.size(); i++) {
			result += this->data[i] * other.data[i];
		}
		
		return result; 
	}
	
	static Vec2 dot(Vec2 a, Vec2 b) {
		return a.dot(b);
	}
	
	Vec2 mix(Vec2 other, T value) const {
		FLAPPY_ASSERT(value >= T {0} && value <= T {1}, "value of value should be between 0 and 1");
		
		Vec2 result;
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = mix(this->data[i], other.data[i], value);
		}
		
		return result;
	}
	
	static Vec2 mix(Vec2 a, Vec2 b, T value) {
		return a.mix(b, value);
	}
	
	T get_length_squared() const {
		return dot(*this);
	}
	
	T get_length() const {
		return std::sqrt(get_length_squared());
	}
	
	void normalize() {
		const T length = get_length();
		
		for(auto& e : this->data) {
			e /= length;
		}
	}
	
	Vec2 abs() const {
		Vec2 result;
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = std::abs(data[i]);
		}
		
		return result;
	}
	
	Vec2 get_normalized() const {
		Vec2 result {*this};
		result.normalize();
		
		return result;
	}
	
	bool is_normalized() const {
		return equal(get_length_squared(), T {1});
	}
	
	T distance_squared(Vec2 other) const {
		return (other - *this).get_length_squared();
	}
	
	T distance(Vec2 other) const {
		return (other - *this).get_length();
	}
	
	Vec2 projection_to(Vec2 value) const {
		return (this->dot(value) / value.get_length_squared()) * value;
	}

	Vec2 projection_to_normalized(Vec2 value) const {
		FLAPPY_ASSERT(value.is_normalized());
		return this->dot(value) * value;
	}
	
	T& operator [](std::size_t index) {
		return this->data[index];
	}

	const T& operator [](std::size_t index) const {
		return this->data[index];
	}
	
	
	const T& x() const {
		return (*this)[0];
	}
	
	T& x() {
		return (*this)[0];
	}
	
	const T& y() const {
		return (*this)[1];
	}
	
	T& y() {
		return (*this)[1];
	}
	
	bool strict_equal(Vec2 value) const {
		for(std::size_t i = 0; i < data.size(); i++) {
			if(this->data[i] != value.data[i]) {
				return false;
			}
		}
		
		return true;
	}
	
	bool equal(Vec2 value) const {
		if(std::is_integral<T>::value) {
			return strictEqual(value);
		}
		
		for(std::size_t i = 0; i < data.size(); i++) {
			if(!equal(this->data[i], value.data[i])) {
				return false;
			}
		}

		return true;
	}
	
	bool equal(Vec2 value, T threshold) const {
		if(std::is_integral<T>::value) {
			return strictEqual(value);
		}
		
		for(std::size_t i = 0; i < data.size(); i++) {
			if(!equal(this->data[i], value.data[i], threshold)) {
				return false;
			}
		}

		return true;
	}


	bool operator ==(Vec2 rhs) const {
		return equal(rhs);
	}

	bool operator !=(Vec2 rhs) const {
		return !(*this == rhs);
	}

	Vec2 operator +(Vec2 rhs) const {
		Vec2 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] += rhs.data[i];
		}
		return n;
	}

	Vec2 operator -(Vec2 rhs) const {
		Vec2 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] -= rhs.data[i];
		}
		return n;
	}
	
	Vec2 operator *(Vec2 rhs) const {
		Vec2 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] *= rhs.data[i];
		}
		return n;
	}
	
	Vec2 operator /(Vec2 rhs) const {
		Vec2 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] /= rhs.data[i];
		}
		return n;
	}
	
	Vec2 operator /(T rhs) {
		Vec2 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] /= rhs;
		}
		return n;
	}
	
	Vec2 operator -() const {
		Vec2 n;

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] = -this->data[i];
		}
		return n;
	}
	
	Vec2 operator *=(Vec2 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] *= rhs.data[i];
		}

		return *this;
	}

	Vec2 operator +=(Vec2 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] += rhs.data[i];
		}

		return *this;
	}

	Vec2 operator -=(Vec2 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] -= rhs.data[i];
		}

		return *this;
	}
	
	Vec2 operator *=(T rhs) {
		for(T& e : this->data) {
			e *= rhs;
		}

		return *this;
	}

	Vec2 operator /=(T rhs) {
		for(T& e : this->data) {
			e /= rhs;
		}

		return *this;
	}
	
	using iterator = typename std::array<T, 2>::iterator;
	using const_iterator = typename std::array<T, 2>::const_iterator;
	
	iterator begin() {
		return this->data.begin();
	}
	
	const_iterator begin() const {
		return this->data.begin();
	}
	
	const_iterator cbegin() const {
		return this->data.cbegin();
	}
	
	iterator end() {
		return this->data.end();
	}
	
	const_iterator end() const {
		return this->data.end();
	}
	
	const_iterator cend() const {
		return this->data.cend();
	}
	
private:
	std::array<T, 2> data;
};

template <typename T>
Vec2<T> operator *(T lhs, Vec2<T> rhs) {
	Vec2<T> n {rhs};

	for(T& i : n) {
		i *= lhs;
	}
	return n;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const Vec2<T>& value) {
	auto it = value.begin();

	os << "{ " << *it++;

	for(; it != value.end(); it++) {
		os << ", " << *it;
	}

	os << " }";
	return os;
}

using Vec2f = Vec2<float>;
static_assert(std::is_pod<Vec2f>::value, "Vec2f should be a POD");

using Vec2d = Vec2<double>;
static_assert(std::is_pod<Vec2d>::value, "Vec2d should be a POD");

using Vec2u = Vec2<std::uint_fast32_t>;
static_assert(std::is_pod<Vec2f>::value, "Vec2u should be a POD");

}

