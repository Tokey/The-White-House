#include<windows.h>
#include <iostream>
#include <bits/stdc++.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include "imageloader.h"
#endif
using namespace std;
// angle of rotation for the camera direction
float angle = 0.0f;
float radius, halfLength;
GLUquadricObj *quadratic;
int slices;

// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f, ly = 0.0f;

// XZ position of the camera
float x=0.0f, z=5.0f, y = 1.75f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

// width and height of the window
int h,w;

// variables to compute frames per second
int frame;
long ttt, timebase;
char s[50];

// variables to hold window identifiers
int mainWindow, subWindow1,subWindow2,subWindow3;
//border between subwindows
int border = 6;

GLuint loadTexture(Image* image) {
	GLuint textureId;

	//The first argument is the number of textures we need, and the second is an array where OpenGL will store the id's of the textures.
	glGenTextures(1, &textureId); //Make room for our texture
	// to assign the texture id to our image data.
//	We call glBindTexture(GL_TEXTURE_2D, textureId) to let OpenGL know
// that we want to work with the texture we just created. Then, we call glTexImage2D to load the image into OpenGL.
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _texturef;
GLuint _textureb;
GLuint _texturel;
GLuint _texturer; //The id of the texture

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


	//Take a look at imageloader.h. This gives us the basic idea of what loadBMP does.
	//(The actual code for loadBMP is in imageloader.cpp.) Given a filename, it returns an Image object,
	//which contains the width and height of the image, as well as the array pixels,
	//which stores the pixels' colors in the format we want.
	Image* image1 = loadBMP("front.bmp");
    Image* image2 = loadBMP("bottom.bmp");
    Image* image3 = loadBMP("left.bmp");
    Image* image4 = loadBMP("right.bmp");

	//Once we've got the image, we have to send it to OpenGL. We do this in a function we write called loadTexture.
	_texturef = loadTexture(image1);
	_textureb = loadTexture(image2);
	_texturel = loadTexture(image3);
	_texturer = loadTexture(image4);


	delete image1;
	delete image2;
	delete image3;
	delete image4;
}

void setProjection(int w1, int h1)
{
    float ratio;
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    ratio = 1.0f * w1 / h1;
    // Reset the coordinate system before modifying
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, w1, h1);

    // Set the clipping volume
    gluPerspective(45,ratio,0.1,1000);
    glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w1,int h1)
{

    if(h1 == 0)
        h1 = 1;

    // we're keeping these values cause we'll need them latter
    w = w1;
    h = h1;

    // set subwindow 1 as the active window
    glutSetWindow(subWindow1);
    // resize and reposition the sub window
    glutPositionWindow(border,border);
    glutReshapeWindow(w-2*border, h/2 - border*3/2);
    setProjection(w-2*border, h/2 - border*3/2);

    // set subwindow 2 as the active window
    glutSetWindow(subWindow2);
    // resize and reposition the sub window
    glutPositionWindow(border,(h+border)/2);
    glutReshapeWindow(w/2-border*3/2, h/2 - border*3/2);
    setProjection(w/2-border*3/2,h/2 - border*3/2);

    // set subwindow 3 as the active window
    glutSetWindow(subWindow3);
    // resize and reposition the sub window
    glutPositionWindow((w+border)/2,(h+border)/2);
    glutReshapeWindow(w/2-border*3/2,h/2 - border*3/2);
    setProjection(w/2-border*3/2,h/2 - border*3/2);
}

void drawSnowMan()
{

   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);

	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);

