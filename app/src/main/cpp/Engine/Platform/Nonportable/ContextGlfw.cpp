#include "ContextGlfw.h"

#include "Engine/Util/Assert.h"

#include "Engine/Log.h"

namespace Flappy {
namespace Platform {

Context::Context() {
}

void Context::init() {
	if(!glfwInit()) {
		Log::error("Fatal error - can't initialize glfw");
		exit(EXIT_FAILURE);
	}
	initialized = true;
	
	window = glfwCreateWindow(1024, 768, "Flappy Bird", monitor, nullptr);
	if(!window) {
		Log::error("Fatal error - can't create glfw window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	
	glewExperimental = true;
	glewInit();
	
	glfwSetWindowUserPointer(window, this);
	
	glfwSetWindowSizeCallback(window, [] (GLFWwindow* window, int width, int height) {
		Context* self = static_cast<Context*>(glfwGetWindowUserPointer(window));
		if(self->window_size_callback) {
			self->window_size_callback({static_cast<std::uint_fast32_t>(width), static_cast<uint_fast32_t>(height)});
		}
	});
	
	glfwSetMouseButtonCallback(window, [] (GLFWwindow* window, int button, int action, int mods) {
		if(action != GLFW_PRESS) {
			return;
		}
	
		Context* self = static_cast<Context*>(glfwGetWindowUserPointer(window));
		if(self->tap_down_callback) {
			double x, y;
			glfwGetCursorPos(window, &x, &y);
			self->tap_down_callback(static_cast<Vec2u>(Vec2d {x, y}));
		}
	});
	
	if(initialized_callback) {
		initialized_callback();
	}
}

Context::~Context() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

Context::Context(Context&& that)
	: initialized {that.initialized}
	, window {that.window}
	, monitor {that.monitor}
	, window_size_callback {std::move(that.window_size_callback)}
	, tap_down_callback {std::move(that.tap_down_callback)}
	, initialized_callback {std::move(that.initialized_callback)} {
	
	that.initialized = false;
	that.window = nullptr;
	that.monitor = nullptr;
	
	that.window_size_callback = nullptr;
	that.tap_down_callback = nullptr;
	that.initialized_callback = nullptr;
	
	FLAPPY_ASSERT(initialized, "moving GLFW context object that has been moved (or has not been initialized)");
}

Context& Context::operator =(Context&& rhs) {
	initialized = rhs.initialized;
	window = rhs.window;
	monitor = rhs.monitor;
	
	window_size_callback = rhs.window_size_callback;
	tap_down_callback = rhs.tap_down_callback;
	initialized_callback = rhs.initialized_callback;
	
	rhs.initialized = false;
	rhs.window = nullptr;
	rhs.monitor = nullptr;
	
	rhs.window_size_callback = nullptr;
	rhs.tap_down_callback = nullptr;
	rhs.initialized_callback = nullptr;
	
	FLAPPY_ASSERT(initialized, "moving GLFW context object that has been moved (or has not been initialized)");
	return *this;
}

void Context::pre_render() {
	
}

void Context::post_render() {
	glfwSwapBuffers(window);
}

bool Context::is_end() const {
	return !initialized || window == nullptr || glfwWindowShouldClose(window);
}

void Context::end() {
	glfwWindowShouldClose(window);
}

void Context::input_poll_events() {
	glfwPollEvents();
}

Size2u Context::get_window_size() const {
	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	return {static_cast<std::uint_fast32_t>(width), static_cast<std::uint_fast32_t>(height)};
}

}
}

