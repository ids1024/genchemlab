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

#include <qmainwindow.h>
#include <qtextbrowser.h>
#include <qstringlist.h>
#include <qmap.h>
#include <qdir.h>
//Added by qt3to4:
#include <QMenu>

class QComboBox;
class QMenu;

class HelpWindow : public QMainWindow
{
    Q_OBJECT
public:
    HelpWindow( const QString& home_, QWidget* parent = 0, const char *name=0 );
    ~HelpWindow();

private slots:
    void setBackwardAvailable( bool );
    void setForwardAvailable( bool );

    void textChanged();
    void about();

    void pathSelected( const QString & );
    
private:
    QTextBrowser* browser;
    QComboBox *pathCombo;
    QAction *backward_action, *forward_action;
    QString selectedURL;
};





#endif

