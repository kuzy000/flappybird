#include "ContextAndroid.h"

#include <cstdlib>

#include <android/sensor.h>
#include <android/log.h>

namespace Flappy {
namespace Platform {

Context::Context(android_app* application)
	: application {[application] {
		if(application == nullptr) {
			Log::error("android_app is nullptr\n");
			std::exit(EXIT_FAILURE);
		}
		return application;
	} ()} {
	app_dummy();

	application->userData = this;
	application->onAppCmd = [] (android_app* application, int32_t command) {
		Context* self = static_cast<Context*>(application->userData);
		self->on_app_event(command);
	};
	application->onInputEvent = [] (android_app* application, AInputEvent* event) {
		Context* self = static_cast<Context*>(application->userData);
		return self->on_input_event(event);
	};
}

Context::~Context() {
	if(display != EGL_NO_DISPLAY) {
		eglMakeCurrent(EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(context != EGL_NO_CONTEXT) {
			eglDestroyContext(display, context);
		}
		if (surface != EGL_NO_SURFACE) {
			eglDestroySurface(display, surface);
		}
		eglTerminate(display);
	}
}

Context::Context(Context&& that)
	: application {that.application}
	, display {that.display}
	, context {that.context}
	, surface {that.surface}
	, width {that.width}
	, height {that.height}
	, window_initialized {that.window_initialized}
	, should_end {that.should_end}
	, window_size_callback {that.window_size_callback}
	, tap_down_callback {that.tap_down_callback}
	, initialized_callback {that.initialized_callback} {
	
	that.application = nullptr;

	window_initialized = false;

	window_size_callback = nullptr;
	tap_down_callback = nullptr;
	initialized_callback = nullptr;

	FLAPPY_ASSERT(application != nullptr, "moving Android context object that has been moved (or has not been initialized)");
}

Context& Context::operator =(Context&& rhs) {
	application = rhs.application;
	display = rhs.display;
	context = rhs.context;
	surface = rhs.surface;

	rhs.application = nullptr;

	FLAPPY_ASSERT(application != nullptr, "moving Android context object that has been moved (or has not been initialized)");
	return *this;
}

void Context::init() {
	while(!window_initialized) {
		input_poll_events();
	}
}

void Context::pre_render() {
}

void Context::post_render() {
	EGLBoolean result = eglSwapBuffers(display, surface);
	FLAPPY_ASSERT(result == EGL_TRUE, "Unable to eglSwapBuffers");
}

bool Context::is_end() const {
	return should_end;
}

void Context::end() {
	should_end = true;
}

void Context::input_poll_events() {
	int32_t result;
	int32_t events;
	android_poll_source* source;

	while((result = ALooper_pollAll(0, nullptr, &events, (void**) &source)) >= 0) {
		if (source != nullptr) {
			source->process(application, source);
		}

		if(application->destroyRequested) {
			end();
		}
	}
}

Size2u Context::get_window_size() const {
	return static_cast<Size2u>(Size2<EGLint> {width, height});
}

void Context::on_app_event(int32_t command) {
	switch(command) {
	case APP_CMD_CONFIG_CHANGED:
		break;
	case APP_CMD_INIT_WINDOW:
		window_initialized = true;
		init_gl();
		if(initialized_callback) {
			initialized_callback();
		}
		break;
	case APP_CMD_DESTROY:
		end();
		break;
	case APP_CMD_GAINED_FOCUS:
		break;
	case APP_CMD_LOST_FOCUS:
		break;
	case APP_CMD_SAVE_STATE:
		break;
	case APP_CMD_PAUSE:
		break;
	case APP_CMD_RESUME:
		break;
	case APP_CMD_START:
		break;
	case APP_CMD_STOP:
		break;
	case APP_CMD_TERM_WINDOW:
		end();
		break;
	case APP_CMD_LOW_MEMORY:
		break;
	}
}

int32_t Context::on_input_event(AInputEvent* event) {
	int32_t event_type = AInputEvent_getType(event);

	switch(event_type) {
	case AINPUT_EVENT_TYPE_MOTION:
		switch (AInputEvent_getSource(event)) {
		case AINPUT_SOURCE_TOUCHSCREEN:
			return on_touch_event(event);
			break;
		}
		break;
	case AINPUT_EVENT_TYPE_KEY:
		break;
	}

	return 0;
}

int32_t Context::on_touch_event(AInputEvent* event) {
	int32_t action = AMotionEvent_getAction(event);
	int index = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

	float x = AMotionEvent_getX(event, index);
	float y = AMotionEvent_getY(event, index);

	switch(action) {
	case AMOTION_EVENT_ACTION_DOWN:
		if(tap_down_callback) {
			tap_down_callback(static_cast<Size2u>(Size2f {x, y}));
		}
		break;
	case AMOTION_EVENT_ACTION_UP:
		break;
	case AMOTION_EVENT_ACTION_MOVE:
		break;
	case AMOTION_EVENT_ACTION_CANCEL:
		break;
	case AMOTION_EVENT_ACTION_OUTSIDE:
		break;
	}

	return true;
}

void Context::init_gl() {
	EGLint format;
	EGLint num_configs;
	EGLConfig config;

	const EGLint* config_attrs;

	const EGLint config_attrs_RGBX_8888[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};

	const EGLint config_attrs_RGB_565[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 5,
		EGL_GREEN_SIZE, 6,
		EGL_BLUE_SIZE, 5,
		EGL_DEPTH_SIZE, 8,
		EGL_NONE
	};

	const EGLint context_attrs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(display == EGL_NO_DISPLAY) {
		Log::error("Unable to eglGetDisplay\n");
	}
	if(eglInitialize(display, nullptr, nullptr) == EGL_FALSE) {
		Log::error("Unable to eglInitialize\n");
	}

	int32_t window_format = ANativeWindow_getFormat(application->window);
	if(window_format == WINDOW_FORMAT_RGBA_8888 || window_format == WINDOW_FORMAT_RGBX_8888) {
		config_attrs = config_attrs_RGBX_8888;
	}
	else {
		config_attrs = config_attrs_RGB_565;
	}

	if(eglChooseConfig(display, config_attrs, &config, 1, &num_configs) == EGL_FALSE) {
		Log::error("Unable to eglChooseConfig\n");
	}

	if(eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format) == EGL_FALSE) {
		Log::error("Unable to eglGetConfigAttrib\n");
	}

	if(ANativeWindow_setBuffersGeometry(application->window, 0, 0, format) != 0) {
		Log::error("Unable to ANativeWindow_setBuffersGeometry");
	}

	surface = eglCreateWindowSurface(display, config, application->window, nullptr);
	if(surface == EGL_NO_SURFACE) {
		EGLint error = eglGetError();
		switch(error) {
		case EGL_BAD_DISPLAY:
			Log::error("eglCreateWindowSurface -> EGL_BAD_DISPLAY\n");
			break;
		case EGL_NOT_INITIALIZED:
			Log::error("eglCreateWindowSurface -> EGL_NOT_INITIALIZED\n");
			break;
		case EGL_BAD_CONFIG:
			Log::error("eglCreateWindowSurface -> EGL_BAD_CONFIG\n");
			break;
		case EGL_BAD_NATIVE_WINDOW:
			Log::error("eglCreateWindowSurface -> EGL_BAD_NATIVE_WINDOW\n");
			break;
		case EGL_BAD_ATTRIBUTE:
			Log::error("eglCreateWindowSurface -> EGL_BAD_ATTRIBUTE\n");
			break;
		case EGL_BAD_ALLOC:
			Log::error("eglCreateWindowSurface -> EGL_BAD_ALLOC\n");
			break;
		case EGL_BAD_MATCH :
			Log::error("eglCreateWindowSurface -> EGL_BAD_MATCH\n");
			break;
		default:
			Log::error("eglCreateWindowSurface -> ?\n");
			break;
		}
	}

	context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attrs);
	if(context == EGL_NO_CONTEXT) {
		Log::error("Unable to eglCreateContext\n");
	}

	if(!eglMakeCurrent(display, surface, surface, context)
	   || !eglQuerySurface(display, surface, EGL_WIDTH, &width)
	   || !eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
		Log::error("Unable to eglMakeCurrent\n");
	}

	glViewport(0, 0, width, height);

	Log::info("OpenGL\n");
	Log::info.indent("Version:  ", glGetString(GL_VERSION),  "\n");
	Log::info.indent("Renderer: ", glGetString(GL_RENDERER), "\n");
	Log::info.indent("Vendor:   ", glGetString(GL_VENDOR),   "\n");
	Log::info.indent("Viewport: ", width, "x", height,       "\n");
}

}
}

