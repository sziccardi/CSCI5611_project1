#include "main.h"

/*INPUTS*/
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

    //cout << cameraPos.toString() << endl;
}

void mouse(int button, int state, int x, int y) {
    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if ((button == 3) || (button == 4)) { // It's a wheel event 
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
    }
    else {  // normal button event
        printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    }
}

/*INIT  OBJECTS*/
void initGroundPlane() {
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);

    glBindVertexArray(groundVAO);

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    groundTexture = loadTexture("groundTexture.png");
    linkTexture();
}

void initParticles() {
    glGenVertexArrays(1, &particleVAO);
    glGenBuffers(1, &particleVBO);

    glBindVertexArray(particleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    particleTexture = loadTexture("testTexture.png");
    linkTexture();

    makeParticles();
}

void makeParticles() {
    if (mParticles.size() >= mMaxNumParticles) return;
    //for (int i = 0; i < mMaxNumParticles; i++) {
    //    float x = (float)(rand() % 120 - 66.f);
    //    float y = (float)(rand() % 240 + 200.f);
    //    float z = (float)(-1.0f * (rand() % 120));
    //    //float x = 0.f;
    //    //float y = 120.f;
    //    //float z = -120.f;
    //    Vec3 pos = Vec3(x, y, z);
    //    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //    float g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    //    float b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

    //    Vec3 color = Vec3(r,g, b);
    //    mParticles.push_back(new Particle(pos, Vec3(0.f, 0.f, 0.f), mParticleRadius, color));
    //}

    /*for testing particle collisions*/
    //mParticles.push_back(new Particle(Vec3(-10, 600.f, 10.f), Vec3(0.f, -60.f, 0.f), mParticleRadius, Vec3(1.f, 0.f, 0.f)));
    //mParticles.push_back(new Particle(Vec3(10.f, 600.f, 10.f), Vec3(0.f, -60.f, 0.f), mParticleRadius, Vec3(0.f, 0.f, 1.f)));

    /*for testing flocking*/
        float theta = (float)(rand()) / (float)(RAND_MAX)*M_PI;
        float amt = (float)(rand() % 50 + 10.f);
        float x = amt * cos(theta);
        float adjust = (float)(rand() % 10 + 10.f);
        float y = 0.f;
        float z = -amt * sin(theta);

        auto myP = new Particle(toVec3(cameraPos + Vec3(adjust, -20.f, -adjust)), Vec3(x, y, z), mParticleRadius, Vec3(0.f, 0.f, 0.f));
        mParticles.push_back(myP);
    
}

void initObstacles() {
    glGenVertexArrays(1, &buildingVAO);
    glGenBuffers(1, &buildingVBO);

    glBindVertexArray(buildingVAO);

    glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buildingVertices), buildingVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load image, create texture and generate mipmaps
    buildingTexture = loadTexture("buildingTexture.png");
    linkTexture();

    float gridSpacing = buildingMin + buildingSize;
    float currBuildX = 0.f;
    float currBuildZ = 0.f;

    for (int r = 0; r < BUILDING_GRID_ROW; r++) {
        for (int c = 0; c < BUILDING_GRID_COL; c++) {

            float buildingWidth = buildingMin + rand() % buildingSize;
            float buildingHeight = buildingMin + rand() % buildingHeightSize;

            Vec3 pos = Vec3(currBuildX, 1.0f, currBuildZ);
            Vec3 size = Vec3(buildingWidth / 2, buildingHeight, buildingWidth);

            Vec3 color = Vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
            mObstacles.push_back(new Obstacle(pos, size, color, "buildingTexture.png"));

            currBuildX += gridSpacing;
        }
        currBuildX = 0.f;
        currBuildZ += gridSpacing;
    }
}

/*DRAWING*/
void drawObstacles() {
    glPushMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buildingTexture);
    // activate shader
    glUseProgram(shaderProgram);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

    // render boxes
    glBindVertexArray(buildingVAO);


    for (auto it : mObstacles) {
        auto model = it->draw(1);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glPopMatrix();
}

void drawParticles() {
    glPushMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleTexture);
    // activate shader
    glUseProgram(shaderProgram);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

    // render boxes
    glBindVertexArray(buildingVAO);

    for (auto it : mParticles) {
        auto model = it->draw();
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glPopMatrix();
}

void drawGroundPlane() {
    glPushMatrix();
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    // activate shader
    glUseProgram(shaderProgram);

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);

    // render boxes
    glBindVertexArray(buildingVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(mGroundPlaneSize * 2, 1.f, mGroundPlaneSize * 2));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glPopMatrix();
}

/*MOTION*/
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
            //flock!
            Vec3 averagePos = a->getCurrentPos();
            Vec3 averageVel = a->getCurrentVelocity();
            Vec3 averageDiff = Vec3(0.f, 0.f, 0.f);
            int neighborCount = 0;
            auto cit = begin(mParticles);
            while (cit != end(mParticles)) {
                auto b = (Particle*)*cit;
                if (cit != it &&
                    toVec3(b->getCurrentPos() - a->getCurrentPos()).length() < mFlockRadius)
                {
                    //we are neighbors!
                    averageDiff += toVec3(b->getCurrentPos() - a->getCurrentPos());
                    averagePos += b->getCurrentPos();
                    averageVel += b->getCurrentVelocity();

                    neighborCount++;
                }
                ++cit;
            }
            if (neighborCount > 0) {
                averageDiff *= (1.f / neighborCount);
                averagePos *= (1.f / neighborCount);
                averageVel.normalize();
                a->flock(averageVel, averagePos, averageDiff);
            }
            

            //a->addForce(mGravity); //only when sparkles not when flocking magic things
            
            a->update(dt);
            checkForParticleInteractions(a); //TODO: does this need to be before the moving?
            checkForGroundInteraction(a);
            checkForObstacleInteraction(a);

            ++it;
        }
    }
    makeParticles(); //if necessary
}

