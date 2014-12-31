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

#include "indicatordialog.h"

IndicatorDialog::IndicatorDialog(QWidget *parent, QString name):
        QDialog(parent)
{
  setObjectName(name);
  setModal(TRUE);
  setWindowTitle( tr("Choose indicator") );
  QLabel *l1 = new QLabel(this);
  l1->setObjectName("Paper size label");
  l1->setGeometry(10,10,80,30);
  l1->setText( tr("Indicator: ") );
  inChoice = new QComboBox(this);
  inChoice->insertItem( 0, tr("<none>") );
  inChoice->insertItem( 1, tr("Bromophenol blue") );
  inChoice->insertItem( 2, tr("Bromothymol blue") );
  inChoice->insertItem( 3, tr("Litmus") );
  inChoice->insertItem( 4, tr("Methyl orange") );
  inChoice->insertItem( 5, tr("Methyl red") );
  inChoice->insertItem( 6, tr("Phenol red") );
  inChoice->insertItem( 7, tr("Phenolphthalein") );
  //inChoice->insertItem( tr("Starch") );
  inChoice->setGeometry(100,10,140,30);
  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(50,70,80,30);
  ok->setPalette(QPalette(Qt::lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(140,70,80,30);
  dismiss->setPalette(QPalette(Qt::lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int IndicatorDialog::getIndicator() {
  // see chemicals.h
  return 999 + inChoice->currentIndex() * 10;
}
