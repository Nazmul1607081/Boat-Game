#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include<bits/stdc++.h>
#include <iostream>
#include<string>

#include "BmpLoader.h"

using namespace std;

double Txval=0,Tyval=0,Tzval=0;
double windowHeight=1000, windowWidth=1000;
GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0;
GLboolean bRotate = false, uRotate = false;
unsigned int ID;

double eyeX=0,eyeY=1;
double dx=0,dy=0;

bool isJump=false;

double boatX=0,boatZ=0;

double obj1Scale = 1;

double speed =4;

double length = 200;

bool isStart = false;

int final_point=0;

float wcsClkDn[3],wcsClkUp[3];

void doMaterial(float a,float b,float c);

static GLfloat v_box[8][3] =
{
    {-.5, 0.5,.5},
    {.5, 0.5,.5},
    {.5, -0.5,.5},
    {-.5, -0.5,.5},

    {-.5, 0.5,-.5},
    {.5, 0.5,-.5},
    {.5, -0.5,-.5},
    {-.5, -0.5,-.5}
};

static GLubyte quadIndices[6][4] =
{
    {0,1,2,3},
    {4,5,6,7},
    {0,4,7,3},
    {1,5,6,2},
    {3,2,6,7},
    {0,1,5,4}
};

static void getNormal3p
(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}
void  face(GLfloat A[],GLfloat B[],GLfloat C[],GLfloat D[])
{
    glBegin(GL_POLYGON);
    glVertex3fv(A);
    glTexCoord2f(1,1);
    glVertex3fv(B);
    glTexCoord2f(1,0);
    glVertex3fv(C);
    glTexCoord2f(0,0);
    glVertex3fv(D);
    glTexCoord2f(0,1);
    glEnd();
}

GLfloat V[8][3]=
{
    {-.5, 0.5,.5},
    {1.5, 1.5,1.5},
    {.5, -0.5,.5},
    {-.5, -0.5,.5},

    {-.5, 0.5,-.5},
    {.5, 0.5,-.5},
    {.5, -0.5,-.5},
    {-.5, -0.5,-.5}

};
void drawPyramid()
{
    face(V[0],V[1],V[2],V[3]);
    ///back
    doMaterial(0.51, 0.42, 0.36);
    face(V[4],V[5],V[6],V[7]);
    ///left
    doMaterial(0.40, 0.34, 0.24);

    face(V[0],V[4],V[7],V[3]);
    ///right
    face(V[1],V[5],V[6],V[2]);
    ///bottom
    doMaterial(1, 1, 1);
    face(V[3],V[2],V[6],V[7]);
    /// top
    doMaterial(0.60, 0.55, 0.49);
    face(V[0],V[1],V[5],V[4]);
}


