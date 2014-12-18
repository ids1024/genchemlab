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
#include <qtextcodec.h>
#include "application.h"

// these #include's are needed on Windows
#ifndef UNIX
#include <malloc.h>
#include <direct.h>
#endif

QString RingDir;

int main( int argc, char ** argv ) {
  // set library directory (RINGDIR)
#ifdef UNIX
  QString dname(RINGHOME);
  if (dname.right(1) != QString("/"))
    dname.append(QString("/"));
  dname.append("doc/");
#else
  char *pwd = (char*)malloc(sizeof(char) * 80);
  _getcwd(pwd, 80);
  QString dname;
  dname = pwd;
  dname.append("\\doc\\");
#endif
  RingDir = dname;
  // end RINGDIR

  QApplication a( argc, argv );
  // translation file for application strings
  QTranslator myapp( 0 );
  myapp.load( QString( "genchemlab-") + QTextCodec::locale(), RingDir );
  a.installTranslator( &myapp );
  ApplicationWindow * mw = new ApplicationWindow();
  mw->setCaption( a.tr("General Chemistry Lab Simulator") );
  mw->show();
  a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
  return a.exec();
}
