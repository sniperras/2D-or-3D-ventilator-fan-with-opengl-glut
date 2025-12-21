#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>

// Global variables
float rotationAngle = 0.0f;
float rotationSpeed = 0.0f;
bool fanOn = false;
int fanSpeedLevel = 0; // 0-5
bool lightingEnabled = true;
float cameraAngleX = 30.0f;
float cameraAngleY = -45.0f;
float cameraDistance = 12.0f;

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Mouse control
bool mouseLeftDown = false;
bool mouseRightDown = false;
int lastMouseX = 0;
int lastMouseY = 0;

// Material colors
GLfloat woodColor[] = {0.55f, 0.27f, 0.07f, 1.0f};
GLfloat metalColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat bladeColors[][4] = {
    {0.8f, 0.2f, 0.2f, 1.0f}, // Red
    {0.2f, 0.8f, 0.2f, 1.0f}, // Green
    {0.2f, 0.2f, 0.8f, 1.0f}, // Blue
    {0.8f, 0.8f, 0.2f, 1.0f}, // Yellow
    {0.8f, 0.2f, 0.8f, 1.0f}  // Magenta
};

// Function to set material properties
void setMaterial(GLfloat ambient[], GLfloat diffuse[], GLfloat specular[], GLfloat shininess) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// Function to set wood material
void setWoodMaterial() {
    GLfloat woodAmbient[] = {0.3f, 0.15f, 0.05f, 1.0f};
    GLfloat woodDiffuse[] = {woodColor[0], woodColor[1], woodColor[2], 1.0f};
    GLfloat woodSpecular[] = {0.1f, 0.1f, 0.1f, 1.0f};
    setMaterial(woodAmbient, woodDiffuse, woodSpecular, 10.0f);
}

// Function to set metal material
void setMetalMaterial() {
    GLfloat metalAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat metalDiffuse[] = {metalColor[0], metalColor[1], metalColor[2], 1.0f};
    GLfloat metalSpecular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    setMaterial(metalAmbient, metalDiffuse, metalSpecular, 100.0f);
}

// Function to set plastic material
void setPlasticMaterial(GLfloat color[]) {
    GLfloat plasticAmbient[] = {color[0]*0.3f, color[1]*0.3f, color[2]*0.3f, 1.0f};
    GLfloat plasticDiffuse[] = {color[0], color[1], color[2], 1.0f};
    GLfloat plasticSpecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    setMaterial(plasticAmbient, plasticDiffuse, plasticSpecular, 30.0f);
}

// Function to draw a cylinder
void drawCylinder(float radius, float height, int slices) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
    
    gluCylinder(quadric, radius, radius, height, slices, 1);
    gluDeleteQuadric(quadric);
}

// Function to draw a disk
void drawDisk(float innerRadius, float outerRadius, int slices, int loops) {
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluQuadricTexture(quadric, GL_TRUE);
    
    gluDisk(quadric, innerRadius, outerRadius, slices, loops);
    gluDeleteQuadric(quadric);
}

