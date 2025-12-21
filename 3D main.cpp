#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

// Global variables
float rotationAngle = 0.0f;
float rotationSpeed = 0.0f;
float targetRotationSpeed = 0.0f;
bool fanOn = false;
int fanSpeedLevel = 0; // 0-5
bool accelerating = false;
bool decelerating = false;
float accelerationRate = 0.5f; // How fast the fan speeds up
float decelerationRate = 1.0f; // How fast the fan slows down

// Camera control
float cameraAngleX = 25.0f;
float cameraAngleY = -30.0f;
float cameraDistance = 25.0f;
bool mouseLeftDown = false;
bool mouseRightDown = false;
int lastMouseX = 0, lastMouseY = 0;

// Colors (keeping your color palette)
float deskColor[3] = {0.55f, 0.27f, 0.07f}; // Brown
float fanColor[3] = {0.4f, 0.4f, 0.4f}; // Gray
float standColor[3] = {0.2f, 0.2f, 0.2f}; // Black
float buttonColor[3] = {0.8f, 0.2f, 0.2f}; // Red for power button
float speedButtonColor[3] = {0.2f, 0.6f, 0.2f}; // Green for speed buttons
float bladeColors[5][3] = {
    {0.9f, 0.2f, 0.2f}, // Red
    {0.2f, 0.9f, 0.2f}, // Green
    {0.2f, 0.2f, 0.9f}, // Blue
    {0.9f, 0.9f, 0.2f}, // Yellow
    {0.9f, 0.2f, 0.9f}  // Magenta
};
float cageColor[3] = {0.5f, 0.5f, 0.5f}; // Gray cage

// Window dimensions
int windowWidth = 1000;
int windowHeight = 700;

// Function to draw a cylinder
void drawCylinder(float radius, float height, int slices) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluCylinder(quadric, radius, radius, height, slices, 1);
    gluDeleteQuadric(quadric);
}

// Function to draw a disk
void drawDisk(float innerRadius, float outerRadius, int slices, int loops) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluDisk(quadric, innerRadius, outerRadius, slices, loops);
    gluDeleteQuadric(quadric);
}

// Function to draw the desk (3D version)
void drawDesk() {
    glColor3fv(deskColor);
    
    // Desk top
    glPushMatrix();
    glTranslatef(0.0f, -2.0f, 0.0f);
    glScalef(8.0f, 0.3f, 4.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Desk legs
    float legPositions[][3] = {
        {-3.8f, -3.0f, -1.8f},
        {3.8f, -3.0f, -1.8f},
        {-3.8f, -3.0f, 1.8f},
        {3.8f, -3.0f, 1.8f}
    };
    
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], legPositions[i][1], legPositions[i][2]);
        glScalef(0.2f, 2.0f, 0.2f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}

// Function to draw the fan stand (3D version)
void drawFanStand() {
    glColor3fv(standColor);
    
    // Base on desk
    glPushMatrix();
    glTranslatef(0.0f, -1.7f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.4f, 0.2f, 20);
    glPopMatrix();
    
    // Main pole
    glPushMatrix();
    glTranslatef(0.0f, -1.6f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.08f, 3.0f, 16);
    glPopMatrix();
    
    // Top joint
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
    glutSolidSphere(0.12f, 16, 16);
    glPopMatrix();
}

// Function to draw the motor housing (3D version)
void drawFanMotor() {
    glColor3fv(fanColor);
    
    // Main motor body
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.15f, 0.3f, 20);
    glPopMatrix();
    
    // Motor face (front)
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.3f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.12f, 0.1f, 16);
    glPopMatrix();
    
    // Connection arm from stand to fan
    glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawCylinder(0.05f, 1.0f, 12);
    glPopMatrix();
}

// Function to draw the fan hub (3D version)
void drawFanHub() {
    glColor3f(0.1f, 0.1f, 0.1f); // Black hub
    
    glPushMatrix();
    glTranslatef(1.0f, 1.4f, 0.0f); // Position at end of arm
    
    // Main hub
    glutSolidSphere(0.1f, 16, 16);
    
    // Hub front
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.05f);
    glutSolidSphere(0.08f, 12, 12);
    glPopMatrix();
    
    glPopMatrix();
}

