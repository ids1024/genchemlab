#ifndef CHEM_DIALOG_H
#define CHEM_DIALOG_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>

#include "chemicals.h"

class TitrationDialog:public QDialog {
  Q_OBJECT
public:
  TitrationDialog(QWidget *parent = 0, QString name = 0);
  int selectedChemical();
  int getBuretChoice();
  int getIndicator();

  // I know, these should be private, but why complicate things?
  QComboBox *chemCompound, *chemUnits, *buretChoice, *inChoice;
  QLineEdit *mass1, *mass2;
  QCheckBox *addWater;
  int con1;
};

#endif
