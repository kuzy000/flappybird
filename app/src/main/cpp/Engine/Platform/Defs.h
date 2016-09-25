#pragma once

#define PLATFORM_DESKTOP 0
#define PLATFORM_ANDROID 1

#ifdef TARGET_DESKTOP 
	#define TARGET_PLATFORM PLATFORM_DESKTOP
#elif TARGET_ANDROID
	#define TARGET_PLATFORM PLATFORM_ANDROID
#else 
	#error "Platform is not defined"
#endif

namespace Flappy {
namespace Platform {

enum class Kind { Linux, Android };

static constexpr Kind get_target() {
	#if TARGET_PLATFORM == PLATFORM_LINUX
		return Kind::Linux;
	#elif TARGET_PLATFORM == PLATFORM_ANDROID
		return Kind::Android;
	#endif
}

}
}
