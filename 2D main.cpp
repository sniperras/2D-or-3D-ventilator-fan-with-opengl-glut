#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <vector>

// Global variables
float rotationAngle = 0.0f;
float rotationSpeed = 0.0f;
float targetRotationSpeed = 0.0f; // For acceleration
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

// Air flow particles
std::vector<std::pair<float, float> > airParticles;

// Acceleration variables
float acceleration = 0.25f;
float deceleration = 0.15f;

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

// Function to draw the fan stand
void drawFanStand() {
    // Fan base (circular base on desk)
    glColor3f(0.2f, 0.2f, 0.2f); // Black base
    drawCircle(400, 250, 40, 30);
    
    // Stand pole (vertical pole)
    glColor3fv(fanColor);
    glLineWidth(8.0);
    glBegin(GL_LINES);
    glVertex2f(400, 250);  // Top of base
    glVertex2f(400, 350);  // Top of stand
    glEnd();
    
    // Stand top (where fan attaches)
    glColor3f(fanColor[0] * 0.7, fanColor[1] * 0.7, fanColor[2] * 0.7);
    drawCircle(400, 350, 15, 20);
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
        glVertex2f(450, 350); // Center at fan position
        glVertex2f(450 + cosf(rad) * 80, 350 + sinf(rad) * 80);
    }
    glEnd();
    
    // Outer ring of safety cage (around fan, not centered on stand)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i++) {
        float angle = i * 10.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(450 + cosf(rad) * 80, 350 + sinf(rad) * 80);
    }
    glEnd();
    
    // Inner ring
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i++) {
        float angle = i * 10.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(450 + cosf(rad) * 70, 350 + sinf(rad) * 70);
    }
    glEnd();
}

// Function to draw a single fan blade (PROPERLY POSITIONED)
void drawBlade(float bladeAngle, int bladeIndex) {
    glColor3fv(bladeColors[bladeIndex]);
    
    // Blade shape - starts at hub and extends outward
    glBegin(GL_TRIANGLE_FAN);
    
    // Blade tip (outer edge)
    float tipX = cosf(bladeAngle) * 60.0f;
    float tipY = sinf(bladeAngle) * 60.0f;
    
    // Blade base points (at hub, slightly offset for thickness)
    float baseAngle1 = bladeAngle - 15.0f * 3.1415926f / 180.0f;
    float baseAngle2 = bladeAngle + 15.0f * 3.1415926f / 180.0f;
    
    float baseX1 = cosf(baseAngle1) * 10.0f;
    float baseY1 = sinf(baseAngle1) * 10.0f;
    
    float baseX2 = cosf(baseAngle2) * 10.0f;
    float baseY2 = sinf(baseAngle2) * 10.0f;
    
    // Draw blade with smooth curve
    glVertex2f(0, 0); // Center point (hub connection)
    
    // Create curved blade edge
    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;
        float angle = bladeAngle + t * 0.2f; // Slight curve
        float radius = 10.0f + t * 50.0f;
        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;
        glVertex2f(x, y);
    }
    
    // Close the blade
    glVertex2f(baseX2, baseY2);
    glEnd();
    
    // Blade outline for definition
    glColor3f(0.1f, 0.1f, 0.1f);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(baseX1, baseY1);
    glVertex2f(tipX, tipY);
    glVertex2f(baseX2, baseY2);
    glEnd();
}

// Function to draw the fan motor and housing
void drawFanMotor() {
    // Motor housing (at end of stand)
    glColor3fv(fanColor);
    drawCircle(400, 350, 20, 30);
    
    // Motor face (forward facing)
    glColor3f(fanColor[0] * 0.8, fanColor[1] * 0.8, fanColor[2] * 0.8);
    drawCircle(425, 350, 15, 20);
    
    // Connection arm from stand to fan
    glColor3fv(fanColor);
    glLineWidth(6.0);
    glBegin(GL_LINES);
    glVertex2f(400, 350);  // End of stand
    glVertex2f(450, 350);  // Center of fan
    glEnd();
    
    // Fan hub (center of blades)
    glColor3f(0.1f, 0.1f, 0.1f);
    drawCircle(450, 350, 12, 24);
}

