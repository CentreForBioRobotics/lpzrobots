/*
 * LandscapeVisualisation.cpp
 *
 *  Created on: 20.01.2010
 *      Author: oni
 */

#include "LandscapeVisualisation.h"
#include "math.h"
#include <iostream>
#include <string>
#include <GL/glut.h>    // Header File For The GLUT Library


using namespace std;

LandscapeVisualisation::LandscapeVisualisation(MatrixPlotChannel *channel, ColorPalette *colorPalette, QWidget *parent)
: AbstractVisualisation(channel, colorPalette, parent){

  if(debug) cout << "TextureVisualisation Konstruktor" << endl;
//  this->channel = channel;
//  this->colorPalette = colorPalette;
  object = 0;
  maxX = this->matrixChannel->getDimension(0);
  maxY = this->matrixChannel->getDimension(1);
  rotX = rotY = 0;
  zoom = 1.;
  //setUpdatesEnabled(true);
  setMouseTracking(true); // enables tooltips while mousemoving over widget
}

LandscapeVisualisation::LandscapeVisualisation(VectorPlotChannel *channel, ColorPalette *colorPalette, QWidget *parent)
: AbstractVisualisation(channel, colorPalette, parent){

  if(debug) cout << "TextureVisualisation Konstruktor" << endl;
//  this->channel = channel;
//  this->colorPalette = colorPalette;
  object = 0;
//  maxX = this->matrixChannel->getDimension(0);
//  maxY = this->matrixChannel->getDimension(1);
  rotX = rotY = 0;
  zoom = 1.;
  //setUpdatesEnabled(true);
  setMouseTracking(true); // enables tooltips while mousemoving over widget
}

LandscapeVisualisation::~LandscapeVisualisation(){
  if(debug) cout << "LandscapeVisualisation Destruktor" << endl;
  makeCurrent();
//  glDeleteLists( object, 1 );
}

void LandscapeVisualisation::initializeGL(){
  if(debug) cout << "LandscapeVisualisation Konstruktor" << endl;
  qglClearColor( Qt::black);    // Let OpenGL clear to black
//  object = makeObject();    // Generate an OpenGL display list
  glEnable(GL_DEPTH_TEST);
  glShadeModel( GL_SMOOTH );
//  glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
}

void LandscapeVisualisation::resizeGL(int w, int h){
  if(debug) cout << "LandscapeVisualisation resizeGL" << endl;
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode( GL_PROJECTION);
  glLoadIdentity();

//  //seitenverhältnis
//  if(w <= h)
//    glOrtho(-1.0 * maxX, 1.0 * maxX, -1.0 * maxY, 1.0 * maxY, maxY, maxY);
//  else
//    glOrtho(-1.0 * maxX, 1.0 * maxX, -1.0 * maxY, 1.0 * maxY, maxY, maxY);

  gluPerspective(30.f,(GLfloat) w/(GLfloat) h, .2f,50.f);
//  glTranslatef(.5f * maxX, .5f * maxY, -.0f); //move to the center

//  glFrustum(-1.0 * maxX, 1.0 * maxX, -1.0 * maxY, 1.0 * maxY, maxY, maxY);
//  glFrustum(-50, 50, -50, 50, .2, 4);
  glMatrixMode( GL_MODELVIEW);
}

