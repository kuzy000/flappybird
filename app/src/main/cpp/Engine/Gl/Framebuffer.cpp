#include "Framebuffer.h"

#include "Engine/Gl/Error.h"
#include "Engine/Util/Assert.h"

namespace Flappy {
namespace Gl {

Framebuffer::Framebuffer() {
	FLAPPY_GL_CHECKED(glGenFramebuffers(1, &id));
}

Framebuffer::~Framebuffer() {
	FLAPPY_GL_CHECKED(glDeleteFramebuffers(1, &id));
}

Framebuffer::Framebuffer(Framebuffer&& that)
	: GLuintId {that.id} {
	FLAPPY_ASSERT(is_ready(), "moving framebuffer that has been moved");
	
	that.not_ready();
}

Framebuffer& Framebuffer::operator =(Framebuffer&& rhs) {
	FLAPPY_ASSERT(is_ready(), "moving framebuffer that has been moved");
	
	id = rhs.id;
	rhs.not_ready();
	
	return *this;
}

void Framebuffer::bind(Target target) {
	FLAPPY_ASSERT(is_ready(), "using framebuffer that has been moved");
	FLAPPY_GL_CHECKED(glBindFramebuffer(static_cast<GLenum>(target), id));
}

void Framebuffer::unbind(Target target) {
	FLAPPY_GL_CHECKED(glBindFramebuffer(static_cast<GLenum>(target), 0));
}

}
}
