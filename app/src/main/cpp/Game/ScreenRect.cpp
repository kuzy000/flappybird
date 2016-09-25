#include "ScreenRect.h"

namespace Flappy {

ScreenRect::ScreenRect() {
	const std::array<Vertex, 4> vertices {{
		{{ 1.0f,  1.0f}},
		{{ 1.0f, -1.0f}},
		{{-1.0f, -1.0f}},
		{{-1.0f,  1.0f}},
	}};
	
	const std::vector<GLuint> indecies {{
		0, 1, 3,
		1, 2, 3
	}};

	vbo.bind(Gl::Buffer::Target::Array);
	Gl::Buffer::data(Gl::Buffer::Target::Array, Gl::Buffer::Usage::StaticDraw, vertices);

	ibo.bind(Gl::Buffer::Target::ElementArray);
	Gl::Buffer::data(Gl::Buffer::Target::ElementArray, Gl::Buffer::Usage::StaticDraw, indecies);
}

}
