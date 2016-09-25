#pragma once

#include "Engine/Gl/Buffer.h"

#include "Game/Column.h"

#include <array>
#include <vector>

namespace Flappy {

class Chunk {
public:
	using Columns = std::vector<Column>;
	struct Vertex {
		Vec2f position;
		Vec2f local;
	};
	
	static constexpr std::size_t chunk_size = 16;
	static constexpr std::size_t vertices_count = chunk_size * 4 * 2;
	static constexpr std::size_t indecies_count = chunk_size * 6 * 2;
	
	void place(Column column);
	bool is_complete() const;
	float get_width() const;
	
	float shift = 0;
	Columns columns;
	
	Gl::Buffer vbo;
	Gl::Buffer ibo;
	
private:
	std::array<Vertex, vertices_count> vertices;
	std::array<GLuint, indecies_count> indecies;
};

}
