project(flappybird)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wstrict-aliasing -pedantic -Wno-long-long -Wunreachable-code")

set(TARGET_PLATFORM "Linux" CACHE STRING "Select a target platform")
set_property(CACHE TARGET_PLATFORM PROPERTY STRINGS "Desktop" "Android")

file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

foreach(TMP_PATH ${SOURCES})
	string(FIND ${TMP_PATH} "/Nonportable/" EXCLUDE_DIR_FOUND)
	if(NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
		list(REMOVE_ITEM SOURCES ${TMP_PATH})
	endif()
endforeach(TMP_PATH)


if(TARGET_PLATFORM STREQUAL "Desktop")
	add_definitions("-DTARGET_DESKTOP")

	list(APPEND SOURCES 
		"Nonportable/desktop_main.cpp"
		"Engine/Platform/Nonportable/ContextGlfw.cpp"
	)
	
	find_package(OpenGL REQUIRED)
	find_package(Threads REQUIRED)
	find_package(GLEW REQUIRED)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED glfw3)

	include_directories(
		SYSTEM # removes unneccecary warnings
		.
		${OPENGL_INCLUDE_DIRS}
		${GLEW_INCLUDE_DIRS}
		${GLFW_INCLUDE_DIRS}
	)

	add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
	
	target_link_libraries(${PROJECT_NAME}
		${OPENGL_LIBRARY}
		${GLEW_LIBRARIES}
		${GLFW_LIBRARIES}
		${CMAKE_THREAD_LIBS_INIT}
	)
elseif(TARGET_PLATFORM STREQUAL "Android")
	add_definitions("-DFLAPPY_DEBUG_OFF")
	add_definitions("-DTARGET_ANDROID")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

	list(APPEND SOURCES 
		"Nonportable/android_main.cpp"
		"Engine/Platform/Nonportable/ContextAndroid.cpp"
	)

	include_directories(
		SYSTEM
		.
		${ANDROID_NDK}/sources/android/native_app_glue
	)

	add_library(app-glue STATIC
				${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

	add_library(native-activity SHARED ${SOURCES})

	target_link_libraries(native-activity log android EGL GLESv2 app-glue)
endif(TARGET_PLATFORM STREQUAL "Linux")

