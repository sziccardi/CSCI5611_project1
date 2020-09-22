#include "particle.h"

Particle::Particle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mRadius(0.f), mColor(Vec3(0.f, 0.f, 0.f)), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mLifespan(0.f), mAge(0.f), mIsDying(false), mIsDead(false)
{ }

Particle::Particle(Vec3 initPos, Vec3 initVel, float radius, Vec3 color) :
	mPosition(initPos), mVelocity(initVel), mRadius(radius), mColor(color), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mAge(0.f), mIsDying(false), mIsDead(false)
{ 
	mLifespan = rand() % (int)mMaxLifespan;
}

void Particle::draw()
{	
	glColor3f(mColor.x(), mColor.y(), mColor.z());
	//glPushMatrix();
	glTranslatef(mPosition.x(), mPosition.y(), mPosition.z());
	glutSolidSphere(mRadius, 50, 50);
	//glPopMatrix();
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

void Particle::setVelocity(Vec3 newVel)
{
	mVelocity = newVel;
}

void Particle::addForce(Vec3 force)
{
	mCurrentForce = force;
}

void Particle::setLifespan(float lifespan)
{
	mLifespan = lifespan;
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

float Particle::getAge()
{
	return mAge;
}

Vec3 Particle::getCurrentVelocity()
{
	return mVelocity;
}

void Particle::update(float dt)
{
	mAge += dt;
	if (mAge > mLifespan) {
		mIsDead = true;
	} else {
		//F = ma assume m = 1
		// delta v = F dt
		// delta x = v dt
		mVelocity += toVec3(mCurrentForce * dt);
		mPosition += toVec3(mVelocity * dt);
		mCurrentForce = Vec3(0.f, 0.f, 0.f);
	}
}

void Particle::reflectOffOf(Vec3 normal)
{
	Vec3 bounce = toVec3(mVelocity.projAB(normal));

	Vec3 newVel = toVec3(mVelocity - (bounce * 1.8f));
	mVelocity = newVel;

	mPosition += (normal * mRadius * 1.01);
}

void Particle::flock(vector<Particle> neighbors)
{
	//TODO: add cohesion/separation/etc. to the force vec3
}
