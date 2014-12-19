#ifndef CALORIMETRY_H
#define CALORIMETRY_H

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

class Calorimetry : public LabWidget {
  Q_OBJECT
    public:
  Calorimetry(QWidget *parent = 0, const char *name = 0);
  void addToBeaker(double, int, int, bool, double);
  int whichLab() { return 2; }

  public slots:
    void process();
  void calculate();
  void showInfo();
  void saveData();
  void calorimeterConstant();

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
  double dripRate, pH, temperature, beakerVolume, runTime;
  int startY, cn, timeCount;
  bool drawDrip, running;
  double beakerRed, beakerGreen, beakerBlue, beakerAlpha;
  double beakerHeight, thermoHeight;
  QTimer *t;
  double beakerConc, calConstant, fudgeFactor, increment;
  int beakerSubstance, cmode, steps;
  double coldVolume, coldTemp, hotVolume, hotTemp, targetTemp;
};

#endif
