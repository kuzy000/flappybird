#pragma once

#include "Engine/Gl/Object.h"
#include "Engine/Gl/Error.h"

#include <vector>

namespace Flappy {
namespace Gl {

class Buffer : public GLuintId {
public:
	enum class Target : GLenum { 
		Array = GL_ARRAY_BUFFER, 
		ElementArray = GL_ELEMENT_ARRAY_BUFFER 
	};
	
	enum class Usage : GLenum {
		StaticDraw = GL_STATIC_DRAW,
		StreamDraw = GL_STREAM_DRAW,
		DynamicDraw = GL_DYNAMIC_DRAW
	};

	
	Buffer();
	~Buffer();
	
	Buffer(const Buffer&) = delete;
	Buffer& operator =(const Buffer&) = delete;
	
	Buffer(Buffer&& that);
	Buffer& operator =(Buffer&& rhs);
	
	void bind(Target target);
	static void unbind(Target target);
	
	template<typename T, std::size_t Size>
	static void data(Target target, Usage usage, const T (&data)[Size]) {
		FLAPPY_GL_CHECKED(glBufferData(static_cast<GLenum>(target), Size * sizeof(T), data, static_cast<GLenum>(usage)));
	}

	template<typename T>
	static void data(Target target, Usage usage, const T* data, GLsizeiptr size) {
		FLAPPY_GL_CHECKED(glBufferData(static_cast<GLenum>(target), size, data, static_cast<GLenum>(usage)));
	}
	
	template<typename T>
	static void data(Target target, Usage usage, const std::vector<T>& data) {
		FLAPPY_GL_CHECKED(glBufferData(static_cast<GLenum>(target), data.size() * sizeof(T), data.data(), static_cast<GLenum>(usage)));
	}

	template<typename T, std::size_t Size>
	static void data(Target target, Usage usage, const std::array<T, Size>& data) {
		FLAPPY_GL_CHECKED(glBufferData(static_cast<GLenum>(target), Size * sizeof(T), data.data(), static_cast<GLenum>(usage)));
	}

	template<typename T, std::size_t Size>
	static void sub_data(Target target, GLintptr offset, const T (&data)[Size]) {
		FLAPPY_GL_CHECKED(glBufferSubData(static_cast<GLenum>(target), offset, Size * sizeof(T), data));
	}

	template<typename T>
	static void sub_data(Target target, GLintptr offset, const T data, GLsizeiptr size) {
		FLAPPY_GL_CHECKED(glBufferSubData(static_cast<GLenum>(target), offset, size, data));
	}

	template<typename T>
	static void sub_data(Target target, GLintptr offset, const std::vector<T>& data) {
		FLAPPY_GL_CHECKED(glBufferSubData(static_cast<GLenum>(target), offset, data.size() * sizeof(T), data.data()));
	}
	
	template<typename T, std::size_t Size>
	static void sub_data(Target target, GLintptr offset, std::array<T, Size> data) {
		FLAPPY_GL_CHECKED(glBufferSubData(static_cast<GLenum>(target), offset, Size * sizeof(T), data.data()));
	}
};

}
}
