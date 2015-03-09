#include "glfactory.h"

#include "camera.h"
#include "vector.h"
#include "debug.h"
#include "clfactory.h"
#include "particlesystem.h"
#include "renderer.h"


#define NB_PARTICLES  200000

int main(int argc, char** argv)
{
	Renderer& renderer = Renderer::Instance();
	renderer.Init();
 	TParticleSystem* ps = new TParticleSystem(NB_PARTICLES);
	ps->setOriginPosition(0.1f, 0.2f, 0.3f);
	ps->setMeanColor(1.0f, 0.2f, 0.2f);
	ps->setVarianceColor(0.1f, 0.05f, 0.5f);
	ps->setMeanDuration(2.0);
	ps->setDurationVariance(1.0);
	ps->initParticleSystem();
	renderer.addPS(ps);
	renderer.Run();
}