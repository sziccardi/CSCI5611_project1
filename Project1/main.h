#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cmath>
#include <chrono> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include "matrices.h"
#include "particle.h"
#include "Obstacle.h"

#define M_PI 3.14159265358979323846
#define BUILDING_GRID_ROW 10
#define BUILDING_GRID_COL 10

using namespace std::chrono;

/* References */
//https://learnopengl.com/Getting-started/Camera
//https://learnopengl.com/Getting-started/Textures

unsigned char keyStates[256] = { 0 };

Vec3 cameraPos = Vec3(0.0f, 72.0f, 0.0f);
Vec3 cameraFront = Vec3(0.f, -1.0f, -5.0f);
Vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);
Vec2 mouseAngles = Vec2(0, 0);

float horizontal = 3.14f;
float vertical = 0.0f;

float cameraSpeed = 2.5f;
float moveMult = 5.0f;
float mouseSpeed = 0.0f;

float previousFrame = 0.0f;
float deltaTime = 0.01667f;


std::vector<Particle*> mParticles;
int mMaxNumParticles = 15;
float mParticleRadius = 5.f;
Vec3 mGravity = Vec3(0.f, -30.f, 0.f);

Matrix buildings[BUILDING_GRID_ROW][BUILDING_GRID_COL] = {};
std::vector<Obstacle*> mObstacles;
float buildingMin = 5;
int buildingSize = 10;
int buildingHeightSize = 20;

unsigned int buildingVBO, buildingVAO;
unsigned int buildingTexture;
int buildingShaderProgram;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model; \n"
"uniform mat4 view; \n"
"uniform mat4 projection; \n"
"void main()\n"
"{\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
"   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture;\n"
"void main()\n"
"{\n"
"   FragColor = texture(texture, TexCoord);\n"
"}\n\0";
float buildingTexCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    1.0, 1.0f,   // upper-right corner
    0.0, 1.0f   // upper-left corner
};

float buildingVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f
};


float groundVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  5.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  5.0f, 5.0f,
         0.5f, -0.5f,  0.5f,  5.0f, 5.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 5.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
};
float mGroundPlanePos = -1.f;
float mGroundPlaneSize = 500.f;
unsigned int groundVBO, groundVAO;
unsigned int groundTexture;
void initGroundPlane();

void initParticles();
void checkForParticleInteractions(Particle* p);
void checkForGroundInteraction(Particle* p);
void checkForObstacleInteraction(Particle* p);
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

/* Input handlers */
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyOperations(void);