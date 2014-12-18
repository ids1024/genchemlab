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

#include "electrodialog.h"
#include "chemicals.h"

ElectroDialog::ElectroDialog(int inm1, int inm2,double inconc1, double inconc2,
			     QWidget *parent, QString name):
  QDialog(parent, name, TRUE)
{
  setCaption( tr("Set up electrochemistry experiment") );
  QLabel *l1 = new QLabel(this, "Left beaker");
  l1->setGeometry(10,10,80,30);
  l1->setText( tr("Left beaker") );
  QLabel *l2 = new QLabel(this, "Right beaker");
  l2->setGeometry(200,10,80,30);
  l2->setText( tr("Right beaker") );
  QLabel *l3 = new QLabel(this, "Left beaker");
  l3->setGeometry(10,50,150,30);
  l3->setText( tr("Electrode metal:") );
  QLabel *l4 = new QLabel(this, "Right beaker");
  l4->setGeometry(200,50,150,30);
  l4->setText( tr("Electrode metal:") );
  m1Choice = new QComboBox(false, this);
  m1Choice->insertItem( tr("Silver") );
  m1Choice->insertItem( tr("Copper") );
  m1Choice->insertItem( tr("Lead") );
  m1Choice->insertItem( tr("Tin") );
  m1Choice->insertItem( tr("Nickel") );
  m1Choice->insertItem( tr("Iron(II)") );
  m1Choice->insertItem( tr("Chromium") );
  m1Choice->insertItem( tr("Zinc") );
  m1Choice->setGeometry(10,90,160,30);
  m1Choice->setCurrentItem(inm1 - 6000);
  m2Choice = new QComboBox(false, this);
  m2Choice->insertItem( tr("Silver") );
  m2Choice->insertItem( tr("Copper") );
  m2Choice->insertItem( tr("Lead") );
  m2Choice->insertItem( tr("Tin") );
  m2Choice->insertItem( tr("Nickel") );
  m2Choice->insertItem( tr("Iron(II)") );
  m2Choice->insertItem( tr("Chromium") );
  m2Choice->insertItem( tr("Zinc") );
  m2Choice->setGeometry(200,90,160,30);
  m2Choice->setCurrentItem(inm2 - 6000);
  QLabel *l5 = new QLabel(this, "Left beaker");
  l5->setGeometry(10,130,150,30);
  l5->setText( tr("Salt concentration (M):") );
  QLabel *l6 = new QLabel(this, "Right beaker");
  l6->setGeometry(200,130,150,30);
  l6->setText( tr("Salt concentration (M):") );
  QString tmp1;
  m1Conc = new QLineEdit(this, "m1conc");
  tmp1.setNum(inconc1);
  m1Conc->setText(tmp1);
  m1Conc->setGeometry(10,170,150,30);
  m2Conc = new QLineEdit(this, "m2conc");
  tmp1.setNum(inconc2);
  m2Conc->setText(tmp1);
  m2Conc->setGeometry(200,170,150,30);
  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(60,210,80,30);
  ok->setPalette(QPalette(lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(260,210,80,30);
  dismiss->setPalette(QPalette(lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int ElectroDialog::getMetal1() {
  // see chemicals.h
  return 6000 + m1Choice->currentItem();
}

int ElectroDialog::getMetal2() {
  // see chemicals.h
  return 6000 + m2Choice->currentItem();
}

double ElectroDialog::getConc1() {
  return m1Conc->text().toDouble();
}

double ElectroDialog::getConc2() {
  return m2Conc->text().toDouble();
}
