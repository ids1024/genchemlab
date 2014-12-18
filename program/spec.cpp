#include "spec.h"
#include "chem_dialog.h"
#include "textdialog.h"

#include <iostream>
#include <math.h>
#ifdef UNIX
#include <unistd.h>
#endif

GLfloat sglass[] = { 0.4f, 0.4f, 0.4f, 0.15f };	// Transparent material
//GLfloat beakerglass[] = { 0.3f, 0.3f, 0.3f, 0.7f };
//GLfloat glass2[] = { 0.4f, 0.4f, 0.4f, 1.0f };

//Constant colours and material factors.
const GLfloat sglblue[] = { 0.2f, 0.2f, 1.0f, 0.9f };
const GLfloat sglwhite[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat sglblack[] = { 0.0, 0.0, 0.0, 1.0 };
const GLfloat sgldull[] = { 1.0 };
//const GLfloat glmidshine[] = { 50.0 };
const GLfloat sglpolished[] = { 90.0 };

// Light at origin.
const GLfloat slight1_pos[] = { 5.0, 5.0, 0.0, 1.0 };
//const GLfloat light2_pos[] = { -5.0, 4.0, -20.0, 1.0 };


Spectrometry::Spectrometry(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cn = 0;
  beakerVolume = 0.0; dispenseVolume = 0.0; pH = 0.0;
  wavelength = 710.0; absorbance = 0.0; ec = 1.0;
  drawDrip = false;
  beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.25;
  beamRed = 1.0; beamGreen = 0.0; beamBlue = 0.0; beamAlpha = 0.8;
  beakerHeight = -4.95;
  beakerConc = 0.0, buretConc = 0.0;
  beakerSubstance = 0, buretSubstance = 0, indicator = 0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 1500, true );
}

void Spectrometry::process() {
  if (drawDrip) {

  }
  calculate();
  emit signalSetStatusBar( tr("Wavelength = %1 nm; A = %2").arg(wavelength).arg(absorbance) );
  updateGL();
}

// figure everything out based on contents of beaker
void Spectrometry::calculate() {
  // set beam color by wavelength
  double ff;
  if (wavelength > 701.0) {  // red
    beamRed = 1.0; beamGreen = 0.0; beamBlue = 0.0;  
    calcAbs();
    return;
  }
  if (wavelength > 609.5) {  // orange - red
    ff = (701.0 - wavelength) / 91.5;
    beamRed = 1.0; beamGreen = 0.647 * ff; beamBlue = 0.0;  
    calcAbs();
    return;
  }
  if (wavelength > 587.0) {  // yellow - orange
    ff = (609.5 - wavelength) / 22.5;
    beamRed = 1.0; beamGreen = 0.647 + 0.353 * ff; beamBlue = 0.0;  
    calcAbs();
    return;
  }
  if (wavelength > 534.5) {  // green - yellow
    ff = (587.0 - wavelength) / 52.5;
    beamRed = 1.0 - ff; beamGreen = 1.0; beamBlue = 0.0;  
    calcAbs();
    return;
  }
  if (wavelength > 473.5) {  // blue - green
    ff = (534.5 - wavelength) / 61.0;
    beamRed = 0.0; beamGreen = 1.0 - ff; beamBlue = ff;  
    calcAbs();
    return;
  }
  if (wavelength > 422.5) {  // violet - blue
    ff = (473.5 - wavelength) / 51.0;
    beamRed = 0.933 * ff; beamGreen = 0.509 * ff; beamBlue = 0.933 + 0.067 * ff;  
    calcAbs();
    return;
  }
  if (wavelength <= 422.5) {  // violet
    beamRed = 0.933; beamGreen = 0.509; beamBlue = 0.933;  
    calcAbs();
    return;
  }
}