void drawBox(double a=1,double b=1, double c=1)
{
    doMaterial(a,b,c);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor3f(colors[4][0],colors[4][1],colors[4][2]);
        getNormal3p(v_box[quadIndices[i][0]][0], v_box[quadIndices[i][0]][1], v_box[quadIndices[i][0]][2],
                    v_box[quadIndices[i][1]][0], v_box[quadIndices[i][1]][1], v_box[quadIndices[i][1]][2],
                    v_box[quadIndices[i][2]][0], v_box[quadIndices[i][2]][1], v_box[quadIndices[i][2]][2]);

        glVertex3fv(&v_box[quadIndices[i][0]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_box[quadIndices[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_box[quadIndices[i][2]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_box[quadIndices[i][3]][0]);
        glTexCoord2f(0,1);
    }
    glEnd();
    //glutSolidSphere (3.0, 20, 16);

}

float cut =25;

bool light1 = true;
bool light2 = true;
bool light3 = true;

bool ambient = true;
bool specular  = true;
bool diffuse = true;

GLenum li[] = {GL_LIGHT0,GL_LIGHT1,GL_LIGHT2};
double sunY=0,lightingSun=-1;

void lightProperty(int no)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat light_position[] = { 3, 1, 0, 1.0 };
    GLfloat light_position1[] = {-3, 1, 0, 1.0 };
    GLfloat light_position2[] = { 0.0, 0.0, 0.0, 1.0 };
    if(ambient)
    {
        glLightfv( li[no], GL_AMBIENT, light_ambient);
    }
    else
    {
        glLightfv( li[no], GL_AMBIENT, no_light);
    }
    if(diffuse)
    {
        glLightfv( li[no], GL_DIFFUSE, light_diffuse);
    }
    else
    {
        glLightfv( li[no], GL_DIFFUSE, no_light);
    }
    if(specular)
    {
        glLightfv( li[no], GL_SPECULAR, light_specular);
    }
    else
    {
        glLightfv( li[no], GL_SPECULAR, no_light);
    }
}

void light()
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

    GLfloat light_position[] = { 3, 1, 0, 1.0 };
    GLfloat light_position1[] = {3,5+sunY,-16-dx, 0.0 };
    GLfloat light_position2[] = {3,5+sunY,-16-dx, 0.0 };
    GLfloat light_position3[] = {.5,2,.8-dx, 1.0 };


    if(light1)
    {
        glEnable(GL_LIGHT0);
        lightProperty(0);
        glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    }
    if(!light1)
    {
        glEnable( GL_LIGHT0);
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
        glLightfv( GL_LIGHT0, GL_POSITION, light_position);
    }


    if(light2)
    {
        glEnable( GL_LIGHT1);
        lightProperty(1);
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1);
    }
    if(!light2)
    {
        glEnable(GL_LIGHT1);
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
        glLightfv( GL_LIGHT1, GL_POSITION, light_position1);
    }

    if(light3)
    {
        glEnable( GL_LIGHT2);
        lightProperty(2);
        glLightfv( GL_LIGHT2, GL_POSITION, light_position3);
        GLfloat spot_direction[] = { 0.0, -0.5, -1 };
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
        glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, cut);
    }
    if(!light3)
    {
        glEnable( GL_LIGHT2);
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
        glLightfv( GL_LIGHT2, GL_POSITION, light_position2);
        GLfloat spot_direction[] = { -1.0, 0.0, 0.0 };
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
        glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 25.0);
    }
}

