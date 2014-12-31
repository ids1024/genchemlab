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

#include "textdialog.h"

TextDialog::TextDialog(QString t1, QWidget *parent, QString name):
        QDialog(parent)
{
  setObjectName(name);
  setModal(TRUE);
  setWindowTitle( tr("Output") );
  QTextBrowser *te = new QTextBrowser(this);
  te->setObjectName("good label");
  te->setText(t1);
  te->setGeometry(10,10,230,200);
  QPushButton *ok;
  ok = new QPushButton(tr("OK"), this);
  ok->setGeometry(85,250,80,30);
  ok->setPalette(QPalette(Qt::lightGray));
  connect(ok, SIGNAL(clicked()), SLOT(accept()) );
}
