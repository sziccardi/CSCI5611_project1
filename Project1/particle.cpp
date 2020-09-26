#include "particle.h"

Particle::Particle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mRadius(0.f), mColor(Vec3(0.f, 0.f, 0.f)), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mLifespan(0.f), mAge(0.f), mIsDying(false), mIsDead(false), mCurrentNormal(Vec3(0.f, 0.f, 0.f)), mFadeOn(0.f)
{ }

Particle::Particle(Vec3 initPos, Vec3 initVel, float radius, Vec3 color) :
	mPosition(initPos), mVelocity(initVel), mRadius(radius), mColor(color), mCurrentForce(Vec3(0.f, 0.f, 0.f)),
	mAge(0.f), mIsDying(false), mIsDead(false)
{ 
	mLifespan = rand() % (int)(mMaxLifespan - mMinLifespan) + mMinLifespan;
	mCurrentNormal = Vec3(0.f, 1.f, 0.f);
	mFadeOn = 3.f;
}

glm::mat4 Particle::draw(Vec3 cameraPos) {
	// calculate the model matrix for each object and pass it to shader before drawing
	glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	
	glm::vec3 newPos = glm::vec3(mPosition.x(), mPosition.y(), mPosition.z());
	glm::vec3 newSize = glm::vec3(2 * mRadius);
	//billboarding
	Vec3 goalNorm = toVec3(toVec3(cameraPos - mPosition).normalized());
	Vec3 cameraRotVec = toVec3(goalNorm.cross(mCurrentNormal).normalized() * -1);
	float dot = goalNorm.dot(mCurrentNormal);
	float cameraRotAmt = acos(dot); //normalized so lengths are 1

	model = glm::translate(model, newPos);
	model = glm::scale(model, newSize); 
	model = glm::rotate(model, cameraRotAmt, glm::vec3(cameraRotVec.x(), cameraRotVec.y(), cameraRotVec.z()));
	
	return model;
}

glm::mat4 Particle::draw(Vec3 cameraFront, Vec3 cameraRight, Vec3 cameraUp) {
	float camMat[3][3] = { {cameraFront.x(), cameraRight.x(), cameraUp.x()},
							{cameraFront.y(), cameraRight.y(), cameraUp.y()},
							{cameraFront.z(), cameraRight.z(), cameraUp.z()} };
	
	Vec3 quadPos = Vec3(0.f, 0.f, mCurrentNormal.dot(mPosition) / mCurrentNormal.z());
	Vec3 quadRight = toVec3(quadPos - mPosition);
	quadRight.normalize();
	Vec3 quadUp = quadRight.cross(mCurrentNormal);
	quadUp.normalize();
	float quadMat[3][3] = { {mCurrentNormal.x(), quadRight.x(), quadUp.x()},
							{mCurrentNormal.y(), quadRight.y(), quadUp.y()},
							{mCurrentNormal.z(), quadRight.z(), quadUp.z()} };

	float camMatInv[3][3] = { {(cameraRight.y() * cameraUp.z() - cameraUp.y() * cameraRight.z()),
								-(cameraRight.x() * cameraUp.z() - cameraUp.x() * cameraRight.z()),
								(cameraRight.x() * cameraUp.y() - cameraUp.x() * cameraRight.y()) },

								{-(mCurrentNormal.y() * cameraUp.z() - cameraUp.y() * mCurrentNormal.z()),
								(mCurrentNormal.x() * cameraUp.z() - cameraUp.x() * mCurrentNormal.z()),
								-(mCurrentNormal.x() * cameraUp.y() - cameraUp.x() * mCurrentNormal.y()) },

								{(mCurrentNormal.y() * cameraFront.z() - cameraFront.y() * mCurrentNormal.z()),
								-(mCurrentNormal.x() * cameraFront.z() - cameraFront.x() * mCurrentNormal.z()),
								(mCurrentNormal.x() * cameraFront.y() - cameraFront.x() * mCurrentNormal.y()) } };
	float det = cameraFront.x() * (cameraRight.y() * cameraUp.z() - cameraUp.y() * cameraRight.z()) + 
				cameraRight.x() * (-(cameraFront.y() * cameraUp.z() - cameraFront.z() * cameraUp.y())) + 
				cameraUp.z() * (cameraFront.x() * cameraRight.z() - cameraRight.y() * cameraFront.z());



	return glm::mat4();
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
	mCurrentForce += force;
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

void Particle::flock(Vec3 averageNormalVel, Vec3 averagePos) {
	//cohesion
	Vec3 cohesionForce = toVec3(averagePos - mPosition);
	cohesionForce.normalize();
	cohesionForce *= cohesionAmt;
	addForce(cohesionForce);
	//alignment
	Vec3 alignmentForce = toVec3(averageNormalVel - mVelocity);
	alignmentForce.normalize();
	alignmentForce *= alignmentAmt;
	addForce(alignmentForce);
	//separation done outside
}
