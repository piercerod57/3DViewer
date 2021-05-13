#include <GL/glew.h>
#include <windows.h>
#include <fstream>
#include "Shaders.h"

static const char* textFileRead(const char *fileName) {
	char* text;

	if (fileName != NULL) 
	{
		FILE *file = fopen(fileName, "r");

		if (file != NULL) 
		{
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);
			if (count > 0) 
			{
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

//shaderNormal, shaderPixel, shaderPhong, shaderCustom1, shaderCustom2

static unsigned int compileShaders(unsigned int type, const char* src)
{
	unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	//TODO: error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		OutputDebugString(L"Shader failed to compile:\n");
		OutputDebugStringA(message);

		glDeleteShader(id);
		return 0;
	}


	return id;
}

unsigned int createShaders()
{
	unsigned int program = glCreateProgram();

	const char* shaderNormalSrc = textFileRead("./shaders/shaderNormal.txt");
	const char* shaderPixelSrc = textFileRead("./shaders/shaderPixel.txt");
	//const char* shaderPhongSrc = textFileRead("./shaders/shaderPhong.txt");
	//const char* shaderCustom1Src = textFileRead("./shaders/shaderCustom1.txt");
	//const char* shaderCustom2Src = textFileRead("./shaders/shaderCustom2.txt");

	unsigned int shaderNormalID = compileShaders(GL_VERTEX_SHADER, shaderNormalSrc);
	unsigned int shaderPixelID = compileShaders(GL_FRAGMENT_SHADER, shaderPixelSrc);
	//unsigned int shaderPhongID = compileShaders(unsigned int type, shaderPhongSrc);
	//unsigned int shaderCustom1ID = compileShaders(GL_VERTEX_SHADER, shaderCustom1Src);
	//unsigned int shaderCustom2ID = compileShaders(GL_FRAGMENT_SHADER, shaderCustom2Src);

	glAttachShader(program, shaderNormalID);
	glAttachShader(program, shaderPixelID);
	//glAttachShader(program, shaderPhongID);
	//glAttachShader(program, shaderCustom1ID);
	//glAttachShader(program, shaderCustom2ID);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(shaderNormalID);
	glDeleteShader(shaderPixelID);
	//glDeleteShader(shaderPhongID);
	//glDeleteShader(shaderCustom1ID);
	//glDeleteShader(shaderCustom2ID);

	return(program);
}
