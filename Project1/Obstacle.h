#pragma once
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "matrices.h"

using namespace std;

class Obstacle {
public:
	
	Obstacle();
	Obstacle(Vec3 initPos, Vec3 initSize, Vec3 color, char* textureFile);

	void draw();
	void moveTo(Vec3 newPos);
	void shift(Vec3 deltaPos);
	void setTexture(Vec3 color);
	void setSize(Vec3 newSize);
	void update(float dt);
	void setupTexture(char* filename);

	Vec3 getCurrentPos();
	Vec3 getCurrentTexture();
	Vec3 getCurrentSize();

	

protected:
	char* textureFile;
	unsigned int texture;
	Vec3 mPosition;
	Vec3 mSize;
	Vec3 mColor; // maybe turn this into a texture?
};