void LandscapeVisualisation::paintGL(){
  if(debug) cout << "LandscapeVisualisation PaintGL" << endl;
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();

  gluLookAt(
      5.0f /* cos(rotX)*/, 5.0f /* sin(rotX)*/, -5.0f, // x - 5.f * cos rotX , y - 5.f *Ssin rotwinkel
      0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f);

  glRotatef(rotX* -1.f, 1.f,0.f,1.f); // neigen.. rotation um die x,-z-achse
  glRotatef((GLfloat) rotY, 0.0f, 1.0f, 0.0f); // drehen y achse

  glTranslatef(((-.5f * maxX) + .5f)*zoom, 0.f, ((.5f * maxY) - .5f)*zoom); // put the object in center
//  glScalef(0.25f,0.25f,0.25f);
  glScalef( zoom, zoom,zoom);

  for (int i = 0; i < maxX - 1; i++)
    for (int j = 0; j < maxY - 1; j++) {
      double p00 = matrixChannel->getValue(i, j),
          p10 = matrixChannel->getValue(i + 1, j),
          p11 = matrixChannel->getValue(i + 1, j + 1),
          p01 = matrixChannel->getValue(i, j + 1);
      QColor cP00 = colorPalette->pickColor(p00),
          cP10 = colorPalette->pickColor(p10),
          cP11 = colorPalette->pickColor(p11),
          cP01 = colorPalette->pickColor(p01);

      //      glTranslatef(1.f, 1.f, 1.f);
      if (fabs((p01 + p11 + p10) / 3) > fabs((p00 + p01 + p10 + p11) / 4) || fabs((p00 + p01 + p10) / 3) > fabs((p00
          + p01 + p10 + p11) / 4)) {
        //triangle p01, p10, p00 / p01, p11, p10
        glBegin( GL_TRIANGLES); //kante zwischen p10 u p01
        qglColor(cP01); glVertex3f((GLfloat) i, (GLfloat) p01, (GLfloat) (-1. * (j + 1)));
        qglColor(cP00); glVertex3f((GLfloat) i, (GLfloat) p00, (GLfloat) (-1. * j));
        qglColor(cP10); glVertex3f((GLfloat) (i + 1), (GLfloat) p10, (GLfloat) (-1. * j));

        qglColor(cP01); glVertex3f((GLfloat) i, (GLfloat) p01, (GLfloat) (-1. * (j + 1)));
        qglColor(cP10); glVertex3f((GLfloat) (i + 1), (GLfloat) p10, (GLfloat) (-1. * j));
        qglColor(cP11); glVertex3f((GLfloat) (i + 1), (GLfloat) p11, (GLfloat) (-1. * (j + 1)));
        glEnd();
      } else {
        //triangle p00, p11, p10 / p00, p01, p11
        glBegin( GL_TRIANGLES); //kante zw p00 u p00
        qglColor(cP00); glVertex3f((GLfloat) i, (GLfloat) p00, (GLfloat) (-1. * j));
        qglColor(cP10); glVertex3f((GLfloat) i + 1,(GLfloat) p10, (GLfloat) (-1. * j));
        qglColor(cP11); glVertex3f((GLfloat) (i + 1), (GLfloat) p11, (GLfloat) (-1. * (j + 1)));

        qglColor(cP00); glVertex3f((GLfloat) i, (GLfloat) p00, (GLfloat) (-1. * j));
        qglColor(cP11); glVertex3f((GLfloat) (i + 1), (GLfloat) p11, (GLfloat) (-1. * (j + 1)));
        qglColor(cP01); glVertex3f((GLfloat) i, (GLfloat) p01, (GLfloat) (-1. * (j + 1)));
        glEnd();
      }
    }
}

void LandscapeVisualisation::divideAndDrawTriangle(VERTEX* v1, VERTEX* v2, VERTEX* v3){
  // y -> values TODO

}

GLuint LandscapeVisualisation::makeObject() { //obsolete
  if(debug) cout << "LandscapeVisualisation makeObject" << endl;
  GLuint list;

  list = glGenLists(1);

  glNewList(list, GL_COMPILE);

  //qglColor(Qt::white); // Shorthand for glColor3f or glIndex

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  //glBindTexture(GL_TEXTURE_2D, texName);
  glBegin( GL_QUADS); // Draw A Quadab
  glTexCoord2d(0.0,0.0);glVertex2f(-1.0f, 1.0f); // Top Left
  glTexCoord2d(0.0155*maxX,0.0);glVertex2f(1.0f, 1.0f); // Top Right
  glTexCoord2d(0.0155*maxX,0.0155*maxY);glVertex2f(1.0f, -1.0f); // Bottom Right
  glTexCoord2d(0.0,0.0155*maxY);glVertex2f(-1.0f, -1.0f); // Bottom Left
  glEnd(); // Done Drawing The Quad
  glFlush();
  glDisable(GL_TEXTURE_2D);
  glEndList();

  return list;
}


void LandscapeVisualisation::mouseMoveEvent ( QMouseEvent *event ){
//  QString tTip = channel->getChannelName().c_str();
//  double xStep = width() / channel->getDimension(0);
//  double yStep = height() / channel->getDimension(1);
//  tTip += "[" + QString::number( (int) (event->x() / xStep)) + ","
//       + QString::number( (int) (event->y() / yStep) ) + "]";
//  setToolTip((const QString) tTip);  // shows up ToolTip "M[x,y]"
  //if ( debug) cout << "MouseCoords: " << event->x() << ", " << event->y() << endl;
  if(event->buttons() == Qt::LeftButton && ( event->x() != mouseX || event->y() != mouseY)){
    rotX += (event->y() - mouseY) / 2; //variable umbenennen.. rot um z in xyebene
    rotY += (event->x() - mouseX)/2;
//    if(debug) cout << "rotX= " << rotX << " rotY= " << rotY << endl;
    mouseY = event->y();
    mouseX = event->x();
    updateGL();
  }
}

void LandscapeVisualisation::wheelEvent(QWheelEvent * event){
  if(debug) cout << event->delta() << endl;
  zoom += ((event->delta() / 120) * 0.1);
  if ( zoom < 0.) zoom = 0.;
  updateGL();
  event->accept();
}

void LandscapeVisualisation::mousePressEvent ( QMouseEvent *event ){
  mouseX = event->x();
  mouseY = event->y();
}
