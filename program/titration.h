#ifndef TITRATION_H
#define TITRATION_H

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

class Titration : public LabWidget {
  Q_OBJECT
    public:
  Titration(QWidget *parent = 0, const char *name = 0);
  void addToBuret(double, int);
  void addToBeaker(double, int, double);
  void addIndicator(int);
  int whichLab() { return 1; }

  public slots:
    void process();
  void calculate();
  void indicatorColor();
  void showInfo();
  void saveData();
  void showSetup();

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
  double rememberMass1;
  double beakerRed, beakerGreen, beakerBlue, beakerAlpha;
  double beakerHeight;
  QTimer *t;
  double beakerConc, beakerCounterionConc, fudgeFactor;
  int beakerSubstance, buretSubstance, indicator;
};

#endif
