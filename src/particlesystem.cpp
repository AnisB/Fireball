#include "particlesystem.h"

#include <algorithm> 

TParticleSystem::TParticleSystem(uint parNbParticles)
: FNbParticles(parNbParticles)
{
	FPositions = new cl_float4[FNbParticles*3](); 
	FColors = FPositions + FNbParticles;
}

TParticleSystem::~TParticleSystem()
{
	delete [] FPositions;
    clReleaseMemObject(FPosBuffer);
    clReleaseMemObject(FVelocBuffer);
    clReleaseMemObject(FVelocBuffer);
    clReleaseMemObject(FLTBuffer);
}

void TParticleSystem::setOriginPosition(cl_float parX, cl_float parY, cl_float parZ)
{
	FOriginPosition.s[0] = parX;
	FOriginPosition.s[1] = parY;
	FOriginPosition.s[2] = parZ;
}

void TParticleSystem::setMeanColor(cl_float parR, cl_float parG, cl_float parB)
{
	FMeanColor.s[0] = parR;
	FMeanColor.s[1] = parG;
	FMeanColor.s[2] = parB;
}

void TParticleSystem::setVarianceColor(cl_float parR, cl_float parG, cl_float parB)
{
	FVarianceColor.s[0] = parR;
	FVarianceColor.s[1] = parG;
	FVarianceColor.s[2] = parB;
}


void TParticleSystem::setMeanDuration(cl_float parMeanDuration)
{
	FMeanDuration = parMeanDuration;
}

void TParticleSystem::setDurationVariance(cl_float parDurationVariance)
{
	FDurationVariance = parDurationVariance;
}

void TParticleSystem::setRotation(cl_float parRotation)
{
	FRotation = parRotation;
}

