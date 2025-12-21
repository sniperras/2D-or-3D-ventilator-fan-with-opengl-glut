#include <GL/glut.h>      // OpenGL Utility Toolkit for window creation and rendering
#include <cmath>          // Math functions (sin, cos, etc.)
#include <cstdlib>        // Standard library for rand(), exit()
#include <cstdio>         // Standard I/O for printf()
#include <vector>         // STL vector for dynamic arrays

// Global variables
float rotationAngle = 0.0f;        // Current rotation angle of fan blades (degrees)
float rotationSpeed = 0.0f;        // Current actual rotation speed (units/frame)
float targetRotationSpeed = 0.0f;  // Desired rotation speed for acceleration simulation
bool fanOn = false;                // Fan power state (on/off)
int fanSpeedLevel = 0;             // Speed level from 0 (off) to 5 (max)

// Color definitions using RGB arrays (each value 0.0-1.0)
float deskColor[3] = {0.55f, 0.27f, 0.07f};      // Brown wood color
float fanColor[3] = {0.4f, 0.4f, 0.4f};          // Gray for fan parts
float buttonColor[3] = {0.8f, 0.2f, 0.2f};       // Red for power button
float speedButtonColor[3] = {0.2f, 0.6f, 0.2f};  // Green for speed buttons

// Array of colors for the 5 fan blades (each blade gets a different color)
float bladeColors[5][3] = {
    {0.9f, 0.2f, 0.2f}, // Red
    {0.2f, 0.9f, 0.2f}, // Green
    {0.2f, 0.2f, 0.9f}, // Blue
    {0.9f, 0.9f, 0.2f}, // Yellow
    {0.9f, 0.2f, 0.9f}  // Magenta
};

// Vector to store air flow particle positions (pairs of x,y coordinates)
std::vector<std::pair<float, float> > airParticles;

// Physics simulation constants
float acceleration = 0.25f;   // Rate of speed increase when accelerating
float deceleration = 0.15f;   // Rate of speed decrease when decelerating

// Window dimensions
int windowWidth = 800;   // Initial window width in pixels
int windowHeight = 600;  // Initial window height in pixels

// Function to draw a circle using triangle fan primitive
void drawCircle(float cx, float cy, float radius, int segments) {
    glBegin(GL_TRIANGLE_FAN);     // Start drawing connected triangles from center
    glVertex2f(cx, cy);           // Center point (all triangles share this vertex)
    
    // Create vertices around the circle
    for (int i = 0; i <= segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);  // Angle in radians
        float x = radius * cosf(theta);  // X coordinate on circle
        float y = radius * sinf(theta);  // Y coordinate on circle
        glVertex2f(cx + x, cy + y);      // Add vertex position
    }
    glEnd();  // End drawing
}

