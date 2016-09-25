#pragma once

#include "Defs.h"

#if TARGET_PLATFORM == PLATFORM_LINUX
	#include "Nonportable/ContextGlfw.h"
#elif TARGET_PLATFORM == PLATFORM_ANDROID
	#include "Nonportable/ContextAndroid.h"
#endif

// Context interface
/*
namespace Flappy {
namespace Platform {

class Context : public Utils::MoveOnly {
	Context(Context&& that);
	Context& operator =(Context&& rhs);
	
	void init();
	
	void pre_render();
	void post_render();
	
	bool is_end() const;
	void end();
	
	void input_poll_events();
	
	Size2u get_window_size() const;
	
	void set_window_size_callback(std::function<void(Size2u)> callback);
	void set_tap_down_callback(std::function<void(Vec2u)> callback);
	void set_initialized_callback(std::function<void()> callback);
}

}
}
*/
