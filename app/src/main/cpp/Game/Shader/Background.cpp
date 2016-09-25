#include "Background.h"

namespace Flappy {
namespace Shader {

const std::string Background::vertex_source = R"glsl(
#version 100

#ifdef GL_ES
	precision mediump float;
#endif

attribute vec2 position;
uniform float aspect_ratio;

varying vec2 uv;

void main(void) {
	uv = (position + vec2(1.0, 1.0)) * 0.5;
	uv.x *= aspect_ratio;
	gl_Position = vec4(position, 0, 1);
}
)glsl";

const std::string Background::fragment_source = R"glsl(
#version 100

#ifdef GL_ES
	precision lowp float;
#endif

uniform float shift;
varying vec2 uv;

#define PI 3.1415

void main(void) {
	vec3 front = vec3(0.65, 0.65, 0.95);
	vec3 back  = vec3(0.85, 0.85, 0.9);

	vec3 result = mix(front, back, length(uv));

	vec3 c;
	float s;
	float t = 1.0;

	if(uv.y < 0.5 + 0.5 + 0.027 * 2.0) {
		// front wave
		c = front;
		
		s  = sin(uv.x * 1.8 * PI * 2. + shift * 3.0) * 0.027;
		s += cos(uv.x * 1.8 * PI * 1. + shift * 1.5) * 0.027;
		
		t = step(s + 0.5, uv.y);
		result = mix(c, result, t);
	}
	if(t == 1.0 && uv.y < 0.5 + 0.65 + 0.027) {
		// back wave
		c = front + (back - front) * 0.5;
	
		s  = sin(uv.x * 3.6 * PI * 2. + shift * 1.5)  * 0.027 * 0.5;
		s += cos(uv.x * 3.6 * PI * 1. + shift * 0.75) * 0.027 * 0.5;
		
		t = step(s + 0.65, uv.y);
		result = mix(c, result, t);
	}

	gl_FragColor = vec4(result, 1.0);
}
)glsl";

}
}
