#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "glfactory.h"

#include "clfactory.h"
#include "debug.h"
#include "matrix.h"

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

	bool initParticleSystem();

	void update(float parTime);
	void draw(const TMatrix4<double>& parProjectionView);

protected:
	// CPU ParticleSetData
	cl_float4* FPositions;
	cl_float4* FColors;

	// GPU ParticleSetData
	cl_mem FPosBuffer;
	cl_mem FVelocBuffer;
	cl_mem FColorBuffer;
	cl_mem FLTBuffer;

	TOpenCLProgram FProgram;
	cl_kernel initKernel;
	cl_kernel updateKernel;

	// GeneratorData
	uint FNbParticles;
	uint FSeed;
	cl_float4 FOriginPosition;
	cl_float4 FMeanColor;
	cl_float4 FVarianceColor;
	cl_float FMeanDuration;
	cl_float FDurationVariance;
	cl_float FRotation;

	TShader FShader;
	GLuint FVAO;
	GLuint FVBO;

};

#endif //PARTICLE_SYSTEM