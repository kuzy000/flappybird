#pragma once

#include "Engine/Platform/Gl.h"

namespace Flappy {
namespace Gl {

template <typename T, T NotReadyValue>
class Object {
public:
	Object() = default;
	
	Object(T id)
		: id {id} {}
	
	constexpr bool is_ready() const {
		return id != NotReadyValue;
	}

	constexpr GLuint get_id() const {
		return static_cast<GLuint>(id);
	}
protected:
	void not_ready() {
		id = NotReadyValue;
	}
	
	T id = NotReadyValue;
};

using GLuintId = Object<GLuint, 0>;
using GLintId  = Object<GLint, -1>;

}
}

