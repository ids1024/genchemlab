/****************************************************************************
** $Id:  qt/application.h   3.0.3   edited Oct 12 12:18 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <qmainwindow.h>
//Added by qt3to4:
#include <QCloseEvent>
#include <QLabel>

#include "labwidget.h"

class Q3TextEdit;
class QLabel;
class Titration;

class ApplicationWindow: public QMainWindow
{
    Q_OBJECT

public:
    ApplicationWindow();
    ~ApplicationWindow();

    public slots:
      void slotSetStatusBar(QString);

protected:
    void closeEvent( QCloseEvent* );

private slots:
    void newDoc();
    void choose();
    void load( const QString &fileName );
    void save();
    void saveAs();

    void switchToWidget1();
    void switchToWidget2();
    void switchToWidget3();
    void switchToWidget4();
    void switchToWidget5();
    void switchToWidget10();
    void switchToWidget11();
    void switchToWidget12();

    void addToBeaker();
    void addToBuret();
    void addIndicator();

    void about();
    void getHelp();
    void doNothing();

private:
    //QTextEdit *e;
    QString filename;
    LabWidget *t1;
    QAction * select_id;
    int widget_id;
};


#endif
