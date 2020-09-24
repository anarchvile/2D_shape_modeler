#include "pch.h"
#include "model.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <istream>
#include <fstream>

ModelData modelData;
Model model;

GLfloat vertices[] =
{
	150.0f, 150.0f, //line 0
	150.0f, 350.0f,
	150.0f, 350.0f, //line 1
	350.0f, 350.0f,
	350.0f, 350.0f, //line 2
	350.0f, 150.0f,
	350.0f, 150.0f, //line 3
	150.0f, 150.0f,
};

GLfloat newVertices[] =
{
	200.0f, 200.0f, //line 0
	200.0f, 300.0f,
	200.0f, 300.0f, //line 1
	300.0f, 300.0f,
	300.0f, 300.0f, //line 2
	300.0f, 200.0f,
	300.0f, 200.0f, //line 3
	200.0f, 200.0f
};

GLfloat minimumDistance = 1000.0f;
GLint closestVector;
GLboolean selectMultipleVertices = GL_FALSE;
std::vector<GLint> vertexID, distance;
GLfloat n = 10.0f;

Model::Model()
{
	this->initModelData();
}

Model::~Model()
{

}

void Model::SelectVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition)
{
	if (modelData.LeftMouseButtonPressed)
	{
		modelData.ShiftLeftMouseButtonPressed = modelData.ShiftLeftMouseButtonDown = modelData.ShiftLeftMouseButtonUp = GL_FALSE;
		modelData.CtrlLeftMouseButtonPressed = modelData.CtrlLeftMouseButtonDown = modelData.CtrlLeftMouseButtonUp = GL_FALSE;
		modelData.AltLeftMouseButtonPressed = modelData.AltLeftMouseButtonDown = modelData.AltLeftMouseButtonUp = GL_FALSE;
		modelData.BoundingBoxAdd = GL_TRUE;
		modelData.BoundingBoxDelete = GL_FALSE;

		vertexID.clear();
		for (int i = 0; i < (int)modelData.VertexSelected.size(); ++i) modelData.VertexSelected[i] = GL_FALSE;

		for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i)
		{
			if (leftMousePosition.x >= modelData.Vertices[2 * i] - 10 && leftMousePosition.x <= modelData.Vertices[2 * i] + 10 && leftMousePosition.y >= modelData.Vertices[2 * i + 1] - 10 && leftMousePosition.y <= modelData.Vertices[2 * i + 1] + 10)
			{
				modelData.VertexSelected[i] = GL_TRUE;
				modelData.BoundingBoxAdd = GL_FALSE;
				vertexID.push_back(i);
			}
		}

		modelData.BufferSelectVertices = GL_TRUE;
		modelData.LeftMouseButtonPressed = GL_FALSE;
		modelData.LeftMouseButtonDown = GL_TRUE;
	}

	else if (modelData.LeftMouseButtonDown && modelData.BoundingBoxAdd)
	{
		model.instantiateBoundingBox(initialLeftMousePosition, leftMousePosition);
	}

	else if (modelData.LeftMouseButtonUp && modelData.BoundingBoxAdd)
	{
		model.destroyBoundingBox();
		modelData.LeftMouseButtonUp = GL_FALSE;
	}

	else if (modelData.AltLeftMouseButtonPressed)
	{
		distance.clear();

		for (int i = 0; i < (int)(0.5f * vertexID.size()); ++i)
		{
			if (modelData.Vertices[2 * vertexID[2 * i]] != modelData.Vertices[vertexID[2 * i + 1]] && modelData.Vertices[2 * vertexID[2 * i] + 1] != modelData.Vertices[2 * vertexID[2 * i + 1] + 1])
			{
				for (int j = 0; j < (int)(0.5f * vertexID.size()); ++j)
				{
					if (i != j && modelData.Vertices[2 * vertexID[2 * i]] == modelData.Vertices[2 * vertexID[2 * j]] && modelData.Vertices[2 * vertexID[2 * i] + 1] == modelData.Vertices[2 * vertexID[2 * j] + 1])
					{
						std::swap(vertexID[2 * i + 1], vertexID[2 * j]);
					}

					else if (i != j && modelData.Vertices[2 * vertexID[2 * i]] == modelData.Vertices[2 * vertexID[2 * j + 1]] && modelData.Vertices[2 * vertexID[2 * i] + 1] == modelData.Vertices[2 * vertexID[2 * j + 1] + 1])
					{
						std::swap(vertexID[2 * i + 1], vertexID[2 * j + 1]);
					}

				}
			}
		}

		for (int i = 0; i < (int)(0.5f * vertexID.size()); ++i)
		{
			if (modelData.SnapToGrid)
			{
				distance.push_back((GLint)(round((initialLeftMousePosition.x - modelData.Vertices[2 * vertexID[2 * i]]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth)));
				distance.push_back((GLint)(round((initialLeftMousePosition.y - modelData.Vertices[2 * vertexID[2 * i] + 1]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth)));
			}

			else
			{
				distance.push_back(GLint(initialLeftMousePosition.x - modelData.Vertices[2 * vertexID[2 * i]]));
				distance.push_back(GLint(initialLeftMousePosition.y - modelData.Vertices[2 * vertexID[2 * i] + 1]));
			}
		}
		modelData.AltLeftMouseButtonPressed = GL_FALSE;
		modelData.AltLeftMouseButtonDown = GL_TRUE;
	}

	else modelData.BufferSelectVertices = GL_FALSE;
}

void Model::DeselectVertices()
{
	if (modelData.Deselect)
	{
		modelData.Deselect = GL_FALSE;
		modelData.BufferDeselectVertices = GL_TRUE;
		distance.clear();
		vertexID.clear();
		for (int i = 0; i < (int)modelData.VertexSelected.size(); ++i) modelData.VertexSelected[i] = GL_FALSE;
	}

	else modelData.BufferDeselectVertices = GL_FALSE;
}

void Model::AddToSelectedVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition)
{
	if (modelData.ShiftLeftMouseButtonPressed)
	{
		modelData.LeftMouseButtonPressed = modelData.LeftMouseButtonDown = modelData.LeftMouseButtonUp = GL_FALSE;
		modelData.CtrlLeftMouseButtonPressed = modelData.CtrlLeftMouseButtonDown = modelData.CtrlLeftMouseButtonUp = GL_FALSE;
		modelData.BoundingBoxAdd = GL_TRUE;
		modelData.BoundingBoxDelete = GL_FALSE;

		for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i)
		{
			if (leftMousePosition.x >= modelData.Vertices[2 * i] - 10 && leftMousePosition.x <= modelData.Vertices[2 * i] + 10 && leftMousePosition.y >= modelData.Vertices[2 * i + 1] - 10 && leftMousePosition.y <= modelData.Vertices[2 * i + 1] + 10)
			{
				modelData.VertexSelected[i] = GL_TRUE;
				modelData.BoundingBoxAdd = GL_FALSE;
				vertexID.push_back(i);
			}
		}

		modelData.ShiftLeftMouseButtonPressed = GL_FALSE;
		modelData.ShiftLeftMouseButtonDown = GL_TRUE;
		modelData.BufferSelectVertices = GL_TRUE;
	}

	else if (modelData.ShiftLeftMouseButtonDown && modelData.BoundingBoxAdd)
	{
		model.instantiateBoundingBox(initialLeftMousePosition, leftMousePosition);
	}

	else if (modelData.ShiftLeftMouseButtonUp && modelData.BoundingBoxAdd)
	{
		model.destroyBoundingBox();
		modelData.ShiftLeftMouseButtonUp = GL_FALSE;
	}
}

