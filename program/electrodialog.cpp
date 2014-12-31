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
  QDialog(parent)
{
  setObjectName(name);
  setModal(true);
  setWindowTitle( tr("Set up electrochemistry experiment") );
  QLabel *l1 = new QLabel(this);
  l1->setObjectName("Left beaker");
  l1->setGeometry(10,10,80,30);
  l1->setText( tr("Left beaker") );
  QLabel *l2 = new QLabel(this);
  l2->setObjectName("Right beaker");
  l2->setGeometry(200,10,80,30);
  l2->setText( tr("Right beaker") );
  QLabel *l3 = new QLabel(this);
  l3->setObjectName("Left beaker");
  l3->setGeometry(10,50,150,30);
  l3->setText( tr("Electrode metal:") );
  QLabel *l4 = new QLabel(this);
  l2->setObjectName("Right beaker");
  l4->setGeometry(200,50,150,30);
  l4->setText( tr("Electrode metal:") );
  m1Choice = new QComboBox(this);
  m1Choice->insertItem( 0, tr("Silver") );
  m1Choice->insertItem( 1, tr("Copper") );
  m1Choice->insertItem( 2, tr("Lead") );
  m1Choice->insertItem( 3, tr("Tin") );
  m1Choice->insertItem( 4, tr("Nickel") );
  m1Choice->insertItem( 5, tr("Iron(II)") );
  m1Choice->insertItem( 6, tr("Chromium") );
  m1Choice->insertItem( 7, tr("Zinc") );
  m1Choice->setGeometry(10,90,160,30);
  m1Choice->setCurrentIndex(inm1 - 6000);
  m2Choice = new QComboBox(this);
  m2Choice->insertItem( 0, tr("Silver") );
  m2Choice->insertItem( 1, tr("Copper") );
  m2Choice->insertItem( 2, tr("Lead") );
  m2Choice->insertItem( 3, tr("Tin") );
  m2Choice->insertItem( 4, tr("Nickel") );
  m2Choice->insertItem( 5, tr("Iron(II)") );
  m2Choice->insertItem( 6, tr("Chromium") );
  m2Choice->insertItem( 7, tr("Zinc") );
  m2Choice->setGeometry(200,90,160,30);
  m2Choice->setCurrentIndex(inm2 - 6000);
  QLabel *l5 = new QLabel(this);
  l5->setObjectName("Left beaker");
  l5->setGeometry(10,130,150,30);
  l5->setText( tr("Salt concentration (M):") );
  QLabel *l6 = new QLabel(this);
  l6->setObjectName("Right beaker");
  l6->setGeometry(200,130,150,30);
  l6->setText( tr("Salt concentration (M):") );
  QString tmp1;
  m1Conc = new QLineEdit(this);
  m1Conc->setObjectName("m1conc");
  tmp1.setNum(inconc1);
  m1Conc->setText(tmp1);
  m1Conc->setGeometry(10,170,150,30);
  m2Conc = new QLineEdit(this);
  m2Conc->setObjectName("m2conc");
  tmp1.setNum(inconc1);
  tmp1.setNum(inconc2);
  m2Conc->setText(tmp1);
  m2Conc->setGeometry(200,170,150,30);
  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(60,210,80,30);
  ok->setPalette(QPalette(Qt::lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(260,210,80,30);
  dismiss->setPalette(QPalette(Qt::lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int ElectroDialog::getMetal1() {
  // see chemicals.h
  return 6000 + m1Choice->currentIndex();
}

int ElectroDialog::getMetal2() {
  // see chemicals.h
  return 6000 + m2Choice->currentIndex();
}

double ElectroDialog::getConc1() {
  return m1Conc->text().toDouble();
}

double ElectroDialog::getConc2() {
  return m2Conc->text().toDouble();
}
