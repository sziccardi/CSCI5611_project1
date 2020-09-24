#include "Obstacle.h"
#include "bmp.h"
#include "stb_image.h"
//#include "Shader.h"


float texCoords[] = {
    0.0f, 0.0f,  // lower-left corner  
    1.0f, 0.0f,  // lower-right corner
    0.5f, 1.0f   // top-center corner
}; 

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};


Obstacle::Obstacle() :
	mPosition(Vec3(0.f, 0.f, 0.f)), mSize(Vec3(0.f, 0.f, 0.f)), mColor(Vec3(0.f, 0.f, 0.f))
{ }

Obstacle::Obstacle(Vec3 initPos, Vec3 initSize, Vec3 color, char* inputTextureFile) :
	mPosition(initPos), mSize(initSize), mColor(color), textureFile(inputTextureFile)
{
   // texture = loadBMP_custom(inputTextureFile) ;
    setupTexture(inputTextureFile);
   
}


void Obstacle::setupTexture(char* filename) {

   // Shader ourShader("VertexShader.hlsl", "ComputeShader.hlsl");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        OutputDebugString("Failed to load texture");
    }

    stbi_image_free(data);

   ///* glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
   // glEnableVertexAttribArray(2);*/
}




void Obstacle::draw()
{
    glPushMatrix();
    glTranslatef(mPosition.x(), mPosition.y(), mPosition.z());

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(1.0f, 1.0f, 1.0f);     // Green
    glTexCoord2f(0, 0);
    glVertex3f(0, mSize.y(), 0);
    glTexCoord2f(1, 0);
    glVertex3f(mSize.x(), mSize.y(), 0);
    glTexCoord2f(1, 1);
    glVertex3f(mSize.x(), mSize.y(), mSize.z());
    glTexCoord2f(0, 1);
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
    glDisable(GL_TEXTURE_2D);
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
