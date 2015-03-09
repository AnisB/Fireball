#include "renderer.h"

#include "common.h"

#define X_ANGLE 1.0
#define Y_ANGLE 1.0
#define SPEED 10.0

static void error_callback(int error, const char* description)
{
    PRINT_RED(error<<" "<<description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
    {
    	Renderer::Instance().keyPressed(key);
    }
    else if(action ==  GLFW_RELEASE)
    {
    	Renderer::Instance().keyReleased(key);
    }
}
static void cursor_callback(GLFWwindow *window, double xpos, double ypos)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float) height;
    float x = ratio*(2*xpos/(float)width - 1);
    float y = 2*-ypos/(float)height + 1;
    Renderer::Instance().mouseMove(x,y);
}

Renderer::Renderer()
: FFoward(false)
, FBackward(false)
, FRight(false)
, FLeft(false)
, FMouseInit(false)
{
	
}


Renderer::~Renderer()
{
	
}

bool Renderer::Init()
{
	PRINT_GREEN("OpenCL particle simulator");

	initCLContext();

 	if (!glfwInit())
        return -1;
    // Defintion du core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    /* Create a FWindowed mode FWindow and its OpenGL context */
    FWindow = glfwCreateWindow(640, 480, "Fireball", NULL, NULL);
    if (!FWindow)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the FWindow's context current */
    glfwMakeContextCurrent(FWindow);

	glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(FWindow, key_callback);
    glfwSetCursorPosCallback(FWindow, cursor_callback);

    glfwSetInputMode(FWindow,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
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

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

	FCamera.DefinePerspective(45, 640/480.0, 0.1, 300);
	FCamera.Translate(TVec3<double>(0.0,0.0,-8.0));

    return true;
}

void Renderer::Run()
{
	glClearColor(0.0,0.0,0.0,0.0);
	double timeA = glfwGetTime();
    while (!glfwWindowShouldClose(FWindow))
    {
		double timeB = glfwGetTime();
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double delta = timeB-timeA;
        UpdateScene(delta);
        for_each_macro(ps, FPS)
        {
            (*ps)->update(delta);
            (*ps)->draw(FCamera.GetProjectionViewMatrix());            
        }

		timeA = timeB;
        glfwSwapBuffers(FWindow);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Renderer::keyPressed(int key)
{
    switch(key)
    {
    	case GLFW_KEY_ESCAPE :
        	glfwSetWindowShouldClose(FWindow, GL_TRUE);
    	break;
    	case GLFW_KEY_W:
    		FFoward = true;
		break;
    	case GLFW_KEY_S:
    		FBackward = true;
		break;
    	case GLFW_KEY_A:
    		FLeft = true;
		break;
    	case GLFW_KEY_D:
    		FRight = true;
		break;
    	default:
    		return;
    }
    return;
}

void Renderer::keyReleased(int key)
{
	switch(key)
    {
    	case GLFW_KEY_W:
    		FFoward = false;
		break;
    	case GLFW_KEY_S:
    		FBackward = false;
		break;
    	case GLFW_KEY_A:
    		FLeft = false;
		break;
    	case GLFW_KEY_D:
    		FRight = false;
		break;
    	default:
    		return;
    }
    return;
}

void Renderer::UpdateScene(double parDelta)
{
	if(FFoward)
	{
		FCamera.Translate(TVec3<double>(0.0,0.0,SPEED*parDelta));
	}
	if(FBackward)
	{
		FCamera.Translate(TVec3<double>(0.0,0.0,-SPEED*parDelta));
	}
	if(FLeft)
	{
		FCamera.Translate(TVec3<double>(SPEED*parDelta,0.0,0.0));
	}
	if(FRight)
	{
		FCamera.Translate(TVec3<double>(-SPEED*parDelta,0.0,0.0));
	}
}

void Renderer::mouseMove(float parX, float parY)
{
    if(!FMouseInit)
    {
        FOldX = parX;
        FOldY = parY;
        FMouseInit = true;
    }
    else
    {
        double deltaX = parX-FOldX;
        double deltaY = parY-FOldY;
        FCamera.Yaw(deltaX*X_ANGLE);
        FCamera.Pitch(deltaY*Y_ANGLE);
        FOldX = parX;
        FOldY = parY;
    }
}


void Renderer::addPS(TParticleSystem* parPS)
{
    FPS.push_back(parPS);
}