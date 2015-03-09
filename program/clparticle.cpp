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

 	TParticleSystem* ps2 = new TParticleSystem(NB_PARTICLES);
	ps2->setOriginPosition(5.0,5.0,5.0);
	ps2->setMeanColor(0.2f, 0.2f, 0.8f);
	ps2->setVarianceColor(0.05f, 0.05f, 0.2f);
	ps2->setMeanDuration(20.0);
	ps2->setDurationVariance(0.0);
	ps2->initParticleSystem();
	renderer.addPS(ps2);
	renderer.Run();
}