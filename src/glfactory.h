#ifndef GL_FACTORY
#define GL_FACTORY

// Include external
#include <GL/glew.h>
#define GLFW_INCLUDE_GL3
#include <GLFW/glfw3.h>

#include "debug.h"


struct TShader
{
	TShader()
	: FActive(false)
	, FProgramID(0)
	{
	}

	bool FActive;
	GLuint FProgramID;
};

void CheckGLState(const std::string& desc);


TShader CreateShader(const std::string& parVertex, const std::string& parFragment);


#endif // GL_FACTORY