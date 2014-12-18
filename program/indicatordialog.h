#ifndef INDIALOG_H
#define INDIALOG_H

#include <qdialog.h>
#include <qcombobox.h>

#include "chemicals.h"

class IndicatorDialog:public QDialog {
  Q_OBJECT
public:
  IndicatorDialog(QWidget *parent = 0, QString name = 0);
  int getIndicator();

private:
  QComboBox *inChoice;
};

#endif