float shininess =60;
void doMaterial(GLfloat x,GLfloat y, GLfloat z)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { x/2, y/2, z/2, 1.0 };
    GLfloat mat_diffuse[] = { x, y, z, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {shininess};


    glMaterialfv( GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv( GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv( GL_FRONT, GL_SHININESS, mat_shininess);
}
/// my drawings


void obj1()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glScalef(obj1Scale,1,1);

    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void sky()
{

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glTranslatef(0,0,-dx);
    glScalef(40,40,40);
    glTranslatef(0,0,0);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void sea()
{
    glPushMatrix();
    doMaterial(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glScalef(20,2,10);
    glTranslatef(0,-2,0);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void seaBank()
{
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(-7,1,-(i*10));
        glScalef(4,2,10);
        glTranslatef(-1,-2,0);
        drawBox(0,.2,0);
        glPopMatrix();
    }

    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(7,1,-(i*10));
        glScalef(4,2,10);
        glTranslatef(1,-2,0);
        drawBox(0,.2,0);
        glPopMatrix();
    }
}

void boat2()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glRotatef(45,0,1,0);
    drawPyramid();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void curve();
void sail()
{

    /*glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glRotatef(45,0,1,0);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();*/

    //doMaterial(.4,0,0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glRotatef(15,0,0,1);
    glRotatef(90,0,1,0);
    glRotatef(-90,1,0,0);
    curve();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}

void lampStand()
{
    glPushMatrix();
    glScalef(.1,2,.1);
    drawBox();
    glPopMatrix();

    glPushMatrix();
    glScalef(.4,.4,.4);
    glTranslatef(0,2,0);
    drawBox();
    glPopMatrix();
}

void oar()
{

    glPushMatrix();
    glRotatef(25,-1,0,0);
    glTranslatef(-3,-.8,.9);
    glScalef(.3,1.5,.1);
    drawBox();
    glPopMatrix();

    glPushMatrix();
    glRotatef(25,-1,0,0);
    glTranslatef(-3,.8,.9);
    glScalef(.1,2.3,.05);
    drawBox();
    glPopMatrix();
}

void boat()
{
    glPushMatrix();
    glTranslatef(boatX,0,boatZ);
    glTranslatef(0,-2.5,-dx);
    glRotatef(90,0,1,0);

    ///lamp stand
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(.8,2,.5);
    lampStand();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///oar
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(.5,0,-.2);
    oar();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///sail
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(0,.5,2);
    glScalef(.2,.4,.5);
    sail();
    glPopMatrix();


    ///boat body
    /*glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glTranslatef(0,1,0);
    glScalef(.2,1,.2);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();*/




    ///boat middle
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glScalef(3,1.2,1.5);
    glTranslatef(0,0,0);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// boat - front side
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(1.5,0,0);
    boat2();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// boat back side
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(-1.5,0,0);
    glRotatef(180,0,1,0);
    boat2();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
}


void extra_boat()
{
    glPushMatrix();
    glTranslatef(-4,0,boatZ);
    glTranslatef(0,-2.5,-3);
    glRotatef(90,0,1,0);

    ///lamp stand
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(.8,2,.5);
    lampStand();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///oar
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(.5,0,-.2);
    oar();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    ///sail
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(0,.5,2);
    glScalef(.2,.4,.5);
    sail();
    glPopMatrix();


    ///boat body
    /*glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glTranslatef(0,1,0);
    glScalef(.2,1,.2);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();*/




    ///boat middle
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glScalef(3,1.2,1.5);
    glTranslatef(0,0,0);
    drawBox();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// boat - front side
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(1.5,0,0);
    boat2();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    /// boat back side
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,4);
    glTranslatef(-1.5,0,0);
    glRotatef(180,0,1,0);
    boat2();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glPopMatrix();
}
double sharkX=0,sharkZ=0;
void shark()
{
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glTranslatef(sharkX,-6,sharkZ-7);
    glScalef(2,2,2);
    glRotatef(90,0,0,1);
    glRotatef(-45,0,0,1);
    drawPyramid();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

double ulkaPoton=4;
double ulkaRotation=0;
void ulkaCurve();

void ulka()
{
    glPushMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glTranslatef(0,ulkaPoton,-6);
    doMaterial(1,0,0);
    glRotatef(ulkaRotation,1,0,0);
    glScalef(.1,.1,.1);
    ulkaCurve();
    //glutSolidSphere (.3, 20, 16);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

double cloudX=0,cloudZ=0;
void cloudCurve();
void cloud()
{
    glPushMatrix();
    glTranslatef(cloudX+.5,2.6,cloudZ);
    doMaterial(1,1,1);
    glScalef(.3,.2,.2);
    cloudCurve();
    glPopMatrix();
}
/*
void cloud()
{
    glPushMatrix();
    glTranslatef(cloudX+.5,2.6,cloudZ);
    doMaterial(1,1,1);
    glScalef(1.5,1.5,1.5);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloudX,2.6,cloudZ);
    doMaterial(1,1,1);
    glScalef(2,2,2);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloudX-.5,2.6,cloudZ);
    doMaterial(1,1,1);
    glScalef(1.5,1.5,1.5);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();
}*/
void sunHand()
{
    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(.1,6,.1);
    drawBox(.9,.7,.2);
    glPopMatrix();
}

void sunRay()
{
    glPushMatrix();
    glTranslatef(3,5,-18-dx);
    for(int i=0; i<5; i++)
    {
        glPushMatrix();
        glRotatef(45*i,0,0,1);
        sunHand();
        glPopMatrix();
    }

    for(int i=0; i<5; i++)
    {
        glPushMatrix();
        glRotatef(45*i+22.5,0,0,1);
        glScalef(.7,.7,.7);
        sunHand();
        glPopMatrix();
    }

    glPopMatrix();
}


void sun()
{
    doMaterial(.9,.7,.2);
    glPushMatrix();
    glTranslatef(3,5,-18-dx);
    glScalef(3,3,3);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();

    sunRay();



}

void treeLeaf()
{
    /*glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(1.5,1.5,1.5);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();*/

    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(2,2,2);
    //glTranslatef(0,.3,0);
    //glScalef(.1,.1,.1);
    //ulkaCurve();
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();

    /*glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(1.5,1.5,1.5);
    glutSolidSphere (.3, 20, 16);
    glPopMatrix();*/
}

void treeHand()
{
    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(.1,1,.1);
    drawBox(0,0,0);
    glPopMatrix();

    glPushMatrix();
    doMaterial(0,1,0);
    glScalef(.5,.5,.5);
    glTranslatef(0,.5,0);
    treeLeaf();
    glPopMatrix();

}

void treeRight()
{
    glPushMatrix();
    glTranslatef(.3,0,0);
    glScalef(1,2,1);
    glRotatef(-45,0,0,1);
    treeHand();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(.3,1,0);
    glScalef(1,2,1);
    glRotatef(-45,0,0,1);
    treeHand();
    glPopMatrix();
}

void tree()
{
    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(1,3,1);
    treeHand();
    glPopMatrix();

    treeRight();

    glPushMatrix();
    glTranslatef(0,-.5,0);
    glRotatef(180,0,1,0);
    treeRight();
    glPopMatrix();


}
///curve

const double PI = 3.14159265389;
int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=15;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;

double wd=3;



GLfloat ctrlpoints[L+1][3] =
{

    {1.4, 3.0, 0.0},
    {1.8, 3.0, 0.0},
    {2.2, 3.0, 0.0},
    {2.6, 3.0, 0.0},
    {3.0, 3.0, 0.0},
    {3.4, 3.0, 0.0},
    {3.8, 3.0, 0.0},
    {4.2, 3.0, 0.0},
    {4.6, 4.0, 0.0},
    {5.0, 3.0, 0.0},
    {5.4, 3.0, 0.0},
    {5.8, 3.0, 0.0},
    {6.2, 3.0, 0.0},
    {6.6, 3.0, 0.0},
    {7.2, 3.0, 0.0},
    {7.5, 3.0, 0.0}
};

GLfloat ulkaControlPoints[L+1][3] =
{
    {1.8, 0, 0.0},
    {2.2, 1.5, 0.0},
    {2.4, 3.3, 0.0},
    {2.6, 3.6, 0.0},
    {3.0, 3.9, 0.0},
    {3.4, 4.2, 0.0},
    {3.8, 4.5, 0.0},
    {4.2, 4.8, 0.0},
    {4.6, 5.3, 0.0},
    {5.0, 4.8, 0.0},
    {5.4, 4.7, 0.0},
    {5.8, 4.4, 0.0},
    {6.2, 3.3, 0.0},
    {6.6, 1.5, 0.0},
    {7.2, 1.2, 0.0},
    {7.5, 0, 0.0}
};

GLfloat cloudControlPoints[L+1][3] =
{
    {1.8, 0, 0.0},
    {2.2, .8, 0.0},
    {2.4, 1.2, 0.0},
    {2.6, 3.6, 0.0},
    {3.0, 1.2, 0.0},
    {3.4, 0.8, 0.0},
    {3.8, 1.2, 0.0},
    {4.2, 3.6, 0.0},
    {4.6, 5.3, 0.0},
    {5.0, 2.6, 0.0},
    {5.4, 0.8, 0.0},
    {5.8, 1.2, 0.0},
    {6.2, 3.6, 0.0},
    {6.6, 1.2, 0.0},
    {7.2, .8, 0.0},
    {7.5, 0, 0.0}
};
int isControlPointShowing = 0;
void showControlPoints(int type)
{
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    //glVertex2f(0,0);
    if(type==1)
    {
        for (int i = 0; i <=L; i++)
        {
            glColor3f(1.0, 0.0, 0.0);
            glVertex3fv(&ctrlpoints[i][0]);
        }
    }
    if(type==2)
    {
        for (int i = 0; i <=L; i++)
        {
            glColor3f(0.0, 1.0, 0.0);
            glVertex3fv(&ulkaControlPoints[i][0]);
        }
    }

    if(type==3)
    {
        for (int i = 0; i <=L; i++)
        {
            glColor3f(0.0, 0.0, 1.0);
            glVertex3fv(&cloudControlPoints[i][0]);
        }
    }

    glEnd();
}


long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void BezierCurve2 ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ulkaControlPoints[i][0];
        y+=coef*ulkaControlPoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}
void BezierCurve3 ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*cloudControlPoints[i][0];
        y+=coef*cloudControlPoints[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}

void curve()
{
    /*glPointSize(4);
    glColor3f(1,0,0);
    glBegin(GL_QUAD_STRIP);
    glVertex3f(0.2, 0.2,0.0);
    glVertex3f(0.8, 0.2,0.0);
    glVertex3f(0.2,0.4,0.0);
    glVertex3f(0.8,0.4,0.0);
    glVertex3f(0.4,0.8,0.0);
    glVertex3f(1,0.8,0.0);
    glEnd();*/

    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta/2; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    }
}

void ulkaCurve()
{


    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[L+1][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve2( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve2( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    }
}

void cloudCurve()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints[L+1][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve3( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve3( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    }
}

bool isInsidePoint = false;

void drawString(char *string, float x, float y,float z)
{
    glRasterPos3f(x, y,z);
    for (char* c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, *c);
    }
}

double zoom =1;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum(-5,5,-5,5, 4,5000);
    //cout<<boatX<<" "<<boatZ<<endl;
    //gluPerspective(60,1,5,100);
    //glOrtho(-5,5,-5,5, 4, 50);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,8-dx, 0,0,-dx, 0,1,0);
    glViewport(0, 0, windowHeight, windowWidth);


    glPushMatrix();
    glScalef(zoom,1,zoom);



    light();
    ///

    /// my drawings

    ///sky
    sky();

    ///sun
    glPushMatrix();
    glTranslatef(0,sunY,0);
    sun();
    glPopMatrix();

    ///tree
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(-10,-.5,-(i*8));
        glScalef(1,1,1);
        tree();
        glPopMatrix();
    }
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(10,-.5,-(i*8));
        glScalef(1,1,1);
        tree();
        glPopMatrix();
    }



    ///cloud
    glPushMatrix();
    glTranslatef(5,1,-dx);
    cloud();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5,1,-dx);
    cloud();
    glPopMatrix();

    ///ulka
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(0,0,-(i*40));
        ulka();
        glPopMatrix();

        glPushMatrix();
        glTranslated(2,0,-((i+1)*30));
        ulka();
        glPopMatrix();

        glPushMatrix();
        glTranslated(-2,0,-((i+1)*50));
        ulka();
        glPopMatrix();
    }

    ///sea
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslated(0,0,-(i*10));
        sea();
        glPopMatrix();
    }

    ///sea-bank
    seaBank();

    ///obj1-right
    for(int i=0; i<100; i++)
    {
        glPushMatrix();
        glTranslatef(4,-2.5,-20*i+15);
        glRotatef(-45,0,1,0);
        obj1();
        glPopMatrix();
    }

    ///obj2-left
    for(int i=0; i<100; i++)
    {
        glPushMatrix();
        glTranslatef(-3,-2.5,-30*i+20);
        glRotatef(-45,0,1,0);
        obj1();
        glPopMatrix();
    }

    ///obj3-middle
    for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslatef(1,-2.5,-40*i+10);
        glRotatef(-45,0,1,0);
        obj1();
        glPopMatrix();
    }

    ///shark
    /*for(int i=0; i<length; i++)
    {
        glPushMatrix();
        glTranslatef(0,0,-i*20);
        shark();
        glPopMatrix();
    }*/

    ///boat
    boat();




    ///extra-boat
    for(int i=0; i<10; i++)
    {
        /// add 4
        glPushMatrix();
        glTranslatef(-2.5,-1.5,-40*i+15);
        //glRotatef(-45,0,1,0);
        glScalef(.5,.5,.5);
        extra_boat();
        glPopMatrix();

        glPushMatrix();
        glTranslatef(2.5+4,-1.5,-40*i+8);
        //glRotatef(-45,0,1,0);
        glScalef(.5,.5,.5);
        extra_boat();
        glPopMatrix();
    }
    ///control point
    if(isControlPointShowing)
    {

        showControlPoints(isControlPointShowing);
    }
    ///flag
    //flag();


    //glDisable(GL_TEXTURE_2D);

    //cout<<boatX<<" "<<dx<<" "<<int(dx-10)%40<<endl;

    if(boatX>=3.5 && boatX<=4.2 && ((int(dx-3)%20)>-2 && (int(dx-3)%20)<2))
    {
        if(!isInsidePoint)
        {
            final_point++;
            isInsidePoint=true;
            cout<<"Point :"<<final_point<<endl;
        }

    }

    else if(boatX>=-3.5 && boatX<=-2.5 && int(dx-8)%30<2 && int(dx-8)%60>-2)
    {
        if(!isInsidePoint)
        {
            final_point++;
            isInsidePoint=true;
            cout<<"Point :"<<final_point<<endl;
        }


    }

    else if(boatX>=0.8 && boatX<=1.2 && ((int(dx-28)%40)>-2 && (int(dx-28)%40)<2))
    {
        if(!isInsidePoint)
        {
            final_point++;
            isInsidePoint=true;
            cout<<"Point :"<<final_point<<endl;
        }


    }
    else
    {
        isInsidePoint=false;
    }


    if(boatX>=-.7 && boatX<=0.3 && (int(dx-5)%40)>=-3 && (int(dx-5)%40)<=3 && ulkaPoton>=-1.5 && ulkaPoton<=.5)
    {
        //cout<<"ULKA"<<endl;
        cout<<"Final Point :"<<final_point<<endl;
        exit(1);
    }
    if(boatX>=1.7 && boatX<=2.3 && (int(dx-5)%30)>=-3 && (int(dx-5)%30)<=3 && ulkaPoton>=-1.5 && ulkaPoton<=.5)
    {
        //cout<<"ULKA"<<endl;
        cout<<"Final Point :"<<final_point<<endl;
        exit(1);
    }
    if(boatX>=-2.3 && boatX<=-1.7 && (int(dx-5)%50)>=-3 && (int(dx-5)%50)<=3 && ulkaPoton>=-1.5 && ulkaPoton<=.5)
    {
        //cout<<"ULKA"<<endl;
        cout<<"Final Point :"<<final_point<<endl;
        exit(1);
    }

    //cout<<int(dx+15)%80<<" "<<boatX<<endl;

    if(boatX>=-7 && boatX<=-5 && (int(dx+15)%40)>=-3 && (int(dx+15)%40)<=3 )
    {
        //cout<<"ULKA"<<endl;
        cout<<"Final Point :"<<final_point<<endl;
        exit(1);
    }
    if(boatX>=5 && boatX<=7 && (int(dx+8)%40)>=-3 && (int(dx+8)%40)<=3 )
    {
        //cout<<"ULKA"<<endl;
        cout<<"Final Point :"<<final_point<<endl;
        exit(1);
    }


    //cout<<"ulka poton = "<<ulkaPoton<<endl;

    glPushMatrix();
    glScalef(3,3,3);
    glColor3f(1,0,0);
    //drawString("Pirates_of_Bengal",1,1,0);
    glPopMatrix();





    glPopMatrix();

    glFlush();
    glutSwapBuffers();

}


void myKeyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 'j':
        eyeX-=.2;
        break;
    case 'l':
        eyeX+=.2;
        break;
    case 'k':
        eyeY-=.2;
        break;
    case 'i':
        eyeY+=.2;
        break;

    case 'a':
        if(boatX>=-7.5)
        {
            boatX-=.3;
        }
        break;
    case 'd':
        if(boatX<=7.5)
        {
            boatX+=.3;
        }
        break;
    case 'w':
        boatZ-=.3;
        dx-=.2;
        break;
    case 's':
        boatZ+=.3;
        dx+=.2;
        break;

    case '+':
        speed++;
        break;
    case '-':
        speed--;
        break;

    case '0':
        isStart=!isStart;
        break;

    case 'v':
        if(eyeX<=-2)
        {
            eyeX=0;
        }
        else
        {
            eyeX=-8;
        }
        break;

    case 'Z':
        zoom+=.05;
        break;
    case 'z':
        zoom-=.05;
        break;

    case '1':
        light1 = !light1;
        break;
    case '2':
        light2 = !light2;
        break;
    case '3':
        light3 = !light3;
        break;
    case 'A':
        cout<<" ambient"<<ambient<<" "<<"speculer "<<specular<<" "<<"Diffuse"<<" "<<diffuse<<endl;
        ambient =! ambient;
        break;
    case 'D':
        cout<<" ambient"<<ambient<<" "<<"speculer "<<specular<<" "<<"Diffuse"<<" "<<diffuse<<endl;
        diffuse  =! diffuse ;
        break;
    case 'S':
        cout<<" ambient"<<ambient<<" "<<"speculer "<<specular<<" "<<"Diffuse"<<" "<<diffuse<<endl;
        specular =! specular;
        break;

    case '4':
        isControlPointShowing=1;
        break;
    case '5':
        isControlPointShowing=2;
        break;
    case '6':
        isControlPointShowing=3;
        break;

    case 27:	// Escape key
        exit(1);
    }
}


