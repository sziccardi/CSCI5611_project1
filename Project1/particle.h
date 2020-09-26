#pragma once
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "matrices.h"

using namespace std;

class Particle {
public:
	Particle();
	Particle(Vec3 initPos, Vec3 initVel, float radius, Vec3 color);

	glm::mat4 draw(Vec3 cameraFront);
	void moveTo(Vec3 newPos);
	void shift(Vec3 deltaPos);
	void setColor(Vec3 color);
	void setRadius(float radius);
	void setVelocity(Vec3 newVel);
	void addForce(Vec3 force);
	void setLifespan(float lifespan);

	float getRadius();
	Vec3 getCurrentPos();
	Vec3 getCurrentColor();
	float getCurrentSize();
	float getAge();
	Vec3 getCurrentVelocity();
	bool getIsAlive();
	void setIsAlive(bool alive) { mIsDead = !alive; }
	float getFadeOn() { return mFadeOn; }
	bool getIsFlocking() { return mIsFlocking; }
	void setIsFlocking(bool flock) { mIsFlocking = flock; }

	void update(float dt);
	void reflectOffOf(Vec3 normal, float amtToMove);
	void flock(Vec3 averageNormalVel, Vec3 averagePos);
	Vec3 getCurrentRot() { return mCurrentNormal; }
	void setCurrentRot(Vec3 newDir) { mCurrentNormal = newDir; }

protected:
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mCurrentForce;
	Vec3 mColor; // maybe turn this into a texture?
	float mRadius;
	Vec3 mCurrentNormal;

	float mLifespan;
	float mFadeOn;
	float mAge;

	bool mIsDying; // only need if we are going to impliment some sort of ease-out for dying
	bool mIsDead;

	float mMaxLifespan = 20.0f; // in seconds
	float mMinLifespan = 5.0f;

	float cohesionAmt = 1.f;
	float alignmentAmt = 5.f;

	bool mIsFlocking = true;
};