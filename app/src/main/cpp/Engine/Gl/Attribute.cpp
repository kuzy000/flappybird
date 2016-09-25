#include "Attribute.h"

namespace Flappy {
namespace Gl {
namespace Impl {

Attribute::Attribute(GLuint id)
	: GLintId {static_cast<GLint>(id)} {}

void Attribute::enable() {
	FLAPPY_GL_CHECKED(glEnableVertexAttribArray(id));
}

void Attribute::disable() {
	FLAPPY_GL_CHECKED(glDisableVertexAttribArray(id));
}

}
}
}
