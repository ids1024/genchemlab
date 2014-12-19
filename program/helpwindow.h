/****************************************************************************
** $Id:  qt/helpwindow.h   3.0.3   edited Oct 12 12:18 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <q3mainwindow.h>
#include <q3textbrowser.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qdir.h>
//Added by qt3to4:
#include <Q3PopupMenu>

class QComboBox;
class Q3PopupMenu;

class HelpWindow : public Q3MainWindow
{
    Q_OBJECT
public:
    HelpWindow( const QString& home_,  const QString& path, QWidget* parent = 0, const char *name=0 );
    ~HelpWindow();

private slots:
    void setBackwardAvailable( bool );
    void setForwardAvailable( bool );

    void textChanged();
    void about();

    void pathSelected( const QString & );
    
private:
    Q3TextBrowser* browser;
    QComboBox *pathCombo;
    int backwardId, forwardId;
    QString selectedURL;
};





#endif

