#include "particle.h"

Particle::Particle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mRadius(0.f), mColor(Vec3(0.f, 0.f, 0.f)), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mLifespan(0.f), mAge(0.f), mIsDying(false), mIsDead(false)
{ }

Particle::Particle(Vec3 initPos, Vec3 initVel, float radius, Vec3 color) :
	mPosition(initPos), mVelocity(initVel), mRadius(radius), mColor(color), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mAge(0.f), mIsDying(false), mIsDead(false)
{ 
	mLifespan = rand() % (int)(mMaxLifespan - mMinLifespan) + mMinLifespan;
}

glm::mat4 Particle::draw() {	
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::vec3 newPos = glm::vec3(mPosition.x(), mPosition.y(), mPosition.z());
	glm::vec3 newSize = glm::vec3(2 * mRadius);
	model = glm::translate(model, newPos);
	model = glm::scale(model, newSize);
	return model;
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

float Particle::getRadius()
{
	return mRadius;
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

bool Particle::getIsAlive()
{
	return !mIsDead;
}

void Particle::update(float dt)
{
	if (mIsDead) return;

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

void Particle::reflectOffOf(Vec3 normal, float amtToMove) {

	Vec3 bounce = toVec3(mVelocity.projAB(normal));

	Vec3 newVel = toVec3(mVelocity - (bounce * 1.8f));
	mVelocity = newVel;

	mPosition += toVec(normal * amtToMove * 1.01f);
}

void Particle::flock(Vec3 averageNormalVel, Vec3 averagePos, Vec3 averageDiff) {
	//cohesion
	mCurrentForce += toVec3(toVec(averagePos - mPosition).normalized() * (cohesionAmt / averageDiff.length()));

}
