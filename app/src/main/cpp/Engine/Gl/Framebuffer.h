#pragma once

#include "Engine/Gl/Object.h"

namespace Flappy {
namespace Gl {

class Framebuffer : GLuintId {
public:
	enum class Target : GLenum {
//		Draw = GL_DRAW_FRAMEBUFFER,
//		Read = GL_READ_FRAMEBUFFER,
		ReadDraw = GL_FRAMEBUFFER
	};
	
	Framebuffer();
	~Framebuffer();
	
	Framebuffer(const Framebuffer&) = delete;
	Framebuffer& operator =(const Framebuffer&) = delete;
	
	Framebuffer(Framebuffer&& that);
	Framebuffer& operator =(Framebuffer&& rhs);
	
	void bind(Target target);
	static void unbind(Target target);
};

}
}
