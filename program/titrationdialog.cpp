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
#include <qlayout.h>

#include "titrationdialog.h"

TitrationDialog::TitrationDialog(QWidget *parent, QString name):
	QDialog(parent)
{
  setObjectName(name);
  setModal(TRUE);
  setWindowTitle( tr("Set up titration") );
  QLabel *l1 = new QLabel(this);
  l1->setObjectName("Paper size label");
  l1->setGeometry(10,10,30,30);
  l1->setText( tr("Add") );
  mass1 = new QLineEdit(this);
  mass1->setGeometry(50,10,50,30);
  QLabel *l2 = new QLabel(this);
  l2->setObjectName("Paper size label");
  l2->setGeometry(120,10,60,30);
  l2->setText( tr("grams of") );
  chemCompound = new QComboBox(this);
  chemCompound->setObjectName("compound");
  chemCompound->setGeometry(200,10,120,30);
  chemCompound->insertItem(0, tr("KHP"));
  chemCompound->insertItem(1, tr("acetic acid"));
  chemCompound->insertItem(2, tr("citric acid"));
  chemCompound->insertItem(3, tr("ascorbic acid"));
  chemCompound->insertItem(4, tr("phosphoric acid"));
  chemCompound->insertItem(5, tr("cola"));    

  QLabel *l4 = new QLabel( this);
  l4->setObjectName("Dissolve");
  l4->setText( tr("Dissove in ") );
  l4->setGeometry(10,60,100,30);
  mass2 = new QLineEdit(this);
  mass2->setText("0.0");
  mass2->setGeometry(85,60,50,30);
  QLabel *l3 = new QLabel(this);
  l3->setObjectName("mL-label2");
  l3->setGeometry(150,60,90,30);
  l3->setText( tr("mL of water") );

  QLabel *l10 = new QLabel(this);
  l10->setObjectName("Paper size label");
  l10->setGeometry(10,100,80,30);
  l10->setText( tr("Fill buret with: ") );
  buretChoice = new QComboBox(this);
  buretChoice->insertItem(0, "0.01 M NaOH");
  buretChoice->insertItem(1, "0.1 M NaOH");
  buretChoice->insertItem(2, "1 M NaOH");
  buretChoice->insertItem(3, "0.1 M HCl");
  //buretChoice->insertItem(4, "0.1 M Na2S2O3");
  buretChoice->setGeometry(110,100,120,30);

  QLabel *l11 = new QLabel(this);
  l11->setObjectName("Paper size label");
  l10->setGeometry(10,100,80,30);
  l11->setGeometry(10,150,80,30);
  l11->setText( tr("Indicator: ") );
  inChoice = new QComboBox(this);
  inChoice->insertItem( 0, tr("<none>") );
  inChoice->insertItem( 1, tr("Bromophenol blue") );
  inChoice->insertItem( 2, tr("Bromothymol blue") );
  inChoice->insertItem( 3, tr("Litmus") );
  inChoice->insertItem( 4, tr("Methyl orange") );
  inChoice->insertItem( 5, tr("Methyl red") );
  inChoice->insertItem( 6, tr("Phenol red") );
  inChoice->insertItem( 7, tr("Phenolphthalein") );
  //inChoice->insertItem( 8, tr("Starch") );
  inChoice->setGeometry(100,150,140,30);

  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(50,200,80,30);
  ok->setPalette(QPalette(Qt::lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(140,200,80,30);
  dismiss->setPalette(QPalette(Qt::lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int TitrationDialog::selectedChemical() {
  int cx, cret;
  
  cx = chemCompound->currentIndex();
  
  cret = cx;

  if (cx == 0) cret = KHP;
  if (cx == 1) cret = ACETIC_ACID;
  if (cx == 2) cret = CITRIC_ACID;
  if (cx == 3) cret = ASCORBIC_ACID;
  if (cx == 4) cret = H3PO4;
  if (cx == 5) cret = COLA;

  return cret;
}

int TitrationDialog::getBuretChoice() {
  int cx = buretChoice->currentIndex();

  int cret = 0;

  switch(cx) {
  case 0:
    cret = BASE_001M_NAOH;
    break;
  case 1:
    cret = BASE_01M_NAOH;
    break;
  case 2:
    cret = BASE_1M_NAOH;
    break;
  case 3:
    cret = ACID_HCL;
    break;
  case 4:
    cret = BASE_01M_NA2S2O3;
    break;
  default:
    break;
  }

  return cret;
}

int TitrationDialog::getIndicator() {
  // see chemicals.h
  return 990 + inChoice->currentIndex() * 10;
}
