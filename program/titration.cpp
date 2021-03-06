#include "titration.h"
#include "titrationdialog.h"

#include <iostream>
#include <math.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>

GLfloat glass[] = { 0.4f, 0.4f, 0.4f, 0.4f };	// Transparent material
GLfloat beakerglass[] = { 0.3f, 0.3f, 0.3f, 0.4f };
GLfloat glass2[] = { 0.4f, 0.4f, 0.4f, 1.0f };

// Constant colours and material factors.
const GLfloat glblue[] = { 0.2f, 0.2f, 1.0f, 0.9f };
const GLfloat glwhite[] = { 1.0, 1.0, 1.0, 1.0 };
const GLfloat glwater[] = { 1.0, 1.0, 1.0, 0.4 };
const GLfloat gldull[] = { 10.0 };
const GLfloat glmidshine[] = { 50.0 };
const GLfloat glpolished[] = { 90.0 };

// Light at origin.
const GLfloat light0_pos[] = { 5.0, 5.0, 0.0, 1.0 };
const GLfloat light1_pos[] = { -5.0, 5.0, -10.0, 1.0 };

Titration::Titration(QWidget *parent, const char *name)
  : LabWidget(parent, name)
{
  setPalette(QPalette(QColor(255,255,255))); 
  setMouseTracking(false);
  setFocusPolicy(Qt::ClickFocus);
  dripRate = 0.0;
  startY = -1;
  cn = 0;
  beakerVolume = 0.0; dispenseVolume = 0.0; pH = 0.0;
  rememberMass1 = 0.0;
  drawDrip = false;
  beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.25;
  beakerHeight = -4.95;
  beakerConc = 0.0, beakerCounterionConc = 0.0000001; fudgeFactor = 0.0;
  beakerSubstance = 0, buretSubstance = 0, indicator = 0;
  t = new QTimer( this );
  connect( t, SIGNAL(timeout()), SLOT(process()) );
  t->start( 500 );
}

void Titration::process() {
  if (drawDrip) {
    if (beakerVolume < 200.0) {
      beakerVolume += dripRate;
      dispenseVolume += dripRate;
    }
    calculate();
    indicatorColor();
  }
  emit signalSetStatusBar( tr("Volume dispensed = %1 mL; pH = %2").arg(dispenseVolume).arg(pH) );
  beakerHeight = -4.95 + (beakerVolume / 50.0);
  updateGL();
}

