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
        cameraPos += toVec3(cameraFront * dCam * cameraSpeedScale);
        if (cameraPos.y() < mGroundPlanePos + 5.f) {
            cameraPos.setVal(1, 0, mGroundPlanePos + 5.f);
        }
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
        // Mouse click, spawn particles
        makeParticles();
        //printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
    }
}

/*INIT  OBJECTS*/
void initGroundPlane() {
    glGenVertexArrays(1, &groundVAO);
    glCheckError();
    glGenBuffers(1, &groundVBO);
    glCheckError();

    glBindVertexArray(groundVAO);

    glCheckError();

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);
    glCheckError();

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();

    groundTexture = loadTexture("groundTexture.png");
    linkTexture();
}

void initParticles() {
    glGenVertexArrays(1, &particleVAO);
    glCheckError();
    glGenBuffers(1, &particleVBO);
    glCheckError();
    glBindVertexArray(particleVAO);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
    glCheckError();
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();
    // load image, create texture and generate mipmaps
    particleTexture = loadTexture("glowTexture.png");
    glCheckError();
    linkTexture();
    glCheckError();

   // makeParticles();
}

void makeParticles() {

    if (mParticles.size() >= mMaxNumParticles) return;
    
    /*for testing flocking*/
    //float theta = (float)(rand()) / (float)(RAND_MAX)*M_PI;
    for (int i = 0; i < mNumParticleBurst; i++) {
        float amt = (float)(rand() % 50 + 10.f);

        Vec3 particleVel = toVec3(cameraFront.normalized() * mParticleMaxVelocity);
        int randomness = 5;
        float adjustXVel = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));
        float adjustYVel = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));
        float adjustZVel = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));
        particleVel += Vec3(adjustXVel, adjustYVel, adjustZVel);

        randomness = 4;
        //Randomness from -random to random
        float adjustX = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));
        float adjustY = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));
        float adjustZ = (float)(rand() % randomness * ((rand() % 2) * 2 - 1));

        Vec3 normalized = toVec3(cameraFront.normalized());
        Vec3 spawnPoint = toVec3(cameraPos + normalized * particleCameraSpawnPoint + Vec3(normalized.x() * adjustX, normalized.y() * adjustY, normalized.z() * adjustZ));
        auto myP = new Particle(spawnPoint, particleVel, mParticleRadius, Vec3(0.f, 0.f, 0.f));
        mParticles.push_back(myP);
    }
}

void initObstacles() {
    glGenVertexArrays(1, &buildingVAO);
    glCheckError();
    glGenBuffers(1, &buildingVBO);
    glCheckError();
    glBindVertexArray(buildingVAO);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, sizeof(buildingVertices), buildingVertices, GL_STATIC_DRAW);
    glCheckError();
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();
    // load image, create texture and generate mipmaps
    buildingTexture = loadTexture("buildingTexture.png");
    glCheckError();
    linkTexture();
    glCheckError();
    float gridSpacing = buildingMin + buildingSize + buildingAdjustment;
    float currBuildX = 100.f;
    float currBuildZ = 100.f;

    for (int r = 0; r < BUILDING_GRID_ROW; r++) {
        for (int c = 0; c < BUILDING_GRID_COL; c++) {

            float buildingWidth = buildingMin + rand() % buildingSize;
            float buildingWidth2 = buildingMin + rand() % buildingSize;
            float buildingHeight = buildingMin + 150 + rand() % buildingHeightSize;

            Vec3 pos = Vec3(currBuildX, 1.0f, currBuildZ);
            Vec3 size = Vec3(buildingWidth, buildingHeight, buildingWidth2);

            Vec3 color = Vec3(static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX), static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
            mObstacles.push_back(new Obstacle(pos, size, color, "buildingTexture.png"));

            currBuildX += gridSpacing;
        }
        currBuildX = 100.f;
        currBuildZ += gridSpacing;
    }
}

//*DRAWING*/
void drawObstacles() {
    glPushMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, buildingVBO);
    glCheckError();
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, buildingTexture);
    glCheckError();
    // activate shader
    glUseProgram(shaderProgram);
    glCheckError();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glCheckError();

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glCheckError();

    // render boxes
    glBindVertexArray(buildingVAO);
    glCheckError();


    for (auto it : mObstacles) {
        auto model = it->draw(1);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glCheckError();

        glDrawArrays(GL_TRIANGLES, 0, 36);
        glCheckError();
    }
    glPopMatrix();
}

