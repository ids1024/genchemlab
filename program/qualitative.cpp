#include "qualitative.h"
#include "chem_dialog.h"

#include <iostream>
#include <math.h>

GLfloat qglass[] = { 0.4f, 0.4f, 0.4f, 0.8f };	// Transparent material
//GLfloat beakerglass[] = { 0.3f, 0.3f, 0.3f, 0.7f };
//GLfloat glass2[] = { 0.4f, 0.4f, 0.4f, 1.0f };

//Constant colours and material factors.
//const GLfloat glblue[] = { 0.2f, 0.2f, 1.0f, 0.9f };
const GLfloat qglwhite[] = { 1.0, 1.0, 1.0, 1.0 };
//const GLfloat gldull[] = { 10.0 };
//const GLfloat glmidshine[] = { 50.0 };
const GLfloat qglpolished[] = { 90.0 };

// Light at origin.
const GLfloat qlight1_pos[] = { 5.0, 0.0, 10.0, 1.0 };
//const GLfloat light2_pos[] = { -5.0, 4.0, -20.0, 1.0 };


Qualitative::Qualitative(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cn = 0;
  beakerVolume = 0.0; dispenseVolume = 0.0; pH = 0.0;
  drawDrip = false;
  beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.25;
  beakerHeight = -4.95;
  beakerConc = 0.0, buretConc = 0.0;
  beakerSubstance = 0, buretSubstance = 0, indicator = 0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 500 );

  int c1;
  for (c1 = 0; c1 < 8; c1++) {
    cations_sol[c1] = 0; anions_sol[c1] = 0; 
    cations_pel[c1] = 0; anions_pel[c1] = 0;
  }
}

void Qualitative::process() {
  if (drawDrip) {
    if (beakerVolume < 200.0) {
      beakerVolume += dripRate;
	  dispenseVolume += dripRate;
      beakerHeight = -4.95 + (beakerVolume / 50.0);
    }
    calculate();
    if (indicator == 0) {
      beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0;
    } else {
      if (indicator == IN_PHENOLPHTHALEIN) {
      }
    }
  }
  emit signalSetStatusBar( tr( QString("Volume dispensed = %1 mL; pH = %2").arg(dispenseVolume).arg(pH) ) );
  updateGL();
}

// figure everything out based on contents of beaker
void Qualitative::calculate() {
}

void Qualitative::showInfo() {
}

void Qualitative::saveData() {
}

void Qualitative::addIndicator(int i1) {
  indicator = i1;
}

void Qualitative::mousePressEvent(QMouseEvent *e1) {

}

void Qualitative::mouseReleaseEvent(QMouseEvent *e1) {

}

void Qualitative::mouseMoveEvent(QMouseEvent *e1) {
}

void Qualitative::initializeGL() {
  // set background color
  qglClearColor( QColor(192,192,192) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, qglwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, qglwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Qualitative::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, qlight1_pos);

  Point3D b1, b2;
  // test tubes
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, qglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, qglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, qglpolished);
  b1.setXYZ(-3.0,-1.0,-10.0);
  b2.setXYZ(-3.0,4.0,-10.0);
  drawCylinder(b1,b2,0.75);
  b1.setXYZ(0.0,-1.0,-10.0);
  b2.setXYZ(0.0,4.0,-10.0);
  drawCylinder(b1,b2,0.75);
  b1.setXYZ(3.0,-1.0,-10.0);
  b2.setXYZ(3.0,4.0,-10.0);
  drawCylinder(b1,b2,0.75);
}

void Qualitative::resizeGL(int w, int h) {
  glViewport( 0, 0, (GLint)w, (GLint)h );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -5.0, 5.0,
	     -5.0, 5.0,
	     5.0, 15.0 );
  glMatrixMode( GL_MODELVIEW );
  solventHotSpot = QRect(0,0,w/3,h);
  solidHotSpot = QRect(2*w/3,0,w/3,h);
  buretHotSpot = QRect(w/3,0,w/3,h);
}

void Qualitative::keyPressEvent(QKeyEvent *e1) {
}

void Qualitative::drawLine(Point3D a, Point3D b)
{
  glBegin(GL_LINES);
  glColor3d(a.red(), a.green(), a.blue());
  glVertex3d( a.x(), a.y(), a.z() );
  glVertex3d( b.x(), b.y(), b.z() );
  glEnd();
}

void Qualitative::drawCylinder(Point3D a, Point3D b, double radius) {
  if (radius > 0.0)
    drawCylinder(a, b, radius, radius);
  else
    drawCylinder(a, b, -radius, radius);
}

void Qualitative::drawCylinder(Point3D a, Point3D b, double bradius, 
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
    gluDisk(q1, 0.0, bradius, 8, 1);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0.0, 0.0, l);
    gluDisk(q1, 0.0, tradius, 8, 1);
    glPopMatrix();
  }

  glPopMatrix();
}

void Qualitative::drawSphere(Point3D a, double r)
{
  GLUquadric *q1;
  q1 = gluNewQuadric();

  glPushMatrix();

  glTranslated(a.x(), a.y(), a.z() );
  gluSphere(q1, r, 6, 6);

  glPopMatrix();
  //cout << "draw sphere" << endl;
}

void Qualitative::drawBox(Point3D a, Point3D b) {
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

