#include "labwidget.h"

#include <math.h>

LabWidget::LabWidget(QWidget *parent, const char *name)
  : QGLWidget(parent, name)
{
  setupDone = false;
}

void LabWidget::showSetup() {
}

void LabWidget::showInfo() {
}

void LabWidget::saveData() {
}

void LabWidget::Demo() {
}

void LabWidget::addToBuret(double v1, int sub1) {
}

void LabWidget::addToBeaker(double m1, int u1, int sub1, bool aw, 
			    double vwater) {
}

void LabWidget::addIndicator(int i1) {
}

void LabWidget::calorimeterConstant() {
}

void LabWidget::targetTemperature() {
}

void LabWidget::setWavelength() {
}

void LabWidget::absScan() {
}

int LabWidget::whichLab() { return 0; }

// drawing functions
void LabWidget::drawLine(Point3D a, Point3D b)
{
  glBegin(GL_LINES);
  glColor3d(a.red(), a.green(), a.blue());
  glVertex3d( a.x(), a.y(), a.z() );
  glVertex3d( b.x(), b.y(), b.z() );
  glEnd();
}

void LabWidget::drawCylinder(Point3D a, Point3D b, double radius) {
  if (radius > 0.0)
    drawCylinder(a, b, radius, radius);
  else
    drawCylinder(a, b, -radius, radius);
}

void LabWidget::drawCylinder(Point3D a, Point3D b, double bradius, 
			     double tradius)
{
  GLUquadric *q1;
  q1 = gluNewQuadric();

  bool capped=false;
  double l;

  if (tradius < 0.0) { capped = true; tradius = -tradius; }

  glPushMatrix();

  glTranslated(a.x(), a.y(), a.z() );

  double dx = b.x() - a.x();
  double dy = b.y() - a.y();
  double dz = b.z() - a.z();
  l = sqrt(dx*dx + dy*dy + dz*dz);

  if (l < 0.001) return;

#define DEGREES(rad) ((rad)*180.0/3.14159265358979323846)
  if (fabs( a.y() - b.y() ) < 0.000001 &&
      fabs( b.x() - a.x() ) < 0.000001) {
    if ( b.z() - a.z() < 0.0) glRotated(180.0,1.0,0.0,0.0);
  } else {
    glRotated(DEGREES(acos(( b.z() - a.z() ) / l)),
              a.y() - b.y(), b.x() - a.x(), 0.0);
  }

  gluCylinder(q1, bradius, tradius, l, 16, 1);

  if (capped) {
    glPushMatrix();
    glRotated(180.0, 0.0, 1.0, 0.0);
    gluDisk(q1, 0.0, bradius, 16, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0.0, 0.0, l);
    gluDisk(q1, 0.0, tradius, 16, 1);
    glPopMatrix();
  }

  glPopMatrix();
}

void LabWidget::drawSphere(Point3D a, double r)
{
  GLUquadric *q1;
  q1 = gluNewQuadric();

  glPushMatrix();

  glTranslated(a.x(), a.y(), a.z() );
  gluSphere(q1, r, 6, 6);

  glPopMatrix();
  //cout << "draw sphere" << endl;
}

void LabWidget::drawDisk(Point3D a, double r)
{
  GLUquadric *q1;
  q1 = gluNewQuadric();

  gluQuadricOrientation(q1, GLU_INSIDE);

  glPushMatrix();

  glTranslated(a.x(), a.y(), a.z() );
  glRotated(90.0, 1.0, 0.0, 0.0);
  gluDisk(q1, 0.0, r, 8, 8);

  glPopMatrix();
  //cout << "draw sphere" << endl;
}

void LabWidget::drawBox(Point3D a, Point3D b) {
  double min_x, min_y, min_z, max_x, max_y, max_z;
  if (a.x() > b.x()) {
    max_x = a.x(); min_x = b.x();
  } else {
    max_x = b.x(); min_x = a.x();
  }
  if (a.y() > b.y()) {
    max_y = a.y(); min_y = b.y();
  } else {
    max_y = b.y(); min_y = a.y();
  }
  if (a.z() > b.z()) {
    max_z = a.z(); min_z = b.z();
  } else {
    max_z = b.z(); min_z = a.z();
  }
  glBegin(GL_QUADS);
  // X-Y faces
  glVertex3d( min_x, min_y, min_z );
  glVertex3d( max_x, min_y, min_z );
  glVertex3d( max_x, max_y, min_z );
  glVertex3d( min_x, max_y, min_z );
  glVertex3d( min_x, min_y, max_z );
  glVertex3d( max_x, min_y, max_z );
  glVertex3d( max_x, max_y, max_z );
  glVertex3d( min_x, max_y, max_z );
  // X-Z faces
  glVertex3d( min_x, min_y, min_z );
  glVertex3d( max_x, min_y, min_z );
  glVertex3d( max_x, min_y, max_z );
  glVertex3d( min_x, min_y, max_z );
  glVertex3d( min_x, max_y, min_z );
  glVertex3d( max_x, max_y, min_z );
  glVertex3d( max_x, max_y, max_z );
  glVertex3d( min_x, max_y, max_z );
  // Y-Z faces
  glVertex3d( min_x, min_y, min_z );
  glVertex3d( min_x, max_y, min_z );
  glVertex3d( min_x, max_y, max_z );
  glVertex3d( min_x, min_y, max_z );
  glVertex3d( max_x, min_y, min_z );
  glVertex3d( max_x, max_y, min_z );
  glVertex3d( max_x, max_y, max_z );
  glVertex3d( max_x, min_y, max_z );
  glEnd();
  //cout << "draw box" << endl;
}