// Function to draw a rectangle with rounded corners
void drawRoundedRect(float x, float y, float width, float height, float radius) {
    int segments = 20;  // Number of segments per quarter circle
    
    glBegin(GL_POLYGON);  // Start drawing filled polygon
    
    // Draw top right rounded corner
    for (int i = 0; i <= segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);  // 0 to 90 degrees
        float px = x + width - radius + radius * cosf(theta);   // X position
        float py = y + height - radius + radius * sinf(theta);  // Y position
        glVertex2f(px, py);  // Add vertex
    }
    
    // Draw bottom right rounded corner (90 to 180 degrees)
    for (int i = segments; i <= 2 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + width - radius + radius * cosf(theta);
        float py = y + radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    
    // Draw bottom left rounded corner (180 to 270 degrees)
    for (int i = 2 * segments; i <= 3 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + radius + radius * cosf(theta);
        float py = y + radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    
    // Draw top left rounded corner (270 to 360 degrees)
    for (int i = 3 * segments; i <= 4 * segments; i++) {
        float theta = 3.1415926f * float(i) / float(segments);
        float px = x + radius + radius * cosf(theta);
        float py = y + height - radius + radius * sinf(theta);
        glVertex2f(px, py);
    }
    glEnd();  // End drawing
}

// Function to draw the desk surface and legs
void drawDesk() {
    glColor3fv(deskColor);  // Set current color to desk color
    
    // Draw main desk surface as a quad (rectangle)
    glBegin(GL_QUADS);
    glVertex2f(100, 150);  // Bottom-left corner
    glVertex2f(700, 150);  // Bottom-right corner
    glVertex2f(700, 350);  // Top-right corner
    glVertex2f(100, 350);  // Top-left corner
    glEnd();
    
    // Draw four desk legs as vertical quads
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
    
    // Draw decorative lines on desk edges
    glColor3f(deskColor[0] * 0.8, deskColor[1] * 0.8, deskColor[2] * 0.8);  // Darker shade
    glLineWidth(2.0);  // Set line thickness
    glBegin(GL_LINES);
    // Draw outline around desk surface
    glVertex2f(100, 150); glVertex2f(700, 150);  // Bottom edge
    glVertex2f(700, 150); glVertex2f(700, 350);  // Right edge
    glVertex2f(700, 350); glVertex2f(100, 350);  // Top edge
    glVertex2f(100, 350); glVertex2f(100, 150);  // Left edge
    glEnd();
}

// Function to draw the fan's stand/base
void drawFanStand() {
    // Draw circular base on desk
    glColor3f(0.2f, 0.2f, 0.2f);  // Black color
    drawCircle(400, 250, 40, 30);  // Center at (400,250), radius 40, 30 segments
    
    // Draw vertical stand pole as a thick line
    glColor3fv(fanColor);  // Use fan color
    glLineWidth(8.0);  // Thick line
    glBegin(GL_LINES);
    glVertex2f(400, 250);  // Top of base
    glVertex2f(400, 350);  // Top of stand (where fan attaches)
    glEnd();
    
    // Draw top of stand where fan motor attaches
    glColor3f(fanColor[0] * 0.7, fanColor[1] * 0.7, fanColor[2] * 0.7);  // Darker gray
    drawCircle(400, 350, 15, 20);  // Small circle at top
}

// Function to draw the safety cage around the fan blades
void drawSafetyCage() {
    // Set semi-transparent gray color with alpha = 0.4 (40% opaque)
    glColor4f(0.5f, 0.5f, 0.5f, 0.4f);
    glLineWidth(1.5);  // Medium line thickness
    
    // Draw radial spokes from center to outer ring
    glBegin(GL_LINES);
    for (int i = 0; i < 12; i++) {  // 12 spokes
        float angle = i * 30.0f;  // 30 degrees between spokes
        float rad = angle * 3.1415926f / 180.0f;  // Convert to radians
        glVertex2f(450, 350);  // Center of fan (not stand!)
        glVertex2f(450 + cosf(rad) * 80, 350 + sinf(rad) * 80);  // Outer point
    }
    glEnd();
    
    // Draw outer ring of cage
    glBegin(GL_LINE_LOOP);  // Connected line that forms a closed loop
    for (int i = 0; i < 36; i++) {  // 36 segments for smooth circle
        float angle = i * 10.0f;  // 10 degrees per segment
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(450 + cosf(rad) * 80, 350 + sinf(rad) * 80);  // Points at radius 80
    }
    glEnd();
    
    // Draw inner ring of cage (closer to blades)
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i++) {
        float angle = i * 10.0f;
        float rad = angle * 3.1415926f / 180.0f;
        glVertex2f(450 + cosf(rad) * 70, 350 + sinf(rad) * 70);  // Points at radius 70
    }
    glEnd();
}

// Function to draw a single fan blade
// bladeAngle: angle of blade from center (in radians)
// bladeIndex: which blade (0-4) for color selection
void drawBlade(float bladeAngle, int bladeIndex) {
    glColor3fv(bladeColors[bladeIndex]);  // Set blade color based on index
    
    glBegin(GL_TRIANGLE_FAN);  // Use triangle fan for smooth blade shape
    
    // Calculate blade tip position (outer edge)
    float tipX = cosf(bladeAngle) * 60.0f;  // 60 pixels from center
    float tipY = sinf(bladeAngle) * 60.0f;
    
    // Calculate blade base points (at hub, 15 degrees on each side for thickness)
    float baseAngle1 = bladeAngle - 15.0f * 3.1415926f / 180.0f;
    float baseAngle2 = bladeAngle + 15.0f * 3.1415926f / 180.0f;
    
    float baseX1 = cosf(baseAngle1) * 10.0f;  // 10 pixels from center (hub radius)
    float baseY1 = sinf(baseAngle1) * 10.0f;
    float baseX2 = cosf(baseAngle2) * 10.0f;
    float baseY2 = sinf(baseAngle2) * 10.0f;
    
    glVertex2f(0, 0);  // Center point where blade attaches to hub
    
    // Create curved edge for blade
    for (int i = 0; i <= 10; i++) {
        float t = i / 10.0f;  // Parameter from 0 to 1
        float angle = bladeAngle + t * 0.2f;  // Slight curve in blade
        float radius = 10.0f + t * 50.0f;     // Interpolate from hub to tip
        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;
        glVertex2f(x, y);
    }
    
    glVertex2f(baseX2, baseY2);  // Close the shape
    glEnd();
    
    // Draw outline around blade for definition
    glColor3f(0.1f, 0.1f, 0.1f);  // Dark color for outline
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(baseX1, baseY1);  // Base point 1
    glVertex2f(tipX, tipY);      // Blade tip
    glVertex2f(baseX2, baseY2);  // Base point 2
    glEnd();
}