// Function to draw the desk
void drawDesk() {
    setWoodMaterial();
    
    // Desk top
    glPushMatrix();
    glScalef(4.0f, 0.2f, 2.0f);
    glutSolidCube(1.0);
    glPopMatrix();
    
    // Desk legs
    float legPositions[][3] = {
        {-1.8f, -1.0f, -0.8f},
        {1.8f, -1.0f, -0.8f},
        {-1.8f, -1.0f, 0.8f},
        {1.8f, -1.0f, 0.8f}
    };
    
    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(legPositions[i][0], legPositions[i][1], legPositions[i][2]);
        glScalef(0.1f, 2.0f, 0.1f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
}

// Function to draw the fan pole
void drawFanPole() {
    setMetalMaterial();
    
    // Main pole
    glPushMatrix();
    glTranslatef(0.0f, -0.8f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.05f, 1.6f, 20);
    glPopMatrix();
    
    // Pole base (weighted bottom)
    glPushMatrix();
    glTranslatef(0.0f, -1.6f, 0.0f);
    glutSolidSphere(0.12f, 20, 20);
    glPopMatrix();
    
    // Pole top joint
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    glutSolidSphere(0.08f, 20, 20);
    glPopMatrix();
}

// Function to draw the motor housing
void drawMotorHousing() {
    setMetalMaterial();
    
    // Main motor body
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    
    // Motor cylinder
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.15f, 0.4f, 30);
    
    // Top cap
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.4f);
    drawDisk(0.0f, 0.15f, 30, 1);
    glPopMatrix();
    
    // Bottom cap
    drawDisk(0.0f, 0.15f, 30, 1);
    glPopMatrix();
    
    // Motor front face
    glPushMatrix();
    glTranslatef(0.0f, 0.15f, 0.0f);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    drawCylinder(0.12f, 0.1f, 20);
    glPopMatrix();
    
    // Cooling fins
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 0.0f);
    for (int i = 0; i < 8; i++) {
        glPushMatrix();
        glRotatef(i * 45.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.2f, 0.0f, 0.0f);
        glScalef(0.3f, 0.02f, 0.05f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    glPopMatrix();
    
    glPopMatrix();
}

// Function to draw a single fan blade
void drawBlade(int bladeIndex) {
    setPlasticMaterial(bladeColors[bladeIndex]);
    
    glBegin(GL_TRIANGLES);
    // Blade geometry
    glNormal3f(0.0f, 1.0f, 0.0f);
    
    // Tip
    glVertex3f(0.0f, 0.0f, 0.02f);
    glVertex3f(0.8f, 0.0f, 0.05f);
    glVertex3f(0.8f, 0.0f, -0.05f);
    
    // Root
    glVertex3f(0.0f, 0.0f, 0.02f);
    glVertex3f(0.8f, 0.0f, -0.05f);
    glVertex3f(0.15f, 0.0f, -0.08f);
    
    glVertex3f(0.0f, 0.0f, 0.02f);
    glVertex3f(0.15f, 0.0f, -0.08f);
    glVertex3f(0.15f, 0.0f, 0.08f);
    
    glVertex3f(0.0f, 0.0f, 0.02f);
    glVertex3f(0.15f, 0.0f, 0.08f);
    glVertex3f(0.8f, 0.0f, 0.05f);
    glEnd();
    
    // Blade thickness
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.02f, 0.02f);
    glVertex3f(0.8f, 0.02f, 0.05f);
    glVertex3f(0.8f, -0.02f, 0.05f);
    glVertex3f(0.0f, -0.02f, 0.02f);
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.02f, -0.02f);
    glVertex3f(0.15f, 0.02f, -0.08f);
    glVertex3f(0.15f, -0.02f, -0.08f);
    glVertex3f(0.0f, -0.02f, -0.02f);
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.15f, 0.02f, -0.08f);
    glVertex3f(0.8f, 0.02f, -0.05f);
    glVertex3f(0.8f, -0.02f, -0.05f);
    glVertex3f(0.15f, -0.02f, -0.08f);
    glEnd();
}

// Function to draw all fan blades
void drawFanBlades() {
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glRotatef(i * 72.0f, 0.0f, 1.0f, 0.0f);
        drawBlade(i);
        glPopMatrix();
    }
    
    // Blade hub
    setMetalMaterial();
    glutSolidSphere(0.1f, 20, 20);
    
    glPopMatrix();
}

// Function to draw the safety cage
void drawSafetyCage() {
    setMetalMaterial();
    glPushMatrix();
    glTranslatef(0.0f, 1.2f, 0.0f);
    
    // Enable wireframe for cage
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    // Front ring
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(0.02f, 0.85f, 10, 36);
    glPopMatrix();
    
    // Middle ring
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(0.02f, 0.7f, 10, 36);
    glPopMatrix();
    
    // Back ring
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);
    glutSolidTorus(0.02f, 0.85f, 10, 36);
    glPopMatrix();
    
    // Vertical supports
    for (int i = 0; i < 12; i++) {
        glPushMatrix();
        glRotatef(i * 30.0f, 0.0f, 1.0f, 0.0f);
        glTranslatef(0.85f, 0.0f, 0.0f);
        glScalef(0.04f, 0.04f, 1.7f);
        glutSolidCube(1.0);
        glPopMatrix();
    }
    
    // Return to filled mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    glPopMatrix();
}

// Function to draw the entire fan
void drawFan() {
    drawFanPole();
    drawMotorHousing();
    drawSafetyCage();
    drawFanBlades();
}

