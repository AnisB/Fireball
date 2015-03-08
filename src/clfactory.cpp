#include "clfactory.h"

#include "debug.h"

#include <fstream>


cl_int CL_ERROR_FLAG;
cl_int CL_LOCAL_DIM = 1;
cl_int CL_GLOBAL_DIM;

bool initCLContext(TOpenCLData& parCLData)
{
	// Initiating opencl
	CL_ERROR_FLAG = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 1, &parCLData.device_id, NULL);
    if (CL_ERROR_FLAG != CL_SUCCESS)
    {
        PRINT_RED("Error in device.");
        return FAILURE;
    }
    parCLData.context = clCreateContext(0, 1, &parCLData.device_id, NULL, NULL, &CL_ERROR_FLAG);
  	if (!parCLData.context)
    {
        PRINT_RED("Error in context.");
        return FAILURE;
    }
    parCLData.commands = clCreateCommandQueue(parCLData.context, parCLData.device_id, 0, &CL_ERROR_FLAG);
    if (!parCLData.commands)
    {
        PRINT_RED("Error in command queue.");
        return FAILURE;
    }
    size_t dim;
    clGetDeviceInfo(parCLData.device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), (void*)&dim, NULL);
    CL_GLOBAL_DIM = dim;
    PRINT_GREEN("Successful opencl init ");
    return true;
}


bool compileProgram(TOpenCLData& parCLData, const std::string& parFileName, TOpenCLProgram& parProgram)
{
	std::ifstream in(parFileName);
	std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    const char* kernelSource = contents.c_str();
    parProgram.program = clCreateProgramWithSource(parCLData.context, 1, &kernelSource, NULL, &CL_ERROR_FLAG);
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
        clGetProgramBuildInfo(parProgram.program, parCLData.device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        PRINT_RED(buffer);
        return FAILURE;
    }
    PRINT_GREEN("Successful program compilation.");
    return true;
}