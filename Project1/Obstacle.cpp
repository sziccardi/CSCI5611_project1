#include "Obstacle.h"

Obstacle::Obstacle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mSize(Vec3(0.f, 0.f, 0.f)), mColor(Vec3(0.f, 0.f, 0.f))
{ }

Obstacle::Obstacle(Vec3 initPos, Vec3 initSize, Vec3 color) :
	mPosition(initPos), mSize(initSize), mColor(color)
{

}

void Obstacle::draw()
{

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
