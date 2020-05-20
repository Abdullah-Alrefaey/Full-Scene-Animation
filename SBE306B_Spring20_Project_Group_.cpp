//
// Created by Abdullah-Mohammed on 12-May-20.
//

#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "include/glm.h"
#include "include/texture.hpp"
#include "include/startRendering.hpp"
#include "include/objLoader.h"

using namespace std;

static int mainBody = 0;
static int leftShoulderY = -90, rightShoulderY = 90;
static int leftShoulderZ = 0, rightShoulderZ = 0;
static int leftElbow = 0, rightElbow = 0;
static signed int leftLeg = 0, rightLeg = 0;
static signed int leftKnee = 0, rightKnee = 0;
static int leftFingerBase1 = 0, leftFingerBase2 = 0, leftFingerBase3 = 0, leftFingerBase4 = 0, leftFingerBase5 = 0;
static int leftFingerUp1 = 0, leftFingerUp2  = 0, leftFingerUp3 = 0, leftFingerUp4 = 0, leftFingerUp5 = 0;

static int rightFingerBase1 = 0, rightFingerBase2 = 0, rightFingerBase3 = 0, rightFingerBase4 = 0, rightFingerBase5 = 0;
static int rightFingerUp1 = 0, rightFingerUp2  = 0, rightFingerUp3 = 0, rightFingerUp4 = 0, rightFingerUp5 = 0;

// Body Positions
static float xBody = -5;
static float yBody = 1;
static float zBody = 4;

// Ball Positions
static float xBall = -6.3;
static float yBall = -0.9;
static float zBall = 3.3;

// Table Positions
static float xTable = 5;
static float yTable = -0.4;
static float zTable = 3.5;

static float ground = 1.0;

// Distance Between Ball and Body
float dist;

// Distance that will increase when kick the ball
static float kickDistance = 0;

// Variables for Jumping Distance
static float boxHeight = 1;
static float returningPosition = 1;
static bool foundObject = false;

// If state = 1 -> jumping to up
// If state = -1 -> returning to ground
static int jump_state = 1;
static int leg_state = 1;
static int kick_state = 1;
static float totalJumpDistance = 0.025 * 40 * 2;
static int isJump = false;

Model table("data/taburet1_update.obj");        // table Model
Model ball("data/soccerball.obj");              // Ball Model
Model dolphin("data/dolphins.obj");             // Dolphin model

int moving, startx, starty;
GLfloat angle = 0.0;        // In degrees
GLuint textureId;
double eye[] = { 0, 5, 10 };
double center[] = { 0, 0, 1 };
double up[] = { 0, 1, 0 };
double direction[3];
double speed = 0.1;

void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);

void createFullScene();
void createFinger(float xBase, float yBase, float zBase, int angleBase, float xrBack,
                  float xUp, float yUp, float zUp, int angleUp);

void createArm(float xShld, float yShld, float zShld,
               float xElb, float yElb, float zElb,
               float xFing, float yFing,
               float zFing1, float zFing2, float zFing3, float zFing4, float zFing5,
               float xrB, float xUp,
               float xShldRotate, int ShldAngleZ, int ShldAngleY,
               float xElbRotate, int ElbAngle,
               int baseAngle1, int upAngle1, int baseAngle2, int upAngle2,
               int baseAngle3, int upAngle3, int baseAngle4, int upAngle4,
               int baseAngle5, int upAngle5);

void createLeg(float xUp, float yUp, float zUp,
               float xLow, float yLow, float zLow,
               float ylegRotate, int legAngle,
               float yKneeRotate, int kneeAngle);

double *crossProduct(const double a[], const double b[]);
void normalize(double a[]);
void rotatePoint(const double a[], double theta, double p[]);

void Left();
void Right();
void Up();
void Down();
void moveForward();
void moveBack();
void specialKeys(int key, int x, int y);
void jump(int value);
void walkForward(int value);
void jumpOver(int value);
void Choose_texture(int id);


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 700);
    glutInitWindowPosition(100, 10);
    glutCreateWindow("Animation Of A Full Scene");

    // The Main Floor Texture for the first view
    initRendering("data/Floor1.bmp",textureId);

    // Models scaling
    table.scale(1.5);
    dolphin.scale(1.5);
    ball.scale(0.5);

    glutCreateMenu(Choose_texture);
        glutAddMenuEntry("Floor 1",1);
        glutAddMenuEntry("Floor 2",2);
        glutAddMenuEntry("Floor 3",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_FLAT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              up[0], up[1], up[2]);

    //Lighting and material stuff
    // x , y, z, w
