#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include "shader.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name);
	static Shader GetShader(std::string name);
	static void Clear();
private:
	ResourceManager() { }
	static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
};

#endif