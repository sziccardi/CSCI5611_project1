#pragma once
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "matrices.h"

using namespace std;

class Particle {
public:
	Particle();
	Particle(Vec3 initPos, Vec3 initVel, float radius, Vec3 color);

	void draw();
	void moveTo(Vec3 newPos);
	void shift(Vec3 deltaPos);
	void setColor(Vec3 color);
	void setRadius(float radius);
	void setVelocity(Vec3 newVel);
	void addForce(Vec3 force);
	void setLifespan(float lifespan);

	Vec3 getCurrentPos();
	Vec3 getCurrentColor();
	float getCurrentSize();
	float getAge();
	Vec3 getCurrentVelocity();
	bool getIsAlive();

	void update(float dt);
	void reflectOffOf(Vec3 normal);
	void flock(vector<Particle> neighbors);

protected:
	Vec3 mPosition;
	Vec3 mVelocity;
	Vec3 mCurrentForce;
	Vec3 mColor; // maybe turn this into a texture?
	float mRadius;

	float mLifespan;
	float mAge;

	bool mIsDying; // only need if we are going to impliment some sort of ease-out for dying
	bool mIsDead;

	float mMaxLifespan = 500.f; // in seconds
	float mMinLifespan = 100.f;

};