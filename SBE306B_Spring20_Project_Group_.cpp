//
// Created by Abdullah-Mohammed on 12-May-20.
//



#include <GL/glut.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "include/glm.h"
#include "imageLoader/imageloader.h"
#include "texture.hpp"
#include "startRendering.hpp"
#include "include/objLoader.h"

using namespace std;

static int mainBody = 0;
static int leftShoulder = -90, rightShoulder = 90;
static int leftElbow = 0, rightElbow = 0;
static signed int leftLeg = 0, rightLeg = 0;
static signed int leftKnee = 0, rightKnee = 0;
static int leftFingerBase1 = 0, leftFingerBase2 = 0, leftFingerBase3 = 0, leftFingerBase4 = 0, leftFingerBase5 = 0;
static int leftFingerUp1 = 0, leftFingerUp2  = 0, leftFingerUp3 = 0, leftFingerUp4 = 0, leftFingerUp5 = 0;

static int rightFingerBase1 = 0, rightFingerBase2 = 0, rightFingerBase3 = 0, rightFingerBase4 = 0, rightFingerBase5 = 0;
static int rightFingerUp1 = 0, rightFingerUp2  = 0, rightFingerUp3 = 0, rightFingerUp4 = 0, rightFingerUp5 = 0;

// Variables for Jumping Distance
static double jumpOffset = 0;
static double forwardOffset = 0;
static float boxHeight = 1;
static float returningPosition = 0;
static bool foundObject = false;


// If state = 1 -> jumping to up
// If state = -1 -> returning to ground
static int state = 1;
static int LegState = 1;
static int kick_state = 1;
static float totalJumpDistance = 0.025 * 40 * 2;
static int isJump = false;

// box Model
Model box("data/taburet1_update.obj");
// Ball Model
Model ball("data/soccerball.obj");

// Dolphin model
Model dolphin("data/dolphins.obj");

int moving, startx, starty;
GLfloat angle = 0.0;   /* in degrees */
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
               float xShldRotate, int ShldAngle,
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

    //Models scaling
    box.scale(1.5);
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

        // Draw box
        glPushMatrix();
            glTranslatef(4,-0.4,3.5);
            glTranslatef(-1.0,0.0,0.0);
            glRotatef(90, 0.0, 1.0, 0.0);
            glTranslatef(1.0,0.0,0.0);
            box.draw();
        glPopMatrix();

        dolphin.draw();

        // Draw ball
        glPushMatrix();
            glTranslatef(-5.3,-1.0, 3.2);
            ball.draw();
        glPopMatrix();

        // Create The Full Body
        // The Translation Movement For the Whole Body
        glTranslatef(-5.0, 1.0, 0.0);
        glTranslatef(forwardOffset, jumpOffset, 3.0);
        glRotatef(90, 0.0, 1.0, 0.0);

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
                      0.5, leftShoulder,
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
                      -0.5, rightShoulder,
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
               float xShldRotate, int ShldAngle,
               float xElbRotate, int ElbAngle,
               int baseAngle1, int upAngle1, int baseAngle2, int upAngle2,
               int baseAngle3, int upAngle3, int baseAngle4, int upAngle4,
               int baseAngle5, int upAngle5)
{
    // Shoulder
    glTranslatef(xShld, yShld, zShld);
    glTranslatef (-xShldRotate, 0.0, 0.0);
    glRotatef ((GLfloat) ShldAngle, 0.0, 0.0, 1.0);
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
    switch (state)
    {
        // Jumping
        case 1:
            if (jumpOffset < heightValue) {
                jumpOffset += 0.05;
                leftShoulder += 1;
                rightShoulder -= 1;
                leftKnee += 1;
                rightKnee += 1;
            } else {
                state = -1;
            }
            glutTimerFunc(1000/60, jump, heightValue);
            break;

        // Returning to ground
        case -1:
            if(jumpOffset > 0) {
                jumpOffset -= 0.05;
                leftShoulder -= 1;
                rightShoulder += 1;
                leftKnee -= 1;
                rightKnee -= 1;
            } else if (jumpOffset < 0) {
                state = 0;
            }
            glutTimerFunc(1000/60, jump, heightValue);
            break;

        // Returned to ground
        case 0:
            state = 1;
            break;

        default:
            break;
    }
    glutPostRedisplay();
}

