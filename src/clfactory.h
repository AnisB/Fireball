#ifndef CL_FACTORY
#define CL_FACTORY

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <cl.h>
#endif

#include <string>

extern cl_int CL_ERROR_FLAG;
extern cl_int CL_LOCAL_DIM;
extern cl_int CL_GLOBAL_DIM;



struct TOpenCLData
{
	cl_device_id device_id;
	cl_context context;
	cl_command_queue commands;
};

struct TOpenCLProgram
{
	cl_program program;
};

bool initCLContext(TOpenCLData& parCLData);
bool compileProgram(TOpenCLData& parCLData, const std::string& parFileName, TOpenCLProgram& parProgram);

#endif // CL_FACTORY