// Function to draw control panel
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
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 150, 50);
    glVertex2f(windowWidth - 50, 50);
    glVertex2f(windowWidth - 50, 250);
    glVertex2f(windowWidth - 150, 250);
    glEnd();
    
    // Power button
    if (fanOn) {
        glColor3f(0.0f, 0.8f, 0.0f); // Green when on
    } else {
        glColor3f(0.8f, 0.0f, 0.0f); // Red when off
    }
    glBegin(GL_QUADS);
    glVertex2f(windowWidth - 140, 200);
    glVertex2f(windowWidth - 60, 200);
    glVertex2f(windowWidth - 60, 220);
    glVertex2f(windowWidth - 140, 220);
    glEnd();
    
    // Power button text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(windowWidth - 130, 210);
    const char* powerText = fanOn ? "ON" : "OFF";
    while (*powerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerText++);
    }
    
    // Speed buttons
    for (int i = 0; i < 5; i++) {
        if (i < fanSpeedLevel) {
            glColor3f(0.0f, 0.5f, 0.8f); // Blue when active
        } else {
            glColor3f(0.3f, 0.3f, 0.3f); // Gray when inactive
        }
        
        glBegin(GL_QUADS);
        glVertex2f(windowWidth - 140, 150 - i * 20);
        glVertex2f(windowWidth - 60, 150 - i * 20);
        glVertex2f(windowWidth - 60, 165 - i * 20);
        glVertex2f(windowWidth - 140, 165 - i * 20);
        glEnd();
        
        // Speed number
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(windowWidth - 120, 158 - i * 20);
        char speedNum = '1' + i;
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, speedNum);
    }
    
    // Labels
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(windowWidth - 140, 230);
    const char* powerLabel = "POWER";
    while (*powerLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerLabel++);
    }
    
    glRasterPos2f(windowWidth - 140, 170);
    const char* speedLabel = "SPEED";
    while (*speedLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedLabel++);
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
    glRasterPos2f(20, windowHeight - 30);
    const char* title = "3D VENTILATOR FAN SIMULATION";
    while (*title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *title++);
    }
    
    // Status
    glRasterPos2f(20, windowHeight - 60);
    char status[100];
    sprintf(status, "FAN: %s | SPEED: %d | LIGHTS: %s", 
            fanOn ? "RUNNING" : "STOPPED", 
            fanSpeedLevel,
            lightingEnabled ? "ON" : "OFF");
    const char* statusPtr = status;
    while (*statusPtr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *statusPtr++);
    }
    
    // Instructions
    glRasterPos2f(20, windowHeight - 90);
    const char* inst1 = "CONTROLS: Left drag = rotate | Right drag = zoom";
    while (*inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *inst1++);
    }
    
    glRasterPos2f(20, windowHeight - 105);
    const char* inst2 = "O/F = Power On/Off | 1-5 = Speed | L = Toggle Lighting | ESC = Exit";
    while (*inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *inst2++);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}

