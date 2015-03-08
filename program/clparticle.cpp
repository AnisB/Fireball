#include "glfactory.h"

#include "debug.h"
#include "clfactory.h"
#include "particlesystem.h"


#define NB_PARTICLES  20	

TOpenCLData clData;
TOpenCLProgram clProgram;

int main(int argc, char** argv)
{
	PRINT_GREEN("OpenCL particle simulator");

	initCLContext(clData);


 	GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    // Defintion du core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Snowman", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	GLenum glewReturn = glewInit();
	if(glewReturn)
	{
	    PRINT_RED("Glew returned: "<<glewGetErrorString(glewReturn));
	}
	// Pour vérifier la version
	const GLubyte* renderer = glGetString (GL_RENDERER); 
	const GLubyte* version = glGetString (GL_VERSION); 
	PRINT_GREEN("Renderer: "<<renderer);
	PRINT_GREEN("Version: "<<version);

	TOpenCLProgram clProgram;
	compileProgram(clData, "data/kernels/particleSystem.cl", clProgram);

 	TParticleSystem* ps = new TParticleSystem(NB_PARTICLES);
	ps->setOriginPosition(0.0f, 0.0f, 0.0f);
	ps->setMeanColor(1.0f, 0.2f, 0.2f);
	ps->setVarianceColor(0.1f, 0.05f, 0.5f);
	ps->initParticleSystem(clData, clProgram);

	

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	ps->update(0.016, clData, clProgram);
    	ps->draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

}