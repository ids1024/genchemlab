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
        QDialog(parent, name, TRUE)
{
  setCaption( tr("Choose indicator") );
  QLabel *l1 = new QLabel(this, "Paper size label");
  l1->setGeometry(10,10,80,30);
  l1->setText( tr("Indicator: ") );
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
  inChoice->setGeometry(100,10,140,30);
  QPushButton *ok, *dismiss;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(50,70,80,30);
  ok->setPalette(QPalette(lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
  dismiss = new QPushButton(tr("Cancel"), this);
  dismiss->setGeometry(140,70,80,30);
  dismiss->setPalette(QPalette(lightGray));
  connect(dismiss, SIGNAL(clicked()), SLOT(reject()) );
}

int IndicatorDialog::getIndicator() {
  // see chemicals.h
  return 999 + inChoice->currentItem() * 10;
}
