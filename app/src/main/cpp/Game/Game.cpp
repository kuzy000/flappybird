#include "Game.h"

#include "Engine/Log.h"
#include "Engine/Gl/Gl.h"
#include "Engine/Gl/Draw.h"
#include "Engine/Util/OffsetOf.h"

#include <chrono>
#include <cmath>

namespace Flappy {

const AABB2f Game::screen {{-1.0f * aspect_ratio, -1.0}, {1.0f * aspect_ratio, 1.0f}};

Game::Game(Engine& engine)
	: engine {engine} {
	
	engine.on_update.watch([this] (float dt) { update(dt); });
	engine.on_render.watch([this] (float dt) { render(dt); });
	engine.on_window_size.watch([this] (Size2u size) { resize(size); });
	engine.on_tap_down.watch([this] (Vec2u pos) { tap_down(pos); });
	
	resize(engine.get_window_size());
	
	fbo_tex.bind(Gl::Texture::Target::_2d);
	FLAPPY_GL_CHECKED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	FLAPPY_GL_CHECKED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	FLAPPY_GL_CHECKED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	FLAPPY_GL_CHECKED(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	fbo.bind(Gl::Framebuffer::Target::ReadDraw);
	FLAPPY_GL_CHECKED(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_tex.get_id(), 0));
}

void Game::new_game() {
	world.new_game();
	bird.new_game();
}

void Game::update(float dt) {
	bg_shift += 0.2f * dt;
	
	// FadeOut -> Prepare (tap to play) -> Play -> FadeIn (on fail) -> FadeOut
	switch(state) {
	case State::FadeOut:
		fade += 2.5f * dt;
		if(fade >= 1.0f) {
			fade = 1.0f;
			state = State::Prepare;
		}
		break;
	case State::Prepare:
	
		break;
	case State::Play:
		if(world.is_collide(bird) || std::abs(bird.position.y()) >= screen.top() + bird.radius) {
			state = State::FadeIn;
		}
		else {
			bird.update(dt);
			world.move(speed * dt);
		}
		bg_shift += (speed - 0.3f) * dt;
	
		break;
	case State::FadeIn:
		fade -= 2.7f * dt;
		if(fade <= -0.2f) {
			state = State::FadeOut;
			new_game();
		}
		break;
	}
	
}

void Game::render(float dt) {
	Gl::set_clear_color({0.f, 0.f, 0.f, 1.f});
	Gl::clear(GL_COLOR_BUFFER_BIT);
	
	// draw into the fbo
	Gl::set_viewport({{0, 0}, viewport.get_size()});
	fbo.bind(Gl::Framebuffer::Target::ReadDraw);
	
	// background
	screen_rect.vbo.bind(Gl::Buffer::Target::Array);
	screen_rect.ibo.bind(Gl::Buffer::Target::ElementArray);
	prog_background.use();
	prog_background.position.bind(sizeof(ScreenRect::Vertex), Util::offset_of(&ScreenRect::Vertex::position));
	prog_background.position.enable();
	prog_background.aspect_ratio.set(aspect_ratio);
	prog_background.shift.set(bg_shift);
	Gl::draw_elements(Gl::DrawMode::Triangles, static_cast<GLsizei>(ScreenRect::indecies_count), Gl::ElementsType::UInt);
	
	// columns
	for(auto& chunk : world.chunks) {
		if(chunk.shift < Game::screen.left() - chunk.get_width() || chunk.shift > Game::screen.right()) {
			continue;
		}
		chunk.vbo.bind(Gl::Buffer::Target::Array);
		chunk.ibo.bind(Gl::Buffer::Target::ElementArray);
		prog_column.use();
		prog_column.position.bind(sizeof(Chunk::Vertex), Util::offset_of(&Chunk::Vertex::position));
		prog_column.position.enable();
		prog_column.local.bind(sizeof(Chunk::Vertex), Util::offset_of(&Chunk::Vertex::local));
		prog_column.local.enable();
		prog_column.aspect_ratio.set(aspect_ratio);
		prog_column.shift.set(chunk.shift);
		Gl::draw_elements(Gl::DrawMode::Triangles, static_cast<GLsizei>(chunk.indecies_count), Gl::ElementsType::UInt);
	}
	
	// draw on the screen
	fbo.unbind(Gl::Framebuffer::Target::ReadDraw);
	Gl::set_viewport(viewport);
	
	// bird & posteffects (only a fading though)
	Gl::Texture::activate(0);
	fbo_tex.bind(Gl::Texture::Target::_2d);
	
	screen_rect.vbo.bind(Gl::Buffer::Target::Array);
	screen_rect.ibo.bind(Gl::Buffer::Target::ElementArray);
	prog_post_and_bird.use();
	prog_post_and_bird.position.bind(sizeof(ScreenRect::Vertex), Util::offset_of(&ScreenRect::Vertex::position));
	prog_post_and_bird.position.enable();
	prog_post_and_bird.aspect_ratio.set(aspect_ratio);
	prog_post_and_bird.bird.set(bird.position);
	prog_post_and_bird.bird_radius.set(bird.radius);
	prog_post_and_bird.fade.set(fade);
	prog_post_and_bird.fbo_tex.set(0);
	Gl::draw_elements(Gl::DrawMode::Triangles, static_cast<GLsizei>(ScreenRect::indecies_count), Gl::ElementsType::UInt);
}

void Game::resize(Size2u size) {
	const auto real_w = static_cast<GLint>(size.width());
	const auto real_h = static_cast<GLint>(size.height());
	const float real_aspect_ratio = static_cast<float>(real_w) / static_cast<float>(real_h);
	
	const auto w = real_aspect_ratio > aspect_ratio ? static_cast<GLint>(real_h * aspect_ratio) : real_w;
	const auto h = real_aspect_ratio < aspect_ratio ? static_cast<GLint>(real_w / aspect_ratio) : real_h;
	
	const auto shift_w = real_aspect_ratio > aspect_ratio ? (real_w - w) / 2 : 0;
	const auto shift_h = real_aspect_ratio < aspect_ratio ? (real_h - h) / 2 : 0;
	
	viewport = {
		{shift_w    , shift_h},
		{shift_w + w, shift_h + h},
	};
	
	Gl::Texture::activate(0);
	fbo_tex.bind(Gl::Texture::Target::_2d);
	Gl::Texture::set_image2d(Gl::Texture::Target2d::_2d, 0, GL_RGB, static_cast<Size2<GLsizei>>(viewport.get_size()), GL_RGB, GL_UNSIGNED_BYTE, nullptr);
}

void Game::tap_down(Vec2u pos) {
	if(state == State::Prepare) {
		state = State::Play;
	}
	bird.jump();
}

}