// figure everything out based on contents of beaker
void Titration::calculate() {
  double molsAdded, m1, m2, m3;
  bool naoh = false, na2s2o3 = false;

  if (buretSubstance == BASE_1M_NAOH) {
    molsAdded = 1.0 * 0.001 * dripRate; naoh = true;
  }
  if (buretSubstance == BASE_01M_NAOH) {
    molsAdded = 0.1 * 0.001 * dripRate; naoh = true;
  }
  if (buretSubstance == BASE_001M_NAOH) {
    molsAdded = 0.01 * 0.001 * dripRate; naoh = true;
  }
  if (buretSubstance == BASE_1M_NA2S2O3) {
    molsAdded = 1.0 * 0.001 * dripRate; na2s2o3 = true;
  }
  if (buretSubstance == BASE_01M_NA2S2O3) {
    molsAdded = 0.1 * 0.001 * dripRate; na2s2o3 = true;
  }
  if (buretSubstance == BASE_001M_NA2S2O3) {
    molsAdded = 0.01 * 0.001 * dripRate; na2s2o3 = true;
  }
  if ( (beakerSubstance == KHP) && (naoh) ) {
    beakerCounterionConc += molsAdded; // NaOH
    beakerConc -= molsAdded; // KHP
    if (beakerConc < 0.0) beakerConc = 0.00000001;
    pH = 5.27 + log10( beakerCounterionConc / beakerConc );
  }
  if ( (beakerSubstance == ACETIC_ACID) && (naoh) ) {
    beakerCounterionConc += molsAdded; // NaOH
    beakerConc -= molsAdded; // acetic acid
    if (beakerConc < 0.0) beakerConc = 0.00000001;
    pH = 4.74 + log10( beakerCounterionConc / beakerConc );
  }
  if ( (beakerSubstance == ASCORBIC_ACID) && (naoh) ) {
    beakerCounterionConc += molsAdded; // NaOH
    beakerConc -= molsAdded; // ascorbic acid
    if (beakerConc < 0.0) beakerConc = 0.00000001;
    pH = 4.10 + log10( beakerCounterionConc / beakerConc );
  }
  if ( (beakerSubstance == H3PO4) && (naoh) ) {
    beakerCounterionConc += molsAdded; // NaOH
    //beakerConc -= molsAdded; // H3PO4
    if (beakerCounterionConc > (2.0 * fudgeFactor) ) {
      m1 = beakerCounterionConc - (2.0 * fudgeFactor);
      m2 = fudgeFactor - m1;
      if (m2 < 0) m2 = 0.0001;
      pH = 12.67 + log10( m1 / m2 );
      return;
    }
    if (beakerCounterionConc > fudgeFactor) {
      m1 = beakerCounterionConc - fudgeFactor;
      m2 = fudgeFactor - m1;
      pH = 7.21 + log10( m1 / m2 );
      return;
    }
    m1 = beakerCounterionConc;
    m2 = fudgeFactor - m1;
    pH = 2.12 + log10( m1 / m2 );
    return;
  }

  if ( (beakerSubstance == COLA) && (naoh) ) {
    beakerCounterionConc += molsAdded; // NaOH
    //beakerConc -= molsAdded; // H3PO4
    if (beakerCounterionConc > (2.0 * fudgeFactor) ) {
      m1 = beakerCounterionConc - (2.0 * fudgeFactor);
      m2 = fudgeFactor - m1;
      if (m2 < 0) m2 = 0.0001;
      pH = 12.67 + log10( m1 / m2 );
      return;
    }
    if (beakerCounterionConc > fudgeFactor) {
      m1 = beakerCounterionConc - fudgeFactor;
      m2 = fudgeFactor - m1;
      pH = 7.21 + log10( m1 / m2 );
      return;
    }
    m1 = beakerCounterionConc;
    m2 = fudgeFactor - m1;
    pH = 2.12 + log10( m1 / m2 );
    return;
  }

  if ( (beakerSubstance == CITRIC_ACID) && (naoh) ) {
    if (fudgeFactor > 0.0) {
      fudgeFactor -= molsAdded;
      if (fudgeFactor < 0.0) {
	beakerCounterionConc -= fudgeFactor; // NaOH
	beakerConc += fudgeFactor; // citric acid
      }
    } else {
      beakerCounterionConc += molsAdded; // NaOH
      beakerConc -= molsAdded; // citric acid
    }
    if (beakerConc < 0.0) beakerConc = 0.00000001;
    pH = 5.40 + log10( beakerCounterionConc / beakerConc );
  }
}

// determine indicator color
void Titration::indicatorColor() {
  if (beakerSubstance == COLA) {
    beakerRed = 0.645; beakerGreen = 0.164; beakerBlue = 0.164; 
    beakerAlpha = 0.6;
    return;
  }

  if (indicator == 0) {
    beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.4;
    if ( (buretSubstance == BASE_1M_NA2S2O3) ||
	 (buretSubstance == BASE_01M_NA2S2O3) ||
	 (buretSubstance == BASE_001M_NA2S2O3) ) {
      if (indicator == IN_STARCH) {
      }
    }
    return;
  } 
  if (indicator == IN_PHENOLPHTHALEIN) {
    if (pH < 8.8) {
      beakerRed = 1.0; beakerGreen = 1.0; beakerBlue = 1.0; beakerAlpha = 0.4;
    } else {
      beakerRed = 1.0; beakerGreen = 0.5; beakerBlue = 0.5;
    }
  }
  if (indicator == IN_METHYLORANGE) {
    if (pH < 3.7) {
      beakerRed = 1.0; beakerGreen = 0.0; beakerBlue = 0.0;
    } else {
      beakerRed = 1.0; beakerGreen = 0.644; beakerBlue = 0.0;
    }
  }
  if (indicator == IN_METHYLRED) {
    if (pH < 5.1) {
      beakerRed = 1.0; beakerGreen = 0.644; beakerBlue = 0.0;
    } else {
      beakerRed = 1.0; beakerGreen = 0.0; beakerBlue = 0.0;
    }
  }
  if (indicator == IN_PHENOLRED) {
    if (pH < 7.9) {
      beakerRed = 1.0; beakerGreen = 0.644; beakerBlue = 0.0;
    } else {
      beakerRed = 1.0; beakerGreen = 0.0; beakerBlue = 0.0;
    }
  }
  if (indicator == IN_BROMOPHENOL) {
    if (pH < 3.8) {
      beakerRed = 1.0; beakerGreen = 0.644; beakerBlue = 0.0;
    } else {
      beakerRed = 0.0; beakerGreen = 0.0; beakerBlue = 1.0;
    }
  }
  if (indicator == IN_BROMOTHYMOL) {
    if (pH < 6.9) {
      beakerRed = 1.0; beakerGreen = 0.644; beakerBlue = 0.0;
    } else {
      beakerRed = 0.0; beakerGreen = 0.0; beakerBlue = 1.0;
    }
  }
  if (indicator == IN_LITMUS) {
    if (pH < 7.0) {
      beakerRed = 1.0; beakerGreen = 0.0; beakerBlue = 0.0;
    } else {
      beakerRed = 0.0; beakerGreen = 0.0; beakerBlue = 1.0;
    }
  }
}

