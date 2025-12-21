#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>  // Added this line for sprintf and printf

// Global variables
float rotationAngle = 0.0f;
float rotationSpeed = 0.0f;
bool fanOn = false;
int fanSpeedLevel = 0; // 0-5
float deskColor[3] = {0.55f, 0.27f, 0.07f}; // Brown
float fanColor[3] = {0.4f, 0.4f, 0.4f}; // Gray
float buttonColor[3] = {0.8f, 0.2f, 0.2f}; // Red for power button
float speedButtonColor[3] = {0.2f, 0.6f, 0.2f}; // Green for speed buttons
float bladeColors[5][3] = {
    {0.9f, 0.2f, 0.2f}, // Red
    {0.2f, 0.9f, 0.2f}, // Green
    {0.2f, 0.2f, 0.9f}, // Blue
    {0.9f, 0.9f, 0.2f}, // Yellow
    {0.9f, 0.2f, 0.9f}  // Magenta
};

// Window dimensions
int windowWidth = 800;
int windowHeight = 600;

// Function to draw a circle
void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Function to draw a rounded rectangle
void drawRoundedRect(float x, float y, float width, float height, float radius) {
    int segments = 20;
    
    glBegin(GL_POLYGON);
    // Top edge
    for (int i = 0; i <= segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + width - radius + radius * cosf(theta);
        float py = y + height - radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    // Right edge
    for (int i = segments; i <= 2 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + width - radius + radius * cosf(theta);
        float py = y + radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    // Bottom edge
    for (int i = 2 * segments; i <= 3 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + radius + radius * cosf(theta);
        float py = y + radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    // Left edge
    for (int i = 3 * segments; i <= 4 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + radius + radius * cosf(theta);
        float py = y + height - radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    glEnd();
}

// Function to draw the desk
void drawDesk() {
    glColor3fv(deskColor);
    
    // Desk surface (main top)
    glBegin(GL_QUADS);
    glVertex2f(100, 150);
    glVertex2f(700, 150);
    glVertex2f(700, 350);
    glVertex2f(100, 350);
    glEnd();
    
    // Desk legs
    glBegin(GL_QUADS);
    // Front left leg
    glVertex2f(120, 50);
    glVertex2f(140, 50);
    glVertex2f(140, 150);
    glVertex2f(120, 150);
    // Front right leg
    glVertex2f(660, 50);
    glVertex2f(680, 50);
    glVertex2f(680, 150);
    glVertex2f(660, 150);
    // Back left leg
    glVertex2f(120, 350);
    glVertex2f(140, 350);
    glVertex2f(140, 450);
    glVertex2f(120, 450);
    // Back right leg
    glVertex2f(660, 350);
    glVertex2f(680, 350);
    glVertex2f(680, 450);
    glVertex2f(660, 450);
    glEnd();
    
    // Draw some details on desk surface
    glColor3f(deskColor[0] * 0.8, deskColor[1] * 0.8, deskColor[2] * 0.8);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    // Desk edge lines
    glVertex2f(100, 150);
    glVertex2f(700, 150);
    glVertex2f(700, 150);
    glVertex2f(700, 350);
    glVertex2f(700, 350);
    glVertex2f(100, 350);
    glVertex2f(100, 350);
    glVertex2f(100, 150);
    glEnd();
}

// Function to draw the safety cage
void drawSafetyCage() {
    // Safety cage - always visible
    glColor4f(0.5f, 0.5f, 0.5f, 0.4f); // Semi-transparent gray
    glLineWidth(1.5);
    
    // Draw radial lines (spokes)
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {
        float angle = i * 30.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(0, 0);
        glVertex2f(cosf(rad) * 135, sinf(rad) * 135);
    }
    glEnd();
    
    // Outer ring of safety cage
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i++) {
        float angle = i * 10.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(cosf(rad) * 135, sinf(rad) * 135);
    }
    glEnd();
    
    // Inner ring
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i++) {
        float angle = i * 10.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(cosf(rad) * 125, sinf(rad) * 125);
    }
    glEnd();
    
    // Middle ring
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 24; i++) {
        float angle = i * 15.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(cosf(rad) * 130, sinf(rad) * 130);
    }
    glEnd();
}

// Function to draw a single fan blade
void drawBlade(float angleOffset, int bladeIndex) {
    glPushMatrix();
    glRotatef(angleOffset, 0.0f, 0.0f, 1.0f);
    
    // Blade with gradient effect
    glBegin(GL_QUADS);
    glColor3f(bladeColors[bladeIndex][0] * 0.7, 
              bladeColors[bladeIndex][1] * 0.7, 
              bladeColors[bladeIndex][2] * 0.7);
    glVertex2f(0, 5);
    glVertex2f(120, 15);
    glColor3fv(bladeColors[bladeIndex]);
    glVertex2f(120, -15);
    glVertex2f(0, -5);
    glEnd();
    
    // Blade outline
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 5);
    glVertex2f(120, 15);
    glVertex2f(120, -15);
    glVertex2f(0, -5);
    glEnd();
    
    glPopMatrix();
}

// Function to draw the fan
void drawFan() {
    glPushMatrix();
    glTranslatef(400, 250, 0); // Center of desk
    
    // Draw safety cage first (behind fan)
    drawSafetyCage();
    
    // Fan base
    glColor3fv(fanColor);
    drawCircle(0, 0, 60, 50);
    
    // Fan motor housing
    glColor3f(fanColor[0] * 0.7, fanColor[1] * 0.7, fanColor[2] * 0.7);
    drawCircle(0, 0, 40, 50);
    
    // Fan center cap
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(0, 0, 15, 30);
    
    // Draw 5 blades
    glPushMatrix();
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    for (int i = 0; i < 5; i++) {
        drawBlade(i * 72, i); // 360/5 = 72 degrees between blades
    }
    
    glPopMatrix();
    
    glPopMatrix();
}

