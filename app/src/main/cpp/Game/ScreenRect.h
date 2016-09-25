#pragma once

#include "Engine/Math/Vec2.h"
#include "Engine/Gl/Buffer.h"

namespace Flappy {

class ScreenRect {
public:
	struct Vertex {
		Vec2f position;
	};
	
	static constexpr std::size_t indecies_count = 6;
	
	ScreenRect();
	
	Gl::Buffer vbo;
	Gl::Buffer ibo;
};

}
