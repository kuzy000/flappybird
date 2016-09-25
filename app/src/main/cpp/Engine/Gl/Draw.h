#pragma once

#include "Engine/Gl/Error.h"

namespace Flappy {
namespace Gl {

enum class DrawMode : GLenum { 
	Points = GL_POINTS, 
	LineStrip = GL_LINE_STRIP, 
	LineLoop = GL_LINE_LOOP, 
	Lines = GL_LINES, 
	TriangleStrip = GL_TRIANGLE_STRIP, 
	TriangleFan = GL_TRIANGLE_FAN, 
	Triangles = GL_TRIANGLES 
};

enum class ElementsType : GLenum {
	UByte = GL_UNSIGNED_BYTE, 
	UShort = GL_UNSIGNED_SHORT,
	UInt = GL_UNSIGNED_INT
};

inline void draw_arrays(DrawMode mode, GLint first, GLsizei count) {
	FLAPPY_GL_CHECKED(glDrawArrays(static_cast<GLenum>(mode), first, count));
}

inline void draw_elements(DrawMode mode, GLsizei count, ElementsType elements_type) {
	FLAPPY_GL_CHECKED(glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(elements_type), 0));
}

inline void draw_elements_data(DrawMode mode, GLsizei count, ElementsType elements_type, const void* indices) {
	FLAPPY_GL_CHECKED(glDrawElements(static_cast<GLenum>(mode), count, static_cast<GLenum>(elements_type), indices));
}

}
}