// Function to draw the fan motor housing and connection arm
void drawFanMotor() {
    // Draw motor housing at end of stand
    glColor3fv(fanColor);
    drawCircle(400, 350, 20, 30);  // Circle at stand top
    
    // Draw motor face (forward facing circle)
    glColor3f(fanColor[0] * 0.8, fanColor[1] * 0.8, fanColor[2] * 0.8);  // Darker
    drawCircle(425, 350, 15, 20);  // Slightly offset forward
    
    // Draw connection arm from stand to fan center
    glColor3fv(fanColor);
    glLineWidth(6.0);  // Thick line for arm
    glBegin(GL_LINES);
    glVertex2f(400, 350);  // End of stand
    glVertex2f(450, 350);  // Center of fan blades
    glEnd();
    
    // Draw fan hub (center where blades attach)
    glColor3f(0.1f, 0.1f, 0.1f);  // Dark color
    drawCircle(450, 350, 12, 24);  // Small dark circle
}

// Function to draw all 5 fan blades with rotation
void drawFanBlades() {
    glPushMatrix();  // Save current transformation matrix
    
    // Move coordinate system to fan center (450,350)
    glTranslatef(450, 350, 0);
    
    // Apply rotation based on current rotationAngle
    glRotatef(rotationAngle, 0.0f, 0.0f, 1.0f);  // Rotate around Z-axis
    
    // Draw 5 blades spaced 72 degrees apart (360/5 = 72)
    for (int i = 0; i < 5; i++) {
        float bladeAngle = i * 72.0f * 3.1415926f / 180.0f;  // Convert to radians
        drawBlade(bladeAngle, i);  // Draw each blade with its color
    }
    
    glPopMatrix();  // Restore original transformation matrix
}

// Function to draw air flow particles
void drawAirFlow() {
    if (!fanOn) return;  // No air flow when fan is off
    
    // Generate new particles randomly
    if (airParticles.size() < 30 && rand() % 10 < fanSpeedLevel) {
        float angle = (rand() % 60 - 30) * 3.1415926f / 180.0f;  // Random angle -30 to +30 degrees
        float distance = 80 + rand() % 20;  // Random distance 80-100 from center
        airParticles.push_back(std::make_pair(
            450 + cosf(angle) * distance,  // X position
            350 + sinf(angle) * distance   // Y position
        ));
    }
    
    // Update and draw all particles
    glPointSize(2.5f);  // Set particle size
    glBegin(GL_POINTS);  // Draw each particle as a point
    for (size_t i = 0; i < airParticles.size(); i++) {
        // Calculate distance from fan center
        float dx = airParticles[i].first - 450;
        float dy = airParticles[i].second - 350;
        float dist = sqrt(dx * dx + dy * dy);
        
        // Remove particles that are too far away
        if (dist > 200) {
            airParticles.erase(airParticles.begin() + i);
            i--;  // Adjust index since we removed an element
            continue;
        }
        
        // Move particle outward from center
        float moveSpeed = 1.5f + fanSpeedLevel * 0.3f;  // Speed increases with fan speed
        airParticles[i].first += dx / dist * moveSpeed;   // Move in X direction
        airParticles[i].second += dy / dist * moveSpeed;  // Move in Y direction
        
        // Set particle color with transparency (fade out as particles move away)
        float alpha = 1.0f - (dist - 80) / 120.0f;  // Alpha decreases with distance
        glColor4f(0.7f, 0.8f, 1.0f, alpha * 0.6f);  // Light blue with transparency
        glVertex2f(airParticles[i].first, airParticles[i].second);  // Draw particle
    }
    glEnd();
}