// Function to draw a single fan blade (3D version)
void drawBlade(int bladeIndex) {
    glColor3fv(bladeColors[bladeIndex]);
    
    // 3D blade shape with thickness
    glBegin(GL_QUADS);
    
    // Front face
    glVertex3f(0.0f, 0.0f, 0.01f);
    glVertex3f(0.0f, 0.0f, 0.01f);
    glVertex3f(0.8f, 0.15f, 0.01f);
    glVertex3f(0.8f, -0.15f, 0.01f);
    
    // Back face
    glVertex3f(0.0f, 0.0f, -0.01f);
    glVertex3f(0.8f, -0.15f, -0.01f);
    glVertex3f(0.8f, 0.15f, -0.01f);
    glVertex3f(0.0f, 0.0f, -0.01f);
    
    // Side faces
    glVertex3f(0.0f, 0.0f, 0.01f);
    glVertex3f(0.0f, 0.0f, -0.01f);
    glVertex3f(0.8f, 0.15f, -0.01f);
    glVertex3f(0.8f, 0.15f, 0.01f);
    
    glVertex3f(0.0f, 0.0f, 0.01f);
    glVertex3f(0.8f, -0.15f, 0.01f);
    glVertex3f(0.8f, -0.15f, -0.01f);
    glVertex3f(0.0f, 0.0f, -0.01f);
    
    glEnd();
}

// Function to draw all fan blades (3D version)
void drawFanBlades() {
    glPushMatrix();
    glTranslatef(1.0f, 1.4f, 0.0f); // Position at end of arm
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f); // Rotate around Z-axis
    
    // Draw 5 blades evenly spaced
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotatef(i * 72.0f, 0.0f, 0.0f, 1.0f); // 360/5 = 72 degrees
        drawBlade(i);
        glPopMatrix();
    }
    
    glPopMatrix();
}

// Function to draw the safety cage (3D version)
void drawSafetyCage() {
    glColor3fv(cageColor);
    
    glPushMatrix();
    glTranslatef(1.0f, 1.4f, 0.0f); // Position with fan
    
    // Enable wireframe for cage
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(1.5);
    
    // Front ring
    glutSolidTorus(0.02f, 0.85f, 8, 32);
    
    // Back ring
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(0.02f, 0.85f, 8, 32);
    glPopMatrix();
    
    // Vertical supports
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        glRotatef(i * 45.0f, 0.0f, 0.0f, 1.0f);
        glBegin(GL_LINES);
        glVertex3f(0.0f, 0.0f, -0.05f);
        glVertex3f(0.0f, 0.85f, -0.05f);
        glVertex3f(0.0f, 0.0f, 0.05f);
        glVertex3f(0.0f, 0.85f, 0.05f);
        glEnd();
        glPopMatrix();
    }
    
    // Return to filled mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glPopMatrix();
}

// Function to draw the entire fan assembly
void drawFan() {
    drawFanStand();
    drawFanMotor();
    drawFanHub();
    drawSafetyCage();
    drawFanBlades();
}

