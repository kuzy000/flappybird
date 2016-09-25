#include "Texture.h"

#include "Engine/Gl/Error.h"
#include "Engine/Util/Assert.h"

namespace Flappy {
namespace Gl {

Texture::Texture() {
	FLAPPY_GL_CHECKED(glGenTextures(1, &id));
}

Texture::~Texture() {
	FLAPPY_GL_CHECKED(glDeleteTextures(1, &id));
}

Texture::Texture(Texture&& that)
	: GLuintId {that.id} {
	FLAPPY_ASSERT(is_ready(), "moving texture that has been moved");
	
	that.not_ready();
}

Texture& Texture::operator =(Texture&& rhs) {
	FLAPPY_ASSERT(is_ready(), "moving texture that has been moved");
	
	id = rhs.id;
	rhs.not_ready();
	
	return *this;
}

void Texture::bind(Target target) {
	FLAPPY_ASSERT(is_ready(), "using texture that has been moved");
	
	FLAPPY_GL_CHECKED(glBindTexture(static_cast<GLenum>(target), this->id));
}

std::uint_fast8_t Texture::get_units_count() {
	GLint result;
	FLAPPY_GL_CHECKED(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &result));
	return static_cast<std::uint_fast8_t>(result);
}

void Texture::activate(std::uint_fast8_t unit) {
	FLAPPY_ASSERT(unit < get_units_count(), "unit number is too big");
	FLAPPY_GL_CHECKED(glActiveTexture(GL_TEXTURE0 + unit));
}

void Texture::set_wrap(Target target, Side side, Wrap wrap) {
	FLAPPY_GL_CHECKED(glTexParameteri(static_cast<GLenum>(target), static_cast<GLenum>(side), static_cast<GLint>(wrap)));
}

void Texture::gen_mipmap(Target target) {
	FLAPPY_GL_CHECKED(glGenerateMipmap(static_cast<GLenum>(target)));
}

void Texture::set_image2d(Target2d target, GLint level, GLint internal_format, Size2<GLsizei> size, GLenum format, GLenum type, const GLvoid* data) {
	FLAPPY_GL_CHECKED(glTexImage2D(static_cast<GLenum>(target), level, internal_format, size.width(), size.height(), 0, static_cast<GLenum>(format), type, data));
}
	

}
}