// Main function to draw the entire fan assembly
void drawFan() {
    // Draw components in correct order (back to front)
    drawFanStand();     // Stand/base first
    drawFanMotor();     // Motor and connection
    drawSafetyCage();   // Cage behind blades
    drawFanBlades();    // Rotating blades
    drawAirFlow();      // Air particles on top
}

// Function to draw the control panel with buttons
void drawControls() {
    // Control panel background
    glColor3f(0.2f, 0.2f, 0.2f);  // Dark gray
    drawRoundedRect(650, 400, 120, 180, 10);  // Positioned top-right
    
    // Power button - color changes based on state
    if (fanOn) {
        glColor3f(0.2f, 0.8f, 0.2f);  // Green when on
    } else {
        glColor3fv(buttonColor);  // Red when off
    }
    drawRoundedRect(670, 420, 80, 40, 5);  // Power button
    
    // Draw "ON" or "OFF" text on button
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    glRasterPos2f(675, 440);  // Position for text
    const char* powerText = fanOn ? "ON" : "OFF";
    while (*powerText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *powerText++);  // Draw each character
    }
    
    // Draw 5 speed buttons (1-5)
    for (int i = 0; i < 5; i++) {
        // Highlight current speed level
        if ((i + 1) == fanSpeedLevel) {
            glColor3fv(speedButtonColor);  // Bright green for active speed
        } else {
            glColor3f(speedButtonColor[0] * 0.5,  // Dim green for inactive
                     speedButtonColor[1] * 0.5, 
                     speedButtonColor[2] * 0.5);
        }
        
        drawRoundedRect(670, 470 + i * 25, 80, 20, 3);  // Position each button
        
        // Draw speed number (1-5)
        glColor3f(1.0f, 1.0f, 1.0f);  // White text
        glRasterPos2f(700, 485 + i * 25);
        char speedNum = '1' + i;  // Convert index to character '1' through '5'
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, speedNum);
    }
    
    // Draw labels for control panel sections
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

// Function to draw status information and instructions
void drawStatus() {
    glColor3f(0.0f, 0.0f, 0.0f);  // Black text
    
    // Main title
    glRasterPos2f(50, 570);
    const char* statusText = "VENTILATOR FAN CONTROL";
    while (*statusText) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *statusText++);
    }
    
    // Fan status (running/stopped)
    glRasterPos2f(50, 550);
    const char* fanStatus = fanOn ? "FAN: RUNNING" : "FAN: STOPPED";
    while (*fanStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *fanStatus++);
    }
    
    // Current speed level
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
    
    // Physics simulation status
    glRasterPos2f(50, 490);
    const char* accelStatus = "ACCEL/DECEL: ENABLED";
    while (*accelStatus) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *accelStatus++);
    }
    
    // Control instructions
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

// Main display callback function (called by GLUT)
void display() {
    // Set background color and clear screen
    glClearColor(0.9f, 0.9f, 0.95f, 1.0f);  // Light blue-gray
    glClear(GL_COLOR_BUFFER_BIT);  // Clear color buffer
    
    // Set up 2D orthographic projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);  // 2D coordinate system
    
    // Switch to modelview matrix for drawing
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Draw all scene components
    drawDesk();      // Desk first (background)
    drawFan();       // Fan on desk
    drawControls();  // Control panel
    drawStatus();    // Text information
    
    glutSwapBuffers();  // Swap front and back buffers (double buffering)
}

// Timer callback function for animation (called every 16ms ˜ 60fps)
void timer(int value) {
    // Acceleration/deceleration physics simulation
    if (fanOn) {
        // Accelerate toward target speed
        if (rotationSpeed < targetRotationSpeed) {
            rotationSpeed += acceleration;
            if (rotationSpeed > targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;  // Don't overshoot
            }
        } else if (rotationSpeed > targetRotationSpeed) {
            rotationSpeed -= deceleration;
            if (rotationSpeed < targetRotationSpeed) {
                rotationSpeed = targetRotationSpeed;
            }
        }
    } else {
        // Decelerate when fan is off
        if (rotationSpeed > 0) {
            rotationSpeed -= deceleration * 1.5f;  // Faster deceleration when off
            if (rotationSpeed < 0) rotationSpeed = 0;  // Stop at zero
        }
    }
    
    // Update fan rotation angle
    if (fanOn || rotationSpeed > 0) {
        rotationAngle += rotationSpeed;
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;  // Keep angle in 0-360 range
        }
    }
    
    // Randomly add air particles when fan is on
    if (fanOn && airParticles.size() < 25) {
        if (rand() % 15 == 0) {  // Random chance each frame
            float angle = (rand() % 360) * 3.1415926f / 180.0f;  // Random direction
            airParticles.push_back(std::make_pair(
                450 + cosf(angle) * 75,  // Start at 75px from center
                350 + sinf(angle) * 75
            ));
        }
    }
    
    glutPostRedisplay();  // Request screen refresh
    glutTimerFunc(16, timer, 0);  // Call this function again in 16ms
}