// Function to draw all fan blades (FIXED POSITIONING)
void drawFanBlades() {
    // Save current transformation
    glPushMatrix();
    
    // Move to fan center
    glTranslatef(450, 350, 0);
    
    // Apply rotation
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);
    
    // Draw 5 blades evenly spaced (72 degrees apart)
    for (int i = 0; i < 5; i++) {
        float bladeAngle = i * 72.0f * 3.1415926f / 180.0f; // Convert to radians
        drawBlade(bladeAngle, i);
    }
    
    // Restore transformation
    glPopMatrix();
}

// Function to draw air flow effect
void drawAirFlow() {
    if (!fanOn) return;
    
    // Generate new particles
    if (airParticles.size() < 30 && rand() % 10 < fanSpeedLevel) {
        float angle = (rand() % 60 - 30) * 3.1415926f / 180.0f;
        float distance = 80 + rand() % 20;
        airParticles.push_back(std::make_pair(
            450 + cosf(angle) * distance,
            350 + sinf(angle) * distance
        ));
    }
    
    // Update and draw particles
    glPointSize(2.5f);
    glBegin(GL_POINTS);
    for (size_t i = 0; i < airParticles.size(); i++) {
        // Move particles outward
        float dx = airParticles[i].first - 450;
        float dy = airParticles[i].second - 350;
        float dist = sqrt(dx * dx + dy * dy);
        
        if (dist > 200) {
            airParticles.erase(airParticles.begin() + i);
            i--;
            continue;
        }
        
        // Move particle
        float moveSpeed = 1.5f + fanSpeedLevel * 0.3f;
        airParticles[i].first += dx / dist * moveSpeed;
        airParticles[i].second += dy / dist * moveSpeed;
        
        // Draw particle
        float alpha = 1.0f - (dist - 80) / 120.0f;
        glColor4f(0.7f, 0.8f, 1.0f, alpha * 0.6f);
        glVertex2f(airParticles[i].first, airParticles[i].second);
    }
    glEnd();
}

// Function to draw the fan
void drawFan() {
    // Draw stand first
    drawFanStand();
    
    // Draw motor and connection
    drawFanMotor();
    
    // Draw safety cage behind blades
    drawSafetyCage();
    
    // Draw the rotating blades
    drawFanBlades();
    
    // Draw air flow
    drawAirFlow();
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
        if ((i + 1) == fanSpeedLevel) {  // Only highlight current speed
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
    glRasterPos2f(50, 570);
    const char* statusText = "VENTILATOR FAN CONTROL";
    while (*statusText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *statusText++);
    }
    
    // Fan status
    glRasterPos2f(50, 550);
    const char* fanStatus = fanOn ? "FAN: RUNNING" : "FAN: STOPPED";
    while (*fanStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *fanStatus++);
    }
    
    // Speed status
    glRasterPos2f(50, 530);
    char speedStatus[50];
    sprintf(speedStatus, "SPEED LEVEL: %d", fanSpeedLevel);
    const char* speedPtr = speedStatus;
    while (*speedPtr) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *speedPtr++);
    }
    
    // Air flow status
    glRasterPos2f(50, 510);
    const char* airflowStatus = "AIR FLOW: ACTIVE";
    while (*airflowStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *airflowStatus++);
    }
    
    // Acceleration status
    glRasterPos2f(50, 490);
    const char* accelStatus = "ACCEL/DECEL: ENABLED";
    while (*accelStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *accelStatus++);
    }
    
    // Instructions
    glRasterPos2f(330, 540);
    const char* inst1 = "CONTROLS:";
    while (*inst1) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst1++);
    }
    
    glRasterPos2f(330, 520);
    const char* inst2 = "Click POWER button to toggle ON/OFF";
    while (*inst2) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst2++);
    }
    
    glRasterPos2f(330, 500);
    const char* inst3 = "Click SPEED buttons 1-5 to adjust speed";
    while (*inst3) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst3++);
    }
    
    glRasterPos2f(330, 480);
    const char* inst4 = "O: On  F: Off  R: Reset  ESC: Exit";
    while (*inst4) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *inst4++);
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

