#include "pch.h"
#include "sprite_renderer.h"
#include <iostream>

GLuint lineVBO[2], pointVBO[2], gridVBO[2];

SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	this->initBufferObjects();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &this->gridVAO);
	glDeleteVertexArrays(1, &this->lineVAO);
	glDeleteVertexArrays(1, &this->pointVAO);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteRenderer::initBufferObjects()
{
	glGenVertexArrays(1, &this->gridVAO);
	glBindVertexArray(this->gridVAO);
	glGenBuffers(2, gridVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.GridLines.size(), &modelData.GridLines[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gridVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.GridColors.size(), &modelData.GridColors[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &this->lineVAO);
	glBindVertexArray(this->lineVAO);
	glGenBuffers(2, lineVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.Vertices.size(), &modelData.Vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.LineColors.size(), &modelData.LineColors[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &this->pointVAO);
	glBindVertexArray(this->pointVAO);
	glGenBuffers(2, pointVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.PointVertices.size(), &modelData.PointVertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pointVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.PointColors.size(), &modelData.PointColors[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(1);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}

void SpriteRenderer::InitRenderData()
{
	if (modelData.BufferSelectVertices || modelData.BufferDeselectVertices || modelData.BufferAddVertices || modelData.BufferMoveVertex || modelData.BufferAddShape || modelData.BufferDeleteVertices || modelData.BufferReset)
	{
		glDeleteVertexArrays(1, &this->lineVAO);
		glDeleteVertexArrays(1, &this->pointVAO);
		glDeleteBuffers(2, lineVBO);
		glDeleteBuffers(2, pointVBO);

		glGenVertexArrays(1, &this->lineVAO);
		glBindVertexArray(this->lineVAO);
		glGenBuffers(2, lineVBO);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.Vertices.size(), &modelData.Vertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.LineColors.size(), &modelData.LineColors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);

		glGenVertexArrays(1, &this->pointVAO);
		glBindVertexArray(this->pointVAO);
		glGenBuffers(2, pointVBO);
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.PointVertices.size(), &modelData.PointVertices[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.PointColors.size(), &modelData.PointColors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);
	}

	else if (modelData.UpdateGrid)
	{
		glDeleteVertexArrays(1, &this->gridVAO);
		glDeleteBuffers(2, gridVBO);

		glGenVertexArrays(1, &this->gridVAO);
		glBindVertexArray(this->gridVAO);
		glGenBuffers(2, gridVBO);
		glBindBuffer(GL_ARRAY_BUFFER, gridVBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.GridLines.size(), &modelData.GridLines[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, gridVBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * modelData.GridColors.size(), &modelData.GridColors[0], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(1);

		modelData.UpdateGrid = GL_FALSE;
	}
}

void SpriteRenderer::Draw()
{
	this->shader.Use();
	if (modelData.Grid)
	{
		glBindVertexArray(this->gridVAO);
		glDrawArrays(GL_LINES, 0, modelData.GridLines.size());
		glBindVertexArray(0);
	}
	glBindVertexArray(this->lineVAO);
	glDrawArrays(GL_LINES, 0, modelData.Vertices.size());
	glBindVertexArray(0);
	glBindVertexArray(this->pointVAO);
	glDrawArrays(GL_POINTS, 0, modelData.PointVertices.size());
	glBindVertexArray(0);
}
