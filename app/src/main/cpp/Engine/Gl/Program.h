#pragma once

#include "Object.h"
#include "Shader.h"
#include "Attribute.h"
#include "Uniform.h"

#include <string>

namespace Flappy {
namespace Gl {

class Program : public GLuintId {
public:
	Program();
	~Program();
	
	Program(const Program&) = delete;
	Program& operator =(const Program&) = delete;
	
	Program(Program&& that);
	Program& operator =(Program&& rhs);
	
	void attach(const Shader& shader);
	void detach(const Shader& shader);
	
	void link();
	void use();
	
	template <typename T>
	Attribute<T> get_attribute(std::string name) {
		FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");
		GLint result = FLAPPY_GL_CHECKED(glGetAttribLocation(id, name.c_str()));

		if(result == -1) {
			Log::error("glGetAttribLocation error, '", name, "' is not an active attribute (has a typo, starts with gl_ or removed by the compiler)", "\n");
		}

		return {static_cast<GLuint>(result)};
	}
	
	template <typename T>
	void bind_attribute(Attribute<T>& attribute, std::string name) {
		attribute = get_attribute<T>(std::move(name));
	}

	template <typename T>
	Uniform<T> get_uniform(std::string name) {
		FLAPPY_ASSERT(is_ready(), "using program that has been moved (or has not been initialized)");

		GLint result = FLAPPY_GL_CHECKED(glGetUniformLocation(id, name.c_str()));

		if(result == -1) {
			Log::error("glGetUniformLocation error, '", name, "' is not an active uniform (has a typo, starts with gl_ or removed by the compiler)", "\n");
		}

		return {result};
	}
	
	template <typename T>
	void bind_uniform(Uniform<T>& uniform, std::string name) {
		uniform = get_uniform<T>(std::move(name));
	}
};

}
}
