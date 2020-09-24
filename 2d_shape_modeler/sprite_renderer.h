#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "model.h"
#include "shader.h"

class SpriteRenderer
{
public:
	//constructor (initialize shaders/shapes)
	SpriteRenderer(Shader &shader);
	//destructor
	~SpriteRenderer();
	//initialize and configure the object's buffer and vertex attributes
	void InitRenderData();
	//render an object with proper coloring
	void Draw();
private:
	//render state
	Shader shader;
	GLuint gridVAO, lineVAO, pointVAO;
	//initialize the VBOs, VAOs, and EBOs
	void initBufferObjects();
};

#endif // !SPRITE_RENDERER
