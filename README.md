# 2d_shape_modeler
A simple 2D shape modeler made using C++ and OpenGL.
Shapes are made using a series of interconnected vertices that share edges.
Shape modeler supports vertex translation, addition, and deletion, and allows one to save the shapes they design for later use.
Before running this code, make sure to download and link the OpenGL Mathematics (glm) library to the project.

Controls:
To move a vertex, select and drag with the left mouse button.
To add a vertex, move the mouse cursor over the desired position for that vertex, and click the right mouse button.
To delete a vertex, select it with the left mouse button and press the delete key.
To select multiple vertices at once, hold down the shift key while selecting each individual vertex
to include in the group, or create a mouse drag-box over all the vertices you would like to select.
To add in a new square shape, hit the 'space' button.
To deselect all vertices, press 'escape'.
To either display or hide the background grid, press 'g'.
To turn on grid snapping functionality, press 's'.
To increase/decrease grid density, press the up/down arrow keys (respectively).
To save a scene, press 'F7'.
