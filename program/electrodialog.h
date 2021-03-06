#ifndef EDIALOG_H
#define EDIALOG_H

#include <qdialog.h>
#include <qcombobox.h>

#include "chemicals.h"

class ElectroDialog:public QDialog {
  Q_OBJECT
public:
  ElectroDialog(int, int, double, double, 
		QWidget *parent = 0, QString name = 0);
  int getMetal1();
  int getMetal2();
  double getConc1();
  double getConc2();

private:
  QComboBox *m1Choice, *m2Choice;
  QLineEdit *m1Conc, *m2Conc;
};

#endif
