#pragma once

#include "Engine/Gl/Object.h"
#include "Engine/Gl/Error.h"
#include "Engine/Util/Assert.h"
#include "Engine/Math/Vec2.h"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Vec4.h"

#include <array>

namespace Flappy {
namespace Gl {

struct Sampler2D {};

namespace Impl {
class Uniform : public GLintId {
public:
	using GLintId::Object;
	
	void raw_set(GLfloat a) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform1f(id, a));
	}
	void raw_set(GLint a) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform1i(id, a));
	}
	
	void raw_set(GLfloat a, GLfloat b) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform2f(id, a, b));
	}
	void raw_set(GLint a, GLint b) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform2i(id, a, b));
	}
	
	void raw_set(GLfloat a, GLfloat b, GLfloat c) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform3f(id, a, b, c));
	}
	void raw_set(GLint a, GLint b, GLint c) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform3i(id, a, b, c));
	}
	
	void raw_set(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform4f(id, a, b, c, d));
	}
	void raw_set(GLint a, GLint b, GLint c, GLint d) {
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		FLAPPY_GL_CHECKED(glUniform4i(id, a, b, c, d));
	}
	
//	void raw_set(Mat4f value) {
//		FLAPPY_GL_CHECKED(glUniformMatrix4fv(id, 1, GL_FALSE, reinterpret_cast<GLfloat*>(&value)));
//	}
	
	template <int Dim, int Size>
	void raw_set_array(const GLfloat (&value)[Size]) {
		static_assert(Dim >= 1 && Dim <= 4, "Dim should be in interval [1, 4]");
		static_assert(Size / Dim == 0, "The number of elements of this dimension in the array can not be integer");
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		switch(Dim) {
		case 1:
			FLAPPY_GL_CHECKED(glUniform1fv(id, Size / Dim, value));
			break;
		case 2:
			FLAPPY_GL_CHECKED(glUniform2fv(id, Size / Dim, value));
			break;
		case 3:
			FLAPPY_GL_CHECKED(glUniform3fv(id, Size / Dim, value));
			break;
		case 4:
			FLAPPY_GL_CHECKED(glUniform4fv(id, Size / Dim, value));
			break;
		}
	}
	
	template <int Dim, int Size>
	void raw_set_array(const GLfloat* value, GLsizei count) {
		static_assert(Dim >= 1 && Dim <= 4, "Dim should be in interval [1, 4]");
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		switch(Dim) {
		case 1:
			FLAPPY_GL_CHECKED(glUniform1fv(id, count, value));
			break;
		case 2:
			FLAPPY_GL_CHECKED(glUniform2fv(id, count, value));
			break;
		case 3:
			FLAPPY_GL_CHECKED(glUniform3fv(id, count, value));
			break;
		case 4:
			FLAPPY_GL_CHECKED(glUniform4fv(id, count, value));
			break;
		}
	}
	
	template <int Dim, int Size>
	void raw_set_array(const GLint (&value)[Size]) {
		static_assert(Dim >= 1 && Dim <= 4, "Dim should be in interval [1, 4]");
		static_assert(Size / Dim == 0, "The number of elements of this dimension in the array can not be integer");
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		switch(Dim) {
		case 1:
			FLAPPY_GL_CHECKED(glUniform1iv(id, Size / Dim, value));
			break;
		case 2:
			FLAPPY_GL_CHECKED(glUniform2iv(id, Size / Dim, value));
			break;
		case 3:
			FLAPPY_GL_CHECKED(glUniform3iv(id, Size / Dim, value));
			break;
		case 4:
			FLAPPY_GL_CHECKED(glUniform4iv(id, Size / Dim, value));
			break;
		}
	}
	
	template <int Dim, int Size>
	void raw_set_array(const GLint* value, GLsizei count) {
		static_assert(Dim >= 1 && Dim <= 4, "Dim should be in interval [1, 4]");
		FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized");
		switch(Dim) {
		case 1:
			FLAPPY_GL_CHECKED(glUniform1iv(id, count, value));
			break;
		case 2:
			FLAPPY_GL_CHECKED(glUniform2iv(id, count, value));
			break;
		case 3:
			FLAPPY_GL_CHECKED(glUniform3iv(id, count, value));
			break;
		case 4:
			FLAPPY_GL_CHECKED(glUniform4iv(id, count, value));
			break;
		}
	}
};
}

template <typename T>
class Uniform : public Impl::Uniform {
public:
	using Impl::Uniform::Uniform;
	void set(T);
};

#define FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(TYPE, FUNC, INTERNAL_TYPE) \
	template <> \
	class Uniform<TYPE> : public Impl::Uniform { \
	public: \
		using Impl::Uniform::Uniform; \
		void set(TYPE value) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), 1, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
	}; \
	 \
	template <std::size_t Size> \
	class Uniform<TYPE[Size]> : public Impl::Uniform { \
	public: \
		using Impl::Uniform::Uniform; \
		void set(TYPE (&value)[Size]) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), Size, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
		void set(const std::array<TYPE, Size>& value) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), Size, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
	};
	
#define FLAPPY_UNIFORM_SYNONIM(SYN, NAME) \
	template <> class Uniform<SYN> : public Uniform<NAME> { public: using Uniform<NAME>::Uniform; };

FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(float, glUniform1fv, GLfloat)
FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec2f, glUniform2fv, GLfloat)
FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec3f, glUniform3fv, GLfloat)
FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec4f, glUniform4fv, GLfloat)

FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(GLint, glUniform1iv, GLint)
//FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec2i, glUniform2iv, GLint)
//FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec3i, glUniform3iv, GLint)
//FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION(Vec4i, glUniform4iv, GLint)

FLAPPY_UNIFORM_SYNONIM(Sampler2D, GLint)

#undef FLAPPY_DEFINE_UNIFORM_SPEZIALIZATION
#undef FLAPPY_UNIFORM_SYNONIM

#define FLAPPY_DEFINE_UNIFORM_MATRIX_SPEZIALIZATION(TYPE, FUNC, INTERNAL_TYPE) \
	template <> \
	class Uniform<TYPE> : public Impl::Uniform { \
	public: \
		using Impl::Uniform::Uniform; \
		void set(TYPE value) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), 1, GL_FALSE, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
	}; \
	 \
	template <std::size_t Size> \
	class Uniform<TYPE[Size]> : public Impl::Uniform { \
	public: \
		using Impl::Uniform::Uniform; \
		void set(TYPE (&value)[Size]) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), Size, GL_FALSE, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
		void set(const std::array<TYPE, Size>& value) { \
			FLAPPY_ASSERT(is_ready(), "using uniform that has not been initialized"); \
			FLAPPY_GL_CHECKED(FUNC(get_id(), Size, GL_FALSE, reinterpret_cast<const INTERNAL_TYPE*>(&value))); \
		} \
	};

//FLAPPY_DEFINE_UNIFORM_MATRIX_SPEZIALIZATION(Mat4f, glUniformMatrix4fv, GLfloat)

#undef FLAPPY_DEFINE_UNIFORM_MATRIX_SPEZIALIZATION

}
}
