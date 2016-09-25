#include "ShaderVertex.h"

#include "Engine/Gl/Error.h"

namespace Flappy {
namespace Gl {

ShaderVertex::ShaderVertex()
	: Shader {FLAPPY_GL_CHECKED(glCreateShader(GL_VERTEX_SHADER))} {
	
}

}
}

