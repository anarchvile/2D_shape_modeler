#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <glm.hpp>
#include <vector>

struct ModelData
{
	std::vector<GLboolean> VertexSelected; //stores which vertices have been selected.
	std::vector<GLfloat> GridLines, Vertices, PointVertices, LineColors, PointColors, GridColors; //vertices stores the blue line positions. pointvertices the purple point positions
	GLboolean AddShape, Deselect, Delete, Grid, UpdateGrid, SnapToGrid, Reset, Save, Print;
	GLboolean LeftMouseButtonPressed, LeftMouseButtonDown, LeftMouseButtonUp;
	GLboolean ShiftLeftMouseButtonPressed, ShiftLeftMouseButtonDown, ShiftLeftMouseButtonUp;
	GLboolean CtrlLeftMouseButtonPressed, CtrlLeftMouseButtonDown, CtrlLeftMouseButtonUp;
	GLboolean AltLeftMouseButtonPressed, AltLeftMouseButtonDown, AltLeftMouseButtonUp;
	GLboolean RightMouseButtonPressed, RightMouseButtonDown, RightMouseButtonUp;
	GLboolean MiddleMouseButtonPressed, MiddleMouseButtonDown, MiddleMouseButtonUp;
	GLboolean UpArrow, DownArrow;
	GLboolean BufferSnapToGrid, BufferSelectVertices, BufferDeselectVertices, BufferMoveVertex, BufferAddVertices, BufferAddShape, BufferDeleteVertices, BufferReset, BoundingBoxAdd, BoundingBoxDelete; //used to update the vbos only if the user interacts with/changes the mesh
	GLint WindowWidth = 500, WindowHeight = 500;
	GLfloat GridScale = 0.1f;
};

class Model
{
public:
	Model();
	~Model();
	void SelectVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition);
	void AddToSelectedVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition);
	void RemoveSelectedVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition);
	void DeselectVertices();
	void MoveVertices(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition);
	void AddVertices(glm::vec2 &initialRightMousePosition);
	void AddShape();
	void DeleteVertices();
	void GridScale();
	void UpdateGrid(int windowWidth, int windowHeight);
	void InsideShape(glm::vec2 &initialMiddleMousePosition);
	void Reset();
	void PointColor();
private:
	void initModelData();
	void closestLine(glm::vec2 &initialRightMousePosition);
	void instantiateBoundingBox(glm::vec2 &initialLeftMousePosition, glm::vec2 &leftMousePosition);
	void destroyBoundingBox();
	bool onSegment(glm::vec2 &p, glm::vec2 &q, glm::vec2 &r);
	int orientation(glm::vec2 &p, glm::vec2 &q, glm::vec2 &r);
	bool intersection(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &q1, glm::vec2 &q2);
};

extern ModelData modelData;
extern Model model;

#endif

