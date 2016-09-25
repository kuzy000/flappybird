#pragma once

#include "Engine/Platform/Gl.h"
#include "Engine/Math/Vec2.h"

#include <vector>
#include <array>

namespace Flappy {

class Column {
public:
	Column(float shift, float hole_pos) 
		: shift {shift} 
		, hole_pos {hole_pos}
		{}

	const float shift;
	const float hole_pos;
	static const float hole_size;
	static const float width;
};

}
