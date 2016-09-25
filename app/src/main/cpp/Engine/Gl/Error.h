#pragma once

#include <vector>
#include <type_traits>

#include "Engine/Platform/Gl.h"
#include "Engine/Debug.h"
#include "Engine/Log.h"

namespace Flappy {
namespace Gl {

enum class Error : GLenum {
	InvalidEnum = GL_INVALID_ENUM,
	InvalidFramebufferOperation = GL_INVALID_FRAMEBUFFER_OPERATION,
	InvalidValue = GL_INVALID_VALUE,
	InvalidOperation = GL_INVALID_OPERATION,
	OutOfMemory = GL_OUT_OF_MEMORY,
	NoError = GL_NO_ERROR
};

inline Error get_error() {
	return static_cast<Error>(glGetError());
}

inline std::vector<Error> get_all_errors() {
	std::vector<Error> result;
	
	Error error;
	while ((error = get_error()) != Error::NoError) {
		result.push_back(error);
	}
	
	return result;
}

inline void log_error(Error error) {
	const char* GL_INVALID_ENUM_str =
	        "An unacceptable value is specified for an enumerated argument.";
	const char* GL_INVALID_VALUE_str =
	        "A numeric argument is out of range.";
	const char* GL_INVALID_OPERATION_str =
	        "The specified operation is not allowed in the current state.";
	const char* GL_INVALID_FRAMEBUFFER_OPERATION_str =
	        "The command is trying to render to or read from the framebuffer while the currently bound framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is not GL_FRAMEBUFFER_COMPLETE).";
	const char* GL_OUT_OF_MEMORY_str =
	        "There is not enough memory left to execute the command.";
	
	switch(error) {
	case Error::InvalidEnum:
		Log::error("GL_INVALID_ENUM", "\n");
		Log::error.indent(GL_INVALID_ENUM_str, "\n");
		break;
	case Error::InvalidFramebufferOperation:
		Log::error("GL_INVALID_FRAMEBUFFER_OPERATION", "\n");
		Log::error.indent(GL_INVALID_FRAMEBUFFER_OPERATION_str, "\n");
		break;
	case Error::InvalidValue:
		Log::error("GL_INVALID_VALUE", "\n");
		Log::error.indent(GL_INVALID_VALUE_str, "\n");
		break;
	case Error::InvalidOperation:
		Log::error("GL_INVALID_OPERATION", "\n");
		Log::error.indent(GL_INVALID_OPERATION_str, "\n");
		break;
	case Error::OutOfMemory:
		Log::error("GL_OUT_OF_MEMORY", "\n");
		Log::error.indent(GL_OUT_OF_MEMORY_str, "\n");
		break;
	case Error::NoError:
		break;
	}
}

inline void check_errors() {
	Error error;
	while ((error = get_error()) != Error::NoError) {
		log_error(error);
	}
}

namespace Impl {

template<typename F> 
inline auto checked(F fun, std::string from, std::string statement, std::string file, std::uint_fast32_t line) 
	-> typename std::enable_if<!std::is_same<decltype(fun()), void>::value, decltype(fun())>::type {
	
	auto result = fun();
	
	bool error_occurred = false;
	Error error;
	while ((error = get_error()) != Error::NoError) {
		if(!error_occurred) {
			Log::error(file ,":", line, ", ", from, "\n");
			Log::error.indent(statement, "\n");
			error_occurred = true;
		}
		Log::error.indent_in([error] { log_error(error); });
	}
	
	return result;
}

template<typename F> 
inline auto checked(F fun, std::string from, std::string statement, std::string file, std::uint_fast32_t line) 
	-> typename std::enable_if<std::is_same<decltype(fun()), void>::value, decltype(fun())>::type {
	
	fun();
	
	bool error_occurred = false;
	Error error;
	while ((error = get_error()) != Error::NoError) {
		if(!error_occurred) {
			Log::error(file ,":", line, " ", from, "\n");
			Log::error.indent(statement, "\n");
			error_occurred = true;
		}
		Log::error.indent_in([error] { log_error(error); });
	}
}

}

#define FLAPPY_GL_CHECKED(STATEMENT) \
	Flappy::Gl::Impl::checked([&] { return STATEMENT; }, __func__, # STATEMENT, __FILE__, __LINE__)

#ifdef FLAPPY_DEBUG_GL_CHECKED_OFF
	#undef FLAPPY_GL_CHECKED
	#define FLAPPY_GL_CHECKED(STATEMENT) STATEMENT
#endif

}
}