void Spectrometry::calcAbs() {
  // caclulate absorbance
  if (beakerSubstance == WATER) absorbance = 0.0;
  if (beakerSubstance == COCL2) {
    if (wavelength < 485.0) {
      absorbance = 0.25 * beakerConc;
      return;
    }
    if (wavelength < 495.0) {
      absorbance = 0.75 * beakerConc;
      return;
    }
    if (wavelength < 505.0) {
      absorbance = 2.0 * beakerConc;
      return;
    }
    if (wavelength < 515.0) {
      absorbance = 5.0 * beakerConc;
      return;
    }
    if (wavelength < 525.0) {
      absorbance = 2.0 * beakerConc;
      return;
    }
    if (wavelength < 535.0) {
      absorbance = 0.75 * beakerConc;
      return;
    }
    absorbance = 0.25 * beakerConc;
    return;
  }
  if (beakerSubstance == CUSO4) {
    if (wavelength < 575.0) {
      absorbance = 0.25 * beakerConc;
      return;
    }
    if (wavelength < 585.0) {
      absorbance = 0.5 * beakerConc;
      return;
    }
    if (wavelength < 595.0) {
      absorbance = 1.0 * beakerConc;
      return;
    }
    if (wavelength < 605.0) {
      absorbance = 2.0 * beakerConc;
      return;
    }
    if (wavelength < 615.0) {
      absorbance = 1.0 * beakerConc;
      return;
    }
    if (wavelength < 625.0) {
      absorbance = 0.5 * beakerConc;
      return;
    }
    absorbance = 0.25 * beakerConc;
    return;
  }
  if (beakerSubstance == NISO4) {
    if (wavelength < 375.0) {
      absorbance = 0.25 * beakerConc;
      return;
    }
    if (wavelength < 385.0) {
      absorbance = 0.75 * beakerConc;
      return;
    }
    if (wavelength < 395.0) {
      absorbance = 1.5 * beakerConc;
      return;
    }
    if (wavelength < 405.0) {
      absorbance = 4.0 * beakerConc;
      return;
    }
    if (wavelength < 415.0) {
      absorbance = 1.5 * beakerConc;
      return;
    }
    if (wavelength < 425.0) {
      absorbance = 0.75 * beakerConc;
      return;
    }
    absorbance = 0.25 * beakerConc;
    return;
  }
}

void Spectrometry::showInfo() {
}

void Spectrometry::saveData() {
}

void Spectrometry::addToBeaker(double m1, int u1, int sub1, bool aw, 
			       double vwater) {
  //cout << m1 << "=" << vwater << endl;
  beakerSubstance = sub1;
  vwater /= 1000.0;  // convert to liters

  if (m1 == 0.0) beakerSubstance = WATER;

  double ff;
  if (sub1 == COCL2) {
    beakerConc = (m1 / 129.8) / vwater;
    ff = beakerConc / 0.25;
    if (ff > 1.0) ff = 1.0;
    beakerRed = 1.0 - 0.455 * ff;
    beakerGreen = 1.0 - ff;
    beakerBlue = 1.0 - ff;
  }
  if (sub1 == NISO4) {
    beakerConc = (m1 / 154.9) / vwater;
    ff = beakerConc / 0.25;
    if (ff > 1.0) ff = 1.0;
    beakerRed = 1.0 - ff;
    beakerGreen = 1.0;
    beakerBlue = 1.0 - ff;
  }
  if (sub1 == CUSO4) {
    beakerConc = (m1 / 159.6) / vwater;
    ff = beakerConc / 0.25;
    if (ff > 1.0) ff = 1.0;
    beakerRed = 1.0 - ff;
    beakerGreen = 1.0 - ff;
    beakerBlue = 1.0;
  }

  drawDrip = true;
  process();
}

void Spectrometry::setWavelength() {
  bool ok;
  double res = 
    QInputDialog::getDouble(tr("Set wavelength"), 
			    tr("Enter the wavelength in nanometers:"), 
			    wavelength, 200,
			    800, 3, &ok, this );
  if ( ok ) {
    // user entered something and pressed OK
    wavelength = res;
    drawDrip = true;
    process();
    return;
  } else {
    // user pressed Cancel
    return;
  }
}

