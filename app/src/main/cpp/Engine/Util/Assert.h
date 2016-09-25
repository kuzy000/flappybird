#pragma once

#include "Engine/Debug.h"
#include "Engine/Log.h"

#define IMPL_FLAPPY_GET_ASSERT_MACRO(_1, _2, NAME, ...) NAME
#define IMPL_FLAPPY_ASSERT(cond) \
	Flappy::Util::Impl::assert_impl((cond), __FILE__, __LINE__, __func__, #cond)
#define IMPL_FLAPPY_ASSERT_WITH_TEXT(cond, text) \
	Flappy::Util::Impl::assert_impl((cond), __FILE__, __LINE__, __func__, #cond, text)

#define FLAPPY_ASSERT(...) \
	IMPL_FLAPPY_GET_ASSERT_MACRO(__VA_ARGS__, IMPL_FLAPPY_ASSERT_WITH_TEXT, IMPL_FLAPPY_ASSERT)(__VA_ARGS__)

#ifdef FLAPPY_DEBUG_ASSERT_OFF
	#undef FLAPPY_ASSERT
	#define FLAPPY_ASSERT(...)
#endif

namespace Flappy { 
namespace Util {
namespace Impl {

inline void assert_impl(bool cond, std::string file, std::uint_fast64_t line, std::string func, std::string expr) {
	if(!cond) {
		Log::error("Assertion failed [ ", expr, " ].\n");
		Log::error.indent(file, ":", line, ", ", func, "\n");
	}
}

inline void assert_impl(bool cond, std::string file, std::uint_fast64_t line, std::string func, std::string expr, std::string text) {
	if(!cond) {
		Log::error("Assertion failed [ ", expr, " ]: ", text, ".\n");
		Log::error.indent(file, ":", line, ", ", func, "\n");
	}
}

}
}
}
