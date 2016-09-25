#include "PostAndBird.h"

namespace Flappy {
namespace Shader {

std::string PostAndBird::vertex_source = R"glsl(
#version 100

#ifdef GL_ES
	precision mediump float;
#endif

attribute vec2 position;
uniform float aspect_ratio;

varying vec2 uv;
varying vec2 screen;

void main(void) {
	uv = (position + vec2(1.0, 1.0)) * 0.5;
	screen.x = position.x * aspect_ratio;
	screen.y = position.y;
	gl_Position = vec4(position, 0, 1);
}
)glsl";

std::string PostAndBird::fragment_source = R"glsl(
#version 100

#ifdef GL_ES
	precision mediump float;
#endif

uniform vec2 bird;
uniform sampler2D fbo_tex;
uniform float fade;

varying vec2 uv;
varying vec2 screen;

void main(void) {
	vec3 c = texture2D(fbo_tex, uv).xyz;
	float t;
	
	t = smoothstep(0.06 - 0.003, 0.06 + 0.003, length(screen - bird));
//	t = step(0.06, length(screen - bird));
	c = mix(vec3(0.8, 0.2, 0.2), c, t);
	
//	c *= fade * pow(cos(uv.x - 0.5) * cos(screen.y), 0.4);
	c *= fade;
	
	gl_FragColor = vec4(c, 1.0);
}
)glsl";


}
}