void Titration::showInfo() {
}

void Titration::saveData() {
}

void Titration::addToBuret(double v1, int sub1) {
  buretSubstance = sub1;
  //std::cout << "fill buret with " << sub1 << std::endl;
}

void Titration::addToBeaker(double m1, int sub1, double vwater) {
  //std::cout << m1 << "=" << sub1 << "=" << vwater << std::endl;
  rememberMass1 = m1;
  beakerSubstance = sub1;
  // determine number of moles of substance...
  if (beakerSubstance == KHP) {
    beakerConc = m1 / 204.2;
  }
  if (beakerSubstance == ACETIC_ACID) {
    beakerConc = m1 / 60.06;
  }
  if (beakerSubstance == ASCORBIC_ACID) {
    beakerConc = m1 / 176.1;
  }
  if (beakerSubstance == CITRIC_ACID) {
    beakerConc = m1 / 192.1;
    fudgeFactor = 2.0 * beakerConc;
  }
  if (beakerSubstance == H3PO4) {
    beakerConc = m1 / 98.0;
    fudgeFactor = beakerConc;
  }
  if (beakerSubstance == COLA) {
    beakerConc = m1 * 0.001 * 0.015;
    fudgeFactor = beakerConc;
    beakerConc /= 2.0;
    beakerCounterionConc = beakerConc;
    pH = 2.12;
    beakerVolume += m1;
    beakerRed = 0.645; beakerGreen = 0.164; beakerBlue = 0.164; 
    beakerAlpha = 0.6;
    updateGL();
  }

  beakerVolume += vwater;
  updateGL();
}

void Titration::showSetup() {
  TitrationDialog td1;
  QString tmp1;
  tmp1.setNum(rememberMass1);
  td1.mass1->setText(tmp1);
  if (buretSubstance == BASE_001M_NAOH)
    td1.buretChoice->setCurrentIndex(0);
  if (buretSubstance == BASE_01M_NAOH)
    td1.buretChoice->setCurrentIndex(1);
  if (buretSubstance == BASE_1M_NAOH)
    td1.buretChoice->setCurrentIndex(2);
  if (buretSubstance == ACID_HCL)
    td1.buretChoice->setCurrentIndex(3);
  if (buretSubstance == BASE_01M_NA2S2O3)
    td1.buretChoice->setCurrentIndex(4);
  td1.inChoice->setCurrentIndex((indicator - 990) / 10);
  if ( !td1.exec() ) return;
  addToBeaker( td1.mass1->text().toDouble(),
	       td1.selectedChemical(),
	       td1.mass2->text().toDouble() );
  addToBuret(0.0, td1.getBuretChoice());
  addIndicator(td1.getIndicator());
}

void Titration::addIndicator(int i1) {
  if (i1 < 1000) return;
  indicator = i1;
  //std::cout << "add indicator " << i1 << std::endl;
}

void Titration::mousePressEvent(QMouseEvent *e1) {
  if (buretHotSpot.contains(e1->pos())) {
    //emit signalSetStatusBar("Start buret");
    startY = e1->pos().y();
    drawDrip = true;
    updateGL();
    return;
  }
}

void Titration::mouseReleaseEvent(QMouseEvent *e1) {
  dripRate = 0.0;
  startY = -1;
  //emit signalSetStatusBar("Stop buret");
  drawDrip = false;
  updateGL();
}

void Titration::mouseMoveEvent(QMouseEvent *e1) {
  int dy;
  //cout << "move" << endl;
  if (startY > 0) {
    dy = e1->pos().y() - startY;
    dripRate = (double)dy * 0.01;
    if (dripRate < 0.0) dripRate = 0.0;
    //emit signalSetStatusBar( QString("Change drip rate to %1").arg(dripRate) );
  }
}

