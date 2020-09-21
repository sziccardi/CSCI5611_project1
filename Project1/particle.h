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
	Particle(Vec3 pos, float radius, Vec3 color);

	void draw();
	void moveTo(Vec3 newPos);
	void shift(Vec3 deltaPos);
	void setColor(Vec3 color);
	void setRadius(float radius);

	Vec3 getCurrentPos();
	Vec3 getCurrentColor();
	float getCurrentSize();



protected:
	Vec3 mPosition;
	Vec3 mColor;
	float mRadius;

};