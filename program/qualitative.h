#ifndef QUALITATIVE_H
#define QUALITATIVE_H

#include <qgl.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qtimer.h>
//Added by qt3to4:
#include <QMouseEvent>
#include <QKeyEvent>

#include "point3d.h"
#include "labwidget.h"
#include "chemicals.h"

class Qualitative : public LabWidget {
  Q_OBJECT
    public:
  Qualitative(QWidget *parent = 0, const char *name = 0);
  void drawLine(Point3D, Point3D);
  void drawCylinder(Point3D, Point3D, double);
  void drawCylinder(Point3D, Point3D, double, double);
  void drawSphere(Point3D, double);
  void drawBox(Point3D, Point3D);
  void addIndicator(int);

  public slots:
    void process();
  void calculate();
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
  double dripRate, pH, beakerVolume, dispenseVolume;
  int startY, cn;
  bool drawDrip;
  double beakerRed, beakerGreen, beakerBlue, beakerAlpha;
  double beakerHeight;
  QTimer *t;
  double beakerConc, buretConc;
  int beakerSubstance, buretSubstance, indicator;
  double cations_sol[8], anions_sol[8], cations_pel[8], anions_pel[8];
};

#endif
