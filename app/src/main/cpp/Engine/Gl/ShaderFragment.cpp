#include "ShaderFragment.h"

#include "Engine/Gl/Error.h"

namespace Flappy {
namespace Gl {

ShaderFragment::ShaderFragment()
	: Shader {FLAPPY_GL_CHECKED(glCreateShader(GL_FRAGMENT_SHADER))} {
	
}

}
}