void checkForParticleInteractions(Particle* p) {
    for (auto otherP : mParticles) {
        if (otherP != p) {
            Vec3 dist = toVec3(p->getCurrentPos() - otherP->getCurrentPos());
            float minDist = p->getRadius() + otherP->getRadius();
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
    for (auto obs : mObstacles) {
        Vec3 diff = toVec3(p->getCurrentPos() - obs->getCurrentPos());
        float yLim = p->getRadius() + obs->getCurrentSize().y() / 2;
        float xLim = p->getRadius() + obs->getCurrentSize().x() / 2;
        float zLim = p->getRadius() + obs->getCurrentSize().z() / 2;
        if (abs(diff.x()) < abs(xLim) &&
            abs(diff.y()) < abs(yLim) &&
            abs(diff.z()) < abs(zLim))
        {
            //cout << "BOUNCE" << endl;
            //collision!
            if (abs(diff.x()) > abs(diff.y()) && abs(diff.x()) > abs(diff.z())) {
                //hitting in the x direction
                if (diff.x() < 0) {
                    //from the left
                    float amtToMove = abs((p->getCurrentPos().x() + p->getRadius()) - (obs->getCurrentPos().x() - obs->getCurrentSize().x() / 2));
                    p->reflectOffOf(Vec3(-1.f, 0.f, 0.f), amtToMove);
                }
                else {
                    // from the right
                    float amtToMove = abs((p->getCurrentPos().x() - p->getRadius()) - (obs->getCurrentPos().x() + obs->getCurrentSize().x() / 2));
                    p->reflectOffOf(Vec3(-1.f, 0.f, 0.f), amtToMove);
                }
            }
            else if (abs(diff.y()) > abs(diff.x()) && abs(diff.y()) > abs(diff.z())) {
                //hitting in the y direction
                if (diff.y() < 0) {
                    //from the underside
                    float amtToMove = abs((p->getCurrentPos().y() + p->getRadius()) - (obs->getCurrentPos().y() - obs->getCurrentSize().y() / 2));
                    p->reflectOffOf(Vec3(0.f, -1.f, 0.f), amtToMove);
                }
                else {
                    // from the top
                    float amtToMove = abs((p->getCurrentPos().y() - p->getRadius()) - (obs->getCurrentPos().y() + obs->getCurrentSize().y() / 2));
                    p->reflectOffOf(Vec3(0.f, 1.f, 0.f), amtToMove);
                }
            }
            else if (abs(diff.z()) > abs(diff.x()) && abs(diff.z()) > abs(diff.y())) {
                //hitting in the z direction
                if (diff.z() < 0) {
                    //from the back
                    float amtToMove = abs((p->getCurrentPos().z() + p->getRadius()) - (obs->getCurrentPos().z() - obs->getCurrentSize().z() / 2));
                    p->reflectOffOf(Vec3(0.f, 0.f, -1.f), amtToMove);
                }
                else {
                    // from the frpnt
                    float amtToMove = abs((p->getCurrentPos().z() - p->getRadius()) - (obs->getCurrentPos().z() + obs->getCurrentSize().z() / 2));
                    p->reflectOffOf(Vec3(0.f, 0.f, 1.f), amtToMove);
                }
            }
        }
    }
}

/*RENDERING*/
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void linkTexture() {
    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler"), 0);
}

void display() {
    //auto start = high_resolution_clock::now();

    Vec3 lookAt = toVec3(cameraFront + cameraPos);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //OutputDebugStringA(cameraPos.toString().c_str());
    glLoadIdentity();
    gluLookAt(cameraPos.x(), cameraPos.y(), cameraPos.z(), lookAt.x(), lookAt.y(), lookAt.z(), cameraUp.x(), cameraUp.y(), cameraUp.z());

    drawGroundPlane();
    drawObstacles();
    drawParticles();


    glutSwapBuffers();
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(stop - start);

    //OutputDebugString(("Time taken by function: " + std::to_string(duration.count() / 1000.0f) + " milliseconds\n").c_str());   
}

void initGL() {
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Project 1");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
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
    gluPerspective(45.0f, aspect, 0.1f, cameraDepth);
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
    initGL();
    glewInit();

    /*particle stuff*/
    initParticles();
    initObstacles();
    initGroundPlane();

    /*interactions stuff*/
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses  
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
    //glutPassiveMotionFunc(mouseMovement);
   // glutMotionFunc(mouseMovement);
    //glutSetCursor(GLUT_CURSOR_NONE);
    glutMouseFunc(mouse);

    glutTimerFunc(1, animLoop, 1);
    glutMainLoop();

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    return 0;
}