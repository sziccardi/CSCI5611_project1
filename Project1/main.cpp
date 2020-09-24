#include "main.h"


float buildingWidth = buildingMin + rand() % buildingSize;
float buildingHeight = buildingMin + rand() % buildingHeightSize;

void keyPressed(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void keyOperations(void) {
    float dCam = cameraSpeed * deltaTime;
    //int ww = glutGet(GLUT_WINDOW_WIDTH);
    //int wh = glutGet(GLUT_WINDOW_HEIGHT);

    //OutputDebugStringA(cameraFront.toString().c_str());

    float phi = atan2(-cameraFront.z(), cameraFront.x());
    float theta = atan2(sqrt(cameraFront.z() * cameraFront.z() + cameraFront.x() * cameraFront.x()), cameraFront.y());
    float oldPhi = phi;
    float oldTheta = theta;


    if (keyStates['w']) { // If the 'a' key has been pressed  
        //turn up
        //in spherical, -- theta
        theta -= dCam;
    }

    if (keyStates['a']) { // If the 'a' key has been pressed  
        //turn left
        //in spherical, ++ phi
        phi += dCam;
    }

    if (keyStates['d']) { // If the 'a' key has been pressed  
        //turn right
        //in spherical, -- phi
        phi -= dCam;
    }

    if (keyStates['s']) { // If the 'a' key has been pressed  
        //turn down
        //in spherical, ++ theta
        //TODO: need a limit!?
        theta += dCam;
    }

    float x = sin(theta) * cos(phi);
    float z = -sin(theta) * sin(phi);
    float y = cos(theta);

    cameraFront.setVal(0, 0, x);
    cameraFront.setVal(1, 0, y);
    cameraFront.setVal(2, 0, z);

    cameraFront.normalize();// maybe not necessary

    if (keyStates[' ']) {
        cameraPos += toVec3(cameraFront * dCam * 15.f);
    }
}


void initParticles() {
    for (int i = 0; i < mMaxNumParticles; i++) {
        float x = (float)(rand() % 120 - 66.f);
        float y = (float)(rand() % 240 + 20.f);
        float z = (float)(-1.0f * (rand() % 120));
        //float x = 0.f;
        //float y = 120.f;
        //float z = -120.f;
        Vec3 pos = Vec3(x, y, z);
        float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        Vec3 color = Vec3(r,g, b);
        mParticles.push_back(new Particle(pos, Vec3(0.f, 0.f, 0.f), mParticleRadius, color));
    }

    /*for testing particle collisions*/
    //mParticles.push_back(new Particle(Vec3(0.f, 55.f, -55.f), Vec3(0.f, 0.f, 0.f), mParticleRadius, Vec3(1.f, 0.f, 0.f)));
    //mParticles.push_back(new Particle(Vec3(0.f, 105.f, -55.f), Vec3(0.f, 0.f, 0.f), mParticleRadius, Vec3(0.f, 0.f, 1.f)));

}

void checkForParticleInteractions(Particle* p) {
    for (auto otherP : mParticles) {
        if (otherP != p) {
            Vec3 dist = toVec3(p->getCurrentPos() - otherP->getCurrentPos());
            float minDist = p->getRadius() + otherP->getRadius() + 5.f;
            if (dist.length() < minDist) {
                //collision!
                float amtToMove = abs(dist.length() - minDist);
                p->reflectOffOf(toVec3(dist.normalized()), amtToMove);
                otherP->reflectOffOf(toVec3(dist.normalized() * -1.f), amtToMove);
            }
        }
    }
}

void checkForGroundInteraction(Particle* p) {
    if (p->getCurrentPos().y() < mGroundPlanePos + p->getRadius()) {
        float amtToMove = abs(p->getCurrentPos().y() - (mGroundPlanePos + p->getRadius()));
        p->reflectOffOf(Vec3(0.f, 1.f, 0.f), amtToMove);
    }
}

void checkForObstacleInteraction(Particle* p) {

}

void drawCube(Vec3 pos) {
	// Render a color-cube consisting of 6 quads with different colors
	
    glPushMatrix();
	glTranslatef(pos.x(), pos.y(), pos.z());

	glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
    glPopMatrix();
}

void drawGroundPlane() {
    glPushMatrix();
    glTranslatef(0.f, 0.f, 0.f);
    glScalef(mGroundPlaneSize, 1.f, mGroundPlaneSize);

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, 0.0f, 1.0f);

    glEnd();  // End of drawing ground plane
    glPopMatrix();
}

void updateParticles(float dt) {
    auto it = begin(mParticles);
    while (it != end(mParticles)) {
        // Do some stuff
        auto a = (Particle*)*it;
        if (!a->getIsAlive()) {
            delete a;
            it = mParticles.erase(it);
        }
        else {
            a->addForce(mGravity);
            a->update(dt);
            //checkForParticleInteractions(a); //TODO: does this need to be before the moving?
            checkForGroundInteraction(a);
            //checkForObstacleInteraction(a);
            
            ++it;
        }
    }
}

void display() {
    //auto start = high_resolution_clock::now();

    Vec3 lookAt = toVec3(cameraFront + cameraPos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //OutputDebugStringA(cameraPos.toString().c_str());
    glLoadIdentity();
    gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(), lookAt.x(), lookAt.y(), lookAt.z(), cameraUp.x(), cameraUp.y(), cameraUp.z());

    drawCube(Vec3(50.5f, 0.0f, -36.0f));
    drawGroundPlane();

    for (auto it : mParticles) {
        it->draw();
    }

    glutSwapBuffers();
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(stop - start);

    //OutputDebugString(("Time taken by function: " + std::to_string(duration.count() / 1000.0f) + " milliseconds\n").c_str());   
}

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    glClearDepth(1.0f);                   
    glEnable(GL_DEPTH_TEST);   
    glDepthFunc(GL_LEQUAL);    
    glShadeModel(GL_SMOOTH);   
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(GLsizei width, GLsizei height) { 
   // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

void animLoop(int val) {
    keyOperations();

    updateParticles(deltaTime);

    glutPostRedisplay();
    glutTimerFunc(16, animLoop, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	/*display stuff*/
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
	glutCreateWindow("Project 1");
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	initGL();
    
    /*particle stuff*/
    initParticles();

    /*interactions stuff*/
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses  
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
    glEnable(GL_CULL_FACE);

    glutTimerFunc(1, animLoop, 1);
	glutMainLoop();
	return 0;
}