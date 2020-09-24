#include "Obstacle.h"

Obstacle::Obstacle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mSize(Vec3(0.f, 0.f, 0.f)), mColor(Vec3(0.f, 0.f, 0.f))
{ }

Obstacle::Obstacle(Vec3 initPos, Vec3 initSize, Vec3 color, char* inputTextureFile) :
	mPosition(initPos), mSize(initSize), mColor(color), textureFile(inputTextureFile)
{
   // texture = loadBMP_custom(inputTextureFile) ;
    setupShader(inputTextureFile);
   
}

Obstacle::~Obstacle() {
}


void Obstacle::setupShader(char* filename) {
    
}

glm::mat4 Obstacle::draw(float scale)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::vec3 newPos = glm::vec3(mPosition.x() / scale, mPosition.y()/ scale,  mPosition.z() / scale);
	glm::vec3 newSize = glm::vec3(mSize.x(), mSize.y(), mSize.z());
	model = glm::scale(model, newSize);
    model = glm::translate(model, newPos);
	return model;
}

void Obstacle::moveTo(Vec3 newPos)
{
	//TODO: probably want to smooth out this transition a bit
	mPosition = Vec3(newPos);
}

void Obstacle::shift(Vec3 deltaPos)
{
	mPosition += deltaPos;
}

void Obstacle::setTexture(Vec3 color)
{
	mColor = color;
}

void Obstacle::setSize(Vec3 newSize)
{
	mSize = Vec3(newSize);
}

Vec3 Obstacle::getCurrentPos()
{
	return mPosition;
}

Vec3 Obstacle::getCurrentTexture()
{
	return mColor;
}

Vec3 Obstacle::getCurrentSize()
{
	return mSize;
}

void Obstacle::update(float dt)
{
	
	
}
