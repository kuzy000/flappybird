#pragma once

#include "Engine/Math/Vec2.h"
#include "Engine/Math/Size2.h"

#include <ostream>

namespace Flappy {

template <typename T>
class AABB2 {
public:
	constexpr AABB2() = default;
	constexpr AABB2(Vec2<T> start, Vec2<T> end)
		: start {start}, end {end} {}
	
	bool contains(Vec2<T> point) const {
		bool result = true;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s = start[i];
			const T e = end[i];
			const T c = point[i];
			
			if(c < s || c > e) {
				result = false;
				break;
			}
		}
		
		return result;
	}
	
	AABB2 intersection(AABB2 other) const {
		Vec2<T> rstart;
		Vec2<T> rend;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s1 = start[i];
			const T e1 = end[i];
			const T s2 = other.start[i];
			const T e2 = other.end[i];
			
			rstart[i] = s1 >= s2 ? s1 : s2;
			rend[i]   = e1 <= e2 ? e1 : e2;
		}
		
		return AABB2 {rstart, rend};
	}
	
	bool is_point() const {
		bool result = true;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s = start[i];
			const T e = end[i];
			
			if(s != e) {
				result = false;
				break;
			}
		}
		
		return result;
	}
	
	bool is_zero() const {
		bool result = false;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s = start[i];
			const T e = end[i];
			
			if(s == e) {
				result = true;
				break;
			}
		}
		
		return result;
	}
	
	bool is_negative() const {
		bool result = false;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s = start[i];
			const T e = end[i];
			
			if(s > e) {
				result = true;
				break;
			}
		}
		
		return result;
	}
	
	bool is_positive() const {
		bool result = false;
		
		for(std::size_t i = 0; i < 2; i++) {
			const T s = start[i];
			const T e = end[i];
			
			if(s < e) {
				result = true;
				break;
			}
		}
		
		return result;
	}
	
	Size2<T> get_size() const {
		return Size2<T> {end - start};
	}
	
	Vec2<T> get_center() const {
		return (start + end) / 2;
	}
	
	T width() const {
		return end.x() - start.x();
	}
	
	T height() const {
		return end.y() - start.y();
	}
	
	T left() const {
		return start.x();
	}
	
	T bottom() const {
		return start.y();
	}
	
	T right() const {
		return end.x();
	}
	
	T top() const {
		return end.y();
	}
	
	bool operator ==(AABB2 rhs) const {
		return start == rhs.start && end == rhs.end;
	}
	
	bool operator !=(AABB2 rhs) const {
		return !(*this == rhs);
	}
	
	bool strictEqual(AABB2 value) const {
		return start.strictEqual(value.start) && end.strictEqual(value.end);
	}
	
	Vec2<T> start;
	Vec2<T> end;
};

template <typename T>
std::ostream& operator << (std::ostream& os, const AABB2<T>& value) {
	os << value.start << " [::] " << value.end;
	return os;
}

using AABB2f = AABB2<float>;
using AABB2i = AABB2<std::int_fast32_t>;

}

