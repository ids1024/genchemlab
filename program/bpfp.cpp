#include "bpfp.h"
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

GLfloat bglass[] = { 0.75f, 0.75f, 0.75f, 0.5f };	// Transparent material
GLfloat bbeakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat bglass2[] = { 0.75f, 0.75f, 0.75f, 0.5f };

// Constant colours and material factors.
const GLfloat bglfloor[] = { 0.2f, 0.2f, 1.0f, 1.0f };
const GLfloat bglblack[] = { 0.1f, 0.1f, 0.1f, 1.0f };
const GLfloat bglblue[] = { 0.2f, 0.2f, 1.0f, 1.0f };
const GLfloat bglwhite[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat bglred[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat bgldull[] = { 10.0 };
const GLfloat bglmidshine[] = { 50.0 };
const GLfloat bglpolished[] = { 90.0 };

// Light at origin.
const GLfloat blight0_pos[] = { 5.0, 5.0, 0.0, 1.0 };
const GLfloat blight1_pos[] = { -5.0, 5.0, -10.0, 1.0 };

Bpfp::Bpfp(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(Qt::ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cmode = 0; running = false; timeCount = 0;
  beakerVolume = 0.0; temperature = 4.0; runTime = 0.0; freezeTemp = 0.0;
  drawDrip = false;
  beakerRed = 0.2; beakerGreen = 0.2; beakerBlue = 1.0; beakerAlpha = 0.2;
  beakerHeight = -4.25; thermoHeight = 3.5;
  beakerConc = 0.0; fudgeFactor = 0.0;
  beakerSubstance = 0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 1000 );
}

void Bpfp::process() {
  if (running) {
    timeCount += 1;
    calculate();
  }
  emit signalSetStatusBar( tr("Time = %1 seconds; temperature = %2 C").arg(timeCount).arg(temperature) );
  thermoHeight = -1.5 + 2.0 + ( (temperature + 20.0) / 10.0 );
  if (thermoHeight < 0.5) thermoHeight = 0.5;
  if (thermoHeight > 7.5) thermoHeight = 7.5;
  updateGL();
}

void Bpfp::targetTemperature() {
  QMessageBox::information( this, tr("set temperature"),
			    tr("The flask will be chilled in a simulated ice/water/rock salt bath to approximately -20 C.\nClick on the flask to start.") );
  //temperature = 10.0;
  targetTemp = -20.0;
  cmode = 2;
  increment = -0.05;
  steps = abs( (int)( (targetTemp - 20.0) * 20) );
}

// figure everything out based on contents of beaker
void Bpfp::calculate() {
  if (steps > 0) {
    temperature += increment;
  }
  steps--;
  if (temperature < freezeTemp) {
    beakerRed = 0.5; beakerGreen = 0.5; beakerBlue = 1.0; beakerAlpha = 0.9;
  }
}

void Bpfp::addToBeaker(double m1, int u1, int sub1, bool aw, 
			      double vwater) {
  //cout << m1 << "=" << sub1 << endl;
  if (vwater < 0.0) vwater = 0.1;
  beakerSubstance = sub1;
  // find MOLAL concentration...
  if (sub1 == NACL) beakerConc = (2.0 * m1 / 58.44) / (vwater / 1000.0);
  if (sub1 == CACL2) beakerConc = (3.0 * m1 / 111.0) / (vwater / 1000.0);
  if (sub1 == SUCROSE) beakerConc = (m1 / 342.3) / (vwater / 1000.0);
  if (sub1 == ASPIRIN) beakerConc = (m1 / 180.2) / (vwater / 1000.0);
  freezeTemp = beakerConc * -1.86;
  beakerVolume = vwater;
  beakerHeight = -4.75 + (vwater / 10.0);
  drawDrip = true;
  updateGL();
}

void Bpfp::showInfo() {
}

void Bpfp::saveData() {
}

void Bpfp::mousePressEvent(QMouseEvent *e1) {
}

void Bpfp::mouseReleaseEvent(QMouseEvent *e1) {
  if (running) { running = false; return; }
  running = true;
  timeCount = 0;
}

void Bpfp::mouseMoveEvent(QMouseEvent *e1) {

}

void Bpfp::initializeGL() {
  // set background color
  qglClearColor( QColor(224,224,224) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, bglwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, bglwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Bpfp::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, blight0_pos);
  //glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

  Point3D b1, b2;

  // thermometer
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglwhite);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
  b1.setXYZ(0.5,7.5,-10.0);
  b2.setXYZ(0.5,-1.5,-10.0);
  drawCylinder(b1,b2,0.25);
  drawSphere(b1,0.25);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglred);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
  b1.setXYZ(0.5,thermoHeight,-9.75);
  b2.setXYZ(0.5,-1.5,-9.75);
  drawCylinder(b1,b2,0.1);
  b1.setXYZ(0.5,-1.5,-10.0);
  b2.setXYZ(0.5,-2.5,-10.0);
  drawCylinder(b1,b2,0.25); 
  drawSphere(b2,0.25);
 
  // contents, if necessary
  const GLfloat bglwater[] = { beakerRed, beakerGreen, beakerBlue, 
			       beakerAlpha };
  if (drawDrip) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglwater);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bglwater);
    glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
    b1.setXYZ(0.0,beakerHeight,-10.0);
    b2.setXYZ(0.0,-4.75,-10.0);
    drawCylinder(b1,b2,1.9);
    drawDisk(b2,1.95);
  }

  // flask
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(0.0,2.5,-10.0);
  b2.setXYZ(0.0,-5.0,-10.0);
  drawCylinder(b1,b2,2.0);

  // pedestal
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglblack);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(0.0,-5.0,-10.0);
  b2.setXYZ(0.0,-6.0,-10.0);
  drawCylinder(b1,b2,-3.0);
  // pedestal indicator
  if (!running) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglwhite);
  } else {
    if (cmode == 1) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglred);
    if (cmode == 2) glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglblue);
  }
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(0.0,-5.5,-7.0);
  drawSphere(b1,0.25);  

  // floor
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglfloor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(0.0,-6.0,-10.0);
  drawDisk(b1,100.0);
}

void Bpfp::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Bpfp::keyPressEvent(QKeyEvent *e1) {
}

