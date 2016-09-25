#pragma once

#include "Game/Chunk.h"
#include "Game/Bird.h"

#include <deque>

namespace Flappy {

class World {
public:
	using Chunks = std::deque<Chunk>;
	static constexpr std::size_t chunk_count = 3;
	
	Chunks chunks;
	
	void new_game();
	
	void move(float shift);
	bool is_collide(const Bird& bird);
	
private:
	Chunk gen_chunk() const;
	bool is_collide(const Bird& bird, Column column, float chunk_shift) const;
	
	const Chunk* chunk_left = nullptr;
	const Chunk* chunk_right = nullptr;
	
	const Column* column_left = nullptr;
	const Column* column_right = nullptr;
	
	Chunks::const_iterator         it_chunk_right;
	Chunk::Columns::const_iterator it_column_right;
};

}
