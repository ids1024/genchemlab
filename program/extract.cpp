#include "extract.h"
#include "chem_dialog.h"

#include <qmessagebox.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GLfloat xglass[] = { 0.75f, 0.75f, 0.75f, 0.5f }; // Transparent material
GLfloat xbeakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat xglass2[] = { 0.75f, 0.75f, 0.75f, 0.5f };

// Constant colours and material factors.
const GLfloat xglfloor[] = { 0.2f, 0.2f, 1.0f, 1.0f };
const GLfloat xglblack[] = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat xglblue[] = { 0.2f, 0.2f, 1.0f, 1.0f };
const GLfloat xglwhite[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat xglred[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat xgldull[] = { 10.0 };
const GLfloat xglmidshine[] = { 50.0 };
const GLfloat xglpolished[] = { 90.0 };

// Light at origin.
const GLfloat xlight0_pos[] = { 5.0, 5.0, 0.0, 1.0 };
const GLfloat xlight1_pos[] = { -5.0, 5.0, -10.0, 1.0 };

Extract::Extract(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(Qt::ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cmode = 0; running = false; timeCount = 0;
  beakerVolume = 0.0; temperature = 0.85; runTime = 0.0; freezeTemp = 6.0;
  drawDrip = false; alreadydone = true;
  beakerRed = 1.0; beakerGreen = 0.6; beakerBlue = 0.0; beakerAlpha = 0.5;
  oilRed = 1.0; oilGreen = 1.0; oilBlue = 1.0; oilAlpha = 0.5;
  beakerHeight = 0.01; thermoHeight = 3.5;
  beakerConc = 0.0; fudgeFactor = 0.0;
  beakerSubstance = 0;
  bvolume = 2.0; pH = 0.0;
}

void Extract::process() {
  emit signalSetStatusBar( tr("Absorbance = %1").arg(temperature) );
  updateGL();
}

void Extract::targetTemperature() {

}

// figure everything out based on contents of beaker
void Extract::calculate() {

}

void Extract::addToBeaker(double m1, int u1, int sub1, bool aw, 
			      double vwater) {
  switch( QMessageBox::information( this, tr("Set up experiment"),
				    tr("Select amount of oil for extraction:"),
				    tr("25 mL"), tr("50 mL"), 
				    tr("100 mL") ) ) {
  case 0: 
    // 25 mL
    pH = 25.0;
    break;
  case 1: 
    // 50 mL
    pH = 50.0;
    break;
  case 2: 
    // 100 mL
    pH = 100.0;
    break;
  }

  beakerHeight = pH / 28.57;
  oilRed = 1.0; oilGreen = 1.0; oilBlue = 1.0; oilAlpha = 0.5;
  drawDrip = true;
  alreadydone = false;
  QMessageBox::information( this, tr("Ready to extract"),
			    tr("Click on the flask to mix and separate.") );
  updateGL();
}

void Extract::showInfo() {
}

void Extract::saveData() {
}

void Extract::mousePressEvent(QMouseEvent *e1) {
}

void Extract::mouseReleaseEvent(QMouseEvent *e1) {
  if (alreadydone == true) return;
  // animate!
  /*
  if (running) { running = false; return; }
  running = true;
  */
  // calculate partition and new abs
  double Ai = temperature, Af, Vh = 100.0, Keq = freezeTemp, Voil = pH, prm;
  Af = (Ai * Vh) / (Voil * Keq + Vh);
  prm = Af / temperature;
  beakerRed += ((1.0 - beakerRed) * prm); 
  beakerGreen += ((1.0 - beakerGreen) * prm); 
  beakerBlue += ((1.0 - beakerBlue) * prm); 
  oilRed = prm;
  oilGreen = 0.0;
  oilBlue = prm;
  temperature = Af;
  alreadydone = true;
  process();
  updateGL();
}

void Extract::mouseMoveEvent(QMouseEvent *e1) {

}

void Extract::initializeGL() {
  // set background color
  qglClearColor( QColor(224,224,224) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, xglwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, xglwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Extract::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, xlight0_pos);
  //glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

  Point3D b1, b2;

  // contents, if necessary
  const GLfloat xglwater[] = { beakerRed, beakerGreen, beakerBlue, 
			       beakerAlpha };
  const GLfloat xgloil[] = { oilRed, oilGreen, oilBlue, oilAlpha };
  const GLfloat vglcyl[] = { 0.0, 0.0, 1.0, 0.4 };

  // no animation
  if (running == false) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglwater);
    glMaterialfv(GL_FRONT, GL_SPECULAR, xglwater);
    glMaterialfv(GL_FRONT, GL_SHININESS, xgldull);
    b1.setXYZ(0.0,-1.25,-10.0);
    b2.setXYZ(0.0,-4.75,-10.0);
    drawCylinder(b1,b2,1.9);
    drawDisk(b2,1.95);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xgloil);
    glMaterialfv(GL_FRONT, GL_SPECULAR, xgloil);
    glMaterialfv(GL_FRONT, GL_SHININESS, xgldull);
    b1.setXYZ(0.0,-1.25 + beakerHeight,-10.0);
    b2.setXYZ(0.0,-1.25,-10.0);
    drawCylinder(b1,b2,1.9);
    drawDisk(b2,1.95);
  }

  // flask
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, xglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, xglpolished);
  b1.setXYZ(0.0,2.5,-10.0);
  b2.setXYZ(0.0,-5.0,-10.0);
  drawCylinder(b1,b2,2.0);

  // pedestal
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglblack);
  glMaterialfv(GL_FRONT, GL_SPECULAR, xglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, xglpolished);
  b1.setXYZ(0.0,-5.0,-10.0);
  b2.setXYZ(0.0,-6.0,-10.0);
  drawCylinder(b1,b2,-3.0);
  // pedestal indicator
  if (!running) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglwhite);
  } else {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglblue);
  }
  glMaterialfv(GL_FRONT, GL_SPECULAR, xglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, xglpolished);
  b1.setXYZ(0.0,-5.5,-7.0);
  drawSphere(b1,0.25);  

  // floor
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, xglfloor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, xglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, xglpolished);
  b1.setXYZ(0.0,-6.0,-10.0);
  drawDisk(b1,100.0);
}

void Extract::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Extract::keyPressEvent(QKeyEvent *e1) {
}

