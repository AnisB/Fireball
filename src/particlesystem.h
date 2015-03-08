#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "clfactory.h"
#include "debug.h"

class TParticleSystem
{
public:
	TParticleSystem(uint parNbParticles);
	~TParticleSystem();

	// Position
	void setOriginPosition(cl_float parX, cl_float parY, cl_float parZ);
	// Color
	void setMeanColor(cl_float parR, cl_float parG, cl_float parB);
	void setVarianceColor(cl_float parR, cl_float parG, cl_float parB);

	// Duration
	void setMeanDuration(cl_float parMeanDuration);
	void setDurationVariance(cl_float parDurationVariance);

	// Rotation
	void setRotation(cl_float parRotation);

	bool initParticleSystem(const TOpenCLData& clData, const TOpenCLProgram& parClProgram);

	void update(float parTime, const TOpenCLData& clData, const TOpenCLProgram& parClProgram);

protected:
	// CPU ParticleSetData
	cl_float3* FPositions;
	cl_float3* FVelocity;
	cl_float3* FColors;

	cl_float* FLifetime;

	// GPU ParticleSetData
	cl_mem FPosBuffer;
	cl_mem FVelocBuffer;
	cl_mem FColorBuffer;
	cl_mem FLTBuffer;

	cl_kernel initKernel;
	cl_kernel updateKernel;

	// GeneratorData
	uint FNbParticles;
	cl_float3 FOriginPosition;
	cl_float3 FMeanColor;
	cl_float3 FVarianceColor;
	cl_float FMeanDuration;
	cl_float FDurationVariance;
	cl_float FRotation;
};

#endif //PARTICLE_SYSTEM