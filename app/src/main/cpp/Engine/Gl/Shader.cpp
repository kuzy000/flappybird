#include "Shader.h"

#include "Engine/Log.h"
#include "Engine/Gl/Error.h"
#include "Engine/Util/Assert.h"

#include <memory>

namespace Flappy {
namespace Gl {

Shader::Shader(GLuint id)
	: GLuintId {id} {
	if(id == 0) {
		Log::error("Failed to create a shader\n");
	}
}

Shader::~Shader() {
	FLAPPY_GL_CHECKED(glDeleteShader(id));
}

Shader::Shader(Shader&& that)
	: GLuintId {that.id} {
	FLAPPY_ASSERT(that.is_ready(), "moving shader that has been moved (or has not been created)");
	that.not_ready();
}

Shader& Shader::operator =(Shader&& rhs) {
	FLAPPY_ASSERT(rhs.is_ready(), "moving shader that has been moved (or has not been created)");
	id = rhs.id;
	rhs.not_ready();
	
	return *this;
}

void Shader::set_source(std::string source) {
	FLAPPY_ASSERT(is_ready(), "using shader that has been moved (or has not been created)");
	
	const char* value = source.c_str();
	FLAPPY_GL_CHECKED(glShaderSource(id, 1, &value, 0));
}

void Shader::compile() {
	FLAPPY_ASSERT(is_ready(), "using shader that has been moved (or has not been created)");
	
	FLAPPY_GL_CHECKED(glCompileShader(id));
	
	GLint status;
	FLAPPY_GL_CHECKED(glGetShaderiv(id, GL_COMPILE_STATUS, &status));

	if(status == GL_FALSE) {
		GLint length;
		FLAPPY_GL_CHECKED(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		
		// some drivers have a bug...
		// so, glGetShaderiv(glId, GL_INFO_LOG_LENGTH, &length) always returns 0
		if(length == 0) { 
			length = 1024;
		}

		std::unique_ptr<GLchar[]> error_log = std::unique_ptr<GLchar[]>(new GLchar[length]);
		FLAPPY_GL_CHECKED(glGetShaderInfoLog(id, sizeof(GLchar) * length, nullptr, error_log.get()));

		Log::error("Shader compilation error: \n");
		Log::error.indent(error_log.get(), "\n");
	}
}

}
}