void animate()
{
    if (bRotate == true)
    {
        theta += 0.2;
        if(theta > 360.0)
            theta -= 360.0*floor(theta/360.0);
    }

    if (uRotate == true)
    {
        alpha += 0.2;
        if(alpha > 360.0)
            alpha -= 360.0*floor(alpha/360.0);
    }

    if(isStart)
    {

        dx+=(speed*.02);
        speed+=.1;

    }
    sharkX+=.4;
    sharkZ+=.2;
    ulkaPoton-=.3;

    ulkaRotation+=10;




    if(sharkX>10)
    {
        sharkX=-10;
        sharkZ=-10;
    }


    if(boatX>=4 && dx >= 30)
    {
        obj1Scale=0;
    }


    if(ulkaPoton<=-10)
    {
        ulkaPoton=5;
    }

    cloudZ+=.2*speed/20;
    if(cloudZ>9)
    {
        cloudZ=-9;
    }

    //cloudZ-=dx;

    sunY+=.2*lightingSun*speed/20;
    if(sunY<-20 && lightingSun==-1)
    {
        lightingSun=1;
    }

    if(sunY>15 && lightingSun==1)
    {
        lightingSun=-1;
    }
    if(sunY<-3)
    {
        light1=false;
    }
    else
    {
        light1=true;
    }
    if(sunY<-10)
    {
        light2=false;
    }
    else
    {
        light2=true;
    }


    //shininess++;
    if(shininess>100)
    {
        shininess=0;
    }
    //cout<<shininess<<endl;

    if(!light1&&!light2)
    {
        light3=true;
    }
    else
    {
        light3=false;
    }






    glutPostRedisplay();

}

