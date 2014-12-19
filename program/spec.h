#ifndef SPEC_H
#define SPEC_H

#include <qgl.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qinputdialog.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>

#include "point3d.h"
#include "labwidget.h"
#include "chemicals.h"

class Spectrometry : public LabWidget {
  Q_OBJECT
    public:
  Spectrometry(QWidget *parent = 0, const char *name = 0);
  void drawLine(Point3D, Point3D);
  void drawCylinder(Point3D, Point3D, double);
  void drawCylinder(Point3D, Point3D, double, double);
  void drawSphere(Point3D, double);
  void drawBox(Point3D, Point3D);
  void addToBeaker(double, int, int, bool, double);
  void addIndicator(int);
  void setWavelength();
  void absScan();
  int whichLab() { return 4; }

  public slots:
    void process();
  void calculate();
  void calcAbs();
  void showInfo();
  void saveData();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL(int w, int h);
  void mousePressEvent(QMouseEvent *);
  void mouseReleaseEvent(QMouseEvent *);
  void mouseMoveEvent(QMouseEvent *);
  void keyPressEvent(QKeyEvent *);

 private:
  QRect buretHotSpot, beakerHotSpot, solventHotSpot, solidHotSpot;
  double dripRate, pH, beakerVolume, dispenseVolume, wavelength, absorbance;
  int startY, cn;
  bool drawDrip;
  double beakerRed, beakerGreen, beakerBlue, beakerAlpha;
  double beamRed, beamGreen, beamBlue, beamAlpha;
  double beakerHeight;
  QTimer *t;
  double beakerConc, buretConc, ec;
  int beakerSubstance, buretSubstance, indicator;
};

#endif
