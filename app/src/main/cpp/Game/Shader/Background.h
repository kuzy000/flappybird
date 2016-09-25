#pragma once

#include "Engine/Gl/Program.h"
#include "Engine/Gl/ShaderVertex.h"
#include "Engine/Gl/ShaderFragment.h"

#include <string>

namespace Flappy {
namespace Shader {

class Background : public Gl::Program {
public:
	Background() {
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
		
		bind_uniform(shift, "shift");
		bind_uniform(aspect_ratio, "aspect_ratio");
	}
	
	Gl::Attribute<Vec2f> position;
	
	Gl::Uniform<float> shift;
	Gl::Uniform<float> aspect_ratio;
	
	static const std::string vertex_source;
	static const std::string fragment_source;
};

}
}
