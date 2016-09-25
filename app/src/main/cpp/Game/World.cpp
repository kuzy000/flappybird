#include "World.h"
#include "Game/Game.h"

#include <random>

namespace Flappy {

void World::new_game() {
	chunks.clear();

	for(std::size_t i = 0; i < chunk_count; i++) {
		chunks.push_back(gen_chunk());
	}
	
	it_chunk_right = chunks.begin();
	it_column_right = it_chunk_right->columns.begin();
	
	chunk_left = nullptr;
	column_left = nullptr;
	
	chunk_right = &(*it_chunk_right);
	column_right = &(*it_column_right);
}

void World::move(float shift) {
	if(chunks.front().shift < Game::screen.left() - chunks.front().get_width()) {
		const auto new_index = it_chunk_right - chunks.begin() - 1;
		
		chunks.pop_front();
		chunks.push_back(gen_chunk());
		
		it_chunk_right = chunks.begin() + new_index;
	}
	
	for(auto& chunk : chunks) {
		chunk.shift -= shift;
	}
}

bool World::is_collide(const Bird& bird) {
	while(chunk_right->shift + column_right->shift < bird.position.x()) {
		it_column_right++;
		
		if(it_column_right == it_chunk_right->columns.end()) {
			it_chunk_right++;
			it_column_right = it_chunk_right->columns.begin();
		}
		
		column_left = column_right;
		chunk_left = chunk_right;
		
		chunk_right = &(*it_chunk_right);
		column_right = &(*it_column_right);
	}
	
	return (column_left  && is_collide(bird, *column_left,  chunk_left ->shift)) ||
	       (column_right && is_collide(bird, *column_right, chunk_right->shift));
}

bool World::is_collide(const Bird& bird, Column column, float chunk_shift) const {
	const float shift = chunk_shift + column.shift;

	const float x = bird.position.x();
	const float y = bird.position.y();
	const float rad = bird.radius;
	
	const float l = shift - column.width / 2;
	const float r = shift + column.width / 2;
	const float t = column.hole_pos + column.hole_size / 2;
	const float b = column.hole_pos - column.hole_size / 2;
	
	if(x + rad >= l && x - rad <= r && (y + rad >= t || y - rad <= b)) {
		if(x >= l && x <= r) return true;
		if(y >= t || y <= b) return true;
		if(Vec2f {x - l, y - b}.get_length_squared() <= rad * rad) return true;
		if(Vec2f {x - r, y - b}.get_length_squared() <= rad * rad) return true;
		if(Vec2f {x - l, y - t}.get_length_squared() <= rad * rad) return true;
		if(Vec2f {x - r, y - t}.get_length_squared() <= rad * rad) return true;
	}
	
	return false;
}

Chunk World::gen_chunk() const {
	std::uniform_real_distribution<float> range {-0.5f, 0.5f};
	std::mt19937 random_engine {std::random_device {}()};
	
	const auto random = [&random_engine, &range] { return range(random_engine); };

	Chunk chunk;
	chunk.shift = chunks.empty() ? 2.0f : chunks.back().shift + chunks.back().get_width();
	float shift = 0.0f;
	
	float last_hole_pos = chunks.empty() ? 0.0f : std::prev(chunks.back().columns.end())->hole_pos;
	while(!chunk.is_complete()) {
		shift += 0.7f;
		Column column {shift, std::min(0.7f, std::max(-0.7f, last_hole_pos + random()))};
		chunk.place(column);
		shift += column.width;
	}
	
	return chunk;
}

}