void Titration::initializeGL() {
  // set background color
  qglClearColor( QColor(224,224,224) );
  glShadeModel( GL_SMOOTH );
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, glwhite);
  glLightfv(GL_LIGHT0, GL_SPECULAR, glwhite);
  //glEnable(GL_LIGHT1);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, glwhite);
  //glLightfv(GL_LIGHT1, GL_SPECULAR, glwhite);
}

void Titration::paintGL() {
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  // light sources
  glLightfv(GL_LIGHT0, GL_POSITION, light0_pos);
  //glLightfv(GL_LIGHT1, GL_POSITION, light1_pos);

  Point3D b1, b2, b3, b4, b5, b6, b7, b8;
  // buret
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glwhite);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, gldull);
  // stopper
  b1.setXYZ(-0.6,0.0,0.0);
  b2.setXYZ(0.6,0.0,0.0);
  b3.setXYZ(-0.7,0.0,0.0);
  b4.setXYZ(1.0,0.0,0.0);
  b5.setXYZ(0.85,3.5,-10.0);
  b6.setXYZ(0.85,5.5,-10.0);
  b7.setXYZ(0.85,4.5,-9.0);
  b8.setXYZ(0.85,4.5,-11.0);
  glPushMatrix();
  glTranslated(0.0,4.5,-10.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glwhite);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, gldull);
  drawCylinder(b3,b4,0.4);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, glpolished);
  drawCylinder(b1,b2,0.6);
  glPopMatrix();
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glblue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, gldull);
  if (dripRate > 0.001) 
    drawCylinder(b5,b6,0.15);
  else
    drawCylinder(b7,b8,0.15);
  // tube
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, glpolished);
  b1.setXYZ(0.0,5.0,-10.0);
  b2.setXYZ(0.0,15.0,-10.0);
  drawCylinder(b1,b2,0.5);
  // end
  b1.setXYZ(0.0,4.0,-10.0);
  b2.setXYZ(0.0,2.0,-10.0);
  drawCylinder(b1,b2,0.4,0.1);
  // marks
  b1.setXYZ(0.0,5.95,-10.0);
  b2.setXYZ(0.0,6.05,-10.0);
  drawCylinder(b1,b2,0.5);
  b1.setXYZ(0.0,6.95,-10.0);
  b2.setXYZ(0.0,7.05,-10.0);
  drawCylinder(b1,b2,0.5);
  b1.setXYZ(0.0,7.95,-10.0);
  b2.setXYZ(0.0,8.05,-10.0);
  drawCylinder(b1,b2,0.5);

  // "new" beaker
  const GLfloat glbeaker[]={ beakerRed, beakerGreen, beakerBlue, beakerAlpha };
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glbeaker);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, gldull);
  b1.setXYZ(0.0,beakerHeight,-10.0);
  b2.setXYZ(0.0,-5.0,-10.0);
  drawCylinder(b1,b2,-2.0);

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, beakerglass);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, gldull);
  b1.setXYZ(0.0,beakerHeight,-10.0);
  b2.setXYZ(0.0,-1.0,-10.0);
  drawCylinder(b1,b2,2.0);

  /*
  // beaker (empty)
  glColor3d( 0.5, 0.5, 0.5 );
  b1.setXYZ(0.0,beakerHeight,-10.0);
  b2.setXYZ(0.0,-1.0,-10.0);
  drawCylinder(b1,b2,2.0);
  // beaker (liquid)
  glColor3d( beakerRed, beakerGreen, beakerBlue );
  b1.setXYZ(0.0,beakerHeight,-10.0);
  b2.setXYZ(0.0,-4.0,-10.0);
  drawCylinder(b1,b2,2.0);
  */

  // floor
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glblue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
  glMaterialfv(GL_FRONT, GL_SHININESS, glpolished);
  b1.setXYZ(0.0,-5.0,-10.0);
  drawDisk(b1,100.0);

  // drip
  if (dripRate > 0.001) {  
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, glwater);
    glMaterialfv(GL_FRONT, GL_SPECULAR, glwhite);
    glMaterialfv(GL_FRONT, GL_SHININESS, glpolished);
    b1.setXYZ(0.0,2.0,-10.0);
    b2.setXYZ(0.0,beakerHeight,-10.0);
    drawCylinder(b1,b2,0.1,0.1);    
  }

  // text status
}

void Titration::resizeGL(int w, int h) {
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

void Titration::keyPressEvent(QKeyEvent *e1) {
}

