#include "electro.h"
#include "electrodialog.h"
#include "chem_dialog.h"

#include <qmessagebox.h>

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GLfloat eglass[] = { 0.75f, 0.75f, 0.75f, 0.5f };	// Transparent material
GLfloat ebeakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat eglass2[] = { 0.75f, 0.75f, 0.75f, 0.5f };

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

Electro::Electro(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cmode = 0; running = false; timeCount = 0;
  beakerVolume = 0.0; temperature = 0.0; runTime = 0.0; freezeTemp = 0.0;
  drawDrip = false;
  beakerRed = 0.2; beakerGreen = 0.2; beakerBlue = 1.0; beakerAlpha = 0.2;
  beakerHeight = -4.25; thermoHeight = 3.5;
  beakerConc = 0.0; fudgeFactor = 0.0;
  beakerSubstance = 0;
  bvolume = 2.0;
  m1 = SILVER; m2 = SILVER; m1conc = 0.1; m2conc = 0.1;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 1000 );
}

void Electro::process() {
  if (running) {
    calculate();
  }
  emit signalSetStatusBar( tr("Temperature = 25 C, Voltage = %1 V").arg(temperature) );
  updateGL();
}

void Electro::targetTemperature() {

}

// figure everything out based on contents of beaker
void Electro::calculate() {
  double deleft, deright;
  double cR = 8.314, cT = 298.15, cF = 96485.0;

  // compute by half-cell reduction potentials
  deleft = srp1 - (0.02569 / (double)ne1) * log(1.0/m1conc);
  deright = srp2 - (0.02569 / (double)ne2) * log(1.0/m2conc);
  
  temperature = deright - deleft;
}

void Electro::addToBeaker(double mx, int u1, int sub1, bool aw, 
			      double vwater) {
  ElectroDialog ed1(m1, m2, m1conc, m2conc);
  if ( !ed1.exec() ) return;

  ne1 = 1; ne2 = 1;

  m1 = ed1.getMetal1();
  switch (m1) {
  case SILVER:
    ne1 = 1;
    srp1 = 0.8;
    break;
  case COPPER:
    ne1 = 2;
    srp1 = 0.345;
    break;
  case LEAD:
    ne1 = 2;
    srp1 = -0.126;
    break;
  case TIN:
    ne1 = 2;
    srp1 = -0.138;
    break;
  case NICKEL:
    ne1 = 2;
    srp1 = -0.257;
    break;
  case IRON:
    ne1 = 2;
    srp1 = -0.447;
    break;
  case CHROMIUM:
    ne1 = 3;
    srp1 = -0.744;
    break;
  case ZINC:
    ne1 = 2;
    srp1 = -0.762;
    break;
  default:
    srp1 = 0.0;
    break;
  }
  m2 = ed1.getMetal2();
  switch (m2) {
  case SILVER:
    ne2 = 1;
    srp2 = 0.8;
    break;
  case COPPER:
    ne2 = 2;
    srp2 = 0.345;
    break;
  case LEAD:
    ne2 = 2;
    srp2 = -0.126;
    break;
  case TIN:
    ne2 = 2;
    srp2 = -0.138;
    break;
  case NICKEL:
    ne2 = 2;
    srp2 = -0.257;
    break;
  case IRON:
    ne2 = 2;
    srp2 = -0.447;
    break;
  case CHROMIUM:
    ne2 = 3;
    srp2 = -0.744;
    break;
  case ZINC:
    ne2 = 2;
    srp2 = -0.762;
    break;
  default:
    srp2 = 0.0;
    break;
  }
  m1conc = ed1.getConc1();
  m2conc = ed1.getConc2();
  drawDrip = true;
  running = true;
  updateGL();
  process();
}

void Electro::showInfo() {
}

void Electro::saveData() {
}

void Electro::mousePressEvent(QMouseEvent *e1) {
}

void Electro::mouseReleaseEvent(QMouseEvent *e1) {

}

void Electro::mouseMoveEvent(QMouseEvent *e1) {

}

void Electro::initializeGL() {
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

void Electro::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, blight0_pos);
  //glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

  Point3D b1, b2;

  // contents, if necessary
  const GLfloat bglwater[] = { beakerRed, beakerGreen, beakerBlue, 
			       beakerAlpha };
  const GLfloat vglcyl[] = { 0.0, 0.0, 1.0, 0.75 };
  const GLfloat vglmetal[] = { 0.7, 0.7, 0.7, 1.0 };
  if (drawDrip) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vglcyl);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, bgldull);
    b1.setXYZ(-3.0,1.5,-10.0);
    b2.setXYZ(-3.0,-4.5,-10.0);
    drawCylinder(b1,b2,1.75);
    b1.setXYZ(3.0,1.5,-10.0);
    b2.setXYZ(3.0,-4.5,-10.0);
    drawCylinder(b1,b2,1.75);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, vglmetal);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
    b1.setXYZ(-400.0,5.0,-10.0);
    b2.setXYZ(-4.0,5.0,-10.0);
    drawCylinder(b1,b2,0.1);
    b1.setXYZ(-4.0,5.0,-10.0);
    b2.setXYZ(-4.0,-2.5,-10.0);
    drawCylinder(b1,b2,0.5);
    b1.setXYZ(4.0,5.0,-10.0);
    b2.setXYZ(4.0,-2.5,-10.0);
    drawCylinder(b1,b2,0.5);
    b1.setXYZ(400.0,5.0,-10.0);
    b2.setXYZ(4.0,5.0,-10.0);
    drawCylinder(b1,b2,0.1);
  }

  // salt bridge
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, eglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(-2.0,5.0,-10.0);
  b2.setXYZ(-2.0,-2.5,-10.0);
  drawCylinder(b1,b2,0.5);
  b1.setXYZ(2.0,5.0,-10.0);
  b2.setXYZ(2.0,-2.5,-10.0);
  drawCylinder(b1,b2,0.5);
  b1.setXYZ(-2.0,5.0,-10.0);
  b2.setXYZ(2.0,5.0,-10.0);
  drawSphere(b1, 0.5);
  drawSphere(b2, 0.5);
  drawCylinder(b1,b2,0.5);

  // left flask
  b1.setXYZ(-3.0,2.5,-10.0);
  b2.setXYZ(-3.0,-5.0,-10.0);
  drawCylinder(b1,b2,2.0);

  // right flask
  b1.setXYZ(3.0,2.5,-10.0);
  b2.setXYZ(3.0,-5.0,-10.0);
  drawCylinder(b1,b2,2.0);

  // floor
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bglfloor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, bglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, bglpolished);
  b1.setXYZ(0.0,-6.0,-10.0);
  drawDisk(b1,100.0);
}

void Electro::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Electro::keyPressEvent(QKeyEvent *e1) {
}

