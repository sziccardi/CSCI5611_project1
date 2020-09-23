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
    glPushMatrix();
    glTranslatef(mPosition.x(), mPosition.y(), mPosition.z());

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(0, mSize.y(), 0);
    glVertex3f(mSize.x(), mSize.y(), 0);
    glVertex3f(mSize.x(), mSize.y(), mSize.z());
    glVertex3f(0, mSize.y(), mSize.z());

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0, 0, 0);
    glVertex3f(mSize.x(), 0, 0);
    glVertex3f(mSize.x(), 0, mSize.z());
    glVertex3f(0, 0, mSize.z());

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0, 0, 0);
    glVertex3f(0, mSize.y(), 0);
    glVertex3f(0, mSize.y(), mSize.z());
    glVertex3f(0, 0, mSize.z());


    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(mSize.x(), 0, 0);
    glVertex3f(mSize.x(), 0, mSize.z());
    glVertex3f(mSize.x(), mSize.y(), mSize.z());
    glVertex3f(mSize.x(), mSize.y(), 0);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(0, 0, 0);
    glVertex3f(mSize.x(), 0, 0);
    glVertex3f(mSize.x(), mSize.y(), 0);
    glVertex3f(0, mSize.y(), 0);


    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0, 0, mSize.z());
    glVertex3f(0, mSize.y(), mSize.z());
    glVertex3f(mSize.x(), mSize.y(), mSize.z());
    glVertex3f(mSize.x(), 0, mSize.z());
    glEnd();  // End of drawing color-cube
    glPopMatrix();
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
