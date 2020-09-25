#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <cmath>
#include <chrono> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, perspective 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "stb_image.h"
#include <algorithm>

#include "matrices.h"
#include "particle.h"
#include "Obstacle.h"

GLenum glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#define M_PI 3.14159265358979323846
#define BUILDING_GRID_ROW 10
#define BUILDING_GRID_COL 10

using namespace std::chrono;

/* References */
//https://learnopengl.com/Getting-started/Camera
//https://learnopengl.com/Getting-started/Textures
//https://learnopengl.com/code_viewer_gh.php?code=src/4.advanced_opengl/3.2.blending_sort/blending_sorted.cpp

unsigned char keyStates[256] = { 0 };

Vec3 cameraPos = Vec3(0.0f, 250.0f, 300.0f);
Vec3 cameraFront = Vec3(0.f, -1.0f, -5.0f);
Vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);
Vec2 mouseAngles = Vec2(0, 0);

float horizontal = 3.14f;
float vertical = 0.0f;

float cameraDepth = 10000.f;
float cameraSpeed = 5.f;
float moveMult = 5.0f;
float mouseSpeed = 0.0f;

float previousFrame = 0.0f;
float deltaTime = 0.01667f;


float particleCameraSpawnPoint = 30.0;


int shaderProgram;
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
"uniform sampler2D sampler;\n"
"void main()\n"
"{\n"
"   vec4 texColor = texture(sampler, TexCoord);\n"
"   FragColor = texColor;\n"
"}\n\0";

/*Building related things*/
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
Matrix buildings[BUILDING_GRID_ROW][BUILDING_GRID_COL] = {};
std::vector<Obstacle*> mObstacles;
float buildingMin = 55;
int buildingSize = 70;
int buildingAdjustment = 75;
int buildingHeightSize = 300;
unsigned int buildingVBO, buildingVAO;
unsigned int buildingTexture;

void initObstacles();
void drawObstacles();

/*Ground plane related things*/
float groundVertices[] = {
         0.f,  0.f,  0.f,  0.0f, 0.0f,
         1.f,  0.f,  0.f,  10.0f, 0.0f,
         1.f,  0.f,  1.f,  10.0f, 10.0f,
         1.f,  0.f,  1.f,  10.0f, 10.0f,
         0.f,  0.f,  1.f,  0.0f, 10.0f,
         0.f,  0.f,  0.f,  0.0f, 0.0f
};
float mGroundPlanePos = -1.f;
float mGroundPlaneSize = 1000.f;
unsigned int groundVBO, groundVAO;
unsigned int groundTexture;

void initGroundPlane();
void drawGroundPlane();

/* Particle related things*/
float particleVertices[] = {
         0.f,  0.f,  0.f,  0.0f, 0.0f,
         1.f,  0.f,  0.f,  1.0f, 0.0f,
         1.f,  0.f,  1.f,  1.0f, 1.0f,
         1.f,  0.f,  1.f,  1.0f, 1.0f,
         0.f,  0.f,  1.f,  0.0f, 1.0f,
         0.f,  0.f,  0.f,  0.0f, 0.0f
};
std::vector<Particle*> mParticles;
int mMaxNumParticles = 35;
float mParticleRadius = 2.f;
Vec3 mGravity = Vec3(0.f, -30.f, 0.f);
unsigned int particleVBO, particleVAO;
unsigned int particleTexture; //https://www.pinpng.com/search/sparkle/
float mFlockRadius = 50.f;
float mParticleMaxVelocity = 2.f;

void initParticles();
void makeParticles();
void checkForParticleInteractions(Particle* p);
void checkForGroundInteraction(Particle* p);
void checkForObstacleInteraction(Particle* p);
void updateParticles(float dt);
bool compareDepth(Particle* p1, Particle* p2);
void drawParticles();

/* Display loop */
void display();

/* OpenGl helper functions */
//Set up the screen and rendering settings
void initGL();
//Handler for window re-size event.
void reshape(GLsizei width, GLsizei height);
//Texture handling
unsigned int loadTexture(char const* path);
void linkTexture();

/* Input handlers */
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void keyOperations(void);