#include "Buffer.h"

#include "Engine/Util/Assert.h"

namespace Flappy {
namespace Gl {

Buffer::Buffer() {
	FLAPPY_GL_CHECKED(glGenBuffers(1, &id));
}

Buffer::~Buffer() {
	FLAPPY_GL_CHECKED(glDeleteBuffers(1, &id));
}

Buffer::Buffer(Buffer&& that)
	: GLuintId {that.id} {
	FLAPPY_ASSERT(is_ready(), "moving buffer that has been moved");
	
	that.not_ready();
}

Buffer& Buffer::operator =(Buffer&& rhs) {
	FLAPPY_ASSERT(is_ready(), "moving buffer that has been moved");
	
	id = rhs.id;
	rhs.not_ready();
	
	return *this;
}

void Buffer::bind(Target target) {
	FLAPPY_ASSERT(is_ready(), "using buffer that has been moved");
	FLAPPY_GL_CHECKED(glBindBuffer(static_cast<GLenum>(target), id));
}

void Buffer::unbind(Target target) {
	FLAPPY_GL_CHECKED(glBindBuffer(static_cast<GLenum>(target), 0));
}

}
}
