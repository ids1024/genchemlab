#include "vapor.h"
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

GLfloat vglass[] = { 0.75f, 0.75f, 0.75f, 0.5f };	// Transparent material
GLfloat vbeakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat vglass2[] = { 0.75f, 0.75f, 0.75f, 0.5f };

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

Vapor::Vapor(QWidget *parent, const char *name)
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
  bvolume = 2.0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 1000 );
}

void Vapor::process() {
  if (running) {
    timeCount += 1;
    calculate();
  }
  emit signalSetStatusBar( tr("Temperature = %1 C; air volume = %2 mL").arg(temperature).arg(bvolume) );
  thermoHeight = -1.5 + 2.0 + ( temperature / 15.0 );
  if (thermoHeight < 0.5) thermoHeight = 0.5;
  if (thermoHeight > 7.5) thermoHeight = 7.5;
  updateGL();
}

void Vapor::targetTemperature() {

}

// figure everything out based on contents of beaker
void Vapor::calculate() {
  if (steps > 0) {
    temperature += increment;
  }
  steps--;

  double psolvent = exp( (pH/8.314)*(1/(freezeTemp+273)-1/(temperature+273))+log(760.0) );
  std::cout << temperature << "," << psolvent << std::endl;
  double nair = (1 * 0.002) / (0.08206 * 277.0);
  bvolume = (nair * 0.08206 * (temperature+273)) / ( (760.0-psolvent) / 760.0);
  bvolume *= 1000.0;

  if (temperature > freezeTemp) {
    beakerRed = 0.5; beakerGreen = 0.5; beakerBlue = 1.0; beakerAlpha = 0.9;
  }
}

void Vapor::addToBeaker(double m1, int u1, int sub1, bool aw, 
			      double vwater) {
  switch( QMessageBox::information( this, tr("Set up experiment"),
				    tr("Fill beaker with:"),
				    tr("&Water"), tr("&Ethanol"), 
				    tr("&Hexane") ) ) {
  case 0: 
    // save
    beakerSubstance = WATER;
    freezeTemp = 100.0;
    pH = 40700.0;  // deltaH_vap
    break;
  case 1: 
    // don't save but exit
    beakerSubstance = ETHANOL;
    freezeTemp = 78.32;
    pH = 38560.0;  // deltaH_vap
    break;
  case 2: 
    // don't exit
    beakerSubstance = HEXANE;
    freezeTemp = 68.7;
    pH = 28810.0;  // deltaH_vap
    break;
  }

  //cout << m1 << "=" << sub1 << endl;
  vwater = 50.0;
  beakerHeight = -4.75 + (vwater / 10.0);
  drawDrip = true;
  QMessageBox::information( this, tr("set temperature"),
			    tr("Click on the flask to start heating.") );
  cmode = 2;
  increment = 1.0;
  steps = 100;
  updateGL();
}

void Vapor::showInfo() {
}

void Vapor::saveData() {
}

void Vapor::mousePressEvent(QMouseEvent *e1) {
}

void Vapor::mouseReleaseEvent(QMouseEvent *e1) {
  if (running) { running = false; return; }
  running = true;
  timeCount = 0;
}

void Vapor::mouseMoveEvent(QMouseEvent *e1) {

}

void Vapor::initializeGL() {
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

void Vapor::paintGL() {
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
  b1.setXYZ(1.25,7.5,-10.0);
  b2.setXYZ(1.25,-1.5,-10.0);
  drawCylinder(b1,b2,0.25);
  drawSphere(b1,0.25);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglred);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
  b1.setXYZ(1.25,thermoHeight,-9.75);
  b2.setXYZ(1.25,-1.5,-9.75);
  drawCylinder(b1,b2,0.1);
  b1.setXYZ(1.25,-1.5,-10.0);
  b2.setXYZ(1.25,-2.5,-10.0);
  drawCylinder(b1,b2,0.25); 
  drawSphere(b2,0.25);
 
  // contents, if necessary
  const GLfloat bglwater[] = { beakerRed, beakerGreen, beakerBlue, 
			       beakerAlpha };
  const GLfloat vglcyl[] = { 0.0, 0.0, 1.0, 0.4 };
  if (drawDrip) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vglcyl);
    glMaterialfv(GL_FRONT, GL_SPECULAR, vglcyl);
    glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
    b1.setXYZ(0.0,-4.7 + (10.0 - bvolume) / 2.0,-10.0);
    b2.setXYZ(0.0,-4.7,-10.0);
    drawCylinder(b1,b2,0.45);   

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vglass);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
    b1.setXYZ(0.0,0.7,-10.0);
    b2.setXYZ(0.0,-4.7,-10.0);
    drawCylinder(b1,b2,0.5);
    drawDisk(b1,0.5);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglwater);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bglwater);
    glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
    b1.setXYZ(0.0,beakerHeight,-10.0);
    b2.setXYZ(0.0,-4.75,-10.0);
    drawCylinder(b1,b2,1.9);
    drawDisk(b2,1.95);
  }

  // flask
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vglass);
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

void Vapor::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Vapor::keyPressEvent(QKeyEvent *e1) {
}