bool TParticleSystem::initParticleSystem()
{
    compileProgram("data/kernels/particleSystem.cl", FProgram);
    initKernel = clCreateKernel(FProgram.program, "initParticles", &CL_ERROR_FLAG);
    if (!initKernel || CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in init kernel "<<CL_ERROR_FLAG);
        return FAILURE;
    }
	FPosBuffer = clCreateBuffer(clData.context,  CL_MEM_READ_WRITE,  sizeof(cl_float4) * FNbParticles, NULL, NULL);
	FColorBuffer = clCreateBuffer(clData.context,  CL_MEM_READ_WRITE,  sizeof(cl_float4) * FNbParticles, NULL, NULL);
    FVelocBuffer = clCreateBuffer(clData.context,  CL_MEM_READ_WRITE,  sizeof(cl_float4) * FNbParticles, NULL, NULL);
	FLTBuffer = clCreateBuffer(clData.context,  CL_MEM_READ_WRITE,  sizeof(cl_float) * FNbParticles, NULL, NULL);

    // Copy data to input buffer
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in copy.");
        return FAILURE;
    }
    CL_ERROR_FLAG  = clSetKernelArg(initKernel, 0, sizeof(cl_mem), &FPosBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 1, sizeof(cl_mem), &FColorBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 2, sizeof(cl_mem), &FVelocBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 3, sizeof(cl_mem), &FLTBuffer);

    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 4, sizeof(cl_float4), &FOriginPosition);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 5, sizeof(cl_float4), &FMeanColor);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 6, sizeof(cl_float4), &FVarianceColor);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 7, sizeof(cl_float), &FMeanDuration);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 8, sizeof(cl_float), &FDurationVariance);
    CL_ERROR_FLAG |= clSetKernelArg(initKernel, 9, sizeof(uint), &FNbParticles);
    CheckGLState("Pre create0");

    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in argument.");
        return FAILURE;
    }
    PRINT_ORANGE("Parameters injected");

    size_t local = CL_LOCAL_DIM;
    size_t global = std::min((int)FNbParticles, (int)CL_GLOBAL_DIM);
    CL_ERROR_FLAG = clEnqueueNDRangeKernel(clData.commands, initKernel, 1, NULL, &global , &local, 0, NULL, NULL);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in pushing to queue "<<CL_ERROR_FLAG);
        return FAILURE;
    }
    
    PRINT_ORANGE("Waiting finish.");

    clFinish(clData.commands);

    PRINT_ORANGE("Kernel done.");
    // Is done now
    CL_ERROR_FLAG = clEnqueueReadBuffer( clData.commands, FPosBuffer, CL_TRUE, 0, sizeof(cl_float4) * FNbParticles, FPositions, 0, NULL, NULL );  
    CL_ERROR_FLAG |= clEnqueueReadBuffer( clData.commands, FColorBuffer, CL_TRUE, 0, sizeof(cl_float4) * FNbParticles, FColors, 0, NULL, NULL );  
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
       PRINT_RED("Error in reading back.");
       return FAILURE;
    }

    updateKernel = clCreateKernel(FProgram.program, "update", &CL_ERROR_FLAG);
	CL_ERROR_FLAG  = clSetKernelArg(updateKernel, 0, sizeof(cl_mem), &FPosBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 1, sizeof(cl_mem), &FLTBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 2, sizeof(cl_mem), &FVelocBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 3, sizeof(cl_mem), &FColorBuffer);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 4, sizeof(cl_float4), &FOriginPosition);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 5, sizeof(cl_float4), &FMeanColor);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 6, sizeof(cl_float4), &FVarianceColor);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 7, sizeof(cl_float), &FMeanDuration);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 8, sizeof(cl_float), &FDurationVariance);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 9, sizeof(uint), &FNbParticles);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in argument.");
        return FAILURE;
    }
    PRINT_ORANGE("Init is done.");
    FShader = CreateShader("data/shaders/ptVertex.glsl", "data/shaders/ptFragment.glsl");
	glGenVertexArrays (1, &FVAO);
	glBindVertexArray (FVAO);
	
	glGenBuffers(1, &FVBO);
	glBindBuffer(GL_ARRAY_BUFFER, FVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cl_float4)*FNbParticles*2, FPositions, GL_DYNAMIC_DRAW);
	GLuint posAtt = glGetAttribLocation(FShader.FProgramID, "position");
	GLuint colorAtt = glGetAttribLocation(FShader.FProgramID, "color");
	glEnableVertexAttribArray (posAtt);
	glEnableVertexAttribArray (colorAtt);
	glVertexAttribPointer (posAtt, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer (colorAtt, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(cl_float4)*FNbParticles));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);
    return SUCCESS;
}

void TParticleSystem::update(float parTime)
{
    // CheckGLState("Preupdate");
    FSeed += 1000;
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 10, sizeof(parTime), &parTime);
    CL_ERROR_FLAG |= clSetKernelArg(updateKernel, 11, sizeof(FSeed), &FSeed);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in argument.");
        return;
    }
    size_t local = CL_LOCAL_DIM;
    size_t global = std::min((int)FNbParticles, (int)CL_GLOBAL_DIM);
    CL_ERROR_FLAG = clEnqueueNDRangeKernel(clData.commands, updateKernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in pushing to queue "<<CL_ERROR_FLAG);
        return;
    }
    clFinish(clData.commands);

    CL_ERROR_FLAG = clEnqueueReadBuffer( clData.commands, FPosBuffer, CL_TRUE, 0, sizeof(cl_float4) * FNbParticles, FPositions, 0, NULL, NULL );  
    CL_ERROR_FLAG = clEnqueueReadBuffer( clData.commands, FColorBuffer, CL_TRUE, 0, sizeof(cl_float4) * FNbParticles, FColors, 0, NULL, NULL );  

    glBindBuffer(GL_ARRAY_BUFFER, FVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cl_float4)*FNbParticles*2, FPositions, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void TParticleSystem::draw(const TMatrix4<double>& parProjectionView)
{
	glUseProgram(FShader.FProgramID);
  	glBindVertexArray (FVAO);
	float mat[16];
	parProjectionView.toTable(&mat[0]);
    glUniformMatrix4fv(glGetUniformLocation(FShader.FProgramID, "projectionview"),1,true, mat);
    glDrawArrays(GL_POINTS, 0, FNbParticles);
	glUseProgram(0);
}
