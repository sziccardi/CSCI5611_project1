#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "matrices.h"

/* Specific draw functions */
void drawCube(Vec3 pos);

/* Display loop */
void display();

/* OpenGl helper functions */
//Set up the screen and rendering settings
void initGL();
//Handler for window re-size event. Called back when the window first appears and
//whenever the window is re-sized with its new width and height
void reshape(GLsizei width, GLsizei height);