#pragma once

#include "Engine/Gl/Object.h"

#include <string>

namespace Flappy {
namespace Gl {

class Shader : public GLuintId {
public:
	Shader() = delete;
	
	~Shader();
	
	Shader(const Shader&) = delete;
	Shader& operator =(const Shader&) = delete;
	
	Shader(Shader&& that);
	Shader& operator =(Shader&& rhs);
	
	void set_source(std::string source);
	void compile();
	
protected:
	Shader(GLuint id);
};

}
}
