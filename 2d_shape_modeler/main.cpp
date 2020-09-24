#include "pch.h"
#include "process.h"
#include <iostream>

#define FPS 30

Process Simulate;
//initialLeftMousePosition refers to the mouse's position when the left button was pressed. leftMousePosition stores the mouse's position
//as it is being dragged and the left button is being held down. initialRightMousePosition stores the mouse's position when the right button was 
//pressed.
glm::vec2 initialLeftMousePosition, leftMousePosition, initialRightMousePosition, initialMiddleMousePosition;

void mousePressed(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT) modelData.ShiftLeftMouseButtonPressed = GL_TRUE;
		else if (glutGetModifiers() == GLUT_ACTIVE_CTRL) modelData.CtrlLeftMouseButtonPressed = GL_TRUE;
		else if (glutGetModifiers() == GLUT_ACTIVE_ALT) modelData.AltLeftMouseButtonPressed = GL_TRUE;
		else modelData.LeftMouseButtonPressed = GL_TRUE;
		initialLeftMousePosition = glm::vec2(x, y);
		leftMousePosition = glm::vec2(x, y);
	}

	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
		{
			modelData.ShiftLeftMouseButtonUp = GL_TRUE;
			modelData.ShiftLeftMouseButtonDown = GL_FALSE;
		}
		else if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
		{
			modelData.CtrlLeftMouseButtonUp = GL_TRUE;
			modelData.CtrlLeftMouseButtonDown = GL_FALSE;
		}
		else if (glutGetModifiers() == GLUT_ACTIVE_ALT)
		{
			modelData.AltLeftMouseButtonUp = GL_TRUE;
			modelData.AltLeftMouseButtonDown = GL_FALSE;
		}
		else
		{
			modelData.LeftMouseButtonUp = GL_TRUE;
			modelData.LeftMouseButtonDown = GL_FALSE;
		}
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		for (int i = 0; i < (int)modelData.VertexSelected.size(); ++i) modelData.VertexSelected[i] = GL_FALSE;
		initialRightMousePosition = glm::vec2(x, y);
		modelData.RightMouseButtonPressed = GL_TRUE;
	}

	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		for (int i = 0; i < (int)modelData.VertexSelected.size(); ++i) modelData.VertexSelected[i] = GL_FALSE;
		modelData.RightMouseButtonUp = GL_TRUE;
		modelData.RightMouseButtonDown = GL_FALSE;
	}

	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		modelData.MiddleMouseButtonPressed = GL_TRUE;
		initialMiddleMousePosition = glm::vec2(x, y);
	}

	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		modelData.MiddleMouseButtonUp = GL_TRUE;
		modelData.MiddleMouseButtonDown = GL_FALSE;
	}
}

void mouseMove(int x, int y)
{
	leftMousePosition = glm::vec2(x, y);
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: //escape - de-select all vertices
		modelData.Deselect = GL_TRUE;
		break;
	case 32: //space - create new square
		modelData.AddShape = GL_TRUE;
		break;
	case 103: //g - display or hide grid
	//case 71: //G
		if (!modelData.Grid) modelData.Grid = GL_TRUE;
		else modelData.Grid = modelData.SnapToGrid = GL_FALSE;
		break;
	case 114: //r - reset program to default state
	//case 82: //R
		modelData.Reset = GL_TRUE;
		break;
	case 115: //s - turn on/off snap-to-grid functionality
	case 83: //S
		if (!modelData.SnapToGrid) modelData.Grid = modelData.SnapToGrid = GL_TRUE;
		else if (modelData.SnapToGrid) modelData.SnapToGrid = GL_FALSE;
		break;
	case 127: //delete
		modelData.Delete = GL_TRUE;
		break;
	}
}

void specialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		if (modelData.Grid)
		{
			modelData.UpArrow = modelData.UpdateGrid = GL_TRUE;
			modelData.DownArrow = GL_FALSE;
		}
		break;
	case GLUT_KEY_DOWN:
		if (modelData.Grid)
		{
			modelData.UpArrow = GL_FALSE;
			modelData.DownArrow = modelData.UpdateGrid = GL_TRUE;
		}
		break;
	case GLUT_KEY_F7:
		modelData.Save = GL_TRUE;
		break;
	}
}

void renderScene(void)
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	Simulate.Update(modelData.VertexSelected, initialLeftMousePosition, leftMousePosition, initialRightMousePosition, initialMiddleMousePosition);
	Simulate.Render();

	glutSwapBuffers();
}

void idle(int)
{
	glutIgnoreKeyRepeat(1);
	glutMouseFunc(mousePressed);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(specialKeyPressed);
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, idle, 0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(modelData.WindowWidth, modelData.WindowHeight);
	glutCreateWindow("2D Shape Editor");

	Simulate.Init();

	glutDisplayFunc(renderScene);
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, idle, 0);

	glutMainLoop();

	return 1;
}