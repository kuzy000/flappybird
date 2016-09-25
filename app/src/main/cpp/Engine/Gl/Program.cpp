#include "Program.h"

#include "Engine/Gl/Error.h"
#include "Engine/Log.h"

#include <memory>

namespace Flappy {
namespace Gl {

Program::Program()
	: GLuintId {FLAPPY_GL_CHECKED(glCreateProgram())} {
	if(id == 0) {
		Log::error("Failed to create a program\n");
	}
}

Program::~Program() {
	FLAPPY_GL_CHECKED(glDeleteProgram(id));
}

Program::Program(Program&& that)
	: GLuintId {that.id} {
	FLAPPY_ASSERT(is_ready(), "moving program that has been moved (or has not been initialized)");
	that.not_ready();
}

Program& Program::operator =(Program&& rhs) {
	FLAPPY_ASSERT(is_ready(), "moving program that has been moved (or has not been initialized)");
	id = rhs.id;
	rhs.not_ready();
	
	return *this;
}

void Program::attach(const Shader& shader) {
	FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");
	FLAPPY_GL_CHECKED(glAttachShader(id, shader.get_id()));
}

void Program::detach(const Shader& shader) {
	FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");
	FLAPPY_GL_CHECKED(glDetachShader(id, shader.get_id()));
}

void Program::link() {
	FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");
	FLAPPY_GL_CHECKED(glLinkProgram(id));
	
	GLint status;
	FLAPPY_GL_CHECKED(glGetProgramiv(id, GL_LINK_STATUS, &status));
	if(status == GL_FALSE) {
		GLint length;
		FLAPPY_GL_CHECKED(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));

		std::unique_ptr<GLchar> error_log = std::unique_ptr<GLchar>(new GLchar[length]);
		FLAPPY_GL_CHECKED(glGetProgramInfoLog(id, sizeof(GLchar) * length, nullptr, error_log.get()));

		Log::error("Shader linking error: \n");
		Log::error.indent(error_log.get(), "\n");
	}

	FLAPPY_GL_CHECKED(glValidateProgram(id));
	
	FLAPPY_GL_CHECKED(glGetProgramiv(id, GL_VALIDATE_STATUS, &status));
	if(status == GL_FALSE) {
		GLsizei length;
		FLAPPY_GL_CHECKED(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));

		std::unique_ptr<GLchar> error_log = std::unique_ptr<GLchar>(new GLchar[length]);
		FLAPPY_GL_CHECKED(glGetProgramInfoLog(id, sizeof(GLchar) * length, nullptr, error_log.get()));

		Log::error("Invalid shader program: \n");
		Log::error.indent(error_log.get(), "\n");
	}
}

void Program::use() {
	FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");
	FLAPPY_GL_CHECKED(glUseProgram(id));
}

}
}

