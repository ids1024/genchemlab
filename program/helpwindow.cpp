/****************************************************************************
** $Id:  qt/helpwindow.cpp   3.0.3   edited Mar 13 14:37 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "helpwindow.h"
#include <qstatusbar.h>
#include <qpixmap.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qicon.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qapplication.h>
#include <qcombobox.h>
#include <qevent.h>
#include <qlineedit.h>
#include <qobject.h>
#include <qfileinfo.h>
#include <qfile.h>
#include <qdatastream.h>
#include <qprinter.h>
#include <qpainter.h>
//Added by qt3to4:
#include <QFrame>

#include <ctype.h>

#include "back.xpm"
#include "forward.xpm"
#include "home.xpm"

HelpWindow::HelpWindow( const QString& home_,
			QWidget* parent, const char *name )
//    : QMainWindow( parent, name, Qt::WDestructiveClose ), TODO: Important?
    : QMainWindow( parent ),
      pathCombo( 0 ), selectedURL()
{
    setObjectName(name);
    browser = new QTextBrowser( this );

    //browser->mimeSourceFactory()->setFilePath( _path ); TODO: NEEDED?
    browser->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    connect( browser, SIGNAL( textChanged() ),
	     this, SLOT( textChanged() ) );

    setCentralWidget( browser );

    if ( !home_.isEmpty() )
	browser->setSource( home_ );

    connect( browser, SIGNAL( highlighted( const QString&) ),
	     statusBar(), SLOT( message( const QString&)) );

    resize( 640,700 );

    QMenu* file = menuBar()->addMenu( tr("&File") );;
    file->addAction( tr("&Close"), this, SLOT( close() ), Qt::CTRL+Qt::Key_W );

    // The same three icons are used twice each.
    QIcon icon_back( QPixmap( ":../doc/back.xpm" ) );
    QIcon icon_forward( QPixmap( ":../doc/forward.xpm" ) );
    QIcon icon_home( QPixmap( ":../doc/home.xpm" ) );

    QMenu* go = menuBar()->addMenu( tr("&Go") );
    backward_action = go->addAction( icon_back,
				 tr("&Backward"), browser, SLOT( backward() ),
				 Qt::CTRL+Qt::Key_Left );
    forward_action = go->addAction( icon_forward,
				tr("&Forward"), browser, SLOT( forward() ),
				Qt::CTRL+Qt::Key_Right );
    go->addAction( icon_home, tr("&Home"), browser, SLOT( home() ) );

    menuBar()->addSeparator();

    QMenu* help = menuBar()->addMenu( tr("&Help") );
    help->addAction( tr("&About ..."), this, SLOT( about() ) );

    forward_action->setEnabled(FALSE);
    backward_action->setEnabled(FALSE);
    connect( browser, SIGNAL( backwardAvailable( bool ) ),
	     this, SLOT( setBackwardAvailable( bool ) ) );
    connect( browser, SIGNAL( forwardAvailable( bool ) ),
	     this, SLOT( setForwardAvailable( bool ) ) );


    QToolBar* toolbar = addToolBar( "ToolBar" );
    QToolButton* button;

    button = new QToolButton( toolbar );
    button->setIcon(icon_back);
    button->setText(tr("Backward"));
    connect( button, SIGNAL(triggered(QAction)), browser, SLOT(backward()) );
    connect( browser, SIGNAL( backwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
    button->setEnabled( FALSE );
    button = new QToolButton( toolbar );
    button->setIcon(icon_forward);
    button->setText(tr("Forward"));
    connect( button, SIGNAL(triggered(QAction)), browser, SLOT(forward()) );
    connect( browser, SIGNAL( forwardAvailable(bool) ), button, SLOT( setEnabled(bool) ) );
    button->setEnabled( FALSE );
    button = new QToolButton( toolbar );
    button->setIcon(icon_home);
    button->setText(tr("Home"));
    connect( button, SIGNAL(triggered(QAction)), browser, SLOT(home()) );


    toolbar->addSeparator();

    pathCombo = new QComboBox( toolbar );
    connect( pathCombo, SIGNAL( activated( const QString & ) ),
	     this, SLOT( pathSelected( const QString & ) ) );
    //toolbar->setStretchableWidget( pathCombo ); TODO: See if it works without this
    //setRightJustification( TRUE ); TODO: Fix if necessary
    //Commented out following: TODO: Important?
//#if QT_VERSION >= 300
//    setDockEnabled( Qt::DockLeft, FALSE );
//    setDockEnabled( Qt::DockRight, FALSE );
//#endif

    pathCombo->insertItem( 0, home_ );
    browser->setFocus();

}


void HelpWindow::setBackwardAvailable( bool b)
{
    backward_action->setEnabled(b);
}

void HelpWindow::setForwardAvailable( bool b)
{
    forward_action->setEnabled(b);
}


void HelpWindow::textChanged()
{
    if ( browser->documentTitle().isNull() )
	setWindowTitle( "GenChemLab - Help viewer - " + browser->source().toString() );
    else
	setWindowTitle( "GenChemLab - Help viewer - " + browser->documentTitle() ) ;

    selectedURL = browser->source().toString();

    if ( !selectedURL.isEmpty() && pathCombo ) {
	bool exists = FALSE;
	int i;
	for ( i = 0; i < pathCombo->count(); ++i ) {
	    if ( pathCombo->itemText( i ) == selectedURL ) {
		exists = TRUE;
		break;
	    }
	}
	if ( !exists ) {
	    pathCombo->insertItem( 0, selectedURL );
	    pathCombo->setCurrentIndex( 0 );
	} else
	    pathCombo->setCurrentIndex( i );
	selectedURL = QString::null;
    }
}

HelpWindow::~HelpWindow()
{

}

void HelpWindow::about()
{
    QMessageBox::about( this, "HelpViewer Example",
			"<p>This example implements a simple HTML help viewer "
			"using Qt's rich text capabilities</p>"
			"<p>It's just about 100 lines of C++ code, so don't expect too much :-)</p>"
			);
}

void HelpWindow::pathSelected( const QString &_path )
{
    browser->setSource( _path );
}
