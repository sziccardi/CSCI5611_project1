#include "particle.h"

Particle::Particle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mRadius(0.f), mColor(Vec3(0.f, 0.f, 0.f))
{ }

Particle::Particle(Vec3 pos, float radius, Vec3 color) :
	mPosition(pos), mRadius(radius), mColor(color) 
{ }

void Particle::draw()
{
	glColor3f(mColor.x(), mColor.y(), mColor.z());
	glTranslatef(mPosition.x(), mPosition.y(), mPosition.z());
	glutSolidSphere(mRadius, 50, 50);
	glEnd();
}

void Particle::moveTo(Vec3 newPos)
{
	//TODO: probably want to smooth out this transition a bit
	mPosition = newPos;
}

void Particle::shift(Vec3 deltaPos)
{
	mPosition += deltaPos;
}

void Particle::setColor(Vec3 color)
{
	mColor = color;
}

void Particle::setRadius(float radius)
{
	mRadius = radius;
}

Vec3 Particle::getCurrentPos()
{
	return mPosition;
}

Vec3 Particle::getCurrentColor()
{
	return mColor;
}

float Particle::getCurrentSize()
{
	return mRadius;
}
