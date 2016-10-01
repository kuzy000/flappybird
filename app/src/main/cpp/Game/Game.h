#pragma once

#include "Engine/Engine.h"

#include "Engine/Gl/Buffer.h"
#include "Engine/Gl/Framebuffer.h"
#include "Engine/Gl/Texture.h"

#include "Engine/Math/AABB2.h"

#include "Game/Column.h"
#include "Game/World.h"
#include "Game/Chunk.h"
#include "Game/Bird.h"
#include "Game/ScreenRect.h"
#include "Game/Shader/Background.h"
#include "Game/Shader/Column.h"
#include "Game/Shader/PostAndBird.h"

#include <deque>

namespace Flappy {

class Game {
public:
	static constexpr float aspect_ratio = 16.f / 9.f;
	static const AABB2f screen;
	
	Game(Engine& engine);
	
	void new_game();
private:
	void update(float dt);
	void render(float dt);
	void resize(Size2u size);
	void tap_down(Vec2u pos);
	
	void shift_background(float value);
	
	Engine& engine;
	
	enum class State { FadeOut, Prepare, Play, FadeIn };
	State state = State::FadeOut;
	
	World world;
	Bird bird;
	ScreenRect screen_rect;
	
	const float speed = 1.3f;
	
	float bg_shift = 0.0f;
	float fade = 0.0f;
	
	AABB2<GLint> viewport;
	
	Gl::Framebuffer fbo;
	Gl::Texture fbo_tex;
	Shader::Background prog_background;
	Shader::Column prog_column;
	Shader::PostAndBird prog_post_and_bird;
};

}