void drawParticles() {
    glPushMatrix();

    glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
    glCheckError();
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, particleTexture);

    // activate shader
    glUseProgram(shaderProgram);
    glCheckError();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glCheckError();

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glCheckError();

    // render boxes
    glBindVertexArray(buildingVAO);
    glCheckError();


    for (auto it : mParticles) {
        
        auto model = it->draw(cameraPos);

        //TODO: adjust the model to face the camera
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
        glCheckError();

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glCheckError();
    }
    glPopMatrix();
}

void drawGroundPlane() {
    glPushMatrix();
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glCheckError();
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glCheckError();
    glEnableVertexAttribArray(0);
    glCheckError();
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glCheckError();
    glEnableVertexAttribArray(1);
    glCheckError();
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, groundTexture);
    glCheckError();
    // activate shader
    glUseProgram(shaderProgram);
    glCheckError();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = glm::perspective(glm::radians(45.f), (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT), 0.1f, cameraDepth);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glCheckError();

    // camera/view transformation
    glm::vec3 newCameraPos = glm::vec3(cameraPos.x(), cameraPos.y(), cameraPos.z());
    glm::vec3 newCameraFront = glm::vec3(cameraFront.x(), cameraFront.y(), cameraFront.z());
    glm::vec3 newCameraUp = glm::vec3(cameraUp.x(), cameraUp.y(), cameraUp.z());
    glm::mat4 view = glm::lookAt(newCameraPos, newCameraPos + newCameraFront, newCameraUp);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glCheckError();

    // render boxes
    glBindVertexArray(buildingVAO);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(mGroundPlaneSize * BUILDING_GRID_COL, 1.f, mGroundPlaneSize * BUILDING_GRID_ROW));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glCheckError();

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glCheckError();

    glPopMatrix();
    glCheckError();
}

/*MOTION*/
void updateParticles(float dt) {
    auto cit = begin(mNewParticles);
    while (cit != end(mNewParticles)) {
        //for (auto it : mNewParticles) {
            mParticles.push_back((*cit));
            cit = mNewParticles.erase(cit);
       // }
    }
    mNewParticles.clear();

    auto it = begin(mParticles);
    while (it != end(mParticles)) {
        // Do some stuff
        auto a = (Particle*)*it;
        if (!a) continue;
        if (!a->getIsAlive()) {
            delete a;
            it = mParticles.erase(it);
        }
        else {
            //flock! only in first phase
            if (a->getIsFlocking()) {
                Vec3 averagePos = a->getCurrentPos();
                Vec3 averageVel = a->getCurrentVelocity();
                int neighborCount = 0;
                for (auto b : mParticles) {
                    Vec3 diff = toVec3(toVec3(a->getCurrentPos() - b->getCurrentPos()));
                    if (b != a && b->getIsFlocking() &&
                        diff.length() < mFlockRadius)
                    {
                        //we are neighbors!

                        //separation
                        Vec3 newDiff = toVec3(diff.normalized());
                        newDiff.setToLength(separationAmt / diff.lengthSqr());
                        a->addForce(newDiff);
                        //tally for attraction
                        averagePos += b->getCurrentPos();
                        //tally for alignment
                        averageVel += b->getCurrentVelocity();

                        neighborCount++;
                    }
                }
                if (neighborCount > 0) {
                    averagePos *= (1.f / neighborCount);
                    averageVel *= (1.f / neighborCount);
                    a->flock(averageVel, averagePos);
                }
            }

            if (!a->getIsFlocking()) {
                checkForParticleInteractions(a); //TODO: does this need to be before the moving?
                a->addForce(mGravity);
            }
            checkForGroundInteraction(a);
            checkForObstacleInteraction(a);
            checkForExplosion(a);
            a->update(dt);

            ++it;
        }
    }
    //makeParticles(); //if necessary

    sort(mParticles.begin(), mParticles.end(), compareDepth);
}
// Compares two intervals according to staring times. 
bool compareDepth(Particle* p1, Particle* p2)
{
    return (toVec3(cameraPos - p1->getCurrentPos()).length() > toVec3(cameraPos - p2->getCurrentPos()).length());
}

