/****************************************************************************
** $Id:  qt/main.cpp   3.0.3   edited Oct 12 12:18 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>
#include <qtranslator.h>
#include <QLocale>
#include <QLibraryInfo>
#include "application.h"

int main( int argc, char ** argv ) {
  QApplication a( argc, argv );
  // translation file for application strings
  QTranslator myapp( 0 );
  myapp.load( "genchemlab-" + QLocale::system().name(),
             QLibraryInfo::location(QLibraryInfo::TranslationsPath) );
  a.installTranslator( &myapp );
  ApplicationWindow * mw = new ApplicationWindow();
  mw->setWindowTitle( a.tr("General Chemistry Lab Simulator") );
  mw->show();
  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
  return a.exec();
}
