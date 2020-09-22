#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "matrices.h"

#include "particle.h"

std::vector<Particle*> mParticles;
int mMaxNumParticles = 10;
float mParticleRadius = 0.5f;

float mTimeElapsed = 0.f;

void initParticles();
void checkForParticleInteractions(Particle* p);
void updateParticles(float dt);

/* Specific draw functions */
void drawCube(Vec3 pos);

/* Display loop */
void display();

/* OpenGl helper functions */
//Set up the screen and rendering settings
void initGL();
//Handler for window re-size event.
void reshape(GLsizei width, GLsizei height);