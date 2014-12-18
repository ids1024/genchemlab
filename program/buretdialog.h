#ifndef BURETDIALOG_H
#define BURETDIALOG_H

#include <qdialog.h>
#include <qcombobox.h>

#include "chemicals.h"

class BuretDialog:public QDialog {
  Q_OBJECT
public:
  BuretDialog(QWidget *parent = 0, QString name = 0);
  int getChoice();

private:
  QComboBox *buretChoice;
};

#endif
