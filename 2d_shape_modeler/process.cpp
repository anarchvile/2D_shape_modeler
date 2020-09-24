#include "pch.h"
#include "process.h"
#include <sstream>
#include <istream>
#include <fstream>

SpriteRenderer *Renderer;

Process::Process()
{

}

Process::~Process()
{
	delete Renderer;
}

void Process::Init()
{
	ResourceManager::LoadShader("vertex.txt", "fragment.txt", "sprite");
	spriteShader = ResourceManager::GetShader("sprite");
	glm::mat4 projection = glm::ortho(0.0f, (float)modelData.WindowWidth, (float)modelData.WindowHeight, 0.0f, -1.0f, 1.0f);
	spriteShader.Use().SetMatrix4("projection", projection);
	Renderer = new SpriteRenderer(spriteShader);
}

void Process::Update(std::vector<GLboolean> &vertexSelected, glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition, glm::vec2 &initialRightMousePosition, glm::vec2 &initialMiddleMousePosition)
{
	model.SelectVertices(initialLeftMousePosition, leftMousePosition);
	model.AddToSelectedVertices(initialLeftMousePosition, leftMousePosition);
	model.RemoveSelectedVertices(initialLeftMousePosition, leftMousePosition);
	model.DeselectVertices();
	model.MoveVertices(initialLeftMousePosition, leftMousePosition);
	model.AddVertices(initialRightMousePosition);
	model.AddShape();
	model.DeleteVertices();
	model.GridScale();
	model.InsideShape(initialMiddleMousePosition);
	model.Reset();
	model.PointColor();

	if (modelData.Save)
	{
		std::ofstream vp_outputFile("vertex_positions.txt");
		for (auto i : modelData.Vertices) vp_outputFile << i << "\n";

		std::ofstream lc_outputFile("line_colors.txt");
		for (auto i : modelData.LineColors) lc_outputFile << i << "\n";

		std::ofstream pc_outputFile("point_colors.txt");
		for (auto i : modelData.PointColors) pc_outputFile << i << "\n";

		modelData.Save = GL_FALSE;
	}

	if (modelData.WindowWidth != glutGet(GLUT_WINDOW_WIDTH))
	{
		modelData.UpdateGrid = GL_TRUE;
		modelData.WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
		glm::mat4 projection = glm::ortho(0.0f, (float)modelData.WindowWidth, (float)modelData.WindowHeight, 0.0f, -1.0f, 1.0f);
		spriteShader.SetMatrix4("projection", projection);
		model.UpdateGrid(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}

	if (modelData.WindowHeight != glutGet(GLUT_WINDOW_HEIGHT))
	{
		modelData.UpdateGrid = GL_TRUE;
		modelData.WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
		glm::mat4 projection = glm::ortho(0.0f, (float)modelData.WindowWidth, (float)modelData.WindowHeight, 0.0f, -1.0f, 1.0f);
		spriteShader.SetMatrix4("projection", projection);
		model.UpdateGrid(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	}
}

void Process::Render()
{
	Renderer->InitRenderData();
	Renderer->Draw();
}