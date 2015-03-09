#include "glfactory.h"

#include "camera.h"
#include "vector.h"
#include "debug.h"
#include "clfactory.h"
#include "particlesystem.h"


#define NB_PARTICLES  20	

TOpenCLData clData;
TOpenCLProgram clProgram;
Camera camera;
static bool mouseInit = false;
double FOldX = 0.0;
double FOldY = 0.0;

#define X_ANGLE 1.0
#define Y_ANGLE 1.0

static void error_callback(int error, const char* description)
{
    PRINT_RED(error<<" "<<description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
    	switch(key)
    	{
	    	case GLFW_KEY_ESCAPE :
	        	glfwSetWindowShouldClose(window, GL_TRUE);
	    	break;
	    	case GLFW_KEY_W:
	    		camera.Translate(TVec3d(0.0,0.0,1.0));
			break;
	    	case GLFW_KEY_S:
	    		camera.Translate(TVec3d(0.0,0.0,-1.0));
			break;
	    	case GLFW_KEY_A:
	    		camera.Translate(TVec3d(1.0,0.0,0.0));
			break;
	    	case GLFW_KEY_D:
	    		camera.Translate(TVec3d(-1.0,0.0,0.0));
			break;
	    	default:
	    		return;
	    }
    }
}
static void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float) height;
    float x = ratio*(2*xpos/(float)width - 1);
    float y = 2*-ypos/(float)height + 1;
	if(!mouseInit)
	{
		FOldX = x;
		FOldY = y;
		mouseInit = true;
	}
	else
	{
		double deltaX = x-FOldX;
		double deltaY = y-FOldY;
		// std::cout<<deltaX<<" "<<deltaY<<std::endl;
		camera.Yaw(deltaX*X_ANGLE);
		camera.Pitch(deltaY*Y_ANGLE);
		FOldX = x;
		FOldY = y;
	}
}

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

	glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);

    glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

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

	glPointSize(6.0f);
 	TParticleSystem* ps = new TParticleSystem(NB_PARTICLES);
	ps->setOriginPosition(0.0f, 0.0f, 0.0f);
	ps->setMeanColor(1.0f, 0.2f, 0.2f);
	ps->setVarianceColor(0.1f, 0.05f, 0.5f);
	ps->setMeanDuration(1.0);
	ps->setDurationVariance(0.05);
	camera.DefinePerspective(45, 640/480.0, 0.1, 300);
	ps->initParticleSystem(clData, clProgram);

	ps->update(0.5, clData, clProgram);
	ps->draw(camera.GetProjectionViewMatrix());

    while (!glfwWindowShouldClose(window))
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

}