// Timer function with acceleration/deceleration
void timer(int value) {
    // Acceleration/deceleration physics
    if (fanOn) {
        // Move toward target speed
        if (rotationSpeed < targetRotationSpeed) {
            rotationSpeed += acceleration;
            if (rotationSpeed > targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;
            }
        } else if (rotationSpeed > targetRotationSpeed) {
            rotationSpeed -= deceleration;
            if (rotationSpeed < targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;
            }
        }
    } else {
        // Slow down when off
        if (rotationSpeed > 0) {
            rotationSpeed -= deceleration * 1.5f;
            if (rotationSpeed < 0) rotationSpeed = 0;
        }
    }
    
    // Update fan rotation
    if (fanOn || rotationSpeed > 0) {
        rotationAngle += rotationSpeed;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }
    }
    
    // Add air particles
    if (fanOn && airParticles.size() < 25) {
        if (rand() % 15 == 0) {
            float angle = (rand() % 360) * 3.1415926f / 180.0f;
            airParticles.push_back(std::make_pair(
                450 + cosf(angle) * 75,
                350 + sinf(angle) * 75
            ));
        }
    }
    
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0); // ~60 FPS
}

// Function to set target speed with acceleration
void setTargetSpeed(int level) {
    if (level >= 0 && level <= 5) {
        fanSpeedLevel = level;
        targetRotationSpeed = fanSpeedLevel * 2.0f;
        
        if (fanSpeedLevel == 0) {
            fanOn = false;
        } else {
            fanOn = true;
        }
    }
}

// Mouse click handler
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        y = windowHeight - y;
        
        // Power button
        if (x >= 670 && x <= 750 && y >= 420 && y <= 460) {
            if (!fanOn) {
                fanOn = true;
                if (fanSpeedLevel == 0) setTargetSpeed(3);
            } else {
                setTargetSpeed(0);
            }
        }
        
        // Speed buttons
        for (int i = 0; i < 5; i++) {
            int buttonY1 = 470 + i * 25;
            int buttonY2 = buttonY1 + 20;
            
            if (x >= 670 && x <= 750 && y >= buttonY1 && y <= buttonY2) {
                setTargetSpeed(i + 1);
            }
        }
    }
}

// Keyboard handler
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'o': case 'O': // Turn on
            fanOn = true;
            if (fanSpeedLevel == 0) setTargetSpeed(3);
            break;
        case 'f': case 'F': // Turn off
            setTargetSpeed(0);
            break;
        case '1': case '2': case '3': case '4': case '5':
            setTargetSpeed(key - '0');
            break;
        case '+': // Increase speed
            if (fanSpeedLevel < 5) {
                setTargetSpeed(fanSpeedLevel + 1);
            }
            break;
        case '-': // Decrease speed
            if (fanSpeedLevel > 0) {
                setTargetSpeed(fanSpeedLevel - 1);
            }
            break;
        case 'r': case 'R': // Reset
            fanOn = false;
            setTargetSpeed(0);
            rotationSpeed = 0.0f;
            targetRotationSpeed = 0.0f;
            airParticles.clear();
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
    glutCreateWindow("5-Blade Ventilator Fan with Air Flow & Acceleration");
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);
    
    // Print instructions to console
    printf("=============================================\n");
    printf("5-BLADE VENTILATOR FAN WITH AIR FLOW & ACCELERATION\n");
    printf("=============================================\n");
    printf("ADDED FEATURES:\n");
    printf("  • Realistic acceleration/deceleration\n");
    printf("  • Air flow particle system\n");
    printf("  • Dynamic speed button highlighting\n");
    printf("\nCONTROLS:\n");
    printf("  MOUSE:\n");
    printf("    - Click POWER button to toggle ON/OFF\n");
    printf("    - Click SPEED buttons (1-5) to adjust speed\n");
    printf("  KEYBOARD:\n");
    printf("    O - Turn fan ON\n");
    printf("    F - Turn fan OFF\n");
    printf("    1-5 - Set speed level (1=slow, 5=fast)\n");
    printf("    + - Increase speed\n");
    printf("    - - Decrease speed\n");
    printf("    R - Reset system\n");
    printf("    ESC - Exit program\n");
    printf("=============================================\n");
    printf("NOTE: Fan now has smooth acceleration and air flow!\n");
    printf("=============================================\n");
    
    glutMainLoop();
    return 0;
}