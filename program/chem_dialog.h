#ifndef CHEM_DIALOG_H
#define CHEM_DIALOG_H

#include <qdialog.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qcheckbox.h>

#include "chemicals.h"

class ChemDialog:public QDialog {
  Q_OBJECT
public:
  ChemDialog(int, QWidget *parent = 0, QString name = 0);
  int selectedChemical();

  // I know, these should be private, but why complicate things?
  QComboBox *chemCompound, *chemUnits;
  QLineEdit *mass1, *mass2;
  QCheckBox *addWater;
  int con1;
};

#endif