// Function to draw the control panel (3D version)
void drawControlPanel() {
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    // Control panel background
    glColor3f(0.15f, 0.15f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 220, 50);
    glVertex2f(windowWidth - 30, 50);
    glVertex2f(windowWidth - 30, 300);
    glVertex2f(windowWidth - 220, 300);
    glEnd();
    
    // Panel border
    glColor3f(0.3f, 0.3f, 0.4f);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(windowWidth - 220, 50);
    glVertex2f(windowWidth - 30, 50);
    glVertex2f(windowWidth - 30, 300);
    glVertex2f(windowWidth - 220, 300);
    glEnd();
    
    // Title
    glColor3f(0.9f, 0.9f, 1.0f);
    glRasterPos2f(windowWidth - 210, 280);
    const char* title = "FAN CONTROLS";
    while (*title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *title++);
    }
    
    // Power button
    glColor3fv(buttonColor);
    if (fanOn) {
        glColor3f(0.0f, 0.7f, 0.0f); // Green when on
    }
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 200, 220);
    glVertex2f(windowWidth - 100, 220);
    glVertex2f(windowWidth - 100, 250);
    glVertex2f(windowWidth - 200, 250);
    glEnd();
    
    // Power button border
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(windowWidth - 200, 220);
    glVertex2f(windowWidth - 100, 220);
    glVertex2f(windowWidth - 100, 250);
    glVertex2f(windowWidth - 200, 250);
    glEnd();
    
    // Power button label
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(windowWidth - 185, 237);
    const char* powerText = fanOn ? "POWER ON" : "POWER OFF";
    while (*powerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerText++);
    }
    
    // Speed label
    glColor3f(0.9f, 0.9f, 1.0f);
    glRasterPos2f(windowWidth - 210, 190);
    const char* speedLabel = "SPEED LEVEL:";
    while (*speedLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedLabel++);
    }
    
    // Speed buttons
    for (int i = 0; i < 5; i++) {
        if (i < fanSpeedLevel) {
            glColor3fv(speedButtonColor); // Active speed
        } else {
            glColor3f(speedButtonColor[0] * 0.3, 
                     speedButtonColor[1] * 0.3, 
                     speedButtonColor[2] * 0.3); // Inactive
        }
        
        // Button with 3D effect
        glBegin(GL_QUADS);
        glVertex2f(windowWidth - 200 + i * 35, 140);
        glVertex2f(windowWidth - 170 + i * 35, 140);
        glVertex2f(windowWidth - 170 + i * 35, 170);
        glVertex2f(windowWidth - 200 + i * 35, 170);
        glEnd();
        
        // Button border
        glColor3f(1.0f, 1.0f, 1.0f);
        glLineWidth(1.0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(windowWidth - 200 + i * 35, 140);
        glVertex2f(windowWidth - 170 + i * 35, 140);
        glVertex2f(windowWidth - 170 + i * 35, 170);
        glVertex2f(windowWidth - 200 + i * 35, 170);
        glEnd();
        
        // Speed number
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(windowWidth - 195 + i * 35, 155);
        char speedNum = '1' + i;
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, speedNum);
    }
    
    // Current speed display
    glColor3f(0.9f, 0.9f, 1.0f);
    glRasterPos2f(windowWidth - 210, 110);
    char speedText[50];
    sprintf(speedText, "Current Speed: %d", fanSpeedLevel);
    const char* speedPtr = speedText;
    while (*speedPtr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedPtr++);
    }
    
    // Status indicators
    glRasterPos2f(windowWidth - 210, 85);
    if (accelerating) {
        const char* accelText = "Status: Accelerating...";
        while (*accelText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *accelText++);
        }
    } else if (decelerating) {
        const char* decelText = "Status: Slowing down...";
        while (*decelText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *decelText++);
        }
    } else if (fanOn && rotationSpeed > 0) {
        const char* runningText = "Status: Running at steady speed";
        while (*runningText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *runningText++);
        }
    } else {
        const char* stoppedText = "Status: Stopped";
        while (*stoppedText) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *stoppedText++);
        }
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}

// Function to draw status text
void drawStatusText() {
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    // Title
    glRasterPos2f(30, windowHeight - 40);
    const char* title = "3D VENTILATOR FAN SIMULATION";
    while (*title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *title++);
    }
    
    // Status
    glRasterPos2f(30, windowHeight - 70);
    char status[100];
    sprintf(status, "FAN: %s | TARGET SPEED: %d | CURRENT SPEED: %.1f", 
            fanOn ? "ON" : "OFF", 
            fanSpeedLevel,
            rotationSpeed);
    const char* statusPtr = status;
    while (*statusPtr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *statusPtr++);
    }
    
    // Instructions
    glRasterPos2f(30, windowHeight - 100);
    const char* inst1 = "CONTROLS: Left drag = rotate view | Right drag = zoom";
    while (*inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *inst1++);
    }
    
    glRasterPos2f(30, windowHeight - 115);
    const char* inst2 = "Click POWER button to toggle ON/OFF | Click SPEED buttons 1-5";
    while (*inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *inst2++);
    }
    
    glRasterPos2f(30, windowHeight - 130);
    const char* inst3 = "Keyboard: O=On F=Off 1-5=Speed +/-=Adjust Z/X=Zoom ESC=Exit";
    while (*inst3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *inst3++);
    }
    
    // Features
    glRasterPos2f(30, windowHeight - 155);
    const char* features = "FEATURES: Realistic acceleration/deceleration | 5 colored blades | Safety cage";
    while (*features) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *features++);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}