void Model::RemoveSelectedVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition)
{
	if (modelData.CtrlLeftMouseButtonPressed)
	{
		modelData.LeftMouseButtonPressed = modelData.LeftMouseButtonDown = modelData.LeftMouseButtonUp = GL_FALSE;
		modelData.ShiftLeftMouseButtonPressed = modelData.ShiftLeftMouseButtonDown = modelData.ShiftLeftMouseButtonUp = GL_FALSE;
		modelData.BoundingBoxAdd = GL_FALSE;
		modelData.BoundingBoxDelete = GL_TRUE;

		for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i)
		{
			if (leftMousePosition.x >= modelData.Vertices[2 * i] - 10 && leftMousePosition.x <= modelData.Vertices[2 * i] + 10 && leftMousePosition.y >= modelData.Vertices[2 * i + 1] - 10 && leftMousePosition.y <= modelData.Vertices[2 * i + 1] + 10)
			{
				modelData.VertexSelected[i] = GL_FALSE;
				modelData.BoundingBoxDelete = GL_FALSE;
				vertexID.erase(vertexID.begin() + i);
			}
		}

		modelData.CtrlLeftMouseButtonPressed = GL_FALSE;
		modelData.CtrlLeftMouseButtonDown = GL_TRUE;
		modelData.BufferSelectVertices = GL_TRUE;
	}

	else if (modelData.CtrlLeftMouseButtonDown && modelData.BoundingBoxDelete)
	{
		model.instantiateBoundingBox(initialLeftMousePosition, leftMousePosition);
	}

	else if (modelData.CtrlLeftMouseButtonUp && modelData.BoundingBoxDelete)
	{
		model.destroyBoundingBox();
		modelData.CtrlLeftMouseButtonUp = GL_FALSE;
	}
}

