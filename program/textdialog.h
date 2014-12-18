#ifndef TEXTDIALOG_H
#define TEXTDIALOG_H

#include <qdialog.h>
#include <qtextbrowser.h>

class TextDialog:public QDialog {
  Q_OBJECT
public:
  TextDialog(QString, QWidget *parent = 0, QString name = 0);
};

#endif
