#pragma once

#include <array>
#include <cstddef>
#include <cmath>

#include "Engine/Util/Assert.h"
#include "Engine/Math/Functions.h"

namespace Flappy {

template <typename T>
class Vec3 {
public:
	Vec3() = default;
	
	Vec3(T x, T y, T z)
		: data {{x, y, z}} {}
	
	template <typename Other>
	explicit operator Vec3<Other>() const {
		Vec3<Other> result;
		
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = static_cast<Other>(this->data[i]);
		}
		
		return result;
	}
	
	static Vec3 zero() {
		Vec3 result;
		result.data.fill(T {0});
		
		return result;
	}
	
	static Vec3 fill(T value) {
		Vec3 result;
		result.data.fill(value);
		
		return result;
	}
	
	static Vec3 unit() {
		Vec3 result;
		result.data.fill(T {1. / std::sqrt(2.)});
		
		return result;
	}
	
	T dot(Vec3 other) const {
		T result = 0;
		for(std::size_t i = 0; i < data.size(); i++) {
			result += this->data[i] * other.data[i];
		}
		
		return result; 
	}
	
	static Vec3 dot(Vec3 a, Vec3 b) {
		return a.dot(b);
	}
	
	Vec3 mix(Vec3 other, T value) const {
		FLAPPY_ASSERT(value >= T {0} && value <= T {1}, "value of value should be between 0 and 1");
		
		Vec3 result;
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = mix(this->data[i], other.data[i], value);
		}
		
		return result;
	}
	
	static Vec3 mix(Vec3 a, Vec3 b, T value) {
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
	
	Vec3 abs() const {
		Vec3 result;
		for(std::size_t i = 0; i < data.size(); i++) {
			result[i] = std::abs(data[i]);
		}
		
		return result;
	}
	
	Vec3 get_normalized() const {
		Vec3 result {*this};
		result.normalize();
		
		return result;
	}
	
	bool is_normalized() const {
		return equal(get_length_squared(), T {1});
	}
	
	T distance_squared(Vec3 other) const {
		return (other - *this).get_length_squared();
	}
	
	T distance(Vec3 other) const {
		return (other - *this).get_length();
	}
	
	Vec3 projection_to(Vec3 value) const {
		return (this->dot(value) / value.get_length_squared()) * value;
	}

	Vec3 projection_to_normalized(Vec3 value) const {
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
	
	const T& z() const {
		return (*this)[2];
	}
	
	T& z() {
		return (*this)[2];
	}
	
	bool strict_equal(Vec3 value) const {
		for(std::size_t i = 0; i < data.size(); i++) {
			if(this->data[i] != value.data[i]) {
				return false;
			}
		}
		
		return true;
	}
	
	bool equal(Vec3 value) const {
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
	
	bool equal(Vec3 value, T threshold) const {
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


	bool operator ==(Vec3 rhs) const {
		return equal(rhs);
	}

	bool operator !=(Vec3 rhs) const {
		return !(*this == rhs);
	}

	Vec3 operator +(Vec3 rhs) const {
		Vec3 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] += rhs.data[i];
		}
		return n;
	}

	Vec3 operator -(Vec3 rhs) const {
		Vec3 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] -= rhs.data[i];
		}
		return n;
	}
	
	Vec3 operator *(Vec3 rhs) const {
		Vec3 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] *= rhs.data[i];
		}
		return n;
	}
	
	Vec3 operator /(Vec3 rhs) const {
		Vec3 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] /= rhs.data[i];
		}
		return n;
	}
	
	Vec3 operator /(T rhs) {
		Vec3 n {*this};

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] /= rhs;
		}
		return n;
	}
	
	Vec3 operator -() const {
		Vec3 n;

		for(std::size_t i = 0; i < data.size(); i++) {
			n.data[i] = -this->data[i];
		}
		return n;
	}
	
	Vec3 operator *=(Vec3 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] *= rhs.data[i];
		}

		return *this;
	}

	Vec3 operator +=(Vec3 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] += rhs.data[i];
		}

		return *this;
	}

	Vec3 operator -=(Vec3 rhs) {
		for(std::size_t i = 0; i < data.size(); i++) {
			this->data[i] -= rhs.data[i];
		}

		return *this;
	}
	
	Vec3 operator *=(T rhs) {
		for(T& e : this->data) {
			e *= rhs;
		}

		return *this;
	}

	Vec3 operator /=(T rhs) {
		for(T& e : this->data) {
			e /= rhs;
		}

		return *this;
	}
	
	using iterator = typename std::array<T, 3>::iterator;
	using const_iterator = typename std::array<T, 3>::const_iterator;
	
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
	std::array<T, 3> data;
};

template <typename T>
Vec3<T> operator *(T lhs, Vec3<T> rhs) {
	Vec3<T> n {rhs};

	for(T& i : n) {
		i *= lhs;
	}
	return n;
}

template <typename T>
std::ostream& operator << (std::ostream& os, const Vec3<T>& value) {
	auto it = value.begin();

	os << "{ " << *it++;

	for(; it != value.end(); it++) {
		os << ", " << *it;
	}

	os << " }";
	return os;
}

using Vec3f = Vec3<float>;
static_assert(std::is_pod<Vec3f>::value, "Vec3f should be a POD");

using Vec3u = Vec3<std::uint_fast32_t>;
static_assert(std::is_pod<Vec3f>::value, "Vec3u should be a POD");

}