// Function to draw control panel
void drawControls() {
    // Control panel background
    glColor3f(0.2f, 0.2f, 0.2f);
    drawRoundedRect(650, 400, 120, 180, 10);
    
    // Power button
    glColor3fv(buttonColor);
    if (fanOn) {
        glColor3f(0.2f, 0.8f, 0.2f); // Green when on
    }
    drawRoundedRect(670, 420, 80, 40, 5);
    
    // Power button label
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(675, 440);
    const char* powerText = fanOn ? "ON" : "OFF";
    while (*powerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerText++);
    }
    
    // Speed buttons
    for (int i = 0; i < 5; i++) {
        if (i < fanSpeedLevel) {
            glColor3fv(speedButtonColor); // Active speed
        } else {
            glColor3f(speedButtonColor[0] * 0.5, 
                     speedButtonColor[1] * 0.5, 
                     speedButtonColor[2] * 0.5); // Inactive
        }
        
        drawRoundedRect(670, 470 + i * 25, 80, 20, 3);
        
        // Speed number label
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(700, 485 + i * 25);
        char speedNum = '1' + i;
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, speedNum);
    }
    
    // Control panel labels
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(660, 410);
    const char* powerLabel = "POWER";
    while (*powerLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerLabel++);
    }
    
    glRasterPos2f(660, 460);
    const char* speedLabel = "SPEED";
    while (*speedLabel) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedLabel++);
    }
}

// Function to draw status display
void drawStatus() {
    glColor3f(0.0f, 0.0f, 0.0f);
    
    // Status text
    glRasterPos2f(50, 500);
    const char* statusText = "VENTILATOR FAN CONTROL";
    while (*statusText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *statusText++);
    }
    
    // Fan status
    glRasterPos2f(50, 480);
    const char* fanStatus = fanOn ? "FAN: RUNNING" : "FAN: STOPPED";
    while (*fanStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *fanStatus++);
    }
    
    // Speed status
    glRasterPos2f(50, 460);
    char speedStatus[50];
    sprintf(speedStatus, "SPEED LEVEL: %d", fanSpeedLevel);
    const char* speedPtr = speedStatus;
    while (*speedPtr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedPtr++);
    }
    
    // Cage status (always on now)
    glRasterPos2f(50, 440);
    const char* cageStatus = "SAFETY CAGE: ALWAYS ON";
    while (*cageStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *cageStatus++);
    }
    
    // Instructions
    glRasterPos2f(50, 410);
    const char* inst1 = "CONTROLS:";
    while (*inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst1++);
    }
    
    glRasterPos2f(50, 390);
    const char* inst2 = "Click POWER button to toggle ON/OFF";
    while (*inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst2++);
    }
    
    glRasterPos2f(50, 370);
    const char* inst3 = "Click SPEED buttons 1-5 to adjust speed";
    while (*inst3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst3++);
    }
}

// Display function
void display() {
    glClearColor(0.9f, 0.9f, 0.95f, 1.0f); // Light blue background
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Draw all components
    drawDesk();
    drawFan();
    drawControls();
    drawStatus();
    
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

// Mouse click handler
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert y coordinate to OpenGL coordinate system
        y = windowHeight - y;
        
        // Check if power button is clicked (670-750, 420-460)
        if (x >= 670 && x <= 750 && y >= 420 && y <= 460) {
            fanOn = !fanOn;
            if (!fanOn) {
                fanSpeedLevel = 0;
                rotationSpeed = 0.0f;
            } else if (fanSpeedLevel == 0) {
                fanSpeedLevel = 3; // Default speed when turning on
                rotationSpeed = fanSpeedLevel * 2.0f;
            }
        }
        
        // Check if speed buttons are clicked
        for (int i = 0; i < 5; i++) {
            int buttonY1 = 470 + i * 25;
            int buttonY2 = buttonY1 + 20;
            
            if (x >= 670 && x <= 750 && y >= buttonY1 && y <= buttonY2) {
                if (fanOn) {
                    fanSpeedLevel = i + 1;
                    rotationSpeed = fanSpeedLevel * 2.0f;
                }
            }
        }
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
        case 27: // ESC key
            exit(0);
            break;
    }
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
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("5-Blade Ventilator Fan with Safety Cage");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    // Print instructions to console
    printf("=============================================\n");
    printf("5-BLADE VENTILATOR FAN WITH SAFETY CAGE\n");
    printf("=============================================\n");
    printf("CONTROLS:\n");
    printf("  MOUSE:\n");
    printf("    - Click POWER button to toggle ON/OFF\n");
    printf("    - Click SPEED buttons (1-5) to adjust speed\n");
    printf("  KEYBOARD:\n");
    printf("    O - Turn fan ON\n");
    printf("    F - Turn fan OFF\n");
    printf("    1-5 - Set speed level (1=slow, 5=fast)\n");
    printf("    + - Increase speed\n");
    printf("    - - Decrease speed\n");
    printf("    ESC - Exit program\n");
    printf("=============================================\n");
    printf("NOTE: Safety cage is ALWAYS VISIBLE for protection\n");
    printf("=============================================\n");
    
    glutMainLoop();
    return 0;
}