void Spectrometry::absScan() {
  QString wa, n1;
  QMessageBox::information( this, tr("Wavelength scan"),
			    tr("The spectrophotometer will now scan from 390 nm to 740 nm and display the results.") );
  wa = tr("Wavelength, absorbance\n");
  for (double j = 390.0; j < 750.0; j += 10.0) {
#ifdef UNIX
    sleep(1);
#else
    Sleep(1000);
#endif
    wavelength = j;
    process();
    n1.setNum(wavelength);
    wa.append(n1);
    wa.append(",");
    n1.setNum(absorbance);
    wa.append(n1);
    wa.append("\n");
  }
  TextDialog d1(wa);
  d1.exec();
}

void Spectrometry::addIndicator(int i1) {

}

void Spectrometry::mousePressEvent(QMouseEvent *e1) {

}

void Spectrometry::mouseReleaseEvent(QMouseEvent *e1) {

}

void Spectrometry::mouseMoveEvent(QMouseEvent *e1) {
}

void Spectrometry::initializeGL() {
  // set background color
  qglClearColor( QColor(255,255,255) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sglwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, sglwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Spectrometry::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, slight1_pos);

  Point3D b1, b2;

  //colors
  const GLfloat glbeaker[]={ beakerRed, beakerGreen, beakerBlue, beakerAlpha };
  const GLfloat glbeam[]={ beamRed, beamGreen, beamBlue, beamAlpha };

  // spec and beam
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sglblack);
  glMaterialfv(GL_FRONT, GL_SPECULAR, sglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, sglpolished);
  b1.setXYZ(0.0,-2.0,-10.0);
  b2.setXYZ(0.0,-5.0,-10.0);
  drawCylinder(b1,b2,-0.75);
  glPushMatrix();
  glTranslated(0.0,0.0,-10.0);
  b1.setXYZ(-10.0,0.0,0.0);
  b2.setXYZ(-2.5,0.0,0.0);
  drawCylinder(b1,b2,-1.0);
  b1.setXYZ(2.5,0.0,0.0);
  b2.setXYZ(10.0,0.0,0.0);
  drawCylinder(b1,b2,-1.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glbeam);
  glMaterialfv(GL_FRONT, GL_SPECULAR, sglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, sgldull);
  b1.setXYZ(-2.5,0.0,0.0);
  b2.setXYZ(2.5,0.0,0.0);
  drawCylinder(b1,b2,-0.25);
  glPopMatrix();
  // test tubes
  // contents
  if (drawDrip) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glbeaker);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sglwhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, sglpolished);
    b1.setXYZ(0.0,-1.9,-10.0);
    b2.setXYZ(0.0,2.0,-10.0);
    drawCylinder(b1,b2,-0.7);
  }
  // tube
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, sglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, sglpolished);
  b1.setXYZ(0.0,-2.0,-10.0);
  b2.setXYZ(0.0,3.0,-10.0);
  drawCylinder(b1,b2,0.75);
  // floor
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sglblue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, sglwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, sglpolished);
  b1.setXYZ(0.0,-5.0,-10.0);
  drawDisk(b1,100.0);
}

void Spectrometry::resizeGL(int w, int h) {
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

void Spectrometry::keyPressEvent(QKeyEvent *e1) {
}

void Spectrometry::drawLine(Point3D a, Point3D b)
{
  glBegin(GL_LINES);
  glColor3d(a.red(), a.green(), a.blue());
  glVertex3d( a.x(), a.y(), a.z() );
  glVertex3d( b.x(), b.y(), b.z() );
  glEnd();
}

void Spectrometry::drawCylinder(Point3D a, Point3D b, double radius) {
  if (radius > 0.0)
    drawCylinder(a, b, radius, radius);
  else
    drawCylinder(a, b, -radius, radius);
}

void Spectrometry::drawCylinder(Point3D a, Point3D b, double bradius, 
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

void Spectrometry::drawSphere(Point3D a, double r)
{
  GLUquadric *q1;
  q1 = gluNewQuadric();

  glPushMatrix();

  glTranslated(a.x(), a.y(), a.z() );
  gluSphere(q1, r, 6, 6);

  glPopMatrix();
  //cout << "draw sphere" << endl;
}

void Spectrometry::drawBox(Point3D a, Point3D b) {
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

