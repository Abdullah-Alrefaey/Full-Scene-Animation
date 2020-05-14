//
// Created by ahmad on 5/13/20.
//

#include "startRendering.hpp"
#include <iostream>
#include "imageLoader/imageloader.h"
#include <GL/glut.h>
#include "texture.hpp"

//Initializes 3D rendering
void initRendering(const char* bmbImg, GLuint & textureID){
    // Loading a Texture
    Image *image = loadBMP(bmbImg);
    textureID = loadTexture(image);
    delete image;
//
//    // Lighting and Rendering Stuff
//
//    // Turn on the power
//    glEnable(GL_LIGHTING);
//
//    // Flip light switch
//    glEnable(GL_LIGHT0);
//    glEnable(GL_LIGHT1);
//
//    // assign light parameters
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
//    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
//    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
//
//    // Material Properties
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
//
//    // Lighting Properties
//    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
//    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
//    glEnable(GL_NORMALIZE);
//
//    //Enable smooth shading
//    glShadeModel(GL_SMOOTH);
//
//    // Enable Depth buffer
//    glEnable(GL_DEPTH_TEST);
}