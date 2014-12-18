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
	QDialog(parent, name, TRUE)
{
  setCaption( tr("Set up titration") );
  QLabel *l1 = new QLabel(this, "Paper size label");
  l1->setGeometry(10,10,30,30);
  l1->setText( tr("Add") );
  mass1 = new QLineEdit(this);
  mass1->setGeometry(50,10,50,30);
  QLabel *l2 = new QLabel(this, "Paper size label");
  l2->setGeometry(120,10,60,30);
  l2->setText( tr("grams of") );
  chemCompound = new QComboBox(false, this, "compound");
  chemCompound->setGeometry(200,10,120,30);
  chemCompound->insertItem(tr("KHP"));
  chemCompound->insertItem(tr("acetic acid"));
  chemCompound->insertItem(tr("citric acid"));
  chemCompound->insertItem(tr("ascorbic acid"));
  chemCompound->insertItem(tr("phosphoric acid"));
  chemCompound->insertItem(tr("cola"));    

  QLabel *l4 = new QLabel( this, "Dissolve");
  l4->setText( tr("Dissove in ") );
  l4->setGeometry(10,60,100,30);
  mass2 = new QLineEdit(this);
  mass2->setText("0.0");
  mass2->setGeometry(85,60,50,30);
  QLabel *l3 = new QLabel(this, "mL-label2");
  l3->setGeometry(150,60,90,30);
  l3->setText( tr("mL of water") );

  QLabel *l10 = new QLabel(this, "Paper size label");
  l10->setGeometry(10,100,80,30);
  l10->setText( tr("Fill buret with: ") );
  buretChoice = new QComboBox(false, this);
  buretChoice->insertItem("0.01 M NaOH");
  buretChoice->insertItem("0.1 M NaOH");
  buretChoice->insertItem("1 M NaOH");
  buretChoice->insertItem("0.1 M HCl");
  //buretChoice->insertItem("0.1 M Na2S2O3");
  buretChoice->setGeometry(110,100,120,30);

  QLabel *l11 = new QLabel(this, "Paper size label");
  l11->setGeometry(10,150,80,30);
  l11->setText( tr("Indicator: ") );
  inChoice = new QComboBox(false, this);
  inChoice->insertItem( tr("<none>") );
  inChoice->insertItem( tr("Bromophenol blue") );
  inChoice->insertItem( tr("Bromothymol blue") );
  inChoice->insertItem( tr("Litmus") );
  inChoice->insertItem( tr("Methyl orange") );
  inChoice->insertItem( tr("Methyl red") );
  inChoice->insertItem( tr("Phenol red") );
  inChoice->insertItem( tr("Phenolphthalein") );
  //inChoice->insertItem( tr("Starch") );
  inChoice->setGeometry(100,150,140,30);

  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(50,200,80,30);
  ok->setPalette(QPalette(lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(140,200,80,30);
  dismiss->setPalette(QPalette(lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int TitrationDialog::selectedChemical() {
  int cx, cret;
  
  cx = chemCompound->currentItem();
  
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
  int cx = buretChoice->currentItem();

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
  return 990 + inChoice->currentItem() * 10;
}
