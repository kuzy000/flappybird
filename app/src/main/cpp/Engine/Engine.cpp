#include "Engine.h"

namespace Flappy {

Engine::Engine(Platform::Context& context)
	: context {context} {
	context.set_window_size_callback([this] (Size2u size) {
		this->on_window_size.call(size);
	});
	
	context.set_tap_down_callback([this] (Vec2u position) {
		this->on_tap_down.call(position);
	});
	
	context.set_initialized_callback([this] () {
		this->loop();
	});
}

void Engine::run() {
	context.init();
}

void Engine::loop() {
	using Time = std::chrono::high_resolution_clock;
	using Secf = std::chrono::duration<float>;
	
	last_time = Time::now();
	
	start();
	while(!context.is_end()) {
		auto current_time = Time::now();
		
		Secf dt = current_time - last_time;
		update(dt.count());
		render(dt.count());
		
		last_time = current_time;
	}
}

void Engine::start() {
	on_start.call();
	on_start.clear();
}

void Engine::update(float dt) {
	context.input_poll_events();
	on_update.call(dt);
}

void Engine::render(float dt) {
	context.pre_render();
	on_render.call(dt);
	context.post_render();
}

}
