#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cmath>
#include <chrono> 

#include "matrices.h"
#include "particle.h"

#define M_PI 3.14159265358979323846

using namespace std::chrono;

/* References */
//https://learnopengl.com/Getting-started/Camera

unsigned char keyStates[256] = { 0 };

Vec3 cameraPos = Vec3(0.0f, 1.0f, 0.0f);
Vec3 cameraFront = Vec3(1.5f, 0.0f, -7.0f);
Vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);
Vec2 mouseAngles = Vec2(0, 0);

float horizontal = 3.14f;
float vertical = 0.0f;

float cameraSpeed = 5.0f;
float mouseSpeed = 0.0f;

float previousFrame = 0.0f;
float deltaTime = 0.01667f;

std::vector<Particle*> mParticles;
int mMaxNumParticles = 10;
float mParticleRadius = 0.5f;

float buildingMin = 20;
int buildingSize = 70;
int buildingHeightSize = 300;

void initParticles();
void checkForParticleInteractions(Particle* p);
void updateParticles(float dt);

/* Specific draw functions */
void drawCube(Vec3 pos);
void drawGroundPlane();

/* Display loop */
void display();

/* OpenGl helper functions */
//Set up the screen and rendering settings
void initGL();
//Handler for window re-size event.
void reshape(GLsizei width, GLsizei height);