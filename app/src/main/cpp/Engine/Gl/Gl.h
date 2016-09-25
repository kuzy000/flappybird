#pragma once

#include "Engine/Math/Vec2.h"
#include "Engine/Math/AABB2.h"
#include "Engine/Math/RGBA.h"
#include "Engine/Gl/Error.h"

namespace Flappy {
namespace Gl {

inline void clear(GLbitfield mask) {
	FLAPPY_GL_CHECKED(glClear(mask));
}

inline void set_clear_color(const RGBAf& color) {
	FLAPPY_GL_CHECKED(glClearColor(color.r(), color.g(), color.b(), color.a()));
}

inline void set_viewport(AABB2<GLint> aabb) {
	FLAPPY_GL_CHECKED(glViewport(aabb.start.x(), aabb.start.y(), aabb.width(), aabb.height()));
}

}
}