// Function to set target speed with level (0-5)
void setTargetSpeed(int level) {
    if (level >= 0 && level <= 5) {
        fanSpeedLevel = level;  // Update speed level
        targetRotationSpeed = fanSpeedLevel * 2.0f;  // Calculate target speed
        
        // Update fan state based on speed level
        if (fanSpeedLevel == 0) {
            fanOn = false;  // Level 0 means fan is off
        } else {
            fanOn = true;  // Any speed level 1-5 means fan is on
        }
    }
}

// Mouse click callback function
void mouse(int button, int state, int x, int y) {
    // Handle left mouse button clicks
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Convert y coordinate (GLUT origin is top-left, OpenGL is bottom-left)
        y = windowHeight - y;
        
        // Check if power button was clicked
        if (x >= 670 && x <= 750 && y >= 420 && y <= 460) {
            if (!fanOn) {
                fanOn = true;  // Turn fan on
                if (fanSpeedLevel == 0) setTargetSpeed(3);  // Default to speed 3
            } else {
                setTargetSpeed(0);  // Turn fan off
            }
        }
        
        // Check if any speed button was clicked
        for (int i = 0; i < 5; i++) {
            int buttonY1 = 470 + i * 25;  // Top of button
            int buttonY2 = buttonY1 + 20; // Bottom of button
            
            if (x >= 670 && x <= 750 && y >= buttonY1 && y <= buttonY2) {
                setTargetSpeed(i + 1);  // Set speed to button number
            }
        }
    }
}

// Keyboard callback function
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'o': case 'O':  // Turn fan on
            fanOn = true;
            if (fanSpeedLevel == 0) setTargetSpeed(3);  // Default speed
            break;
            
        case 'f': case 'F':  // Turn fan off
            setTargetSpeed(0);
            break;
            
        case '1': case '2': case '3': case '4': case '5':  // Set speed directly
            setTargetSpeed(key - '0');  // Convert char to int
            break;
            
        case '+':  // Increase speed
            if (fanSpeedLevel < 5) {
                setTargetSpeed(fanSpeedLevel + 1);
            }
            break;
            
        case '-':  // Decrease speed
            if (fanSpeedLevel > 0) {
                setTargetSpeed(fanSpeedLevel - 1);
            }
            break;
            
        case 'r': case 'R':  // Reset everything
            fanOn = false;
            setTargetSpeed(0);
            rotationSpeed = 0.0f;
            targetRotationSpeed = 0.0f;
            airParticles.clear();  // Remove all air particles
            break;
            
        case 27:  // ESC key - exit program
            exit(0);
            break;
    }
}

// Window reshape callback (when window is resized)
void reshape(int width, int height) {
    windowWidth = width;    // Update global width
    windowHeight = height;  // Update global height
    glViewport(0, 0, width, height);  // Set OpenGL viewport to new size
}

// Main function - program entry point
int main(int argc, char** argv) {
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  // Double buffering, RGB color
    glutInitWindowSize(windowWidth, windowHeight);  // Set initial window size
    glutCreateWindow("5-Blade Ventilator Fan with Air Flow & Acceleration");  // Create window
    
    // Register callback functions
    glutDisplayFunc(display);   // Called when window needs redrawing
    glutReshapeFunc(reshape);   // Called when window is resized
    glutMouseFunc(mouse);       // Called for mouse events
    glutKeyboardFunc(keyboard); // Called for keyboard events
    glutTimerFunc(0, timer, 0); // Start animation timer
    
    // Print instructions to console
    printf("=============================================\n");
    printf("5-BLADE VENTILATOR FAN WITH AIR FLOW & ACCELERATION\n");
    printf("=============================================\n");
    printf("ADDED FEATURES:\n");
    printf("    Realistic acceleration/deceleration\n");
    printf("    Air flow particle system\n");
    printf("    Dynamic speed button highlighting\n");
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
    
    // Start GLUT main loop (this function never returns)
    glutMainLoop();
    return 0;
}