// Display function
void display() {
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
    
    gluLookAt(cameraX, cameraY, cameraZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    
    // Set up lighting
    if (lightingEnabled) {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        
        GLfloat lightPosition[] = {5.0f, 10.0f, 5.0f, 1.0f};
        GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat lightDiffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat lightSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    } else {
        glDisable(GL_LIGHTING);
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Draw scene
    drawDesk();
    drawFan();
    
    // Draw 2D overlays
    drawControlPanel();
    drawStatusText();
    
    glutSwapBuffers();
}

// Timer function for animation
void timer(int value) {
    if (fanOn) {
        rotationAngle += rotationSpeed;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Mouse button handler
void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = true;
            lastMouseX = x;
            lastMouseY = y;
            
            // Check if control panel buttons were clicked
            int glY = windowHeight - y;
            
            // Power button (in control panel)
            if (x >= windowWidth - 140 && x <= windowWidth - 60 &&
                glY >= 200 && glY <= 220) {
                fanOn = !fanOn;
                if (!fanOn) {
                    fanSpeedLevel = 0;
                    rotationSpeed = 0.0f;
                } else if (fanSpeedLevel == 0) {
                    fanSpeedLevel = 3;
                    rotationSpeed = fanSpeedLevel * 2.0f;
                }
                glutPostRedisplay();
                return;
            }
            
            // Speed buttons (in control panel)
            for (int i = 0; i < 5; i++) {
                int buttonY1 = 150 - i * 20;
                int buttonY2 = buttonY1 + 15;
                
                if (x >= windowWidth - 140 && x <= windowWidth - 60 &&
                    glY >= buttonY1 && glY <= buttonY2) {
                    if (fanOn) {
                        fanSpeedLevel = i + 1;
                        rotationSpeed = fanSpeedLevel * 2.0f;
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
        // Rotate camera
        cameraAngleY += (x - lastMouseX) * 0.5f;
        cameraAngleX += (y - lastMouseY) * 0.5f;
        
        // Clamp vertical rotation
        if (cameraAngleX > 89.0f) cameraAngleX = 89.0f;
        if (cameraAngleX < -89.0f) cameraAngleX = -89.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
    else if (mouseRightDown) {
        // Zoom camera (using right mouse drag)
        float zoomChange = (y - lastMouseY) * 0.1f;
        cameraDistance += zoomChange;
        
        // Clamp distance
        if (cameraDistance < 3.0f) cameraDistance = 3.0f;
        if (cameraDistance > 30.0f) cameraDistance = 30.0f;
        
        lastMouseX = x;
        lastMouseY = y;
        
        glutPostRedisplay();
    }
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'o': case 'O': // Turn on
            fanOn = true;
            if (fanSpeedLevel == 0) fanSpeedLevel = 3;
            rotationSpeed = fanSpeedLevel * 2.0f;
            break;
        case 'f': case 'F': // Turn off
            fanOn = false;
            fanSpeedLevel = 0;
            rotationSpeed = 0.0f;
            break;
        case '1': case '2': case '3': case '4': case '5':
            if (fanOn) {
                fanSpeedLevel = key - '0';
                rotationSpeed = fanSpeedLevel * 2.0f;
            }
            break;
        case '+': // Increase speed
            if (fanOn && fanSpeedLevel < 5) {
                fanSpeedLevel++;
                rotationSpeed = fanSpeedLevel * 2.0f;
            }
            break;
        case '-': // Decrease speed
            if (fanOn && fanSpeedLevel > 1) {
                fanSpeedLevel--;
                rotationSpeed = fanSpeedLevel * 2.0f;
            }
            break;
        case 'l': case 'L': // Toggle lighting
            lightingEnabled = !lightingEnabled;
            break;
        case 'z': case 'Z': // Zoom in
            cameraDistance -= 1.0f;
            if (cameraDistance < 3.0f) cameraDistance = 3.0f;
            break;
        case 'x': case 'X': // Zoom out
            cameraDistance += 1.0f;
            if (cameraDistance > 30.0f) cameraDistance = 30.0f;
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
    glutCreateWindow("3D Ventilator Fan with Realistic Desk");
    
    // Enable depth testing and smooth shading
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    
    // Set clear color
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    // Print instructions to console
    printf("=============================================\n");
    printf("3D VENTILATOR FAN WITH REALISTIC DESK\n");
    printf("=============================================\n");
    printf("FEATURES:\n");
    printf("  - Full 3D with lighting and shadows\n");
    printf("  - Realistic wooden desk with 4 legs\n");
    printf("  - Detailed fan with pole, motor, and 5 blades\n");
    printf("  - Metal safety cage\n");
    printf("  - Interactive speed control\n");
    printf("\nCONTROLS:\n");
    printf("  MOUSE:\n");
    printf("    - Left drag to rotate camera\n");
    printf("    - Right drag to zoom in/out\n");
    printf("    - Click buttons in control panel\n");
    printf("  KEYBOARD:\n");
    printf("    O - Turn fan ON\n");
    printf("    F - Turn fan OFF\n");
    printf("    1-5 - Set speed level (1=slow, 5=fast)\n");
    printf("    + - Increase speed\n");
    printf("    - - Decrease speed\n");
    printf("    L - Toggle lighting\n");
    printf("    Z - Zoom in\n");
    printf("    X - Zoom out\n");
    printf("    ESC - Exit program\n");
    printf("=============================================\n");
    
    glutMainLoop();
    return 0;
}