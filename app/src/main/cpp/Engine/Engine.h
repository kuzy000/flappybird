#pragma once

#include "Math/Size2.h"
#include "Util/MoveOnly.h"
#include "Util/MoveOnly.h"
#include "Util/Event.h"
#include "Platform/Context.h"

#include <chrono>

namespace Flappy {

class Engine : public Util::MoveOnly {
public:
	Engine(Platform::Context& context);
	
	void run();
	
	Size2u get_window_size() const {
		return context.get_window_size();
	};
	
	Event<void(), Engine> on_start;
	Event<void(float), Engine> on_update;
	Event<void(float), Engine> on_render;
	
	Event<void(Size2u), Engine> on_window_size;
	
	Event<void(Vec2u), Engine> on_tap_down;
private:
	void loop();
	void start();
	void update(float dt);
	void render(float dt);
	
	Platform::Context& context;
	
	std::chrono::time_point<std::chrono::high_resolution_clock> last_time;
};

}
