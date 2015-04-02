#include "ShaderLoader.h"

#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

/*
ShaderLoader::ShaderLoader()
{

}

ShaderLoader::~ShaderLoader()
{

}
*/

unsigned int LoadShaderProgram(unsigned int type, const char* path) 
{
	FILE* file = fopen(path, "rb");
	if (file == nullptr)
	{
		return 0;
	}
	/* read the shader source*/
	fseek(file, 0, SEEK_END);
	unsigned int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* source = new char[length + 1];
	memset(source, 0, length + 1);
	fread(source, sizeof(char), length, file);
	fclose(file);

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	delete[] source;

	return shader;
}