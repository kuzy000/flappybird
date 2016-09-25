#pragma once

#include "Engine/Util/MoveOnly.h"
#include "Engine/Math/Size2.h"

#include <jni.h>
#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android_native_app_glue.h>

#include <functional>

namespace Flappy {
namespace Platform {

class Context : public Util::MoveOnly {
public:
	Context(android_app* application);
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
	void on_app_event(int32_t command);
	int32_t on_input_event(AInputEvent* event);
	int32_t on_touch_event(AInputEvent* event);
	void init_gl();

	android_app* application;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;

	EGLint width;
	EGLint height;

	bool window_initialized = false;
	bool should_end = false;

	std::function<void(Size2u)> window_size_callback;
	std::function<void(Vec2u)> tap_down_callback;
	std::function<void()> initialized_callback;
};

}
}
