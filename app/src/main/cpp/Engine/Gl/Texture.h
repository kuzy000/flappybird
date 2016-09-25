#pragma once

#include "Engine/Gl/Object.h"
#include "Engine/Math/Size2.h"

namespace Flappy {
namespace Gl {

class Texture : public GLuintId {
public:
	enum class Target : GLenum {
		_2d = GL_TEXTURE_2D,
		CubeMap = GL_TEXTURE_CUBE_MAP,
	};
	
	enum class Side : GLenum {
		S = GL_TEXTURE_WRAP_S,
		T = GL_TEXTURE_WRAP_T,
//		R = GL_TEXTURE_WRAP_R
	};
	
	enum class Wrap : GLint {
		ClampToEdge = GL_CLAMP_TO_EDGE,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT,
	};
	
	enum class Target2d : GLenum {
		_2d = GL_TEXTURE_2D,
		CubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		CubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		CubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		CubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		CubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		CubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};
	
	
	Texture();
	~Texture();
	
	Texture(const Texture&) = delete;
	Texture& operator =(const Texture&) = delete;
	
	Texture(Texture&& that);
	Texture& operator =(Texture&& rhs);
	
	void bind(Target target);
	
	static std::uint_fast8_t get_units_count();
	static void activate(std::uint_fast8_t unit);
	static void set_wrap(Target target, Side side, Wrap wrap);
	static void gen_mipmap(Target target);
	static void set_image2d(Target2d target, GLint level, GLint internal_format, Size2<GLsizei> size, GLenum format, GLenum type, const GLvoid* data);
};

}
}
