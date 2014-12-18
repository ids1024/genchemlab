#ifndef BPFP_H
#define BPFP_H

#include <qgl.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qtimer.h>

#include "point3d.h"
#include "labwidget.h"
#include "chemicals.h"

class Bpfp : public LabWidget {
  Q_OBJECT
    public:
  Bpfp(QWidget *parent = 0, const char *name = 0);
  void addToBeaker(double, int, int, bool, double);
  int whichLab() { return 3; }

  public slots:
    void process();
  void calculate();
  void showInfo();
  void saveData();
  void targetTemperature();

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
  double coldVolume, coldTemp, hotVolume, hotTemp, targetTemp, freezeTemp;
};

#endif