void Model::MoveVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition)
{
	if (modelData.LeftMouseButtonDown)
	{
		if (vertexID.size() != 0)
		{
			if (modelData.VertexSelected[vertexID[0]] == GL_TRUE || modelData.VertexSelected[vertexID[1]] == GL_TRUE)
			{
				if (modelData.SnapToGrid)
				{
					glm::vec2 roundedLeftMousePosition = glm::vec2(round(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth), round(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));

					modelData.Vertices[2 * vertexID[0]] = modelData.PointVertices[2 * vertexID[0]] = roundedLeftMousePosition.x;
					modelData.Vertices[2 * vertexID[1]] = modelData.PointVertices[2 * vertexID[1]] = roundedLeftMousePosition.x;
					modelData.Vertices[2 * vertexID[0] + 1] = modelData.PointVertices[2 * vertexID[0] + 1] = roundedLeftMousePosition.y;
					modelData.Vertices[2 * vertexID[1] + 1] = modelData.PointVertices[2 * vertexID[1] + 1] = roundedLeftMousePosition.y;

					//prevent vertex overlap by offsetting any point(s) that are translated to another point's position
					for (int j = 0; j < (int)(0.5f * modelData.Vertices.size()); ++j)
					{
						if (j != vertexID[0] && j != vertexID[1] && modelData.Vertices[2 * vertexID[0]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[1]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[0] + 1] == modelData.Vertices[2 * j + 1] && modelData.Vertices[2 * vertexID[1] + 1] == modelData.Vertices[2 * j + 1])
						{
							glm::vec2 leftMousePositionDecimal = glm::vec2(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth) - round(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)), leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight) - round(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)));

							if (leftMousePositionDecimal.x >= 0)
							{
								modelData.Vertices[2 * vertexID[0]] = modelData.PointVertices[2 * vertexID[0]] = (GLfloat)(ceil(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
								modelData.Vertices[2 * vertexID[1]] = modelData.PointVertices[2 * vertexID[1]] = (GLfloat)(ceil(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
							}

							else if (leftMousePositionDecimal.x < 0)
							{
								modelData.Vertices[2 * vertexID[0]] = modelData.PointVertices[2 * vertexID[0]] = (GLfloat)(floor(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
								modelData.Vertices[2 * vertexID[1]] = modelData.PointVertices[2 * vertexID[1]] = (GLfloat)(floor(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
							}

							if (leftMousePositionDecimal.y >= 0)
							{
								modelData.Vertices[2 * vertexID[0] + 1] = modelData.PointVertices[2 * vertexID[0] + 1] = (GLfloat)(ceil(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
								modelData.Vertices[2 * vertexID[1] + 1] = modelData.PointVertices[2 * vertexID[1] + 1] = (GLfloat)(ceil(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
							}

							else if (leftMousePositionDecimal.y < 0)
							{
								modelData.Vertices[2 * vertexID[0] + 1] = modelData.PointVertices[2 * vertexID[0] + 1] = (GLfloat)(floor(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
								modelData.Vertices[2 * vertexID[1] + 1] = modelData.PointVertices[2 * vertexID[1] + 1] = (GLfloat)(floor(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
							}
						}

					}
				}

				else
				{
					glm::vec2 leftMousePositionDecimal = glm::vec2(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth) - round(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)), leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight) - round(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)));

					modelData.Vertices[2 * vertexID[0]] = modelData.PointVertices[2 * vertexID[0]] = leftMousePosition.x;
					modelData.Vertices[2 * vertexID[1]] = modelData.PointVertices[2 * vertexID[1]] = leftMousePosition.x;
					modelData.Vertices[2 * vertexID[0] + 1] = modelData.PointVertices[2 * vertexID[0] + 1] = leftMousePosition.y;
					modelData.Vertices[2 * vertexID[1] + 1] = modelData.PointVertices[2 * vertexID[1] + 1] = leftMousePosition.y;

					for (int j = 0; j < (int)(0.5f * modelData.Vertices.size()); ++j)
					{
						if (j != vertexID[0] && j != vertexID[1] && modelData.Vertices[2 * vertexID[0]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[1]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[0] + 1] == modelData.Vertices[2 * j + 1] && modelData.Vertices[2 * vertexID[1] + 1] == modelData.Vertices[2 * j + 1])
						{
							modelData.Vertices[2 * vertexID[0]] += 1;
							modelData.Vertices[2 * vertexID[1]] += 1;
							modelData.Vertices[2 * vertexID[0] + 1] += 1;
							modelData.Vertices[2 * vertexID[1] + 1] += 1;

							modelData.PointVertices[2 * vertexID[0]] += 1;
							modelData.PointVertices[2 * vertexID[1]] += 1;
							modelData.PointVertices[2 * vertexID[0] + 1] += 1;
							modelData.PointVertices[2 * vertexID[1] + 1] += 1;
						}
					}
				}
			}

			modelData.BufferMoveVertex = GL_TRUE;
		}
	}

	else if (modelData.AltLeftMouseButtonDown)
	{
		if (vertexID.size() != 0)
		{
			for (int i = 0; i < (int)(0.5f * vertexID.size()); ++i)
			{
				if (modelData.SnapToGrid)
				{
					glm::vec2 roundedDistance = glm::vec2(round((leftMousePosition.x - distance[2 * i]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth), round((leftMousePosition.y - distance[2 * i + 1]) / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));

					modelData.Vertices[2 * vertexID[2 * i]] = modelData.PointVertices[2 * vertexID[2 * i]] = roundedDistance.x;
					modelData.Vertices[2 * vertexID[2 * i + 1]] = modelData.PointVertices[2 * vertexID[2 * i + 1]] = roundedDistance.x;
					modelData.Vertices[2 * vertexID[2 * i] + 1] = modelData.PointVertices[2 * vertexID[2 * i] + 1] = roundedDistance.y;
					modelData.Vertices[2 * vertexID[2 * i + 1] + 1] = modelData.PointVertices[2 * vertexID[2 * i + 1] + 1] = roundedDistance.y;

					for (int j = 0; j < (int)(0.5f * modelData.Vertices.size()); ++j)
					{
						if (j != vertexID[2 * i] && j != vertexID[2 * i + 1] && modelData.Vertices[2 * vertexID[2 * i]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[2 * i + 1]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[2 * i] + 1] == modelData.Vertices[2 * j + 1] && modelData.Vertices[2 * vertexID[2 * i + 1] + 1] == modelData.Vertices[2 * j + 1])
						{
							glm::vec2 leftMousePositionDecimal = glm::vec2(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth) - round(leftMousePosition.x / (modelData.GridScale * modelData.WindowWidth)), leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight) - round(leftMousePosition.y / (modelData.GridScale * modelData.WindowHeight)));
							if (leftMousePositionDecimal.x >= 0)
							{
								modelData.Vertices[2 * vertexID[2 * i]] = modelData.PointVertices[2 * vertexID[2 * i]] = (GLfloat)(ceil((leftMousePosition.x - distance[2 * i]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
								modelData.Vertices[2 * vertexID[2 * i + 1]] = modelData.PointVertices[2 * vertexID[2 * i + 1]] = (GLfloat)(ceil((leftMousePosition.x - distance[2 * i]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
							}

							else if (leftMousePositionDecimal.x < 0)
							{
								modelData.Vertices[2 * vertexID[2 * i]] = modelData.PointVertices[2 * vertexID[2 * i]] = (GLfloat)(floor((leftMousePosition.x - distance[2 * i]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
								modelData.Vertices[2 * vertexID[2 * i + 1]] = modelData.PointVertices[2 * vertexID[2 * i + 1]] = (GLfloat)(floor((leftMousePosition.x - distance[2 * i]) / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth));
							}

							if (leftMousePositionDecimal.y >= 0)
							{
								modelData.Vertices[2 * vertexID[2 * i] + 1] = modelData.PointVertices[2 * vertexID[2 * i] + 1] = (GLfloat)(ceil((leftMousePosition.y - distance[2 * i + 1]) / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
								modelData.Vertices[2 * vertexID[2 * i + 1] + 1] = modelData.PointVertices[2 * vertexID[2 * i + 1] + 1] = (GLfloat)(ceil((leftMousePosition.y - distance[2 * i + 1]) / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
							}

							else if (leftMousePositionDecimal.y < 0)
							{
								modelData.Vertices[2 * vertexID[2 * i] + 1] = modelData.PointVertices[2 * vertexID[2 * i] + 1] = (GLfloat)(floor((leftMousePosition.y - distance[2 * i + 1]) / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
								modelData.Vertices[2 * vertexID[2 * i + 1] + 1] = modelData.PointVertices[2 * vertexID[2 * i + 1] + 1] = (GLfloat)(floor((leftMousePosition.y - distance[2 * i + 1]) / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight));
							}
						}
					}
				}

				else
				{
					modelData.Vertices[2 * vertexID[2 * i]] = modelData.PointVertices[2 * vertexID[2 * i]] = leftMousePosition.x - distance[2 * i];
					modelData.Vertices[2 * vertexID[2 * i + 1]] = modelData.PointVertices[2 * vertexID[2 * i + 1]] = leftMousePosition.x - distance[2 * i];
					modelData.Vertices[2 * vertexID[2 * i] + 1] = modelData.PointVertices[2 * vertexID[2 * i] + 1] = leftMousePosition.y - distance[2 * i + 1];
					modelData.Vertices[2 * vertexID[2 * i + 1] + 1] = modelData.PointVertices[2 * vertexID[2 * i + 1] + 1] = leftMousePosition.y - distance[2 * i + 1];

					for (int j = 0; j < (int)(0.5f * modelData.Vertices.size()); ++j)
					{
						if (j != vertexID[2 * i] && j != vertexID[2 * i + 1] && modelData.Vertices[2 * vertexID[2 * i]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[2 * i + 1]] == modelData.Vertices[2 * j] && modelData.Vertices[2 * vertexID[2 * i] + 1] == modelData.Vertices[2 * j + 1] && modelData.Vertices[2 * vertexID[2 * i + 1] + 1] == modelData.Vertices[2 * j + 1])
						{
							modelData.Vertices[2 * vertexID[2 * i]] += 1;
							modelData.Vertices[2 * vertexID[2 * i + 1]] += 1;
							modelData.Vertices[2 * vertexID[2 * i] + 1] += 1;
							modelData.Vertices[2 * vertexID[2 * i + 1] + 1] += 1;

							modelData.PointVertices[2 * vertexID[2 * i]] += 1;
							modelData.PointVertices[2 * vertexID[2 * i + 1]] += 1;
							modelData.PointVertices[2 * vertexID[2 * i] + 1] += 1;
							modelData.PointVertices[2 * vertexID[2 * i + 1] + 1] += 1;
						}
					}
				}
			}

			modelData.BufferMoveVertex = GL_TRUE;
		}
	}

	else modelData.BufferMoveVertex = GL_FALSE;
}

void Model::AddVertices(glm::vec2 &initialRightMousePosition)
{
	if (modelData.RightMouseButtonPressed)
	{
		if (modelData.SnapToGrid)
		{
			glm::vec2 roundedInitialRightMousePosition = glm::vec2((float)(round(initialRightMousePosition.x / (modelData.GridScale * modelData.WindowWidth)) * (modelData.GridScale * modelData.WindowWidth)), (float)(round(initialRightMousePosition.y / (modelData.GridScale * modelData.WindowHeight)) * (modelData.GridScale * modelData.WindowHeight)));
			this->closestLine(roundedInitialRightMousePosition);

			modelData.Vertices.push_back(roundedInitialRightMousePosition.x);
			modelData.Vertices.push_back(roundedInitialRightMousePosition.y);
			modelData.Vertices.push_back(modelData.Vertices[4 * closestVector + 2]);
			modelData.Vertices.push_back(modelData.Vertices[4 * closestVector + 3]);
			modelData.Vertices[4 * closestVector + 2] = roundedInitialRightMousePosition.x;
			modelData.Vertices[4 * closestVector + 3] = roundedInitialRightMousePosition.y;

			modelData.PointVertices.push_back(roundedInitialRightMousePosition.x);
			modelData.PointVertices.push_back(roundedInitialRightMousePosition.y);
			modelData.PointVertices.push_back(modelData.PointVertices[4 * closestVector + 2]);
			modelData.PointVertices.push_back(modelData.PointVertices[4 * closestVector + 3]);
			modelData.PointVertices[4 * closestVector + 2] = roundedInitialRightMousePosition.x;
			modelData.PointVertices[4 * closestVector + 3] = roundedInitialRightMousePosition.y;
		}

		else
		{
			this->closestLine(initialRightMousePosition);

			modelData.Vertices.push_back(initialRightMousePosition.x);
			modelData.Vertices.push_back(initialRightMousePosition.y);
			modelData.Vertices.push_back(modelData.Vertices[4 * closestVector + 2]);
			modelData.Vertices.push_back(modelData.Vertices[4 * closestVector + 3]);
			modelData.Vertices[4 * closestVector + 2] = initialRightMousePosition.x;
			modelData.Vertices[4 * closestVector + 3] = initialRightMousePosition.y;

			modelData.PointVertices.push_back(initialRightMousePosition.x);
			modelData.PointVertices.push_back(initialRightMousePosition.y);
			modelData.PointVertices.push_back(modelData.PointVertices[4 * closestVector + 2]);
			modelData.PointVertices.push_back(modelData.PointVertices[4 * closestVector + 3]);
			modelData.PointVertices[4 * closestVector + 2] = initialRightMousePosition.x;
			modelData.PointVertices[4 * closestVector + 3] = initialRightMousePosition.y;
		}

		modelData.LineColors.push_back(0.0f);
		modelData.LineColors.push_back(1.0f);
		modelData.LineColors.push_back(1.0f);
		modelData.LineColors.push_back(0.0f);
		modelData.LineColors.push_back(1.0f);
		modelData.LineColors.push_back(1.0f);

		modelData.PointColors.push_back(1.0f);
		modelData.PointColors.push_back(0.25f);
		modelData.PointColors.push_back(0.75f);
		modelData.PointColors.push_back(1.0f);
		modelData.PointColors.push_back(0.25f);
		modelData.PointColors.push_back(0.75f);

		modelData.VertexSelected.push_back(GL_FALSE);
		modelData.VertexSelected.push_back(GL_FALSE);

		minimumDistance = 1000.0f;
		closestVector = -1;
		modelData.RightMouseButtonPressed = GL_FALSE;
		modelData.RightMouseButtonDown = GL_TRUE;
		modelData.BufferAddVertices = GL_TRUE;
	}

	else modelData.BufferAddVertices = GL_FALSE;
}

void Model::AddShape()
{
	if (modelData.AddShape)
	{
		modelData.AddShape = GL_FALSE;

		for (int i = 0; i < sizeof(newVertices) / sizeof(*newVertices); ++i)
		{
			modelData.Vertices.push_back(newVertices[i]);
			modelData.PointVertices.push_back(newVertices[i]);
		}

		for (int i = 0; i < 8; ++i)
		{
			modelData.VertexSelected.push_back(i);
			modelData.LineColors.push_back(0.0f);
			modelData.LineColors.push_back(1.0f);
			modelData.LineColors.push_back(1.0f);
			modelData.PointColors.push_back(1.0f);
			modelData.PointColors.push_back(0.25f);
			modelData.PointColors.push_back(0.75f);
		}

		for (int i = (int)modelData.VertexSelected.size() - 8; i < (int)modelData.VertexSelected.size(); ++i)
		{
			modelData.VertexSelected[i] = GL_FALSE;
		}

		modelData.BufferAddShape = GL_TRUE;
	}

	else modelData.BufferAddShape = GL_FALSE;
}

void Model::DeleteVertices()
{
	if (modelData.Delete)
	{
		GLuint count = 0;
		std::vector<GLfloat> vertexPositions;

		for (int i = 0; i < (int)(modelData.VertexSelected.size()); ++i)
		{
			if (modelData.VertexSelected[i] == GL_TRUE)
			{
				++count;
				vertexPositions.push_back(modelData.Vertices[2 * i]);
				vertexPositions.push_back(modelData.Vertices[2 * i + 1]);
			}
		}

		while (count > 0 && modelData.Vertices.size() > 12)
		{
			GLint index1 = -1, index2 = -1;
			for (int i = 0; i < (int)(0.5f * vertexPositions.size()); ++i)
			{
				for (int j = 0; j < (int)(0.5f * modelData.Vertices.size()); ++j)
				{
					if (modelData.Vertices[2 * j] == vertexPositions[2 * i] && modelData.Vertices[2 * j + 1] == vertexPositions[2 * i + 1] && index1 == -1)
					{
						index1 = j;
					}

					else if (modelData.Vertices[2 * j] == vertexPositions[2 * i] && modelData.Vertices[2 * j + 1] == vertexPositions[2 * i + 1] && index2 == -1)
					{
						index2 = j;
					}

					if (index1 > -1 && index2 > -1) break;
				}
			}

			GLint adjacent1 = -1, adjacent2 = -1;

			if (index1 == 0) adjacent1 = 1;
			else if (index1 == (int)(0.5f * modelData.Vertices.size()) - 1) adjacent1 = (int)(0.5f * modelData.Vertices.size()) - 2;
			else if (index1 % 2 == 1) adjacent1 = index1 - 1;
			else if (index1 % 2 == 0) adjacent1 = index1 + 1;

			if (index2 == 0) adjacent2 = 1;
			else if (index2 == (int)(0.5f * modelData.Vertices.size()) - 1) adjacent2 = (int)(0.5f * modelData.Vertices.size()) - 2;
			else if (index2 % 2 == 1) adjacent2 = index2 - 1;
			else if (index2 % 2 == 0) adjacent2 = index2 + 1;

			GLint overlappingAdjacent1 = -1, overlappingAdjacent2 = -1;

			for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i)
			{
				if (i == adjacent1) continue;
				if (modelData.Vertices[2 * i] == modelData.Vertices[2 * adjacent1] && modelData.Vertices[2 * i + 1] == modelData.Vertices[2 * adjacent1 + 1])
				{
					overlappingAdjacent1 = i;
					break;
				}
			}

			for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i)
			{
				if (i == adjacent2) continue;
				if (modelData.Vertices[2 * i] == modelData.Vertices[2 * adjacent2] && modelData.Vertices[2 * i + 1] == modelData.Vertices[2 * adjacent2 + 1])
				{
					overlappingAdjacent2 = i;
					break;
				}
			}

			modelData.Vertices.push_back(modelData.Vertices[2 * overlappingAdjacent1]);
			modelData.Vertices.push_back(modelData.Vertices[2 * overlappingAdjacent1 + 1]);
			modelData.Vertices.push_back(modelData.Vertices[2 * overlappingAdjacent2]);
			modelData.Vertices.push_back(modelData.Vertices[2 * overlappingAdjacent2 + 1]);

			modelData.PointVertices.push_back(modelData.PointVertices[2 * overlappingAdjacent1]);
			modelData.PointVertices.push_back(modelData.PointVertices[2 * overlappingAdjacent1 + 1]);
			modelData.PointVertices.push_back(modelData.PointVertices[2 * overlappingAdjacent2]);
			modelData.PointVertices.push_back(modelData.PointVertices[2 * overlappingAdjacent2 + 1]);

			if (index1 > index2)
			{
				if (index1 % 2 == 1 && index2 % 2 == 1)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
				}

				else if (index1 % 2 == 0 && index2 % 2 == 1)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
				}

				else if (index1 % 2 == 1 && index2 % 2 == 0)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
				}

				else if (index1 % 2 == 0 && index2 % 2 == 0)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
				}
			}

			else if (index1 < index2)
			{
				if (index1 % 2 == 1 && index2 % 2 == 1)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
				}

				else if (index2 % 2 == 0 && index1 % 2 == 1)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
				}

				else if (index2 % 2 == 1 && index1 % 2 == 0)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
				}

				else if (index1 % 2 == 0 && index2 % 2 == 0)
				{
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index2);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * adjacent1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);
					modelData.Vertices.erase(modelData.Vertices.begin() + 2 * index1);

					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index2);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * adjacent1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
					modelData.PointVertices.erase(modelData.PointVertices.begin() + 2 * index1);
				}
			}

			for (int i = 0; i < 6; ++i)
			{
				modelData.LineColors.erase(modelData.LineColors.end() - 1);
				modelData.PointColors.erase(modelData.PointColors.end() - 1);
			}

			count -= 2;
		}

		modelData.VertexSelected.clear();
		for (int i = 0; i < (int)(0.5f * modelData.Vertices.size()); ++i) modelData.VertexSelected.push_back(GL_FALSE);

		modelData.BufferDeleteVertices = GL_TRUE;
		modelData.Delete = GL_FALSE;
	}

	else modelData.BufferDeleteVertices = GL_FALSE;
}

void Model::GridScale()
{
	if (modelData.UpArrow)
	{
		n += 1;
		modelData.GridScale = 1 / n;
		this->UpdateGrid(modelData.WindowWidth, modelData.WindowHeight);
		modelData.UpArrow = GL_FALSE;
	}

	else if (modelData.DownArrow)
	{
		n -= 1;
		modelData.GridScale = 1 / n;
		this->UpdateGrid(modelData.WindowWidth, modelData.WindowHeight);
		modelData.DownArrow = GL_FALSE;
	}
}

void Model::UpdateGrid(int windowWidth, int windowHeight)
{
	modelData.GridLines.clear();
	modelData.GridColors.clear();

	for (float i = 0; i < windowWidth; i += modelData.GridScale * windowWidth)
	{
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back(0);
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back((GLfloat)windowHeight);
	}

	for (float i = 0; i < windowHeight; i += modelData.GridScale * windowHeight)
	{
		modelData.GridLines.push_back(0);
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back((GLfloat)windowWidth);
		modelData.GridLines.push_back(i);
	}

	for (int i = 0; i < (int)(1.5f * modelData.GridLines.size()); ++i) modelData.GridColors.push_back(0.35f);
}

void Model::InsideShape(glm::vec2 &initialMiddleMousePosition)
{
	if (modelData.MiddleMouseButtonPressed)
	{
		GLuint count = 0;
		GLboolean in;
		for (int i = 0; i < (int)(0.25f * modelData.Vertices.size()); ++i)
		{
			glm::vec2 p1 = glm::vec2(modelData.Vertices[4 * i], modelData.Vertices[4 * i + 1]);
			glm::vec2 q1 = glm::vec2(modelData.Vertices[4 * i + 2], modelData.Vertices[4 * i + 3]);
			glm::vec2 p2 = initialMiddleMousePosition;
			glm::vec2 q2 = glm::vec2(modelData.WindowWidth, initialMiddleMousePosition.y);

			if (this->intersection(p1, q1, p2, q2))
			{
				if (this->orientation(p1, p2, q1) == 0)
				{
					if (this->onSegment(p1, p2, q1)) in = GL_TRUE;
					else in = GL_FALSE;
				}

				++count;
			}
		}

		if (count % 2 == 1)
		{
			in = GL_TRUE;
			std::cout << "IN" << std::endl;
		}
		else
		{
			in = GL_FALSE;
			std::cout << "OUT" << std::endl;
		}
		modelData.MiddleMouseButtonPressed = GL_FALSE;
		modelData.MiddleMouseButtonDown = GL_TRUE;
	}
}

void Model::Reset()
{
	if (modelData.Reset)
	{
		modelData.GridLines.clear();
		modelData.Vertices.clear();
		modelData.PointVertices.clear();
		modelData.VertexSelected.clear();
		modelData.GridColors.clear();
		modelData.LineColors.clear();
		modelData.PointColors.clear();
		modelData.Grid = modelData.UpdateGrid = modelData.SnapToGrid = GL_FALSE;

		for (float i = 0; i < modelData.WindowWidth; i += modelData.GridScale * modelData.WindowWidth)
		{
			modelData.GridLines.push_back(i);
			modelData.GridLines.push_back(0);
			modelData.GridLines.push_back(i);
			modelData.GridLines.push_back((GLfloat)modelData.WindowHeight);
		}

		for (float i = 0; i < modelData.WindowHeight; i += modelData.GridScale * modelData.WindowHeight)
		{
			modelData.GridLines.push_back(0);
			modelData.GridLines.push_back(i);
			modelData.GridLines.push_back((GLfloat)modelData.WindowWidth);
			modelData.GridLines.push_back(i);
		}

		for (int i = 0; i < (int)(1.5f * modelData.GridLines.size()); ++i) modelData.GridColors.push_back(0.35f);

		for (int i = 0; i < 0.5f * modelData.Vertices.size(); ++i) modelData.VertexSelected.push_back(GL_FALSE);

		for (int i = 0; i < sizeof(vertices) / sizeof(*vertices); ++i)
		{
			modelData.Vertices.push_back(vertices[i]);
			modelData.PointVertices.push_back(vertices[i]);
		}

		for (int i = 0; i < 8; ++i)
		{
			modelData.LineColors.push_back(0);
			modelData.LineColors.push_back(1);
			modelData.LineColors.push_back(1);

			modelData.PointColors.push_back(1.0f);
			modelData.PointColors.push_back(0.25f);
			modelData.PointColors.push_back(0.75f);
		}

		std::ofstream vp_outputFile("vertex_positions.txt");
		for (auto i : modelData.Vertices) vp_outputFile << i << "\n";

		std::ofstream lc_outputFile("line_colors.txt");
		for (auto i : modelData.LineColors) lc_outputFile << i << "\n";

		std::ofstream pc_outputFile("point_colors.txt");
		for (auto i : modelData.PointColors) pc_outputFile << i << "\n";

		modelData.Reset = GL_FALSE;
		modelData.BufferReset = GL_TRUE;
	}

	else modelData.BufferReset = GL_FALSE;
}

void Model::PointColor()
{
	if (modelData.BufferSelectVertices || modelData.BufferDeselectVertices || modelData.BufferMoveVertex || modelData.BufferAddVertices || modelData.BufferAddShape || modelData.BufferDeleteVertices || modelData.BufferReset)
	{
		for (int i = 0; i < (int)modelData.VertexSelected.size(); ++i)
		{
			if (modelData.VertexSelected[i])
			{
				modelData.PointColors[3 * i] = 1.0f;
				modelData.PointColors[3 * i + 1] = 1.0f;
				modelData.PointColors[3 * i + 2] = 0.0f;
			}

			else
			{
				modelData.PointColors[3 * i] = 1.0f;
				modelData.PointColors[3 * i + 1] = 0.25f;
				modelData.PointColors[3 * i + 2] = 0.75f;
			}
		}
	}
}

void Model::initModelData()
{
	for (float i = 0; i < modelData.WindowWidth; i += modelData.GridScale * modelData.WindowWidth)
	{
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back(0);
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back((GLfloat)modelData.WindowHeight);
	}

	for (float i = 0; i < modelData.WindowHeight; i += modelData.GridScale * modelData.WindowHeight)
	{
		modelData.GridLines.push_back(0);
		modelData.GridLines.push_back(i);
		modelData.GridLines.push_back((GLfloat)modelData.WindowWidth);
		modelData.GridLines.push_back(i);
	}

	GLfloat vertexPositions, lineColors, pointColors;
	std::string line1, line2, line3;
	std::ifstream vp_fstream("vertex_positions.txt");
	std::ifstream lc_fstream("line_colors.txt");
	std::ifstream pc_fstream("point_colors.txt");

	if (vp_fstream)
	{
		while (std::getline(vp_fstream, line1))
		{
			std::istringstream vp_sstream(line1);
			while (vp_sstream >> vertexPositions)
			{
				modelData.Vertices.push_back(vertexPositions);
				modelData.PointVertices.push_back(vertexPositions);
			}
		}
	}

	if (lc_fstream)
	{
		while (std::getline(lc_fstream, line2))
		{
			std::istringstream lc_sstream(line2);
			while (lc_sstream >> lineColors)
			{
				modelData.LineColors.push_back(lineColors);
			}
		}
	}

	if (pc_fstream)
	{
		while (std::getline(pc_fstream, line3))
		{
			std::istringstream pc_sstream(line3);
			while (pc_sstream >> pointColors)
			{
				modelData.PointColors.push_back(pointColors);
			}
		}
	}

	for (int i = 0; i < 0.5f * modelData.Vertices.size(); ++i) modelData.VertexSelected.push_back(GL_FALSE);

	for (int i = 0; i < (int)(1.5f * modelData.GridLines.size()); ++i) modelData.GridColors.push_back(0.35f);
}

void Model::closestLine(glm::vec2 &initialRightMousePosition)
{
	for (int i = 0; i < (int)(0.25f * modelData.Vertices.size()); ++i)
	{
		glm::vec2 polygonVectors = glm::vec2(modelData.Vertices[4 * i + 2] - modelData.Vertices[4 * i], modelData.Vertices[4 * i + 3] - modelData.Vertices[4 * i + 1]);
		glm::vec2 pointVector = initialRightMousePosition - glm::vec2(modelData.Vertices[4 * i], modelData.Vertices[4 * i + 1]);
		GLfloat projection = glm::dot(polygonVectors, pointVector) / (glm::length(polygonVectors) * glm::length(polygonVectors));
		if (projection < 0) projection = 0;
		if (projection > 1) projection = 1;
		GLfloat distance = sqrtf(glm::dot(pointVector, pointVector) - glm::dot(projection * polygonVectors, projection * polygonVectors));
		if (distance < minimumDistance)
		{
			minimumDistance = distance;
			closestVector = i;
		}
	}
}

void Model::instantiateBoundingBox(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition)
{
	if (selectMultipleVertices)
	{
		modelData.Vertices[modelData.Vertices.size() - 16 + 3] = leftMousePosition.y;
		modelData.Vertices[modelData.Vertices.size() - 16 + 5] = leftMousePosition.y;
		modelData.Vertices[modelData.Vertices.size() - 16 + 6] = leftMousePosition.x;
		modelData.Vertices[modelData.Vertices.size() - 16 + 7] = leftMousePosition.y;
		modelData.Vertices[modelData.Vertices.size() - 16 + 8] = leftMousePosition.x;
		modelData.Vertices[modelData.Vertices.size() - 16 + 9] = leftMousePosition.y;
		modelData.Vertices[modelData.Vertices.size() - 16 + 10] = leftMousePosition.x;
		modelData.Vertices[modelData.Vertices.size() - 16 + 12] = leftMousePosition.x;
	}

	else if (!selectMultipleVertices)
	{
		selectMultipleVertices = GL_TRUE;

		for (int i = 0; i < 8; ++i)
		{
			modelData.Vertices.push_back(initialLeftMousePosition.x);
			modelData.Vertices.push_back(initialLeftMousePosition.y);
		}

		for (int i = 0; i < 8; ++i)
		{
			modelData.LineColors.push_back(1.0f);
			modelData.LineColors.push_back(1.0f);
			modelData.LineColors.push_back(1.0f);

			modelData.PointColors.push_back(0.0f);
			modelData.PointColors.push_back(1.0f);
			modelData.PointColors.push_back(1.0f);
		}
	}

	modelData.BufferSelectVertices = GL_TRUE;
}

void Model::destroyBoundingBox()
{
	selectMultipleVertices = GL_FALSE;
	GLfloat xMax = -1.0f, xMin = 1000.0f, yMax = -1.0f, yMin = 1000.0f;

	for (int i = (int)(0.5f * (modelData.Vertices.size() - 16)); i < (int)(0.5f * modelData.Vertices.size()); ++i)
	{
		if (modelData.Vertices[2 * i] > xMax) xMax = modelData.Vertices[2 * i];
		if (modelData.Vertices[2 * i] < xMin) xMin = modelData.Vertices[2 * i];

		if (modelData.Vertices[2 * i + 1] > yMax) yMax = modelData.Vertices[2 * i + 1];
		if (modelData.Vertices[2 * i + 1] < yMin) yMin = modelData.Vertices[2 * i + 1];
	}

	for (int i = 0; i < (int)(0.5f * (modelData.Vertices.size() - 16)); ++i)
	{
		if (modelData.Vertices[2 * i] >= xMin && modelData.Vertices[2 * i] <= xMax && modelData.Vertices[2 * i + 1] >= yMin && modelData.Vertices[2 * i + 1] <= yMax)
		{
			if (modelData.BoundingBoxAdd)
			{
				modelData.VertexSelected[i] = GL_TRUE;
				vertexID.push_back(i);
			}
			else if (modelData.BoundingBoxDelete)
			{
				modelData.VertexSelected[i] = GL_FALSE;
			}
		}
	}

	for (int i = 0; i < 16; ++i) modelData.Vertices.erase(modelData.Vertices.begin() + modelData.Vertices.size() - 1);

	for (int i = 0; i < 24; ++i)
	{
		modelData.LineColors.erase(modelData.LineColors.end() - 1);
		modelData.PointColors.erase(modelData.PointColors.end() - 1);
	}

	modelData.BufferSelectVertices = GL_TRUE;
}

bool Model::onSegment(glm::vec2 &p, glm::vec2 &q, glm::vec2 &r)
{
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) && q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

int Model::orientation(glm::vec2 &p, glm::vec2 &q, glm::vec2 &r)
{
	GLfloat value = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);

	if (value == 0) return 0; //colinear(0)
	return (value > 0) ? 1 : 2; //clockwise(1) or counterclockwise(2)
}

bool Model::intersection(glm::vec2 &p1, glm::vec2 &q1, glm::vec2 &p2, glm::vec2 &q2)
{
	GLuint o1 = this->orientation(p1, q1, p2);
	GLuint o2 = this->orientation(p1, q1, q2);
	GLuint o3 = this->orientation(p2, q2, p1);
	GLuint o4 = this->orientation(p2, q2, q1);

	if (o1 != o2 && o3 != o4) return true;
	if (o1 == 0 && this->onSegment(p1, p2, q1)) return true;
	if (o2 == 0 && this->onSegment(p1, q2, q1)) return true;
	if (o3 == 0 && this->onSegment(p2, p1, q2)) return true;
	if (o4 == 0 && this->onSegment(p2, q1, q2)) return true;

	return false;
}