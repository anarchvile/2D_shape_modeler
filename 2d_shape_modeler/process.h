#ifndef PROCESS_H
#define PROCESS_H

#include "resource_manager.h"
#include "sprite_renderer.h"

class Process
{
public:
	Shader spriteShader;

	Process();
	~Process();
	void Init();
	void Update(std::vector<GLboolean> &vertexSelected, glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition, glm::vec2 &initialRightMousePosition, glm::vec2 &initialMiddleMousePosition);
	void Render();
};

#endif