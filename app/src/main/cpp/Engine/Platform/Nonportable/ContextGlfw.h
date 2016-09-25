#pragma once

#include "Engine/Util/MoveOnly.h"
#include "Engine/Math/Size2.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <functional>

namespace Flappy {
namespace Platform {

class Context : public Util::MoveOnly {
public:
	Context();
	~Context();
	
	Context(Context&& that);
	Context& operator =(Context&& rhs);
	
	void init();
	
	void pre_render();
	void post_render();
	
	bool is_end() const;
	void end();
	
	void input_poll_events();
	
	Size2u get_window_size() const;
	void set_window_size_callback(std::function<void(Size2u)> callback) {
		window_size_callback = callback;
	}
	
	void set_tap_down_callback(std::function<void(Vec2u)> callback) {
		tap_down_callback = callback;
	}
	
	void set_initialized_callback(std::function<void()> callback) {
		initialized_callback = callback;
	}
	
private:
	bool initialized = false;
	GLFWwindow* window = nullptr;
	GLFWmonitor* monitor = nullptr;
	
	std::function<void(Size2u)> window_size_callback;
	std::function<void(Vec2u)> tap_down_callback;
	std::function<void()> initialized_callback;
};

}
}