void checkForParticleInteractions(Particle* p) {
    for (auto otherP : mParticles) {
        if (otherP != p) {
            if (otherP->getAge() > otherP->getFadeOn() && p->getAge() > p->getFadeOn()) {
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
}

void checkForGroundInteraction(Particle* p) {
    if (p->getCurrentPos().y() < mGroundPlanePos + p->getRadius()) {
        float amtToMove = abs(p->getCurrentPos().y() - (mGroundPlanePos + p->getRadius()));
        p->reflectOffOf(Vec3(0.f, 1.f, 0.f), amtToMove);
    }
}

void checkForObstacleInteraction(Particle* p) {
    for (auto obs : mObstacles) {
        if (p->getIsFlocking()) {
            Vec3 diff = Vec3(p->getCurrentPos().x() - obs->getCurrentPos().x(), 0.f, p->getCurrentPos().z() - obs->getCurrentPos().z());
            float rad = sqrt((obs->getCurrentSize().x() / 2) * (obs->getCurrentSize().x() / 2) + (obs->getCurrentSize().z() / 2) * (obs->getCurrentSize().z() / 2));
            float diffLength = diff.length();
            if (diffLength < rad + mFlockRadius + 100.f && p->getCurrentPos().y() < obs->getCurrentPos().y() + obs->getCurrentSize().y() / 2 + mFlockRadius + 100.f) {
                if (p->getCurrentPos().y() > obs->getCurrentPos().y() + obs->getCurrentSize().y() / 2) {
                    auto newDiff = Vec3(0.0f, 1.f, 0.f);
                    float dist = p->getCurrentPos().y() - (obs->getCurrentPos().y() + obs->getCurrentSize().y() / 2);
                    newDiff.setToLength(obstacleAvoidAmt / (dist * dist));
                    p->addForce(newDiff);
                }
                else {
                    auto newDiff = diff;
                    float denomonator = max((diffLength - rad), 0.f);
                    //cout << denomonator << endl;
                    newDiff.setToLength(obstacleAvoidAmt / (denomonator * denomonator));
                    p->addForce(newDiff);
                }
            }
        }   
        else {
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
}

void checkForExplosion(Particle* p) {
    if (p->getIsFlocking() && toVec(p->getCurrentPos() - cameraPos).length() > mExplosionDistance) {
        for (int i = 0; i < mNumParticlesToExplode; i++) {
            float theta = (float)rand() / (float)RAND_MAX * 2 * M_PI;
            float phi = (float)rand() / (float)RAND_MAX * M_PI;
            
            auto vel = Vec3(cos(theta) * sin(phi), cos(phi), sin(theta) * sin(phi));
            vel.normalize();
            vel *= mExplosionForce;

            auto myP = new Particle(p->getCurrentPos(), vel, mParticleRadius / 2, Vec3(0.f, 0.f, 0.f));
            myP->setIsFlocking(false);
            myP->setLifespan(5.f);
            mNewParticles.push_back(myP);
        }
        p->setIsAlive(false);
    }
}

/*RENDERING*/
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glCheckError();

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
        glCheckError();
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glCheckError();
        glGenerateMipmap(GL_TEXTURE_2D);
        glCheckError();

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glCheckError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glCheckError();

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
    glCheckError();
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCheckError();
    glCompileShader(vertex);
    glCheckError();
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCheckError();
    glCompileShader(fragment);
    glCheckError();

    shaderProgram = glCreateProgram();
    glCheckError();
    glAttachShader(shaderProgram, vertex);
    glCheckError();
    glAttachShader(shaderProgram, fragment);
    glCheckError();
    glLinkProgram(shaderProgram);
    glCheckError();

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glCheckError();
    glDeleteShader(fragment);
    glCheckError();
    glUseProgram(shaderProgram);
    glCheckError();
    glUniform1i(glGetUniformLocation(shaderProgram, "sampler"), 0);
    glCheckError();
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

    int numParticles = mParticles.size();

    framesSinceLast++;
    long deltaTime = glutGet(GLUT_ELAPSED_TIME);

    if (deltaTime - lastTimeSecond > 1000) {
        int fps = (int)(framesSinceLast * 1000.0 / (deltaTime - lastTimeSecond));
        lastTimeSecond = deltaTime;
        framesSinceLast = 0;
        cout << "FPS: " << fps << "\t Particles: " << numParticles << endl;
    }
    //auto stop = high_resolution_clock::now();
    //auto duration = duration_cast<microseconds>(stop - start);

    //OutputDebugString(("Time taken by function: " + std::to_string(duration.count() / 1000.0f) + " milliseconds\n").c_str());   
}

void initGL() {
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(908, 640);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("5611 Project 1 - Ziccardi & Stoffel");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glCheckError();
}

void reshape(GLsizei width, GLsizei height) {
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    glCheckError();
    glLoadIdentity();             // Reset
    glCheckError();
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, cameraDepth);
    glCheckError();
}

void animLoop(int val) {
    keyOperations();

    updateParticles(deltaTime);
    framesSinceLast += 1;
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
    
    glCheckError();
    glutMainLoop();

    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    return 0;
}