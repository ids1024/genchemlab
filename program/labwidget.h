#ifndef LABWIDGET_H
#define LABWIDGET_H

#include <qgl.h>
#include <qwidget.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qtimer.h>

#include "point3d.h"

class LabWidget : public QGLWidget {
  Q_OBJECT
    public:
  LabWidget(QWidget *parent = 0, const char *name = 0);
  void drawLine(Point3D, Point3D);
  void drawCylinder(Point3D, Point3D, double);
  void drawCylinder(Point3D, Point3D, double, double);
  void drawSphere(Point3D, double);
  void drawDisk(Point3D, double);
  void drawBox(Point3D, Point3D);
  virtual int whichLab();

  public slots:
    virtual void showInfo();
  virtual void saveData();
  virtual void addToBuret(double, int);
  virtual void addToBeaker(double, int, int, bool, double);
  virtual void addIndicator(int);
  virtual void showSetup();
  virtual void calorimeterConstant();
  virtual void targetTemperature();
  virtual void setWavelength();
  virtual void absScan();
  virtual void Demo();

 signals:
  void signalSetStatusBar(QString);

 protected:
  bool setupDone;
};

#endif
