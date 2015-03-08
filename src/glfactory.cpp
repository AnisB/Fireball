#include "glfactory.h"

#include "debug.h"


char * LoadFile( char const* fn) 
{
	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

void CheckShader(GLuint parShaderID, const std::string& parShadePath)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    glGetShaderiv(parShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(parShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

    if(InfoLogLength>1)
    {
        char errorMessage[InfoLogLength];
        glGetShaderInfoLog(parShaderID, InfoLogLength, NULL, errorMessage);
        PRINT_RED( "Shader error:"<<parShadePath<<" "<<parShaderID);
        PRINT_RED( errorMessage );
    }
}


bool CheckProgram(GLuint parProgramID)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    glGetProgramiv(parProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(parProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if(InfoLogLength>1)
    {   
        char errorMessage[InfoLogLength];
        glGetProgramInfoLog(parProgramID, InfoLogLength, NULL, errorMessage);
        PRINT_RED("Program linking error: "<<std::endl<<errorMessage );
        return false;
    }
    return true;
}


TShader CreateShader(const std::string& parVertex, const std::string& parFragment)
{
	TShader shader;
	shader.FProgramID = glCreateProgram();

	char * vsFile = NULL;

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	vsFile = LoadFile(parVertex.c_str());

	const char * vsFile_ptr = vsFile;
	glShaderSource(vertexShader, 1, (const char **)&vsFile, NULL);

	free(vsFile);
	glCompileShader(vertexShader);
	CheckShader(vertexShader, parVertex);
	glAttachShader(shader.FProgramID,vertexShader);	

	char * fsFile = NULL;
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fsFile = LoadFile(parFragment.c_str());
	const char * fsFile_ptr = fsFile;
	glShaderSource(fragmentShader, 1, (const char **)&fsFile, NULL);
	free(fsFile);
	glCompileShader(fragmentShader);
	CheckShader(fragmentShader, parFragment);
	glAttachShader(shader.FProgramID,fragmentShader);

	glLinkProgram(shader.FProgramID);
	CheckProgram(shader.FProgramID);

	return shader;
}