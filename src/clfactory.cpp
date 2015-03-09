#include "clfactory.h"

#include "debug.h"

#include <fstream>


cl_int CL_ERROR_FLAG;
cl_int CL_LOCAL_DIM = 1;
cl_int CL_GLOBAL_DIM;

TOpenCLData clData;

bool initCLContext()
{
	// Initiating opencl
	CL_ERROR_FLAG = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 1, &clData.device_id, NULL);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in device.");
        return FAILURE;
    }
    clData.context = clCreateContext(0, 1, &clData.device_id, NULL, NULL, &CL_ERROR_FLAG);
  	if (!clData.context)
    {
        PRINT_RED("Error in context.");
        return FAILURE;
    }
    clData.commands = clCreateCommandQueue(clData.context, clData.device_id, 0, &CL_ERROR_FLAG);
    if (!clData.commands)
    {
        PRINT_RED("Error in command queue.");
        return FAILURE;
    }
    size_t dim;
    clGetDeviceInfo(clData.device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*)&dim, NULL);
    CL_GLOBAL_DIM = dim;
    PRINT_GREEN("Successful opencl init ");
    return true;
}


bool compileProgram(const std::string& parFileName, TOpenCLProgram& parProgram)
{
	std::ifstream in(parFileName);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const char* kernelSource = contents.c_str();
    parProgram.program = clCreateProgramWithSource(clData.context, 1, &kernelSource, NULL, &CL_ERROR_FLAG);
    if (!parProgram.program)
    {
        PRINT_RED("Error in program.");
        return FAILURE;
    }    
	CL_ERROR_FLAG = clBuildProgram(parProgram.program, 0, NULL, NULL, NULL, NULL);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];
        std::cout<<"Error in compiling the opencl program."<<std::endl;
        clGetProgramBuildInfo(parProgram.program, clData.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        PRINT_RED(buffer);
        return FAILURE;
    }
    PRINT_GREEN("Successful program compilation.");
    return true;
}