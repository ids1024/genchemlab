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

#include "buretdialog.h"

BuretDialog::BuretDialog(QWidget *parent, QString name):
        QDialog(parent, name, TRUE)
{
  setCaption( tr("Choose indicator") );
  QLabel *l1 = new QLabel(this, "Paper size label");
  l1->setGeometry(10,10,80,30);
  l1->setText( tr("Fill buret with: ") );
  buretChoice = new QComboBox(false, this);
  buretChoice->insertItem("0.01 M NaOH");
  buretChoice->insertItem("0.1 M NaOH");
  buretChoice->insertItem("1 M NaOH");
  buretChoice->insertItem("0.1 M HCl");
  //buretChoice->insertItem("0.1 M Na2S2O3");
  buretChoice->setGeometry(100,10,120,30);
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

int BuretDialog::getChoice() {
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
