#ifndef RENDERER
#define RENDERER

#include "glfactory.h"

#include "camera.h"
#include "vector.h"
#include "debug.h"
#include "clfactory.h"
#include "particlesystem.h"
#include "singleton.h"

#include <vector>

class Renderer : public Singleton<Renderer>
{
	public:
		Renderer();
		~Renderer();

		bool Init();

		void Run();
		void UpdateScene(double parDelta);
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMove(float parX, float parY);

		void addPS(TParticleSystem* parPS);

	protected:
 		GLFWwindow* FWindow;

		TOpenCLProgram FClProgram;
		Camera FCamera;

		std::vector<TParticleSystem*> FPS;

		bool FFoward;
		bool FBackward;
		bool FLeft;
		bool FRight;

		double FOldX;
		double FOldY;
		bool FMouseInit;
};

#endif // RENDERER