//    GLfloat light_position[] = {0.5, 5.0, 0.0, 1.0 };
//    GLfloat lightPos1[] = {-0.5, -5.0,-2.0, 1.0 };
    GLfloat left_light_position[] =
            {0.0, 10.0, 1.0, 0.0}, right_light_position[] =
            {0.0, -10.0, 1.0, 0.0}, white_light[] =
            {1.0, 1.0, 1.0, 0.0};
    // Material Properties
    GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0};
    GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat shininess[] = {100.0};

    glPushMatrix();
        glLightfv(GL_LIGHT0, GL_POSITION, left_light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, white_light);
        glLightfv(GL_LIGHT1, GL_POSITION, right_light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT_AND_DIFFUSE, white_light);
    glPopMatrix();

    //materials properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

    // Create The Scene
    createFullScene();

    glutSwapBuffers();
}

void createFullScene()
{
    glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

        // Draw The Texture on The Floor
        glPushMatrix();
            glTranslatef(0,1.5,0);
            drawFloorTexture(textureId);
        glPopMatrix();

        // Draw table
        glPushMatrix();
            glTranslatef(xTable,yTable,zTable);
            glTranslatef(-1.0,0.0,0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(1.0,0.0,0.0);
            table.draw();
        glPopMatrix();

        // Draw Dolphin
        glPushMatrix();
//            glTranslatef(4,-0.4,3.5);
//            glTranslatef(-1.0,0.0,0.0);
//            glRotatef(90, 0.0, 1.0, 0.0);
//            glTranslatef(1.0,0.0,0.0);
            dolphin.draw();
        glPopMatrix();

        // Draw ball
        glPushMatrix();
            glTranslatef(xBall, yBall, zBall);
            ball.draw();
        glPopMatrix();

        // Create The Full Body
        // The Translation Movement For the Whole Body
        glTranslatef(xBody, yBody, zBody);
        glTranslatef(-1.0, 0.0, 0.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glTranslatef(1.0, 0.0, 0.0);

        // Draw The Head
        glTranslatef (0.0, 0.0, -2.0);
        glTranslatef (-1.0, 0.0, 0.0);
        glRotatef ((GLfloat) mainBody, 0.0, 1.0, 0.0);
        glTranslatef (1.0, 0.0, 0.0);
        glPushMatrix();
            glTranslatef(0.0, 2.00, 0.0);
            glutWireSphere(0.3, 20, 20);
        glPopMatrix();

        // Draw The Body
        glPushMatrix();
            glTranslatef(0.0, 0.70, 0.0);
            glScalef(1.2, 2.0, 0.5);
            glutWireCube(1.0);
        glPopMatrix();

        // Draw Left Arm
        glPushMatrix();
            createArm(1.3, 1.5, 0.0,
                      1.0, 0.0, 0.0,
                      1.5, 0.05,
                      0.2, 0.1, 0.0, -0.1, -0.1,
                      0.1, 0.1,
                      0.5, leftShoulderY, leftShoulderZ,
                      -0.5, leftElbow,
                      leftFingerBase1, leftFingerUp1,
                      leftFingerBase2, leftFingerUp2,
                      leftFingerBase3, leftFingerUp3,
                      leftFingerBase4, leftFingerUp4,
                      leftFingerBase5, leftFingerUp5);
        glPopMatrix();

        // Draw Right Arm
        glPushMatrix();
            createArm(-1.3, 1.5, 0.0,
                      -1.0, 0.0, 0.0,
                      -1.5, 0.05,
                      0.2, 0.1, 0.0, -0.1, -0.1,
                      -0.1, -0.1,
                      -0.5, rightShoulderY, rightShoulderZ,
                      0.5, rightElbow,
                      rightFingerBase1, rightFingerUp1,
                      rightFingerBase2, rightFingerUp2,
                      rightFingerBase3, rightFingerUp3,
                      rightFingerBase4, rightFingerUp4,
                      rightFingerBase5, rightFingerUp5);
        glPopMatrix();

        // Draw Left Leg
        glPushMatrix();
            createLeg(0.45, -0.7, 0.0,
                      0.0, -1.0, 0.0,
                      -0.5, leftLeg,
                      -0.5, leftKnee);
        glPopMatrix();

        // Draw Right Leg
        glPushMatrix();
            createLeg(-0.45, -0.7, 0.0,
                      0.0, -1.0, 0.0,
                      -0.5, rightLeg,
                      -0.5, rightKnee);
        glPopMatrix();

    glPopMatrix();

}

void createFinger(float xBase, float yBase, float zBase, int angleBase, float xrBack,
                  float xUp, float yUp, float zUp, int angleUp)
{
    //Draw finger Base
    glTranslatef(xBase, yBase, zBase);
    glRotatef((GLfloat)angleBase, 0.0, 0.0, 1.0);
    glTranslatef(xrBack, 0.0, 0.0);
    glPushMatrix();
        glScalef(0.2, 0.05, 0.05);
        glutWireCube(1);
    glPopMatrix();

    //Draw finger Up
    glTranslatef(xUp, yUp, zUp);
    glRotatef((GLfloat)angleUp, 0.0, 0.0, 1.0);
    glTranslatef(xUp, 0.0, 0.0);
    glPushMatrix();
        glScalef(2 * xUp, 0.05, 0.05);
        glutWireCube(1);
    glPopMatrix();
}

void createArm(float xShld, float yShld, float zShld,
               float xElb, float yElb, float zElb,
               float xFing, float yFing,
               float zFing1, float zFing2, float zFing3, float zFing4, float zFing5,
               float xrB, float xUp,
               float xShldRotate, int ShldAngleZ, int ShldAngleY,
               float xElbRotate, int ElbAngle,
               int baseAngle1, int upAngle1, int baseAngle2, int upAngle2,
               int baseAngle3, int upAngle3, int baseAngle4, int upAngle4,
               int baseAngle5, int upAngle5)
{
    // Shoulder
    glTranslatef(xShld, yShld, zShld);
    glTranslatef (-xShldRotate, 0.0, 0.0);
    glRotatef ((GLfloat) ShldAngleZ, 0.0, 0, 1.0);
    glRotatef ((GLfloat) ShldAngleY, 0.0, 1.0, 0.0);
    glTranslatef (xShldRotate, 0.0, 0.0);

    glPushMatrix();
        glScalef (1.0, 0.2, 0.5);
        glutWireCube (1.0);
    glPopMatrix();

    // Elbow
    glTranslatef (-xElbRotate, 0.0, 0.0);
    glRotatef ((GLfloat) ElbAngle, 0.0, 0.0, 1.0);
    glTranslatef (xElbRotate, 0.0, 0.0);
    glPushMatrix();
        glTranslatef(xElb, yElb, zElb);
        glScalef (1.0, 0.2, 0.5);
        glutWireCube (1.0);
    glPopMatrix();

    // Draw Finger 1
    glPushMatrix();
        createFinger(xFing, yFing, zFing1, baseAngle1, xrB, xUp, 0.0, 0.0, upAngle1);
    glPopMatrix();

    // Draw Finger 2
    glPushMatrix();
        createFinger(xFing, yFing, zFing2, baseAngle2, xrB, xUp, 0.0, 0.0, upAngle2);
    glPopMatrix();

    // Draw Finger 3
    glPushMatrix();
        createFinger(xFing, yFing, zFing3, baseAngle3, xrB, xUp, 0.0, 0.0, upAngle3);
    glPopMatrix();

    // Draw Finger 4
    glPushMatrix();
        createFinger(xFing, yFing, zFing4, baseAngle4, xrB, xUp, 0.0, 0.0, upAngle4);
    glPopMatrix();

    // Draw Finger 5
    glPushMatrix();
        createFinger(xFing, yFing-0.1 , zFing5, baseAngle5, xrB, xUp, 0.0, 0.0, upAngle5);
    glPopMatrix();
}

void createLeg(float xUp, float yUp, float zUp,
               float xLow, float yLow, float zLow,
               float ylegRotate, int legAngle,
               float yKneeRotate, int kneeAngle)
{
    // Upper Leg
    glTranslatef(xUp, yUp, zUp);
    glTranslatef (0.0, -ylegRotate, 0.0);
    glRotatef ((GLfloat) legAngle, 1.0, 0.0, 0.0);
    glTranslatef (0.0, ylegRotate, 0.0);
    glPushMatrix();
        glScalef (0.3, 1.0, 0.5);
        glutWireCube (1.0);
    glPopMatrix();

    // Lower Leg
    glTranslatef(xLow, yLow, zLow);
    glTranslatef (0.0, -yKneeRotate, 0.0);
    glRotatef ((GLfloat) kneeAngle, 1.0, 0.0, 0.0);
    glTranslatef (0.0, yKneeRotate, 0.0);
    glPushMatrix();
        glScalef (0.3, 1.0, 0.5);
        glutWireCube (1.0);
    glPopMatrix();

    // Bottom Leg
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslatef(0.0, -0.6, 0.20);
        glScalef(0.3, 0.2, 1.0);
        glutWireCube(1.0);
    glPopMatrix();
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(100.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}

double *crossProduct(const double a[], const double b[])
{
    static double c[] = {};
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
    return c;
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(const double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta)*p[0];
    temp[1] += cos(theta)*p[1];
    temp[2] += cos(theta)*p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];

}

void Left()
{
    // implement camera rotation arround vertical window screen axis to the left
    // used by mouse and left arrow
    rotatePoint(up, -0.1, eye);
}

void Right()
{
    // implement camera rotation arround vertical window screen axis to the right
    // used by mouse and right arrow
    rotatePoint(up, 0.1, eye);
}

void Up()
{
    // implement camera rotation arround horizontal window screen axis +ve
    // used by up arrow
    double *h_direction;
    h_direction = crossProduct(up, eye);
    normalize(h_direction);

    // Rotate the eye about the horizontal direction
    rotatePoint(h_direction, 0.1, eye);

    // Rotate the up vector about the horizontal direction
    rotatePoint(h_direction, 0.1, up);

}

void Down()
{
    // implement camera rotation arround horizontal window screen axis
    // used by down arrow
    double *h_direction;
    h_direction = crossProduct(up, eye);
    normalize(h_direction);

    // Rotate the eye about the horizontal direction
    rotatePoint(h_direction, -0.1, eye);

    // Rotate the up vector about the horizontal direction
    rotatePoint(h_direction, -0.1, up);
}

void moveForward()
{
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;
    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void moveBack()
{
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;
    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            Left();
            break;
        case GLUT_KEY_RIGHT:
            Right();
            break;
        case GLUT_KEY_UP:
            Up();
            break;
        case GLUT_KEY_DOWN:
            Down();
            break;
        default:
            break;
    }

    glutPostRedisplay();
}


void jump(int heightValue)
{
    switch (jump_state)
    {
        // Jumping
        case 1:
            if (yBody < float(heightValue)) {
                yBody += 0.05;
                leftShoulderY += 1;
                rightShoulderY -= 1;
                leftKnee += 1;
                rightKnee += 1;
            } else {
                jump_state = -1;
            }
            glutTimerFunc(1000/60, jump, heightValue);
            break;

        // Returning to ground
        case -1:
            if(yBody > ground) {
                yBody -= 0.05;
                leftShoulderY -= 1;
                rightShoulderY += 1;
                leftKnee -= 1;
                rightKnee -= 1;
            } else if (yBody < ground) {
                jump_state = 0;
            }
            glutTimerFunc(1000/60, jump, heightValue);
            break;

        // Returned to ground
        case 0:
            jump_state = 1;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void walkForward(int value)
{
    /*
     * leg_state 1: Moving The Right Leg and Knee Forward
     *             Moving The Left Leg and Knee Backward
     * leg_state 2: Finished Moving The Right Leg and Knee
     *             Finished Moving The Right Leg and Knee
     *             And Start Moving in x-direction
     *
     */
    switch (leg_state)
    {
        case 1:
//            cout<<"leg_state: "<<leg_state<<" Right Leg: "<<rightLeg;
//            cout<<" Left Leg: "<<leftLeg<<endl;
            if (rightLeg > -45) {
                // Move Body
                xBody += 0.03;

                // Move Right Leg Forward
                rightLeg -= 1;
                rightKnee += 1;

                // Move Left Leg Backward
                if (leftLeg < 30)
                {
                    cout<<"leftLeg "<<leftLeg<<endl;
                    leftLeg += 1;
                    leftKnee += 1;
                }

                // Move Arms
                if (rightShoulderZ < 45)
                {
                    rightShoulderZ += 1;
                    leftShoulderZ += 1;
                }
            } else {
                leg_state = 2;
            }
            glutTimerFunc(1000/60, walkForward, 0);
            break;

        case 2:
//            cout<<"leg_state: "<<leg_state<<"  Right Leg: "<<rightLeg;
//            cout<<" Left Leg: "<<leftLeg<<endl;
            if (rightLeg < 0) {
                // Move Body
                xBody += 0.03;

                // Move Right Leg
                rightLeg += 1;
                rightKnee -= 1;

                // Return Left Leg
                if (leftLeg > 0)
                {
                    leftLeg -= 1;
                    leftKnee -= 1;
                }

                // Return Arms
                if (rightShoulderZ > 0)
                {
                    rightShoulderZ -= 1;
                    leftShoulderZ -= 1;
                }
            }
            else {
                leg_state = 0;
            }
            glutTimerFunc(1000/60, walkForward, 0);
            break;

        case 0:
            leg_state = 1;
            break;

        default:
            break;

    }
    glutPostRedisplay();
}

void jumpOver(int heightValue)
{
    // Check Distance between object and robot
    // Check if the object is near

//    if (10 - xBody < totalJumpDistance) {
//        foundObject = true;
//        returningPosition = boxHeight;
//    }
//
//    if (!foundObject)
//    {
//        // DO JUMP
//
//    }

    if (10 - xBody > totalJumpDistance)
    {
        switch (jump_state)
        {
            // Jumping
            case 1:
                if (yBody < float(heightValue)) {
                    yBody += 0.05;
                    xBody += 0.025;
                    leftShoulderY += 1;
                    rightShoulderY -= 1;
                    leftKnee += 1;
                    rightKnee += 1;
                } else {
                    jump_state = -1;
                }
                glutTimerFunc(1000/60, jumpOver, heightValue);
                break;

            // Returning Back
            case -1:
                if(yBody > returningPosition) {
                    yBody -= 0.05;
                    xBody += 0.025;
                    leftShoulderY -= 1;
                    rightShoulderY += 1;
                    leftKnee -= 1;
                    rightKnee -= 1;
                } else if (yBody < returningPosition) {
                    jump_state = 0;
                }
                glutTimerFunc(1000/60, jumpOver, heightValue);
                break;

                // Returned to ground
            case 0:
                jump_state = 1;
                break;

            default:
                break;
        }
        glutPostRedisplay();
    }

    else
    {
        foundObject = true;
        returningPosition = boxHeight;
        glutTimerFunc(1000/60, jumpOver, heightValue);
    }

//    switch (state)
//    {
//        // Jumping
//        case 1:
//            if (yBody < heightValue) {
//                yBody += 0.05;
//                xBody += 0.025;
//                leftShoulderY += 1;
//                rightShoulderY -= 1;
//                leftKnee += 1;
//                rightKnee += 1;
//            } else {
//                state = -1;
//            }
//            glutTimerFunc(1000/60, jumpOver, heightValue);
//            break;
//
//            // Returning to ground
//        case -1:
//            if(yBody > 0) {
//                yBody -= 0.05;
//                xBody += 0.025;
//                leftShoulderY -= 1;
//                rightShoulderY += 1;
//                leftKnee -= 1;
//                rightKnee -= 1;
//            } else if (yBody < 0) {
//                state = 0;
//            }
//            glutTimerFunc(1000/60, jumpOver, heightValue);
//            break;
//
//            // Returned to ground
//        case 0:
//            state = 1;
//            break;
//
//        default:
//            break;
//    }
//    glutPostRedisplay();
}

void kick(int kickValue)
{
    switch (kick_state)
    {
        case 1:
            if (rightKnee < 45) {
                rightLeg += 1;
                rightKnee += 1;
            } else {
                // rightLeg: 45
                // rightKnee: 45
                kick_state = 2;
                cout<<"Leg is Back State Now"<<endl;
            }
            glutTimerFunc(10, kick,kickValue);
            break;

        case 2:
            if (rightLeg > -15)
            {
                rightLeg -= 1;
                if (rightKnee > 0)
                {
                    rightKnee -= 1;
                }
            }
            else {
                // Now rightLeg: -15
                // Now rightKnee: 0

                // If the ball exists
                dist = abs(xBall) - abs(xBody);
                cout<<"Distance Between Ball and Body "<<dist<<endl;
                if (dist > 1.0f && dist < 1.4f)
                {
                    cout<<"Ball Exists"<<endl;
                    kick_state = 3;
                } else {
                    cout<<"Ball Doesn't Exist"<<endl;
                    kick_state = 4;
                }
            }
            glutTimerFunc(10, kick,kickValue);
            break;

        // If the ball exists -> Return Leg to Original State and Move the Ball
        case 3:
            if (kickDistance < float(kickValue)) {
                kickDistance += 0.1;

                // Change Ball Positions
                xBall += 0.1;
                if (kickDistance < float(kickValue)/2) {
                    yBall += 0.02;
                } else {
                    yBall -= 0.02;
                }

                if (rightLeg < 0) {
                    rightLeg += 1;
                }
            }
            else {
                cout<<"Ball has reached the target"<<endl;
                kickDistance = 0;
                kick_state = 0;
            }
            glutTimerFunc(10, kick,kickValue);
            break;

        // If Ball doesn't exists -> Return Leg Only
        case 4:
            if (rightLeg < 0) {
                rightLeg += 1;
            } else {
                cout<<"Leg Returned to Origin State"<<endl;
                kick_state = 0;
            }
            glutTimerFunc(10, kick,kickValue);
            break;

        case 0:
            kick_state = 1;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        // Jump Case
        case ' ':
            jump(3);
            break;

        // walkForward Case
        // Change to d
        case 'd':
            walkForward(0);
            break;

        // Kick Case
        case 'k':
            kick(4);
            break;       

        // Long Jump
        case 'l':
            jumpOver(3);
            break;

        case '1':
            moveForward();
            break;
        case '2':
            moveBack();
            break;

        // Whole Body
        case 'b':
            mainBody = (mainBody - 5) % 360;
            break;
        case 'B':
            mainBody = (mainBody + 5) % 360;
            break;

        // Left ShoulderY
        case 'y':
            if(leftShoulderY < 90)
                leftShoulderY += 5;
            break;
        case 'Y':
            if(leftShoulderY > -90)
                leftShoulderY -= 5;
            break;

        // Left ShoulderZ
        case 'u':
            if(leftShoulderZ > -180)
                leftShoulderZ -= 5;
            break;
        case 'U':
            if(leftShoulderZ < 0)
                leftShoulderZ += 5;
            break;

        // Left Elbow
        case 'h':
            if(leftElbow >- 120)
                leftElbow -= 5;
            break;
        case 'H':
            if(leftElbow < 0)
                leftElbow += 5;
            break;

        // Right ShoulderY
        case 'r':
            if(rightShoulderY > -90)
                rightShoulderY -= 5;
            break;
        case 'R':
            if(rightShoulderY < 90)
                rightShoulderY += 5;
            break;

        // Right ShoulderZ
        case 't':
            if(rightShoulderZ < 180)
                rightShoulderZ += 5;
            break;
        case 'T':
            if(rightShoulderZ > 0)
                rightShoulderZ -= 5;
            break;

        // Right Elbow
        case 'f':
           if(rightElbow < 120)
               rightElbow += 5;
           break;
        case 'F':
            if(rightElbow > 0)
                rightElbow -= 5;
            break;

        // Left Leg
        case 'z':
            if(leftLeg > -90)
                leftLeg-=5;
            break;
        case 'Z':
            if(leftLeg < 75)
                leftLeg+=5;
            break;

        // Left Knee
        case 'x':
            if (leftKnee < 90)
                leftKnee += 5;
            break;
        case 'X':
            if(leftKnee > 0)
                leftKnee -= 5;
            break;

        // Right Leg
        case 'c':
            if(rightLeg > -90)
                rightLeg -= 5;
            break;
        case 'C':
            if(rightLeg < 75)
                rightLeg += 5;
            break;

        // Right Knee
        case 'v':
            if(rightKnee < 90)
                rightKnee += 5;
            break;
        case 'V':
            if(rightKnee > 0)
                rightKnee -= 5;
            break;

        case 27:
            exit(0);

        default:
            break;
    }
    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            moving = 1;
            startx = x;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}

static void motion(int x, int y)
{
    if (moving)
    {
        angle = (x - startx) / 25.0;
        rotatePoint(up, angle, eye);
        startx = x;
        glutPostRedisplay();
    }

}

void Choose_texture(int id)
{
    switch(id)
    {
        case 1:
            initRendering("data/Floor1.bmp",textureId);
            break;

        case 2:
            initRendering("data/Floor2.bmp",textureId);
            break;

        case 3:
            initRendering("data/Floor3.bmp",textureId);
            break;

        default:
            break;
    }
    glutPostRedisplay();
}


