#pragma once

typedef struct 
{
	const char* shaderNormal;
	const char* shaderPixel;
	const char* shaderPhong;
	const char* shaderCustom1;
	const char* shaderCustom2;
}shaderList, *ptr_shaderList;

static const char* textFileRead(const char *fileName);

static unsigned int compileShaders(unsigned int type, const char* src);

unsigned int createShaders();