glEnable(GL_TEXTURE_2D);

    glBegin(GL_TRIANGLES);

    glColor3f(0.8f, 0.8f, 0.8f);

    glNormal3f(1.0, 1.0f, 1.0f);


    glVertex3f(-1.0f, 1.5f, -1.0f);


    glVertex3f(3.0f, 1.5f, -1.0f);


    glVertex3f(1.0f, 2.5f, -1.0f);

    glEnd();


     //Front TRIANGLE

    ///TEXTURE

	glBindTexture(GL_TEXTURE_2D, _textureb);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBegin(GL_QUADS);

    glColor3f(0.8f, 0.8f, 0.8f);
    glNormal3f(1.0, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);

    glVertex3f(-1.0f, 1.5f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-1.0f, 1.5f, -5.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 2.5f, -5.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 2.5f, -1.0f);


    glEnd();

    glBindTexture(GL_TEXTURE_2D, _texturel);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);

    glColor3f(0.8f, 0.8f, 0.8f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(3.0f, 1.5f, -5.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, 2.5f, -5.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(1.0f, 2.5f, -1.0f);


    glEnd();

    glColor3f(0.8f, 0.8f, 0.8f);

    glColor3f(0.8f, 0.8f, 0.8f);

    glVertex3f(-1.0f, 1.5f, -5.0f);
    glVertex3f(3.0f, 1.5f, -5.0f);
    glVertex3f(1.0f, 2.5f, -5.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(-1.0f, 1.5f, -1.0f);
    glVertex3f(-1.0f, 1.5f, -5.0f);
    glVertex3f(3.0f, 1.5f, -5.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);


    glEnd();





    //front small quad





    glBegin(GL_QUADS);

    glColor3f(0.85f, 0.85f, 0.85f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(3.0f, 1.0f, -1.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);
    glVertex3f(-1.0f, 1.5f, -1.0f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.85f, 0.85f, 0.85f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -5.0f);
    glVertex3f(-1.0f, 1.5f, -5.0f);
    glVertex3f(-1.0f, 1.5f, -1.0f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.85f, 0.85f, 0.85f);
    glVertex3f(3.0f, 1.0f, -1.0f);
    glVertex3f(3.0f, 1.0f, -5.0f);
    glVertex3f(3.0f, 1.5f, -5.0f);
    glVertex3f(3.0f, 1.5f, -1.0f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.85f, 0.85f, 0.85f);
    glVertex3f(-1.0f, 1.0f, -5.0f);
    glVertex3f(3.0f, 1.0f, -5.0f);
    glVertex3f(3.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);


    glEnd();


    //WINDOWS LEFT UP


    glBindTexture(GL_TEXTURE_2D, _texturef);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(1.0, 1.0f, 1.0f);
    glColor3f(0.70f, 0.70f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.4f, -1.1f, -3.9f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-5.5f, -1.1f, -3.9f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-5.5f, 0.5f, -3.9f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.4f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-5.1f, -1.1f, -3.9f);
    glVertex3f(-4.2f, -1.1f, -3.9f);
    glVertex3f(-4.2f, 0.5f, -3.9f);
    glVertex3f(-5.1f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-3.8f, -1.1f, -3.9f);
    glVertex3f(-2.9f, -1.1f, -3.9f);
    glVertex3f(-2.9f, 0.5f, -3.9f);
    glVertex3f(-3.8f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-2.5f, -1.1f, -3.9f);
    glVertex3f(-1.6f, -1.1f, -3.9f);
    glVertex3f(-1.6f, 0.5f, -3.9f);
    glVertex3f(-2.5f, 0.5f, -3.9f);

    glEnd();









    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-6.4f, -1.1f, -12.1f);
    glVertex3f(-5.5f, -1.1f, -12.1f);
    glVertex3f(-5.5f, 0.5f, -12.1f);
    glVertex3f(-6.4f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-5.1f, -1.1f, -12.1f);
    glVertex3f(-4.2f, -1.1f, -12.1f);
    glVertex3f(-4.2f, 0.5f, -12.1f);
    glVertex3f(-5.1f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-3.8f, -1.1f, -12.1f);
    glVertex3f(-2.9f, -1.1f, -12.1f);
    glVertex3f(-2.9f, 0.5f, -12.1f);
    glVertex3f(-3.8f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-2.5f, -1.1f, -12.1f);
    glVertex3f(-1.6f, -1.1f, -12.1f);
    glVertex3f(-1.6f, 0.5f, -12.1f);
    glVertex3f(-2.5f, 0.5f, -12.1f);

    glEnd();

//WINDOWS LEFT DOWN


//DESIGN



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-6.4f, -4.1f, -3.9f);
    glVertex3f(-5.5f, -4.1f, -3.9f);
    glVertex3f(-5.5f, -2.5f, -3.9f);
    glVertex3f(-6.4f, -2.5f, -3.9f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-6.4f, -4.1f, -12.1f);
    glVertex3f(-5.5f, -4.1f, -12.1f);
    glVertex3f(-5.5f, -2.5f, -12.1f);
    glVertex3f(-6.4f, -2.5f, -12.1f);

    glEnd();

    //DESIGN


    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-5.2f, -2.4f, -3.9f);
    glVertex3f(-4.1f, -2.4f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-4.1f, -2.4f, -3.9f);
    glVertex3f(-4.65f, -2.1f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-4.65f, -2.1f, -3.9f);
    glVertex3f(-5.2f, -2.4f, -3.9f);
    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-5.1f, -4.1f, -3.9f);
    glVertex3f(-4.2f, -4.1f, -3.9f);
    glVertex3f(-4.2f, -2.5f, -3.9f);
    glVertex3f(-5.1f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-5.1f, -4.1f, -12.1f);
    glVertex3f(-4.2f, -4.1f, -12.1f);
    glVertex3f(-4.2f, -2.5f, -12.1f);
    glVertex3f(-5.1f, -2.5f, -12.1f);

    glEnd();

    //DESIGN

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-3.8f, -4.1f, -3.9f);
    glVertex3f(-2.9f, -4.1f, -3.9f);
    glVertex3f(-2.9f, -2.5f, -3.9f);
    glVertex3f(-3.8f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-3.8f, -4.1f, -12.1f);
    glVertex3f(-2.9f, -4.1f, -12.1f);
    glVertex3f(-2.9f, -2.5f, -12.1f);
    glVertex3f(-3.8f, -2.5f, -12.1f);

    glEnd();

    //DESIGN



    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-2.6f, -2.4f, -3.9f);
    glVertex3f(-1.5f, -2.4f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-1.5f, -2.4f, -3.9f);
    glVertex3f(-2.05f, -2.1f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(-2.05f, -2.1f, -3.9f);
    glVertex3f(-2.6f, -2.4f, -3.9f);
    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-2.5f, -4.1f, -3.9f);
    glVertex3f(-1.6f, -4.1f, -3.9f);
    glVertex3f(-1.6f, -2.5f, -3.9f);
    glVertex3f(-2.5f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-2.5f, -4.1f, -12.1f);
    glVertex3f(-1.6f, -4.1f, -12.1f);
    glVertex3f(-1.6f, -2.5f, -12.1f);
    glVertex3f(-2.5f, -2.5f, -12.1f);

    glEnd();







    //WINDOWS RIGHT UP


    glBegin(GL_QUADS);

   glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(8.4f, -1.1f, -3.9f);
    glVertex3f(7.5f, -1.1f, -3.9f);
    glVertex3f(7.5f, 0.5f, -3.9f);
    glVertex3f(8.4f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(7.1f, -1.1f, -3.9f);
    glVertex3f(6.2f, -1.1f, -3.9f);
    glVertex3f(6.2f, 0.5f, -3.9f);
    glVertex3f(7.1f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(5.8f, -1.1f, -3.9f);
    glVertex3f(4.9f, -1.1f, -3.9f);
    glVertex3f(4.9f, 0.5f, -3.9f);
    glVertex3f(5.8f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(4.5f, -1.1f, -3.9f);
    glVertex3f(3.6f, -1.1f, -3.9f);
    glVertex3f(3.6f, 0.5f, -3.9f);
    glVertex3f(4.5f, 0.5f, -3.9f);

    glEnd();




    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(8.4f, -1.1f, -12.1f);
    glVertex3f(7.5f, -1.1f, -12.1f);
    glVertex3f(7.5f, 0.5f, -12.1f);
    glVertex3f(8.4f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(7.1f, -1.1f, -12.1f);
    glVertex3f(6.2f, -1.1f, -12.1f);
    glVertex3f(6.2f, 0.5f, -12.1f);
    glVertex3f(7.1f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(5.8f, -1.1f, -12.1f);
    glVertex3f(4.9f, -1.1f, -12.1f);
    glVertex3f(4.9f, 0.5f, -12.1f);
    glVertex3f(5.8f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(4.5f, -1.1f, -12.1f);
    glVertex3f(3.6f, -1.1f, -12.1f);
    glVertex3f(3.6f, 0.5f, -12.1f);
    glVertex3f(4.5f, 0.5f, -12.1f);

    glEnd();




    //WINDOWS RIGHT DOWN


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(8.4f, -4.1f, -3.9f);
    glVertex3f(7.5f, -4.1f, -3.9f);
    glVertex3f(7.5f, -2.5f, -3.9f);
    glVertex3f(8.4f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(8.4f, -4.1f, -12.1f);
    glVertex3f(7.5f, -4.1f, -12.1f);
    glVertex3f(7.5f, -2.5f, -12.1f);
    glVertex3f(8.4f, -2.5f, -12.1f);

    glEnd();


    //DESIGN

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(7.2f, -2.4f, -3.9f);
    glVertex3f(6.1f, -2.4f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(6.1f, -2.4f, -3.9f);
    glVertex3f(6.65f, -2.1f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(6.65f, -2.1f, -3.9f);
    glVertex3f(7.2f, -2.4f, -3.9f);
    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(7.1f, -4.1f, -3.9f);
    glVertex3f(6.2f, -4.1f, -3.9f);
    glVertex3f(6.2f, -2.5f, -3.9f);
    glVertex3f(7.1f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(5.8f, -4.1f, -3.9f);
    glVertex3f(4.9f, -4.1f, -3.9f);
    glVertex3f(4.9f, -2.5f, -3.9f);
    glVertex3f(5.8f, -2.5f, -3.9f);

    glEnd();




    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(7.1f, -4.1f, -12.1f);
    glVertex3f(6.2f, -4.1f, -12.1f);
    glVertex3f(6.2f, -2.5f, -12.1f);
    glVertex3f(7.1f, -2.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(5.8f, -4.1f, -12.1f);
    glVertex3f(4.9f, -4.1f, -12.1f);
    glVertex3f(4.9f, -2.5f, -12.1f);
    glVertex3f(5.8f, -2.5f, -12.1f);

    glEnd();



    //DESIGN

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(4.6f, -2.4f, -3.9f);
    glVertex3f(3.5f, -2.4f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(3.5f, -2.4f, -3.9f);
    glVertex3f(4.05f, -2.1f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(4.05f, -2.1f, -3.9f);
    glVertex3f(4.6f, -2.4f, -3.9f);
    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(4.5f, -4.1f, -3.9f);
    glVertex3f(3.6f, -4.1f, -3.9f);
    glVertex3f(3.6f, -2.5f, -3.9f);
    glVertex3f(4.5f, -2.5f, -3.9f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(4.5f, -4.1f, -12.1f);
    glVertex3f(3.6f, -4.1f, -12.1f);
    glVertex3f(3.6f, -2.5f, -12.1f);
    glVertex3f(4.5f, -2.5f, -12.1f);

    glEnd();



    //WINDOWS MIDDLE UP


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-0.6f, -1.1f, -3.9f);
    glVertex3f(0.1f, -1.1f, -3.9f);
    glVertex3f(0.1f, 0.5f, -3.9f);
    glVertex3f(-0.6f, 0.5f, -3.9f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(0.65f, -1.1f, -3.9f);
    glVertex3f(1.35f, -1.1f, -3.9f);
    glVertex3f(1.35f, 0.5f, -3.9f);
    glVertex3f(0.65f, 0.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(1.9f, -1.1f, -3.9f);
    glVertex3f(2.55f, -1.1f, -3.9f);
    glVertex3f(2.55f, 0.5f, -3.9f);
    glVertex3f(1.9f, 0.5f, -3.9f);

    glEnd();






    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-0.6f, -1.1f, -12.1f);
    glVertex3f(0.1f, -1.1f, -12.1f);
    glVertex3f(0.1f, 0.5f, -12.1f);
    glVertex3f(-0.6f, 0.5f, -12.1f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(0.65f, -1.1f, -12.1f);
    glVertex3f(1.35f, -1.1f, -12.1f);
    glVertex3f(1.35f, 0.5f, -12.1f);
    glVertex3f(0.65f, 0.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(1.9f, -1.1f, -12.1f);
    glVertex3f(2.55f, -1.1f, -12.1f);
    glVertex3f(2.55f, 0.5f, -12.1f);
    glVertex3f(1.9f, 0.5f, -12.1f);

    glEnd();


    //WINDOWS MIDDLE DOWM


    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-0.6f, -4.1f, -3.9f);
    glVertex3f(0.1f, -4.1f, -3.9f);
    glVertex3f(0.1f, -2.5f, -3.9f);
    glVertex3f(-0.6f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(-0.6f, -4.1f, -12.1f);
    glVertex3f(0.1f, -4.1f, -12.1f);
    glVertex3f(0.1f, -2.5f, -12.1f);
    glVertex3f(-0.6f, -2.5f, -12.1f);

    glEnd();


    //DESIGn

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0.55f, -2.4f, -3.9f);
    glVertex3f(1.45f, -2.4f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(1.45f, -2.4f, -3.9f);
    glVertex3f(1.0f, -2.1f, -3.9f);
    glEnd();

    glLineWidth(2.5);
    glColor3f(0.70f, 0.70f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(1.0f, -2.1f, -3.9f);
    glVertex3f(0.55f, -2.4f, -3.9f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(0.65f, -4.1f, -3.9f);
    glVertex3f(1.35f, -4.1f, -3.9f);
    glVertex3f(1.35f, -2.5f, -3.9f);
    glVertex3f(0.65f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(1.9f, -4.1f, -3.9f);
    glVertex3f(2.55f, -4.1f, -3.9f);
    glVertex3f(2.55f, -2.5f, -3.9f);
    glVertex3f(1.9f, -2.5f, -3.9f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(0.65f, -4.1f, -12.1f);
    glVertex3f(1.35f, -4.1f, -12.1f);
    glVertex3f(1.35f, -2.5f, -12.1f);
    glVertex3f(0.65f, -2.5f, -12.1f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 1.0f);
    glVertex3f(1.9f, -4.1f, -12.1f);
    glVertex3f(2.55f, -4.1f, -12.1f);
    glVertex3f(2.55f, -2.5f, -12.1f);
    glVertex3f(1.9f, -2.5f, -12.1f);

    glEnd();


    //WINDOW DESIGN



    //Front Big QUAD

    glBegin(GL_QUADS);

    glColor3f(0.98f, 0.98f, 0.98f);
    glVertex3f(-7.0f, -5.0f, -4.0f);
    glVertex3f(9.0f, -5.0f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);
    glVertex3f(-7.0f, 1.5f, -4.0f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.98f, 0.98f, 0.98f);
    glVertex3f(-7.0f, -5.0f, -4.0f);
    glVertex3f(-7.0f, -5.0f, -12.0f);
    glVertex3f(-7.0f, 1.5f, -12.0f);
    glVertex3f(-7.0f, 1.5f, -4.0f);


    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.98f, 0.98f, 0.98f);
    glVertex3f(9.0f, -5.0f, -4.0f);
    glVertex3f(9.0f, -5.0f, -12.0f);
    glVertex3f(9.0f, 1.5f, -12.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);


    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.98f, 0.98f, 0.98f);
    glVertex3f(-7.0f, -5.0f, -12.0f);
    glVertex3f(9.0f, -5.0f, -12.0f);
    glVertex3f(9.0f, 1.5f, -12.0f);
    glVertex3f(-7.0f, 1.5f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.98f, 0.98f, 0.98f);
    glVertex3f(-7.0f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.5f, -12.0f);
    glVertex3f(-7.0f, 1.5f, -12.0f);

    glEnd();



//reiling front

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -4.0f);
    glVertex3f(9.0f, 1.8f, -4.5f);
    glVertex3f(-7.0f, 1.8f, -4.5f);
    glVertex3f(-7.0f, 1.8f, -4.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.5f);
    glVertex3f(-6.5f, 1.8f, -4.5f);
    glVertex3f(-6.5f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -12.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.0f);
    glVertex3f(-7.0f, 1.7f, -4.0f);
    glVertex3f(9.0f, 1.7f, -4.0f);
    glVertex3f(9.0f, 1.8f, -4.0f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -11.5f);
    glVertex3f(9.0f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -11.5f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f);

    glVertex3f(-7.0f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -11.5f);
    glVertex3f(9.0f, 1.8f, -11.5f);
    glVertex3f(9.0f, 1.8f, -12.0f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);

    glVertex3f(-3.0f, 3.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.5f, 1.8f, -4.5f);
    glVertex3f(9.0f, 1.8f, -4.5f);
    glVertex3f(9.0f, 1.8f, -12.0f);
    glVertex3f(8.5f, 1.8f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.0f);
    glVertex3f(-7.0f, 1.7f, -4.0f);
    glVertex3f(9.0f, 1.7f, -4.0f);
    glVertex3f(9.0f, 1.8f, -4.0f);
    glEnd();

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.5f);
    glVertex3f(-7.0f, 1.7f, -4.5f);
    glVertex3f(9.0f, 1.7f, -4.5f);
    glVertex3f(9.0f, 1.8f, -4.5f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-6.5f, 1.8f, -4.0f);
    glVertex3f(-6.5f, 1.7f, -4.0f);
    glVertex3f(-6.5f, 1.7f, -12.0f);
    glVertex3f(-6.5f, 1.8f, -12.0f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);

    glVertex3f(-3.0f, 3.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -10.5f);

    glEnd();






    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.7f, -12.0f);
    glVertex3f(9.0f, 1.7f, -12.0f);
    glVertex3f(9.0f, 1.8f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -11.5f);
    glVertex3f(-7.0f, 1.7f, -11.5f);
    glVertex3f(9.0f, 1.7f, -11.5f);
    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);

    glVertex3f(-3.0f, 3.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -10.5f);

    glEnd();
    glVertex3f(9.0f, 1.8f, -11.5f);

    glEnd();




    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -4.0f);
    glVertex3f(9.0f, 1.7f, -4.0f);
    glVertex3f(9.0f, 1.7f, -12.0f);
    glVertex3f(9.0f, 1.8f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.5f, 1.8f, -4.0f);
    glVertex3f(8.5f, 1.7f, -4.0f);
    glVertex3f(8.5f, 1.7f, -12.0f);
    glVertex3f(8.5f, 1.8f, -12.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.0f);
    glVertex3f(-7.0f, 1.7f, -4.0f);
    glVertex3f(-7.0f, 1.7f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-6.5f, 1.8f, -4.0f);
    glVertex3f(-6.5f, 1.7f, -4.0f);
    glVertex3f(-6.5f, 1.7f, -12.0f);
    glVertex3f(-6.5f, 1.8f, -12.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.0f);
    glVertex3f(-7.0f, 1.5f, -4.0f);
    glVertex3f(-6.7f, 1.5f, -4.0f);
    glVertex3f(-6.7f, 1.8f, -4.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.3f);
    glVertex3f(-7.0f, 1.5f, -4.3f);
    glVertex3f(-6.7f, 1.5f, -4.3f);
    glVertex3f(-6.7f, 1.8f, -4.3f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.3f);
    glVertex3f(-7.0f, 1.5f, -4.3f);
    glVertex3f(-6.7f, 1.5f, -4.3f);
    glVertex3f(-6.7f, 1.8f, -4.3f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -11.7f);
    glVertex3f(-7.0f, 1.5f, -11.7f);
    glVertex3f(-6.7f, 1.5f, -11.7f);
    glVertex3f(-6.7f, 1.8f, -11.7f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -12.0f);
    glVertex3f(-7.0f, 1.5f, -12.0f);
    glVertex3f(-6.7f, 1.5f, -12.0f);
    glVertex3f(-6.7f, 1.8f, -12.0f);

    glEnd();











    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -4.0f);
    glVertex3f(8.7f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.8f, -4.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -4.3f);
    glVertex3f(8.7f, 1.5f, -4.3f);
    glVertex3f(9.0f, 1.5f, -4.3f);
    glVertex3f(9.0f, 1.8f, -4.3f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -11.7f);
    glVertex3f(8.7f, 1.5f, -11.7f);
    glVertex3f(9.0f, 1.5f, -11.7f);
    glVertex3f(9.0f, 1.8f, -11.7f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -12.0f);
    glVertex3f(8.7f, 1.5f, -12.0f);
    glVertex3f(9.0f, 1.5f, -12.0f);
    glVertex3f(9.0f, 1.8f, -12.0f);

    glEnd();











    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.3f);
    glVertex3f(9.0f, 1.8f, -4.3f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.0f);
    glVertex3f(9.0f, 1.5f, -4.3f);
    glVertex3f(9.0f, 1.8f, -4.3f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -4.0f);
    glVertex3f(8.7f, 1.5f, -4.0f);
    glVertex3f(8.7f, 1.5f, -4.3f);
    glVertex3f(8.7f, 1.8F, -4.3f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -4.0f);
    glVertex3f(-7.0f, 1.5f, -4.0f);
    glVertex3f(-7.0f, 1.5f, -4.3f);
    glVertex3f(-7.0f, 1.8f, -4.3f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-7.0f, 1.8f, -11.7f);
    glVertex3f(-7.0f, 1.5f, -11.7f);
    glVertex3f(-7.0f, 1.5f, -12.0f);
    glVertex3f(-7.0f, 1.8f, -12.0f);

    glEnd();







    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-6.7f, 1.8f, -4.0f);
    glVertex3f(-6.7f, 1.5f, -4.0f);
    glVertex3f(-6.7f, 1.5f, -4.3f);
    glVertex3f(-6.7f, 1.8f, -4.3f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(-6.7f, 1.8f, -11.7f);
    glVertex3f(-6.7f, 1.5f, -11.7f);
    glVertex3f(-6.7f, 1.5f, -12.0f);
    glVertex3f(-6.7f, 1.8f, -12.0f);

    glEnd();









    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(8.7f, 1.8f, -11.7f);
    glVertex3f(8.7f, 1.5f, -11.7f);
    glVertex3f(8.7f, 1.5f, -12.0f);
    glVertex3f(8.7f, 1.8f, -12.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.4f, 0.4f, 0.4f);
    glVertex3f(9.0f, 1.8f, -11.7f);
    glVertex3f(9.0f, 1.5f, -11.7f);
    glVertex3f(9.0f, 1.5f, -12.0f);
    glVertex3f(9.0f, 1.8f, -12.0f);

    glEnd();





// bottom roof

    glBegin(GL_QUADS);

    glColor3f(0.95f, 0.95f, 0.95f);
    glVertex3f(-5.0f, 2.0f, -5.0f);
    glVertex3f(-5.0f, 1.5f, -5.0f);
    glVertex3f(7.0f, 1.5f, -5.0f);
    glVertex3f(7.0f, 2.0f, -5.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.95f, 0.95f, 0.95f);
    glVertex3f(-5.0f, 2.0f, -11.0f);
    glVertex3f(-5.0f, 1.5f, -11.0f);
    glVertex3f(7.0f, 1.5f, -11.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.95f, 0.95f, 0.95f);
    glVertex3f(-5.0f, 2.0f, -5.0f);
    glVertex3f(-5.0f, 1.5f, -5.0f);
    glVertex3f(-5.0f, 1.5f, -11.0f);
    glVertex3f(-5.0f, 2.0f, -11.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.95f, 0.95f, 0.95f);
    glVertex3f(7.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 1.5f, -5.0f);
    glVertex3f(7.0f, 1.5f, -11.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.95f, 0.95f, 0.95f);
    glVertex3f(-5.0f, 2.0f, -11.0f);
    glVertex3f(-5.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);

    glEnd();


    // top roof

    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.9f, 0.9f);


    glVertex3f(-2.0f, 2.5f, -7.0f);
    glVertex3f(-2.0f, 2.0f, -7.0f);
    glVertex3f(4.0f, 2.0f, -7.0f);
    glVertex3f(4.0f, 2.5f, -7.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(-2.0f, 2.5f, -9.0f);
    glVertex3f(-2.0f, 2.0f, -9.0f);
    glVertex3f(4.0f, 2.0f, -9.0f);
    glVertex3f(4.0f, 2.5f, -9.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(-2.0f, 2.5f, -7.0f);
    glVertex3f(-2.0f, 2.0f, -7.0f);
    glVertex3f(-2.0f, 2.0f, -9.0f);
    glVertex3f(-2.0f, 2.5f, -9.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(4.0f, 2.5f, -7.0f);
    glVertex3f(4.0f, 2.0f, -7.0f);
    glVertex3f(4.0f, 2.0f, -9.0f);
    glVertex3f(4.0f, 2.5f, -9.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.9f, 0.9f, 0.9f);

    glVertex3f(-2.0f, 2.5f, -9.0f);
    glVertex3f(-2.0f, 2.5f, -7.0f);
    glVertex3f(4.0f, 2.5f, -7.0f);
    glVertex3f(4.0f, 2.5f, -9.0f);

    glEnd();






    //roof

    glBegin(GL_QUADS);

    glColor3f(0.3f, 0.3f, 1.0f);

    glVertex3f(-5.0f, 2.0f, -11.0f);
    glVertex3f(-5.0f, 2.0f, -5.0f);
    glVertex3f(-2.0f, 2.5f, -7.0f);
    glVertex3f(-2.0f, 2.5f, -9.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.3f, 0.3f, 1.0f);

    glVertex3f(4.0f, 2.5f, -9.0f);
    glVertex3f(4.0f, 2.5f, -7.0f);
    glVertex3f(7.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.3f, 0.3f, 1.0f);

    glVertex3f(-2.0f, 2.5f, -7.0f);
    glVertex3f(-5.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 2.0f, -5.0f);
    glVertex3f(4.0f, 2.5f, -7.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.3f, 0.3f, 1.0f);

    glVertex3f(4.0f, 2.5f, -9.0f);
    glVertex3f(4.0f, 2.5f, -7.0f);
    glVertex3f(7.0f, 2.0f, -5.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.3f, 0.3f, 1.0f);

    glVertex3f(-2.0f, 2.5f, -9.0f);
    glVertex3f(-5.0f, 2.0f, -11.0f);
    glVertex3f(7.0f, 2.0f, -11.0f);
    glVertex3f(4.0f, 2.5f, -9.0f);

    glEnd();



    //roof pillars


    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -5.5f);
    glVertex3f(-4.0f, 2.0f, -5.5f);
    glVertex3f(-3.0f, 2.0f, -5.5f);
    glVertex3f(-3.0f, 3.0f, -5.5f);

    glEnd();


    glBegin(GL_QUADS);

   glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -6.5f);
    glVertex3f(-4.0f, 2.0f, -6.5f);
    glVertex3f(-3.0f, 2.0f, -6.5f);
    glVertex3f(-3.0f, 3.0f, -6.5f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -5.5f);
    glVertex3f(-4.0f, 2.0f, -5.5f);
    glVertex3f(-4.0f, 2.0f, -6.5f);
    glVertex3f(-4.0f, 3.0f, -6.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-3.0f, 3.0f, -5.5f);
    glVertex3f(-3.0f, 2.0f, -5.5f);
    glVertex3f(-3.0f, 2.0f, -6.5f);
    glVertex3f(-3.0f, 3.0f, -6.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-3.0f, 3.0f, -5.5f);
    glVertex3f(-3.0f, 3.0f, -6.5f);
    glVertex3f(-4.0f, 3.0f, -6.5f);
    glVertex3f(-4.0f, 3.0f, -5.5f);

    glEnd();












    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -5.5f);
    glVertex3f(5.0f, 2.0f, -5.5f);
    glVertex3f(6.0f, 2.0f, -5.5f);
    glVertex3f(6.0f, 3.0f, -5.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -6.5f);
    glVertex3f(5.0f, 2.0f, -6.5f);
    glVertex3f(6.0f, 2.0f, -6.5f);
    glVertex3f(6.0f, 3.0f, -6.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -5.5f);
    glVertex3f(5.0f, 2.0f, -5.5f);
    glVertex3f(5.0f, 2.0f, -6.5f);
    glVertex3f(5.0f, 3.0f, -6.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(6.0f, 3.0f, -5.5f);
    glVertex3f(6.0f, 2.0f, -5.5f);
    glVertex3f(6.0f, 2.0f, -6.5f);
    glVertex3f(6.0f, 3.0f, -6.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -5.5f);
    glVertex3f(5.0f, 3.0f, -6.5f);
    glVertex3f(6.0f, 3.0f, -6.5f);
    glVertex3f(6.0f, 3.0f, -5.5f);

    glEnd();













    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 2.0f, -9.5f);
    glVertex3f(-3.0f, 2.0f, -9.5f);
    glVertex3f(-3.0f, 3.0f, -9.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -10.5f);
    glVertex3f(-4.0f, 2.0f, -10.5f);
    glVertex3f(-3.0f, 2.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -9.5f);
    glVertex3f(5.0f, 2.0f, -9.5f);
    glVertex3f(5.0f, 2.0f, -10.5f);
    glVertex3f(5.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(6.0f, 3.0f, -9.5f);
    glVertex3f(6.0f, 2.0f, -9.5f);
    glVertex3f(6.0f, 2.0f, -10.5f);
    glVertex3f(6.0f, 3.0f, -10.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -10.5f);
    glVertex3f(5.0f, 3.0f, -9.5f);
    glVertex3f(6.0f, 3.0f, -9.5f);
    glVertex3f(6.0f, 3.0f, -10.5f);

    glEnd();







    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -9.5f);
    glVertex3f(5.0f, 2.0f, -9.5f);
    glVertex3f(6.0f, 2.0f, -9.5f);
    glVertex3f(6.0f, 3.0f, -9.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(5.0f, 3.0f, -10.5f);
    glVertex3f(5.0f, 2.0f, -10.5f);
    glVertex3f(6.0f, 2.0f, -10.5f);
    glVertex3f(6.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 2.0f, -9.5f);
    glVertex3f(-4.0f, 2.0f, -10.5f);
    glVertex3f(-4.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-3.0f, 3.0f, -9.5f);
    glVertex3f(-3.0f, 2.0f, -9.5f);
    glVertex3f(-3.0f, 2.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -10.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.91f, 0.91f, 0.91f);

    glVertex3f(-3.0f, 3.0f, -10.5f);
    glVertex3f(-3.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -9.5f);
    glVertex3f(-4.0f, 3.0f, -10.5f);

    glEnd();

    //stairs bottom



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(-3.0f, -4.5f, 1.5f);
    glVertex3f(-3.0f, -5.0f, 1.5f);
    glVertex3f(5.0f, -5.0f, 1.5f);
    glVertex3f(5.0f, -4.5f, 1.5f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(-3.0f, -4.5f, -3.5f);
    glVertex3f(-3.0f, -4.5f, 1.5f);
    glVertex3f(5.0f, -4.5f, 1.5f);
    glVertex3f(5.0f, -4.5f, -3.5f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);
    glVertex3f(-3.0f, -4.5f, -3.5f);
    glVertex3f(-3.0f, -5.0f, -3.5f);
    glVertex3f(-3.0f, -5.0f, 1.5f);
    glVertex3f(-3.0f, -4.5f, 1.5f);

    glEnd();

    glBegin(GL_QUADS);


    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(5.0f, -4.5f, -3.5f);
    glVertex3f(5.0f, -5.0f, -3.5f);
    glVertex3f(5.0f, -5.0f, 1.5f);
    glVertex3f(5.0f, -4.5f, 1.5f);

    glEnd();


    //stairs back

    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(-3.0f, -4.5f, -14.5f);
    glVertex3f(-3.0f, -5.0f, -14.5f);
    glVertex3f(5.0f, -5.0f, -14.5f);
    glVertex3f(5.0f, -4.5f, -14.5f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(-3.0f, -4.5f, -12.0f);
    glVertex3f(-3.0f, -4.5f, -14.5f);
    glVertex3f(5.0f, -4.5f, -14.5f);
    glVertex3f(5.0f, -4.5f, -12.0f);

    glEnd();



    glBegin(GL_QUADS);

    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(-3.0f, -4.5f, -14.5f);
    glVertex3f(-3.0f, -5.0f, -14.5f);
    glVertex3f(-3.0f, -5.0f, -12.0f);
    glVertex3f(-3.0f, -4.5f, -12.0f);

    glEnd();

    glBegin(GL_QUADS);


    glColor3f(0.70f, 0.70f, 0.70f);

    glVertex3f(5.0f, -4.5f, -14.5f);
    glVertex3f(5.0f, -5.0f, -14.5f);
    glVertex3f(5.0f, -5.0f, -12.0f);
    glVertex3f(5.0f, -4.5f, -12.0f);

    glEnd();




    //stairs bottom 2

    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, 0.5f);
    glVertex3f(-2.0f, -4.5f, 0.5f);
    glVertex3f(4.5f, -4.5f, 0.5f);
    glVertex3f(4.5f, -4.0f, 0.5f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, -3.5f);
    glVertex3f(-2.0f, -4.0f, 0.5f);
    glVertex3f(4.5f, -4.0f, 0.5f);
    glVertex3f(4.5f, -4.0f, -3.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, -3.5f);
    glVertex3f(-2.0f, -4.5f, -3.5f);
    glVertex3f(-2.0f, -4.5f, 0.5f);
    glVertex3f(-2.0f, -4.0f, 0.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(4.5f, -4.0f, -3.5f);
    glVertex3f(4.5f, -4.5f, -3.5f);
    glVertex3f(4.5f, -4.5f, 0.5f);
    glVertex3f(4.5f, -4.0f, 0.5f);

    glEnd();

    //stairs back

    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, -14.0f);
    glVertex3f(-2.0f, -4.5f, -14.0f);
    glVertex3f(4.5f, -4.5f, -14.0f);
    glVertex3f(4.5f, -4.0f, -14.0f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, -12.0f);
    glVertex3f(-2.0f, -4.0f, -14.0f);
    glVertex3f(4.5f, -4.0f, -14.0f);
    glVertex3f(4.5f, -4.0f, -12.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(-2.0f, -4.0f, -12.0f);
    glVertex3f(-2.0f, -4.5f, -12.0f);
    glVertex3f(-2.0f, -4.5f, -14.0f);
    glVertex3f(-2.0f, -4.0f, -14.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.65f, 0.65f, 0.65f);

    glVertex3f(4.5f, -4.0f, -14.0f);
    glVertex3f(4.5f, -4.5f, -14.0f);
    glVertex3f(4.5f, -4.5f, -12.0f);
    glVertex3f(4.5f, -4.0f, -12.0f);

    glEnd();







    //stairs bottom 3


    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f, -1.0f);
    glVertex3f(-1.0f, -4.0f, -1.0f);
    glVertex3f(3.0f, -4.0f, -1.0f);
    glVertex3f(3.0f, -3.5f, -1.0f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f,-3.5f);
    glVertex3f(-1.0f, -3.5f,-1.0f);
    glVertex3f(3.0f, -3.5f, -1.0f);
    glVertex3f(3.0f, -3.5f, -3.5f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f,-3.5f);
    glVertex3f(-1.0f, -4.0f,-3.0f);
    glVertex3f(-1.0f, -4.0f, -1.0f);
    glVertex3f(-1.0f, -3.5f, -1.0f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(3.0f, -3.5f, -3.5f);
    glVertex3f(3.0f, -4.0f, -3.5f);
    glVertex3f(3.0f, -4.0f, -1.0f);
    glVertex3f(3.0f, -3.5f, -1.0f);

    glEnd();







    //stairs back


    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f, -13.5f);
    glVertex3f(-1.0f, -4.0f, -13.5f);
    glVertex3f(3.0f, -4.0f, -13.5f);
    glVertex3f(3.0f, -3.5f, -13.5f);

    glEnd();
    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f,-12.0f);
    glVertex3f(-1.0f, -3.5f,-13.5f);
    glVertex3f(3.0f, -3.5f, -13.5f);
    glVertex3f(3.0f, -3.5f, -12.0f);

    glEnd();


    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(-1.0f, -3.5f,-12.0f);
    glVertex3f(-1.0f, -4.0f,-12.0f);
    glVertex3f(-1.0f, -4.0f, -13.5f);
    glVertex3f(-1.0f, -3.5f, -13.5f);

    glEnd();

    glBegin(GL_QUADS);

    glColor3f(0.60f, 0.60f, 0.60f);

    glVertex3f(3.0f, -3.5f, -13.5f);
    glVertex3f(3.0f, -4.0f, -13.5f);
    glVertex3f(3.0f, -4.0f, -12.0f);
    glVertex3f(3.0f, -3.5f, -12.0f);

    glEnd();







    //PILLARS 1

    glTranslatef(-0.8f, 1.0f, -1.2f);

    quadratic = gluNewQuadric();



    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);


//ROW 0
    glColor3f(0.40f, 0.40f, 0.40f);

    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);


    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);

    //ROW 1
    glTranslatef(1.2f, 0.0f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);

    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);


    //ROW2


    glTranslatef(1.2f, 0.0f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);

    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, -0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);


    //ROW3

    glTranslatef(1.2f, 0.0f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);

    //ROW 2
    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE X


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);



    glTranslatef(0.0f, 0.8f, 0.0f);          //CHANGE Y


    gluCylinder(quadratic,0.15f,0.15f,5.0f,32,32);


    //PILLARS












    /*
        glColor3f(1.0f, 0.0f, 0.0f);


    	glVertex3f(-3.4f, -1.3f, -2.0f);
    	glVertex3f(3.4f, -1.3f, -2.0f);
    	glVertex3f(3.4f, -1.2f, -2.0f);
    	glVertex3f(-3.4f, -1.2f, -2.0f);

      */





}

void renderBitmapString(
    float x,
    float y,
    float z,
    void *font,
    char *string)
{

    char *c;
    glRasterPos3f(x, y,z);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

void restorePerspectiveProjection()
{

    glMatrixMode(GL_PROJECTION);
    // restore previous projection matrix
    glPopMatrix();

    // get back to modelview mode
    glMatrixMode(GL_MODELVIEW);
}

void setOrthographicProjection()
{

    // switch to projection mode
    glMatrixMode(GL_PROJECTION);

    // save previous matrix which contains the
    //settings for the perspective projection
    glPushMatrix();

    // reset matrix
    glLoadIdentity();

    // set a 2D orthographic projection
    gluOrtho2D(0, w, h, 0);

    // switch back to modelview mode
    glMatrixMode(GL_MODELVIEW);
}

void computePos(float deltaMove)
{

    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

// Common Render Items for all subwindows
void renderScene2()
{

// Draw ground

    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, -10.0f, -100.0f);
    glVertex3f(-100.0f, -10.0f,  100.0f);
    glVertex3f( 100.0f, -10.0f,  100.0f);
    glVertex3f( 100.0f, -10.0f, -100.0f);
    glEnd();

// Draw 36 SnowMen

    glPushMatrix();
    // glTranslatef(i*10.0f, 0.0f, j * 10.0f);
    drawSnowMan();
    glPopMatrix();

}

// Display func for main window
void renderScene()
{
    glutSetWindow(mainWindow);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

// Display func for sub window 1
void renderScenesw1()
{

    glutSetWindow(subWindow1);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(x, y, z,
              x + lx,y + ly,z + lz,
              0.0f,1.0f,0.0f);

    renderScene2();

    // display fps in the top window
    frame++;

    ttt=glutGet(GLUT_ELAPSED_TIME);
    if (ttt - timebase > 1000)
    {
        sprintf(s,"The White House - FPS:%4.2f",
                frame*1000.0/(ttt-timebase));
        timebase = ttt;
        frame = 0;
    }

    setOrthographicProjection();

    glPushMatrix();
    glLoadIdentity();
    renderBitmapString(5,30,0,GLUT_BITMAP_HELVETICA_12,s);
    glPopMatrix();

    restorePerspectiveProjection();

    glutSwapBuffers();
}

// Display func for sub window 2
void renderScenesw2()
{

    glutSetWindow(subWindow2);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(x, y+15, z,
              x,y - 1,z,
              lx,0,lz);

    // Draw red cone at the location of the main camera
    glPushMatrix();
    glColor3f(1.0,0.0,0.0);
    glTranslatef(x,y,z);
    glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
    glutSolidCone(0.2,0.8f,4,4);
    glPopMatrix();

    renderScene2();

    glutSwapBuffers();
}

// Display func for sub window 3
void renderScenesw3()
{

    glutSetWindow(subWindow3);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(x-lz*10, y, z+lx*10,
              x,y,z,
              0.0f,1.0f,0.0f);

    // Draw red cone at the location of the main camera
    glPushMatrix();
    glColor3f(1.0,0.0,0.0);
    glTranslatef(x,y,z);
    glRotatef(180-(angle+deltaAngle)*180.0/3.14,0.0,1.0,0.0);
    glutSolidCone(0.2,0.8f,4,4);
    glPopMatrix();

    renderScene2();

    glutSwapBuffers();
}

// Global render func
void renderSceneAll()
{

    // check for keyboard movement
    if (deltaMove)
    {
        computePos(deltaMove);
        glutSetWindow(mainWindow);
        glutPostRedisplay();
    }

    renderScene();
    renderScenesw1();
    renderScenesw2();
    renderScenesw3();
}

// -----------------------------------
//             KEYBOARD
// -----------------------------------

void processNormalKeys(unsigned char key, int xx, int yy)
{

    if (key == 27)
    {
        glutDestroyWindow(mainWindow);
        exit(0);
    }
    switch (key)
    {
    case 'a' :
    {
        deltaAngle += -0.05f;

        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
        glutSetWindow(mainWindow);
        glutPostRedisplay();
        break;
    }
    case 'd':
    {
        deltaAngle += 0.05f;

        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
        glutSetWindow(mainWindow);
        glutPostRedisplay();
        break;
    }
    case 'w' :
        deltaMove = 0.5f;
        break;
    case 's' :
        deltaMove = -0.5f;
        break;

    }
    glutSetWindow(mainWindow);
    glutPostRedisplay();
}

void pressKey(int key, int xx, int yy)
{

    switch (key)
    {
    case GLUT_KEY_LEFT :
    {
        deltaAngle += -0.05f;

        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
        glutSetWindow(mainWindow);
        glutPostRedisplay();
        break;
    }
    case GLUT_KEY_RIGHT:
    {
        deltaAngle += 0.05f;

        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);
        glutSetWindow(mainWindow);
        glutPostRedisplay();
        break;
    }
    case GLUT_KEY_UP :
        deltaMove = 0.5f;
        break;
    case GLUT_KEY_DOWN :
        deltaMove = -0.5f;
        break;

    }

    glutSetWindow(mainWindow);
    glutPostRedisplay();

}

void releaseKey(int key, int x, int y)
{

    switch (key)
    {
    // case GLUT_KEY_LEFT :
    //case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
    case GLUT_KEY_UP :
    case GLUT_KEY_DOWN :
        deltaMove = 0;
        break;


    }
    lx = sin(angle + deltaAngle);
    lz = -cos(angle + deltaAngle);
}

// -----------------------------------
//             MOUSE
// -----------------------------------

void mouseMove(int x, int y)
{

    // this will only be true when the left button is down
    if (xOrigin >= 0)
    {

        // update deltaAngle
        deltaAngle = (x - xOrigin) * 0.001f;

        // update camera's direction
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);

        glutSetWindow(mainWindow);
        glutPostRedisplay();
    }
}

void mouseButton(int button, int state, int x, int y)
{

    // only start motion if the left button is pressed
    if (button == GLUT_LEFT_BUTTON)
    {

        // when the button is released
        if (state == GLUT_UP)
        {
            angle += deltaAngle;
            deltaAngle = 0.0f;
            xOrigin = -1;
        }
        else   // state = GLUT_DOWN
        {
            xOrigin = x;

        }
    }
}

// -----------------------------------
//             MAIN and INIT
// -----------------------------------

void init()
{

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // register callbacks
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(processNormalKeys);
    glutSpecialFunc(pressKey);
    glutSpecialUpFunc(releaseKey);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
}

int main(int argc, char **argv)
{

    // init GLUT and create main window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(1200,600);
    mainWindow = glutCreateWindow("The White House");

    // callbacks for main window
    glutDisplayFunc(renderSceneAll);
    glutReshapeFunc(changeSize);

    // Removing the idle function to save CPU and GPU
    //glutIdleFunc(renderSceneAll);
    init();

    // sub windows
    subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h/2 - border*3/2);
    glutDisplayFunc(renderScenesw1);
    init();

    subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
    glutDisplayFunc(renderScenesw2);
    init();

    subWindow3 = glutCreateSubWindow(mainWindow, (w+border)/2,(h+border)/2,w/2-border*3/2,h/2 - border*3/2);
    glutDisplayFunc(renderScenesw3);
    init();

    // enter GLUT event processing cycle
    glutMainLoop();

    return 1;
}
