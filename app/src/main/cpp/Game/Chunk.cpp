#include "Chunk.h"

#include "Engine/Util/Assert.h"

namespace Flappy {

void Chunk::place(Column column) {
	const auto placed = columns.size();
	
	FLAPPY_ASSERT(placed < chunk_size, "too many columns have been placed in a chunk");
	
	const float top_h = (1.0f - column.hole_pos - column.hole_size / 2.0f) / column.width;
	// top
	vertices[placed * 8 + 0] = {{ column.width / 2 + column.shift,  1.0f},                                   { 1.0f, 1.0f - top_h * 2.0f}};
	vertices[placed * 8 + 1] = {{ column.width / 2 + column.shift,  column.hole_pos + column.hole_size / 2}, { 1.0f, 1.0f}};
	vertices[placed * 8 + 2] = {{-column.width / 2 + column.shift,  column.hole_pos + column.hole_size / 2}, {-1.0f, 1.0f}};
	vertices[placed * 8 + 3] = {{-column.width / 2 + column.shift,  1.0f},                                   {-1.0f, 1.0f - top_h * 2.0f}};
	
	const float bot_h = (1.0f + column.hole_pos - column.hole_size / 2.0f) / column.width;
	// bottom
	vertices[placed * 8 + 4] = {{ column.width / 2 + column.shift,  column.hole_pos - column.hole_size / 2}, { 1.0f, 1.0f}};
	vertices[placed * 8 + 5] = {{ column.width / 2 + column.shift, -1.0f},                                   { 1.0f, 1.0f - bot_h * 2.0f}};
	vertices[placed * 8 + 6] = {{-column.width / 2 + column.shift, -1.0f},                                   {-1.0f, 1.0f - bot_h * 2.0f}};
	vertices[placed * 8 + 7] = {{-column.width / 2 + column.shift,  column.hole_pos - column.hole_size / 2}, {-1.0f, 1.0f}};
	
	indecies[placed * 12 + 0 ] = 0 + placed * 8;
	indecies[placed * 12 + 1 ] = 1 + placed * 8;
	indecies[placed * 12 + 2 ] = 3 + placed * 8;
	indecies[placed * 12 + 3 ] = 1 + placed * 8;
	indecies[placed * 12 + 4 ] = 2 + placed * 8;
	indecies[placed * 12 + 5 ] = 3 + placed * 8;
	
	indecies[placed * 12 + 6 ] = 4 + placed * 8;
	indecies[placed * 12 + 7 ] = 5 + placed * 8;
	indecies[placed * 12 + 8 ] = 7 + placed * 8;
	indecies[placed * 12 + 9 ] = 5 + placed * 8;
	indecies[placed * 12 + 10] = 6 + placed * 8;
	indecies[placed * 12 + 11] = 7 + placed * 8;
	
	columns.push_back(column);
	
	if(is_complete()) {
		vbo.bind(Gl::Buffer::Target::Array);
		Gl::Buffer::data(Gl::Buffer::Target::Array, Gl::Buffer::Usage::StaticDraw, vertices);
		
		ibo.bind(Gl::Buffer::Target::ElementArray);
		Gl::Buffer::data(Gl::Buffer::Target::ElementArray, Gl::Buffer::Usage::StaticDraw, indecies);
	}
}

bool Chunk::is_complete() const {
	return columns.size() == chunk_size;
}

float Chunk::get_width() const {
	if(columns.rbegin() != columns.rend()) {
		return columns.rbegin()->shift + columns.rbegin()->width;
	}
	else {
		return 0.0f;
	}
}

}
