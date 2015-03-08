#include "debug.h"
#include "clfactory.h"
#include "particlesystem.h"

#define NB_PARTICLES  200	

int main(int argc, char** argv)
{
	PRINT_GREEN("OpenCL particle simulator");
	TOpenCLData clData;
	initCLContext(clData);
	TOpenCLProgram clProgram;
	compileProgram(clData, "data/kernels/particleSystem.cl", clProgram);

	TParticleSystem* ps = new TParticleSystem(NB_PARTICLES);
	ps->setOriginPosition(0.0f, 0.0f, 0.0f);
	ps->setMeanColor(1.0f, 0.2f, 0.2f);
	ps->setVarianceColor(0.1f, 0.05f, 0.5f);
	ps->initParticleSystem(clData, clProgram);

	ps->update(0.0, clData, clProgram);
}