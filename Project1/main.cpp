#include "main.h"

void keyPressed(unsigned char key, int x, int y) {
    keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

void keyOperations(void) {
    float cameraAdjustedSpeed = cameraSpeed * deltaTime;
    int ww = glutGet(GLUT_WINDOW_WIDTH);
    int wh = glutGet(GLUT_WINDOW_HEIGHT);

    horizontal += mouseSpeed * deltaTime * float(ww / 2 - mouseAngles.x());
    vertical += mouseSpeed * deltaTime * float(wh / 2 - mouseAngles.y());

    //Check for mousing too far
    if (horizontal < -M_PI)
        horizontal += M_PI * 2;
    else if (horizontal > M_PI)
        horizontal -= M_PI * 2;

    if (vertical < -M_PI / 2)
        vertical = -M_PI / 2;
    if (vertical > M_PI / 2)
        vertical = M_PI / 2;

    cameraFront.setVal(0, 0, cos(vertical) * sin(horizontal));
    cameraFront.setVal(1, 0, sin(vertical));
    cameraFront.setVal(2, 0, cos(vertical) * cos(horizontal));

    OutputDebugStringA(cameraFront.toString().c_str());

    if (keyStates['a']) { // If the 'a' key has been pressed  
        // Perform 'a' key operations  
        cameraPos -= cameraFront.cross(cameraUp).normalized() * cameraAdjustedSpeed;
    }

    if (keyStates['w']) { // If the 'a' key has been pressed  
            // Perform 'w' key operations

        //cameraPos += cameraSpeed * cameraFront;
        cameraPos += toVec3(cameraFront * cameraAdjustedSpeed);
    }

    if (keyStates['s']) { // If the 'a' key has been pressed  
        // Perform 's' key operations  
        cameraPos -= toVec3(cameraFront * cameraAdjustedSpeed);
    }

    if (keyStates['d']) { // If the 'a' key has been pressed  
        // Perform 'd' key operations  
        cameraPos += cameraFront.cross(cameraUp).normalized() * cameraAdjustedSpeed;
    }



}

void mouseMovement(int x, int y) {

    mouseAngles.setVal(0, 0, x);
    mouseAngles.setVal(1, 0, y);
    //static bool wrap = false;

    //if (!wrap) {
        int ww = glutGet(GLUT_WINDOW_WIDTH);
        int wh = glutGet(GLUT_WINDOW_HEIGHT);

    //    int dx = x - ww / 2;
    //    int dy = y - wh / 2;

    //    // Do something with dx and dy here

    //    mouseAngles.setVal(0, 0, mouseAngles.x() + dx * mouseSpeed);
    //    mouseAngles.setVal(1, 0, mouseAngles.y() + dy * mouseSpeed);

    //    if (mouseAngles.x() < -M_PI)
    //        mouseAngles.setVal(0, 0, mouseAngles.x() + M_PI * 2);

    //    else if (mouseAngles.x() > M_PI)
    //        mouseAngles.setVal(0, 0, mouseAngles.x() - M_PI * 2);

    //    if (mouseAngles.y() < -M_PI / 2)
    //        mouseAngles.setVal(1, 0, -M_PI / 2);
    //    if (mouseAngles.y() > M_PI / 2)
    //        mouseAngles.setVal(1, 0, M_PI / 2);


    //    cameraFront.setVal(0, 0, cos(mouseAngles.y() * sin(mouseAngles.x())))

    //    // move mouse pointer back to the center of the window
    //    wrap = true;
        glutWarpPointer(ww / 2, wh / 2);
    //}
    //else {
    //    wrap = false;
    //}
}

void initParticles() {
    for (int i = 0; i < mMaxNumParticles; i++) {
        Vec3 pos = Vec3(rand() % 10 - 5.f, rand() % 10 - 5.f, rand() % 10 - 12.f);
        Vec3 color = Vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        mParticles.push_back(new Particle(pos, Vec3(0.f, 0.f, 0.f), mParticleRadius, color));
    }
}

void checkForParticleInteractions(Particle* p)
{
}

void moveParticles(float dt)
{
    for (auto particle : mParticles) {
        particle->update(dt);
    }
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

void drawGroundPlane()
{
    glPushMatrix();

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     
    glVertex3f(100.0f, -1.0f, -100.0f);
    glVertex3f(-100.0f, -1.0f, -100.0f);
    glVertex3f(-100.0f, -1.0f, 100.0f);
    glVertex3f(100.0f, -1.0f, 100.0f);

    glEnd();  // End of drawing ground plane
    glPopMatrix();
}

void updateParticles(float dt) {
    for (auto it : mParticles) {
        //it->update(dt);
        //checkForParticleInteractions(it); //TODO: does this need to be before the moving?
        it->draw();
    }
}

void display() {
    //time in seconds
    float currTime = glutGet(GLUT_ELAPSED_TIME) / 1000;
    deltaTime = currTime - previousFrame;
    previousFrame = currTime;

    auto start = high_resolution_clock::now();
    keyOperations();

    Vec3 lookAt = toVec3(cameraFront + cameraPos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    updateParticles(deltaTime);

    //OutputDebugStringA(cameraPos.toString().c_str());
    glLoadIdentity();
    gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(), lookAt.x(), lookAt.y(), lookAt.z(), cameraUp.x(), cameraUp.y(), cameraUp.z());

    drawCube(Vec3(1.5f, 0.0f, -7.0f));
    drawGroundPlane();

    glutSwapBuffers();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    OutputDebugString(("Time taken by function: " + std::to_string(duration.count() / 1000.0f) + " milliseconds\n").c_str());
    
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
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void glutTimer(int val) {
    glutPostRedisplay();
    glutTimerFunc(16, glutTimer, 1);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);

	/*display stuff*/
    glutInitDisplayMode(GLUT_DOUBLE);
    //glutGameModeString("640x480:32@120");
    //glutEnterGameMode();
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
    glutPassiveMotionFunc(mouseMovement);
    glutMotionFunc(mouseMovement);
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_CULL_FACE);

    glutTimerFunc(1, glutTimer, 1);
	glutMainLoop();
	return 0;
}