void LoadTexture(const char*filename,int id)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    // gluPerspective(60,ar, 2.0, 100.0);

    //glOrtho(-8.0, 8.0, -8.0*(GLfloat)height/(GLfloat)width, 8.0*(GLfloat)height/(GLfloat)width, 2.0, 25.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(eyeX,eyeY,8-dx, 0,0,-dx, 0,1,0);

}

int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(200,100);
    glutInitWindowSize(windowHeight, windowWidth);
    glutCreateWindow("Boat Game");

    cout<<"Instructions:"<<endl<<endl;
    cout<<"..Boat speed will increase automatically with respect to time."<<endl;
    cout<<"..The sun light is increasing and decreasing and show its normal behavior and change position with respect to time"<<endl;
    cout<<"..Avoid red meteor and other boats to save life."<<endl;
    cout<<"..Collect floating objects to increasing points and see points on console."<<endl;
    cout<<"press \"a\" to shift boat to the left."<<endl;
    cout<<"press \"d\" to shift boat to the right."<<endl;
    cout<<"press \"w\" to shift boat to the up."<<endl;
    cout<<"press \"s\" to shift boat to the down."<<endl;
    cout<<"press \"j\" to change view to the left."<<endl;
    cout<<"press \"l\" to change view to the right."<<endl;
    cout<<"press \"i\" to change view to the up."<<endl;
    cout<<"press \"k\" to change view to the down."<<endl;
    cout<<"press \"v\" to to change side/normal view."<<endl;
    cout<<"press \"A S D\" to to toggle ambient light property."<<endl;
    cout<<"press \"1 2 3\" on/off three light 3 for spot light of boat 1 and 2 for sun."<<endl;
    cout<<"press \"4 5 6\" to see control points of Bezier curve."<<endl;
    cout<<"press \"+ -\" to up/down the boat."<<endl;
    cout<<"press \"Z z\" to zoom in/out the."<<endl<<endl;

    cout<<"press \"0\" to start/stop... "<<endl<<endl;



    //string path = "C:\\Users\\Nazmul81\\Desktop";

    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\boat.bmp",1);
    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\sea.bmp",2);
    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\sky1.bmp",3);
    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\boat2.bmp",4);
    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\mateor.bmp",5);
    LoadTexture("F:\\4-2\\GDLAB\\FinalProject\\texture\\ice.bmp",6);
    //LoadTexture("C:\\Users\\Nazmul81\\Desktop\\shark.bmp",7);

    //LoadTexture("C:\\Users\\Asus\\Documents\\Graphics Lab\\Texture\\brick.bmp");
    glutReshapeFunc(resize);
    glutKeyboardFunc(myKeyboardFunc);
    glutDisplayFunc(display);
    glutIdleFunc(animate);

    //glClearColor(0.23, 0.49, 0.79,1);


    glShadeModel( GL_SMOOTH );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);




    glutMainLoop();

    return 0;
}

