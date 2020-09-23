#include "main.h"



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
        cameraPos += toVec3(cameraFront * dCam * moveMult);
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

void initObstacles() {

    float currBuildX = -1.0f;
    float currBuildZ = -1.0f;
    
    float gridSpacing = buildingMin + buildingSize;

    for (int r = 0; r < BUILDING_GRID_ROW; r++) {
        for (int c = 0; c < BUILDING_GRID_COL; c++) {
               
            float buildingWidth = buildingMin + rand() % buildingSize;
            float buildingHeight = buildingMin + rand() % buildingHeightSize;

            Vec3 pos = Vec3(currBuildX, 1.0f, currBuildZ);
            Vec3 size = Vec3(10, buildingHeight, -buildingWidth);

            Vec3 color = Vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
            mObstacles.push_back(new Obstacle(pos, size, color));

            currBuildX += gridSpacing;
        }
        currBuildX = -1.0f;
        currBuildZ += gridSpacing;
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

void drawBuilding(Vec3 pos, Vec3 size) {
    // Render a color-cube consisting of 6 quads with different colors

    glPushMatrix();
    glTranslatef(pos.x(), pos.y(), pos.z());

    glBegin(GL_QUADS);
    // Top face (y = 1.0f)
    glColor3f(0.0f, 1.0f, 0.0f);     // Green
    glVertex3f(0, size.y(), 0);
    glVertex3f(size.x(), size.y(), 0);
    glVertex3f(size.x(), size.y(), size.z());
    glVertex3f(0, size.y(), size.z());

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f);     // Orange
    glVertex3f(0, 0, 0);
    glVertex3f(size.x(), 0, 0);
    glVertex3f(size.x(), 0, size.z());
    glVertex3f(0, 0, size.z());

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);     // Red
    glVertex3f(0, 0, 0);
    glVertex3f(0, size.y(), 0);
    glVertex3f(0, size.y(), size.z());
    glVertex3f(0, 0, size.z());


    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
    glVertex3f(size.x(), 0, 0);
    glVertex3f(size.x(), 0, size.z());
    glVertex3f(size.x(), size.y(), size.z());
    glVertex3f(size.x(), size.y(), 0);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);     // Blue
    glVertex3f(0, 0, 0);
    glVertex3f(size.x(), 0, 0);
    glVertex3f(size.x(), size.y(), 0);
    glVertex3f(0, size.y(), 0);
    

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
    glVertex3f(0, 0, size.z());
    glVertex3f(0, size.y(), size.z());
    glVertex3f(size.x(), size.y(), size.z());
    glVertex3f(size.x(), 0, size.z());
    glEnd();  // End of drawing color-cube
    glPopMatrix();
}

void drawObstacles() {
    for (auto it : mObstacles) {
        //it->update(dt);
        //checkForParticleInteractions(it); //TODO: does this need to be before the moving?
        it->draw();
    }
}


//void setupBuilding() {
//    Vec3 pos = Vec3(-1.0f, 1.0f, -1.0f);
//    Vec3 size = Vec3(50, buildingHeight, -buildingWidth);
//    drawBuilding(pos, size);
//}

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
    //update time info
    //float currTime = glutGet(GLUT_ELAPSED_TIME) / 1000;
   // deltaTime = currTime - previousFrame;
    //previousFrame = currTime;

    //auto start = high_resolution_clock::now();

    Vec3 lookAt = toVec3(cameraFront + cameraPos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    //OutputDebugStringA(cameraPos.toString().c_str());
    glLoadIdentity();
    gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(), lookAt.x(), lookAt.y(), lookAt.z(), cameraUp.x(), cameraUp.y(), cameraUp.z());

    drawCube(Vec3(1.5f, 0.0f, -7.0f));
    drawGroundPlane();
    updateParticles(deltaTime);
    drawObstacles();

    glutSwapBuffers();
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(stop - start);

    //OutputDebugString(("Time taken by function: " + std::to_string(duration.count() / 1000.0f) + " milliseconds\n").c_str());
    
}

void mouse(int button, int state, int x, int y) {
    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) { // It's a wheel event 
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
    } else {  // normal button event
        printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    }
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

void animLoop(int val) {

    keyOperations();

    glutPostRedisplay();
    glutTimerFunc(16, animLoop, 1);
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
    initObstacles();

    /*interactions stuff*/
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses  
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
    //glutPassiveMotionFunc(mouseMovement);
   // glutMotionFunc(mouseMovement);
    //glutSetCursor(GLUT_CURSOR_NONE);
    glutMouseFunc(mouse);
    glEnable(GL_CULL_FACE);

    glutTimerFunc(1, animLoop, 1);
	glutMainLoop();
	return 0;
}