// Function to handle acceleration/deceleration
void updateFanSpeed() {
    if (fanOn) {
        // Calculate target speed based on speed level
        targetRotationSpeed = fanSpeedLevel * 3.0f;
        
        if (rotationSpeed < targetRotationSpeed - 0.1f) {
            // Accelerating
            accelerating = true;
            decelerating = false;
            rotationSpeed += accelerationRate * 0.05f;
            if (rotationSpeed > targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;
            }
        } else if (rotationSpeed > targetRotationSpeed + 0.1f) {
            // Decelerating (when switching to lower speed)
            accelerating = false;
            decelerating = true;
            rotationSpeed -= decelerationRate * 0.05f;
            if (rotationSpeed < targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;
            }
        } else {
            // At target speed
            accelerating = false;
            decelerating = false;
            rotationSpeed = targetRotationSpeed;
        }
    } else {
        // Fan is off - decelerate to zero
        targetRotationSpeed = 0.0f;
        if (rotationSpeed > 0.1f) {
            decelerating = true;
            accelerating = false;
            rotationSpeed -= decelerationRate * 0.1f;
            if (rotationSpeed < 0) rotationSpeed = 0;
        } else {
            decelerating = false;
            rotationSpeed = 0;
        }
    }
}

// Display function
void display() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)windowWidth / (double)windowHeight, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Camera positioning
    float cameraX = cameraDistance * sin(cameraAngleY * 3.14159f / 180.0f) * cos(cameraAngleX * 3.14159f / 180.0f);
    float cameraY = cameraDistance * sin(cameraAngleX * 3.14159f / 180.0f);
    float cameraZ = cameraDistance * cos(cameraAngleY * 3.14159f / 180.0f) * cos(cameraAngleX * 3.14159f / 180.0f);
    
    gluLookAt(cameraX, cameraY + 3.0f, cameraZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    
    // Set up light
    GLfloat lightPosition[] = {10.0f, 15.0f, 10.0f, 1.0f};
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    
    // Update fan speed with acceleration/deceleration
    updateFanSpeed();
    
    // Update rotation angle
    rotationAngle += rotationSpeed;
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f;
    }
    
    // Draw 3D scene
    drawDesk();
    drawFan();
    
    // Draw 2D overlays
    drawControlPanel();
    drawStatusText();
    
    glutSwapBuffers();
}

// Timer function for smooth animation
void timer(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Mouse button handler
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = true;
            lastMouseX = x;
            lastMouseY = y;
            
            // Check if control panel buttons were clicked
            int glY = windowHeight - y;
            
            // Power button
            if (x >= windowWidth - 200 && x <= windowWidth - 100 &&
                glY >= 220 && glY <= 250) {
                fanOn = !fanOn;
                if (!fanOn) {
                    // Start decelerating when turning off
                    decelerating = true;
                    accelerating = false;
                    fanSpeedLevel = 0;
                } else if (fanSpeedLevel == 0) {
                    // Start accelerating when turning on
                    fanSpeedLevel = 3;
                    accelerating = true;
                    decelerating = false;
                }
                glutPostRedisplay();
                return;
            }
            
            // Speed buttons
            for (int i = 0; i < 5; i++) {
                int buttonX1 = windowWidth - 200 + i * 35;
                int buttonX2 = buttonX1 + 30;
                
                if (x >= buttonX1 && x <= buttonX2 &&
                    glY >= 140 && glY <= 170) {
                    if (fanOn) {
                        fanSpeedLevel = i + 1;
                        if (rotationSpeed < fanSpeedLevel * 3.0f) {
                            accelerating = true;
                            decelerating = false;
                        } else if (rotationSpeed > fanSpeedLevel * 3.0f) {
                            accelerating = false;
                            decelerating = true;
                        }
                    }
                    glutPostRedisplay();
                    return;
                }
            }
        } else {
            mouseLeftDown = false;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseRightDown = true;
            lastMouseX = x;
            lastMouseY = y;
        } else {
            mouseRightDown = false;
        }
    }
}