void walkForward(int value)
{
    /*
     * LegState 1: Moving The Right Leg and Knee
     * LegState 2: Finished Moving The Right Leg and Knee
     *             And Start Moving in x-direction
     *
     */
    switch (LegState)
    {
        case 1:
//            cout<<"LegState: "<<LegState<<" Right Leg: "<<rightLeg;
//            cout<<" Left Leg: "<<leftLeg<<endl;
            if (rightLeg > -30) {
                rightLeg = (rightLeg - 1) % 360;
                rightKnee = (rightKnee + 1) % 360;
            } else {
                LegState = 2;
            }
            glutTimerFunc(1000/60, walkForward, 0);
            break;

        case 2:
//            cout<<"LegState: "<<LegState<<"  Right Leg: "<<rightLeg;
//            cout<<" Left Leg: "<<leftLeg<<endl;
            if (leftLeg > -30) {
                leftLeg = (leftLeg - 1) % 360;
                leftKnee = (leftKnee + 1) % 360;
                rightLeg = (rightLeg + 1) % 360;
                rightKnee = (rightKnee - 1) % 360;
                forwardOffset += 0.02;
            }
            else {
                LegState = 3;
            }
            glutTimerFunc(1000/60, walkForward, 0);
            break;

        case 3:
            if (leftLeg < 0)
            {
                leftLeg = (leftLeg + 1) % 360;
                leftKnee = (leftKnee - 1) % 360;
            } else {
                LegState = 0;
            }
            glutTimerFunc(1000/60, walkForward, 0);
            break;

        case 0:
            LegState = 1;
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

//    if (10 - forwardOffset < totalJumpDistance) {
//        foundObject = true;
//        returningPosition = boxHeight;
//    }
//
//    if (!foundObject)
//    {
//        // DO JUMP
//
//    }

    if (10 - forwardOffset > totalJumpDistance || foundObject)
    {
        switch (state)
        {
            // Jumping
            case 1:
                if (jumpOffset < heightValue) {
                    jumpOffset += 0.05;
                    forwardOffset += 0.025;
                    leftShoulder += 1;
                    rightShoulder -= 1;
                    leftKnee += 1;
                    rightKnee += 1;
                } else {
                    state = -1;
                }
                glutTimerFunc(1000/60, jumpOver, heightValue);
                break;

            // Returning Back
            case -1:
                if(jumpOffset > returningPosition) {
                    jumpOffset -= 0.05;
                    forwardOffset += 0.025;
                    leftShoulder -= 1;
                    rightShoulder += 1;
                    leftKnee -= 1;
                    rightKnee -= 1;
                } else if (jumpOffset < returningPosition) {
                    state = 0;
                }
                glutTimerFunc(1000/60, jumpOver, heightValue);
                break;

                // Returned to ground
            case 0:
                state = 1;
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
//            if (jumpOffset < heightValue) {
//                jumpOffset += 0.05;
//                forwardOffset += 0.025;
//                leftShoulder += 1;
//                rightShoulder -= 1;
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
//            if(jumpOffset > 0) {
//                jumpOffset -= 0.05;
//                forwardOffset += 0.025;
//                leftShoulder -= 1;
//                rightShoulder += 1;
//                leftKnee -= 1;
//                rightKnee -= 1;
//            } else if (jumpOffset < 0) {
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
void kick(int value)
{
    switch (kick_state)
    {
        case 1:
            if (rightKnee < 45) {
                rightLeg+=1;
                rightKnee+=1;
            } else {
                kick_state=-1;
            }
            glutTimerFunc(10, kick,1);
            break;

        case -1:
            if (rightLeg > -15)
            {
                rightLeg-=1;
                if (rightKnee >0)
                {
                    rightKnee-=1;
                }
            }
            else if(rightLeg == -15)
            {
                //z_sphere is the z coordinate for the sphere or ball object
                // if(z_sphere < 5){
                //     z_sphere +=.1;
                // }
                // else
                // {
                //     kick_state=0;
                // }
                kick_state = 0;
            }
            glutTimerFunc(10, kick,1);
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
            jump(2);
            break;

        // walkForward Case
        case '3':
            walkForward(0);
            break;
        // kick Case
        case 'l':
            //z_sphere=1.6;
            kick_state=1;
            rightKnee=0;
            rightLeg=0;
            kick(1);
            break;       

        case '4':
            jumpOver(2);
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

       // Left Shoulder
        case 'r':
            if(leftShoulder < 90)
                leftShoulder+=5;
            break;
        case 'R':
          if(leftShoulder > -90)
                leftShoulder-=5;
            break;

        // Left Elbow
        case 'f':
        if(leftElbow>-120)
            leftElbow-=5;
            break;
        case 'F':
           if(leftElbow < 0)
           leftElbow+=5;
            break;

        // Right Shoulder
        case 'q':
                if(rightShoulder > -90)
                rightShoulder-=5;
            
            break;
        case 'Q':
            if(rightShoulder < 90)
                rightShoulder+=5;
            break;

        // Right Elbow
        case 'a':
           if(rightElbow<120)
            rightElbow+=5;
            break;
        case 'A':
        if(rightElbow > 0)
           rightElbow-=5;
            break;

        // Left Leg
        case 'e':
            if(leftLeg > -90)
                leftLeg-=5;
            break;
        case 'E':
            if(leftLeg < 75)
                leftLeg+=5;
            break;

        // Left Knee
        case 'd':
            if(leftKnee <90)
            leftKnee+=5;
            break;
        case 'D':
            if(leftKnee >0)
            leftKnee-=5;
            break;


        // Right Leg
        case 'w':
            if(rightLeg > -90)
                rightLeg-=5;
            break;
        case 'W':
            if(rightLeg < 75)
                rightLeg+=5;
            break;

        // Right Knee
        case 's':
        if(rightKnee <90)
            rightKnee+=5;
            break;
        case 'S':
            if(rightKnee >0)
            rightKnee-=5;
            break;


        // Left Finger 1 Control
        case 't':
            leftFingerBase1 = (leftFingerBase1 - 5) % 360;
            break;
        case 'T':
            leftFingerBase1 = (leftFingerBase1 + 5) % 360;
            break;
        case 'y':
            leftFingerUp1 = (leftFingerUp1 - 5) % 360;
            break;
        case 'Y':
            leftFingerUp1 = (leftFingerUp1 + 5) % 360;
            break;

        // Left Finger 2 Control
        case 'u':
            leftFingerBase2 = (leftFingerBase2 - 5) % 360;
            break;
        case 'U':
            leftFingerBase2 = (leftFingerBase2 + 5) % 360;
            break;
        case 'i':
            leftFingerUp2 = (leftFingerUp2 - 5) % 360;
            break;
        case 'I':
            leftFingerUp2 = (leftFingerUp2 + 5) % 360;
            break;

        // Left Finger 3 Control
        case 'o':
            leftFingerBase3 = (leftFingerBase3 - 5) % 360;
            break;
        case 'O':
            leftFingerBase3 = (leftFingerBase3 + 5) % 360;
            break;
        case 'p':
            leftFingerUp3 = (leftFingerUp3 - 5) % 360;
            break;
        case 'P':
            leftFingerUp3 = (leftFingerUp3 + 5) % 360;
            break;

        // Left Finger 4 Control
        case 'g':
            leftFingerBase4 = (leftFingerBase4 - 5) % 360;
            break;
        case 'G':
            leftFingerBase4 = (leftFingerBase4 + 5) % 360;
            break;
        case 'h':
            leftFingerUp4 = (leftFingerUp4 - 5) % 360;
            break;
        case 'H':
            leftFingerUp4 = (leftFingerUp4 + 5) % 360;
            break;

        // Left Finger 5 Control
        case 'j':
            leftFingerBase5 = (leftFingerBase5 - 5) % 360;
            break;
        case 'J':
            leftFingerBase5 = (leftFingerBase5 + 5) % 360;
            break;
        case 'k':
            leftFingerUp5 = (leftFingerUp5 - 5) % 360;
            break;
        case 'K':
            leftFingerUp5 = (leftFingerUp5 + 5) % 360;
            break;


        // Right Finger 1 Control
        case 'z':
            rightFingerBase1 = (rightFingerBase1 - 5) % 360;
            break;
        case 'Z':
            rightFingerBase1 = (rightFingerBase1 + 5) % 360;
            break;
        case 'x':
            rightFingerUp1 = (rightFingerUp1 - 5) % 360;
            break;
        case 'X':
            rightFingerUp1 = (rightFingerUp1 + 5) % 360;
            break;

        // Left Finger 2 Control
        case 'c':
            rightFingerBase2 = (rightFingerBase2 - 5) % 360;
            break;
        case 'C':
            rightFingerBase2 = (rightFingerBase2 + 5) % 360;
            break;
        case 'v':
            rightFingerUp2 = (rightFingerUp2 - 5) % 360;
            break;
        case 'V':
            rightFingerUp2 = (rightFingerUp2 + 5) % 360;
            break;

        // Left Finger 3 Control
        case 'n':
            rightFingerBase3 = (rightFingerBase3 - 5) % 360;
            break;
        case 'N':
            rightFingerBase3 = (rightFingerBase3 + 5) % 360;
            break;
        case 'm':
            rightFingerUp3 = (rightFingerUp3 - 5) % 360;
            break;
        case 'M':
            rightFingerUp3 = (rightFingerUp3 + 5) % 360;
            break;

        // Left Finger 4 Control
        case '[':
            rightFingerBase4 = (rightFingerBase4 - 5) % 360;
            break;
        case '{':
            rightFingerBase4 = (rightFingerBase4 + 5) % 360;
            break;
        case ']':
            rightFingerUp4 = (rightFingerUp4 - 5) % 360;
            break;
        case '}':
            rightFingerUp4 = (rightFingerUp4 + 5) % 360;
            break;

        // Left Finger 5 Control
        case ';':
            rightFingerBase5 = (rightFingerBase5 - 5) % 360;
            break;
        case ':':
            rightFingerBase5 = (rightFingerBase5 + 5) % 360;
            break;
        case '/':
            rightFingerUp5 = (rightFingerUp5 - 5) % 360;
            break;
        case '?':
            rightFingerUp5 = (rightFingerUp5 + 5) % 360;
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


