#pragma once

#include <utility>

#include "Engine/Gl/Object.h"
#include "Engine/Gl/Error.h"
#include "Engine/Util/Assert.h"
#include "Engine/Math/Vec2.h"
#include "Engine/Math/Vec3.h"
#include "Engine/Math/Vec4.h"

namespace Flappy {
namespace Gl {

namespace Impl {
class Attribute : public GLintId {
public:
	Attribute() = default;
	Attribute(GLuint id);
	
	void enable();
	void disable();
};
}

template <typename T>
class Attribute : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE);
};

template <std::size_t Size>
class Attribute<GLbyte[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_BYTE, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

template <std::size_t Size>
class Attribute<GLubyte[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_UNSIGNED_BYTE, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

template <std::size_t Size>
class Attribute<GLshort[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_SHORT, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

template <std::size_t Size>
class Attribute<GLushort[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_UNSIGNED_SHORT, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

template <std::size_t Size>
class Attribute<GLfixed[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_FIXED, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

template <std::size_t Size>
class Attribute<GLfloat[Size]> : public Impl::Attribute {
public:
	using Impl::Attribute::Attribute;
	void bind(std::size_t stride, std::size_t offset, GLboolean normalized = GL_FALSE) {
		FLAPPY_ASSERT(is_ready(), "using attribute that has not been initialized");
		FLAPPY_GL_CHECKED(glVertexAttribPointer(get_id(), Size, GL_FLOAT, normalized, stride, reinterpret_cast<void*>(offset)));
	}
};

#define FLAPPY_ATTRIBUTE_SYNONIM(SYN, NAME) \
	template <> class Attribute<SYN> : public Attribute<NAME> { public: using Attribute<NAME>::Attribute; };

FLAPPY_ATTRIBUTE_SYNONIM(GLbyte, GLbyte[1])
FLAPPY_ATTRIBUTE_SYNONIM(GLubyte, GLubyte[1])
FLAPPY_ATTRIBUTE_SYNONIM(GLshort, GLshort[1])
FLAPPY_ATTRIBUTE_SYNONIM(GLushort, GLushort[1])
FLAPPY_ATTRIBUTE_SYNONIM(GLfixed, GLfixed[1])
FLAPPY_ATTRIBUTE_SYNONIM(GLfloat, GLfloat[1])

FLAPPY_ATTRIBUTE_SYNONIM(Vec2f, GLfloat[2])
FLAPPY_ATTRIBUTE_SYNONIM(Vec3f, GLfloat[3])
FLAPPY_ATTRIBUTE_SYNONIM(Vec4f, GLfloat[4])

//FLAPPY_ATTRIBUTE_SYNONIM(Vec2i, GLint[2])
//FLAPPY_ATTRIBUTE_SYNONIM(Vec3i, GLint[3])
//FLAPPY_ATTRIBUTE_SYNONIM(Vec4i, GLint[4])

#undef FLAPPY_ATTRIBUTE_SYNONIM

}
}
