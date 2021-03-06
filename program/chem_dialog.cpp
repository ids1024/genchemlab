#include <qwidget.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qdialog.h>
#include <qcolor.h>
#include <qfont.h>
#include <qnamespace.h>
#include <qpixmap.h>
#include <qprinter.h>

#include "chem_dialog.h"

ChemDialog::ChemDialog(int config, QWidget *parent, QString name):
	QDialog(parent)
{
  setObjectName(name);
  setModal(true);
  con1 = config;
  if (config == 1)
    setWindowTitle( tr("Add chemical to beaker") );
  if (config == 2)
    setWindowTitle( tr("Add chemical to calorimeter") );
  QLabel *l1 = new QLabel(this);
  l1->setObjectName("Paper size label");
  l1->setGeometry(10,10,30,30);
  l1->setText( tr("Add") );
  mass1 = new QLineEdit(this);
  mass1->setGeometry(50,10,50,30);
  chemUnits = new QComboBox(this);
  chemUnits->setObjectName("Paper size");
  chemUnits->setGeometry(110,10,60,30);
  chemUnits->insertItem(0, tr("grams"));
  chemUnits->insertItem(1, tr("mL"));
  QLabel *l2 = new QLabel(this);
  l2->setObjectName("Paper size label");
  l2->setGeometry(180,10,18,30);
  l2->setText( tr("of") );
  chemCompound = new QComboBox(this);
  chemCompound->setObjectName("compound");
  chemCompound->setGeometry(200,10,110,30);
  if (config == 1) { // titration
    chemCompound->insertItem(0, tr("KHP"));
    chemCompound->insertItem(1, tr("acetic acid"));
    chemCompound->insertItem(2, tr("citric acid"));
    chemCompound->insertItem(3, tr("ascorbic acid"));
    chemCompound->insertItem(4, tr("phosphoric acid"));
    chemCompound->insertItem(5, tr("cola"));    
  }
  if (config == 2) { // calorimetry
    chemCompound->insertItem(0, tr("H2O2"));
    chemCompound->insertItem(1, tr("(NH4)2SO4"));
    chemCompound->insertItem(2, tr("MgSO4"));
  }
  if (config == 3) { // freezing point
    chemCompound->insertItem(0, tr("table salt"));
    chemCompound->insertItem(1, tr("calcium chloride"));
    chemCompound->insertItem(2, tr("table sugar"));
    chemCompound->insertItem(3, tr("aspirin"));
  }
  if (config == 4) { // spec
    chemCompound->insertItem(0, tr("cobalt chloride"));
    chemCompound->insertItem(1, tr("copper sulfate"));
    chemCompound->insertItem(2, tr("nickel sulfate"));
  }
  addWater = new QCheckBox( tr("Dissolve in water:"), this);
  addWater->setChecked(true);
  addWater->setGeometry(10,60,120,30);
  mass2 = new QLineEdit(this);
  mass2->setGeometry(135,60,50,30);
  QLabel *l3 = new QLabel(this);
  l3->setObjectName("mL-label2");
  l3->setGeometry(190,60,30,30);
  l3->setText( tr("mL") );
  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(50,120,80,30);
  ok->setPalette(QPalette(Qt::lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(140,120,80,30);
  dismiss->setPalette(QPalette(Qt::lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int ChemDialog::selectedChemical() {
  int cx, cret;
  
  cx = chemCompound->currentIndex();
  
  cret = cx;

  // translation tables
  if (con1 == 1) {
    if (cx == 0) cret = KHP;
    if (cx == 1) cret = ACETIC_ACID;
    if (cx == 2) cret = CITRIC_ACID;
    if (cx == 3) cret = ASCORBIC_ACID;
    if (cx == 4) cret = H3PO4;
    if (cx == 5) cret = COLA;
  }
  if (con1 == 2) {
    if (cx == 0) cret = H2O2;
    if (cx == 1) cret = NH4SO4;
    if (cx == 2) cret = MGSO4;
  }
  if (con1 == 3) {
    if (cx == 0) cret = NACL;
    if (cx == 1) cret = CACL2;
    if (cx == 2) cret = SUCROSE;
    if (cx == 3) cret = ASPIRIN;
  }
  if (con1 == 4) {
    if (cx == 0) cret = COCL2;
    if (cx == 1) cret = CUSO4;
    if (cx == 2) cret = NISO4;
  }

  return cret;
}
