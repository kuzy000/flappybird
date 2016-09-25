#include "Bird.h"

namespace Flappy {

void Bird::new_game() {
	position.y() = 0.0f;
}

void Bird::update(float dt) {
	velocity -= 9.0f * dt;
	position.y() += velocity * dt;
}

void Bird::jump() {
	velocity = 2.0f;
}

}