// Mouse motion handler
void mouseMotion(int x, int y) {
    if (mouseLeftDown) {
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;
        
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
    else if (mouseRightDown) {
        float zoomChange = (y - lastMouseY) * 0.1f;
        cameraDistance += zoomChange;
        
        if (cameraDistance < 10.0f) cameraDistance = 10.0f;
        if (cameraDistance > 50.0f) cameraDistance = 50.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'o': case 'O': // Turn on with smooth acceleration
            fanOn = true;
            if (fanSpeedLevel == 0) fanSpeedLevel = 3;
            accelerating = true;
            decelerating = false;
            break;
        case 'f': case 'F': // Turn off with smooth deceleration
            fanOn = false;
            fanSpeedLevel = 0;
            accelerating = false;
            decelerating = true;
            break;
        case '1': case '2': case '3': case '4': case '5':
            if (fanOn) {
                fanSpeedLevel = key - '0';
                if (rotationSpeed < fanSpeedLevel * 3.0f) {
                    accelerating = true;
                    decelerating = false;
                } else if (rotationSpeed > fanSpeedLevel * 3.0f) {
                    accelerating = false;
                    decelerating = true;
                }
            }
            break;
        case '+': // Increase speed
            if (fanOn && fanSpeedLevel < 5) {
                fanSpeedLevel++;
                accelerating = true;
                decelerating = false;
            }
            break;
        case '-': // Decrease speed
            if (fanOn && fanSpeedLevel > 1) {
                fanSpeedLevel--;
                accelerating = false;
                decelerating = true;
            }
            break;
        case 'z': case 'Z': // Zoom in
            cameraDistance -= 2.0f;
            if (cameraDistance < 10.0f) cameraDistance = 10.0f;
            break;
        case 'x': case 'X': // Zoom out
            cameraDistance += 2.0f;
            if (cameraDistance > 50.0f) cameraDistance = 50.0f;
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
    glutPostRedisplay();
}

// Reshape function
void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("3D Ventilator Fan with Realistic Acceleration");
    
    // Enable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    
    // Set clear color
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    // Print instructions
    printf("==================================================\n");
    printf("3D VENTILATOR FAN WITH REALISTIC ACCELERATION\n");
    printf("==================================================\n");
    printf("ENHANCEMENTS:\n");
    printf("  • Smooth acceleration when turning ON (starts slow)\n");
    printf("  • Smooth deceleration when turning OFF (slows gradually)\n");
    printf("  • Full 3D environment with lighting\n");
    printf("  • Rotatable and zoomable camera view\n");
    printf("  • Realistic 3D desk and fan stand\n");
    printf("  • 5 colored 3D blades with proper spacing\n");
    printf("  • 3D safety cage around blades\n");
    printf("\nCONTROLS:\n");
    printf("  MOUSE:\n");
    printf("    • Left drag = Rotate camera view\n");
    printf("    • Right drag = Zoom in/out\n");
    printf("    • Click buttons in control panel\n");
    printf("  KEYBOARD:\n");
    printf("    • O = Turn fan ON (with smooth acceleration)\n");
    printf("    • F = Turn fan OFF (with smooth deceleration)\n");
    printf("    • 1-5 = Set speed level\n");
    printf("    • +/- = Adjust speed gradually\n");
    printf("    • Z/X = Zoom in/out\n");
    printf("    • ESC = Exit program\n");
    printf("==================================================\n");
    printf("NOTE: Fan starts slowly and accelerates to speed 3 when turned on!\n");
    printf("      Fan slows down gradually when turned off!\n");
    printf("==================================================\n");
    
    glutMainLoop();
    return 0;
}