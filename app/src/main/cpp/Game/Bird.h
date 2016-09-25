#pragma once 

#include "Engine/Math/Vec2.h"

namespace Flappy {

class Bird {
public:
	void new_game();
	
	void update(float dt);
	void jump();
	
	const float radius = 0.06f;
	Vec2f position {-1.0f, 0.0f}; // x is a const
	float velocity = 0.0f;
};

}
