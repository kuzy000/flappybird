#pragma once

#include "Engine/Gl/Program.h"
#include "Engine/Gl/ShaderVertex.h"
#include "Engine/Gl/ShaderFragment.h"

#include <string>

namespace Flappy {
namespace Shader {

class PostAndBird : public Gl::Program {
public:
	PostAndBird() {
		Gl::ShaderVertex vertex;
		vertex.set_source(vertex_source);
		vertex.compile();
		attach(vertex);
		
		Gl::ShaderFragment fragment;
		fragment.set_source(fragment_source);
		fragment.compile();
		attach(fragment);
		
		link();
		
		bind_attribute(position, "position");
		
		bind_uniform(aspect_ratio, "aspect_ratio");
		bind_uniform(fbo_tex, "fbo_tex");
		bind_uniform(bird, "bird");
		bind_uniform(fade, "fade");
	}
	
	
	Gl::Attribute<Vec2f> position;
	
	Gl::Uniform<float> aspect_ratio;
	Gl::Uniform<Gl::Sampler2D> fbo_tex;
	Gl::Uniform<Vec2f> bird;
	Gl::Uniform<float> fade;
	
	static const std::string vertex_source;
	static const std::string fragment_source;
};

}
}
