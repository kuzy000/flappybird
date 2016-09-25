#pragma once

#include "Engine/Platform/Defs.h"

#if TARGET_PLATFORM == PLATFORM_DESKTOP
	#include <GL/glew.h>
#elif TARGET_PLATFORM == PLATFORM_ANDROID
	#include <GLES2/gl2.h>
#endif

