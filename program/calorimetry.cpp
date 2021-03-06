#include "calorimetry.h"
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

GLfloat cglass[] = { 0.75f, 0.75f, 0.75f, 0.5f };	// Transparent material
GLfloat cbeakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat cglass2[] = { 0.75f, 0.75f, 0.75f, 0.5f };

// Constant colours and material factors.
const GLfloat cglfloor[] = { 0.2f, 0.2f, 1.0f, 1.0f };
const GLfloat cglrun[] = { 0.2f, 1.0f, 0.2f, 1.0f };
const GLfloat cglblue[] = { 0.2f, 0.2f, 1.0f, 0.5f };
const GLfloat cglwhite[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat cglred[] = { 1.0, 0.0, 0.0, 1.0 };
const GLfloat cgldull[] = { 10.0 };
const GLfloat cglmidshine[] = { 50.0 };
const GLfloat cglpolished[] = { 90.0 };

// Light at origin.
const GLfloat clight0_pos[] = { 5.0, 5.0, 0.0, 1.0 };
const GLfloat clight1_pos[] = { -5.0, 5.0, -10.0, 1.0 };

Calorimetry::Calorimetry(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(Qt::ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cn = 0; running = false; timeCount = 0;
  beakerVolume = 0.0; temperature = 20.0; runTime = 0.0;
  srand( time(0) );
  calConstant = 50.0 + (double)(rand() % 150);
  drawDrip = false;
  beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.25;
  beakerHeight = -4.25; thermoHeight = 4.5;
  beakerConc = 0.0; fudgeFactor = 0.0;
  beakerSubstance = 0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 500 );
}

void Calorimetry::process() {
  if (running) {
    if (cmode == 1) {
      // change volume after mixing
      beakerVolume += hotVolume;
      beakerHeight = -4.5 + (beakerVolume / 50.0);
      cmode = 0;
    }
    cn++;
    if (cn >= 20) {
      // update experiment
      cn = 0;
      timeCount += 10;
      calculate();
    }
  }
  emit signalSetStatusBar( tr("Time = %1 seconds; temperature = %2 C").arg(timeCount).arg(temperature) );
  thermoHeight = -1.5 + 2.0 + ( (temperature + 20.0) / 10.0 );
  if (thermoHeight < 0.5) thermoHeight = 0.5;
  if (thermoHeight > 7.5) thermoHeight = 7.5;
  updateGL();
}

// figure everything out based on contents of beaker
void Calorimetry::calculate() {
  if (steps > 0) {
    temperature += increment;
  }
  if (steps < -5) {
    if (temperature < 20)
      temperature += 0.05;
    else
      temperature -= 0.05;
  }
  steps--;
}

void Calorimetry::addToBeaker(double m1, int u1, int sub1, bool aw, 
			      double vwater) {
  //cout << m1 << "=" << sub1 << endl;
  beakerSubstance = sub1;
  if (sub1 == H2O2) beakerConc = m1 / 34.0;
  if (sub1 == MGSO4) beakerConc = m1 / 120.4;
  if (sub1 == NH4SO4) beakerConc = m1 / 132.1;
  beakerVolume = vwater;
  beakerHeight = -4.5 + (vwater / 50.0);
  drawDrip = true;
  cmode = 0; coldTemp = 20.0;
  double Qrxn = 0.0;
  if (sub1 == H2O2) Qrxn = 98073.0 * beakerConc;
  if (sub1 == MGSO4) Qrxn = 91220.0 * beakerConc;
  if (sub1 == NH4SO4) Qrxn = -6560.0 * beakerConc;
  double Ch = 4.167 * beakerVolume;
  double Ccal = calConstant;
  double dt = Qrxn / (Ch + Ccal);
  targetTemp = 20.0 + dt;
  //std::cout << Qrxn << "-->" << dt << std::endl;
  timeCount = 0; steps = 6 + rand() % 18;
  increment = (targetTemp - coldTemp) / (double)steps;
  drawDrip = true;
  QMessageBox::information(this, tr("Experiment setup"), tr("Click on calorimeter to start.") );
  updateGL();
}

void Calorimetry::calorimeterConstant() {
  // get cold and hot water volumes
  coldVolume = 100.0;
  coldTemp = 4.0;
  hotVolume = 100.0;
  hotTemp = 20.0;

  QMessageBox::information(this, tr("Experiment setup"), tr("100 mL of cold water\n100 mL of room temperature water = 20 C\nClick on calorimeter to start.") );
  beakerVolume = coldVolume;
  beakerHeight = -4.5 + (beakerVolume / 50.0);
  temperature = coldTemp;
  double Ch = 4.167 * (coldVolume + hotVolume);
  double Ccal = calConstant;
  double Tr = hotTemp;
  double Tc = coldTemp;
  targetTemp = (Ch * Tr + Ch * Tc + Ccal * Tc) / (2.0 * Ch + Ccal);
  //std::cout << targetTemp << std::endl;
  //targetTemp = (coldTemp + hotTemp) / 2.0;
  cmode = 1; timeCount = 0; steps = 6 + rand() % 18;
  increment = (targetTemp - coldTemp) / (double)steps;
  drawDrip = true;
  updateGL();
}

void Calorimetry::showInfo() {
}

void Calorimetry::saveData() {
}

void Calorimetry::mousePressEvent(QMouseEvent *e1) {
}

void Calorimetry::mouseReleaseEvent(QMouseEvent *e1) {
  cn = 0;
  if (running) { running = false; return; }
  running = true;
  timeCount = 0;
}

void Calorimetry::mouseMoveEvent(QMouseEvent *e1) {

}

void Calorimetry::initializeGL() {
  // set background color
  qglClearColor( QColor(224,224,224) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, cglwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, cglwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Calorimetry::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, clight0_pos);
  //glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

  Point3D b1, b2;

  // thermometer
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglwhite);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, cgldull);
  b1.setXYZ(1.0,7.5,-10.0);
  b2.setXYZ(1.0,-1.5,-10.0);
  drawCylinder(b1,b2,0.25);
  drawSphere(b1,0.25);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglred);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, cgldull);
  b1.setXYZ(1.0,thermoHeight,-9.75);
  b2.setXYZ(1.0,-1.5,-9.75);
  drawCylinder(b1,b2,0.1);
  b1.setXYZ(1.0,-1.5,-10.0);
  b2.setXYZ(1.0,-2.5,-10.0);
  drawCylinder(b1,b2,0.25); 
  drawSphere(b2,0.25);
 
  // contents, if necessary
  if (drawDrip) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglblue);
    glMaterialfv(GL_FRONT, GL_SPECULAR, cglblue);
    glMaterialfv(GL_FRONT, GL_SHININESS, cgldull);
    b1.setXYZ(0.0,beakerHeight,-10.0);
    b2.setXYZ(0.0,-4.5,-10.0);
    drawCylinder(b1,b2,2.5);
    drawDisk(b2,2.0);
  }

  // Dewar flask
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglass2);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, cgldull);
  b1.setXYZ(0.0,2.5,-10.0);
  b2.setXYZ(0.0,-4.5,-10.0);
  drawCylinder(b1,b2,2.5);
  drawDisk(b2,2.0);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, cglpolished);
  b1.setXYZ(0.0,2.5,-10.0);
  b2.setXYZ(0.0,-5.0,-10.0);
  drawCylinder(b1,b2,3.0);

  // floor
  if (running)
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglrun);
  else
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cglfloor);
  glMaterialfv(GL_FRONT, GL_SPECULAR, cglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, cglpolished);
  b1.setXYZ(0.0,-5.0,-10.0);
  drawDisk(b1,100.0);
}

void Calorimetry::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
}

void Calorimetry::keyPressEvent(QKeyEvent *e1) {
}

