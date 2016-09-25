#include "Column.h"

namespace Flappy {
namespace Shader {

std::string Column::vertex_source = R"glsl(
#version 100

#ifdef GL_ES
	precision mediump float;
#endif

attribute vec2 position;
attribute vec2 local;

uniform float aspect_ratio;
uniform float shift;

varying vec2 uv;

void main(void) {
	uv = local;
	gl_Position = vec4((position.x + shift) / aspect_ratio, position.y, 0, 1);
}
)glsl";

std::string Column::fragment_source = R"glsl(
#version 100

#ifdef GL_ES
	precision mediump float;
#endif

varying vec2 uv;

void main(void) {
	vec3 c1 = vec3(0.25, 0.25, 0.25);
	vec3 c2 = vec3(0.45, 0.45, 0.45);
	
	float t = step(0.8 * 0.8, dot(uv, uv) * float(uv.y > 0.0) + float(abs(uv.x) > 0.8));
	vec3 c = mix(c2, c1, t);
	
	gl_FragColor = vec4(c, 1.0);
}
)glsl";

}
}
