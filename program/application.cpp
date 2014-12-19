/****************************************************************************
** $Id:  qt/application.cpp   3.0.3   edited Oct 12 12:18 $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <iostream>

#include <qimage.h>
#include <qpixmap.h>
#include <q3toolbar.h>
#include <qtoolbutton.h>
#include <qmenubar.h>
//#include <qtextedit.h>
#include <qlabel.h>
#include <qfile.h>
#include <q3filedialog.h>
#include <qstatusbar.h>
#include <qmessagebox.h>
#include <qprinter.h>
#include <qapplication.h>
#include <q3accel.h>
#include <q3textstream.h>
#include <qpainter.h>
#include <q3paintdevicemetrics.h>
#include <q3whatsthis.h>
//Added by qt3to4:
#include <QCloseEvent>

#include "application.h"
#include "titration.h"
#include "calorimetry.h"
#include "bpfp.h"
#include "vapor.h"
#include "electro.h"
#include "qualitative.h"
#include "spec.h"
#include "buretdialog.h"
#include "chem_dialog.h"
#include "indicatordialog.h"
#include "extract.h"
#include "helpwindow.h"
#include <QDesktopWidget>

extern QString RingDir;

ApplicationWindow::ApplicationWindow()
    : QMainWindow( 0, "example application main window", Qt::WDestructiveClose )
{
  t1 = 0;
  QMenu * file = menuBar()->addMenu( tr("&File") );
 
  file->addAction( tr("&New"), this, SLOT(newDoc()), Qt::CTRL+Qt::Key_N );

  /*
  int id;
  id = file->insertItem( tr("&Open..."),
			 this, SLOT(choose()), CTRL+Key_O );

  id = file->insertItem( tr("&Save"),
			 this, SLOT(save()), CTRL+Key_S );
  */

  file->addSeparator();

  file->addAction( tr("&Close"), this, SLOT(close()), Qt::CTRL+Qt::Key_W );

  file->addAction( tr("&Quit"), qApp, SLOT( closeAllWindows() ), Qt::CTRL+Qt::Key_Q );

  QMenu * switcher = new QMenu( tr("&Select Lab"), this );
  select_id = menuBar()->addMenu( switcher );

  switcher->addAction( tr("&Titration"), this, SLOT(switchToWidget1()) );
  //switcher->addAction( tr("&Qualitative analysis"), this, 
  //		SLOT(switchToWidget2()) );
  switcher->addAction( tr("&Calorimetry"), this, SLOT(switchToWidget3()) );
  switcher->addAction( tr("&Freezing point depression"), this, 
			SLOT(switchToWidget4()) );
  switcher->addAction( tr("&Vapor pressure"), this, 
			SLOT(switchToWidget10()) );
  switcher->addAction( tr("&Electrochemistry"), this, 
			SLOT(switchToWidget11()) );
  switcher->addAction( tr("&Spectrophotometry"), this, 
			SLOT(switchToWidget5()) );
  switcher->addAction( tr("E&xtraction"), this, 
			SLOT(switchToWidget12()) );

  menuBar()->addSeparator();

  QMenu * help = menuBar()->addMenu( tr("&Help") );

  help->addAction( tr("&Help"), this, SLOT(getHelp()), Qt::Key_F1 );
  help->addSeparator();
  help->addAction( tr("&About"), this, SLOT(about()) );
  //help->addAction( "What's &This", this, SLOT(whatsThis()), SHIFT+Key_F1 );

  //e = new QTextEdit( this, "editor" );
  //e->setFocus();
  //setCentralWidget( ll );
  statusBar()->message( tr("Choose a lab from the 'Select Lab' menu") );

  resize(400, 400);
}

ApplicationWindow::~ApplicationWindow()
{

}

void ApplicationWindow::addToBeaker() {
  if (widget_id == 10) {
    t1->addToBeaker( 0, 0, 0, 0, 0 );
    return;
  }
  if (widget_id == 11) {
    t1->addToBeaker( 0, 0, 0, 0, 0 );
    return;
  }
  if (widget_id == 12) {
    t1->addToBeaker( 0, 0, 0, 0, 0 );
    return;
  }
  ChemDialog ch1(widget_id);
  if ( !ch1.exec() ) return;
  t1->addToBeaker( ch1.mass1->text().toDouble(),
		   ch1.chemUnits->currentItem(),
		   ch1.selectedChemical(),
		   ch1.addWater->isChecked(),
		   ch1.mass2->text().toDouble() );
}

void ApplicationWindow::addToBuret() {
  BuretDialog bu1;
  if ( !bu1.exec() ) return;
  t1->addToBuret( 0.0, bu1.getChoice() );
}

void ApplicationWindow::addIndicator() {
  IndicatorDialog in1;
  if ( !in1.exec() ) return;
  t1->addIndicator( in1.getIndicator() );
}

void ApplicationWindow::newDoc()
{
  ApplicationWindow *ed = new ApplicationWindow;
  ed->setCaption( tr("General Chemistry Lab Simulator") );
  ed->show();
}

void ApplicationWindow::choose()
{
    QString fn = Q3FileDialog::getOpenFileName( QString::null, QString::null,
					       this);
    if ( !fn.isEmpty() )
	load( fn );
    else
	statusBar()->message( "Loading aborted", 2000 );
}


void ApplicationWindow::load( const QString &fileName )
{
    QFile f( fileName );
    if ( !f.open( QIODevice::ReadOnly ) )
	return;

    Q3TextStream ts( &f );
    setCaption( fileName );
    statusBar()->message( "Loaded document " + fileName, 2000 );
}


void ApplicationWindow::save()
{
    if ( filename.isEmpty() ) {
	saveAs();
	return;
    }

    QString text = "";
    QFile f( filename );
    if ( !f.open( QIODevice::WriteOnly ) ) {
	statusBar()->message( QString("Could not write to %1").arg(filename),
			      2000 );
	return;
    }

    Q3TextStream t( &f );
    t << text;
    f.close();

    setCaption( filename );

    statusBar()->message( QString( "File %1 saved" ).arg( filename ), 2000 );
}


void ApplicationWindow::saveAs()
{
    QString fn = Q3FileDialog::getSaveFileName( QString::null, QString::null,
					       this );
    if ( !fn.isEmpty() ) {
	filename = fn;
	save();
    } else {
	statusBar()->message( "Saving aborted", 2000 );
    }
}

void ApplicationWindow::closeEvent( QCloseEvent* ce )
{
  ce->accept();
}

void ApplicationWindow::getHelp() {
  QString home;
  if (t1 == 0) {
    home = "index.html";
  } else {
    if (t1->whichLab() == 1) home = "titration.html";
    if (t1->whichLab() == 2) home = "calorimetry.html";
    if (t1->whichLab() == 3) home = "bpfp.html";
    if (t1->whichLab() == 4) home = "spec.html";
    if (t1->whichLab() == 10) home = "vapor.html";
    if (t1->whichLab() == 11) home = "electro.html"; // electrochemistry
    if (t1->whichLab() == 12) home = "extract.html"; // extraction
    if (t1->whichLab() == 20) home = "index.html"; // qualitative
  }
  HelpWindow *help = new HelpWindow(home, RingDir, 0, "help viewer");
  help->setCaption( tr("GenChemLab - Help viewer") );
  if ( QApplication::desktop()->width() > 400
       && QApplication::desktop()->height() > 500 )
    help->show();
  else
    help->showMaximized();
}

void ApplicationWindow::about()
{
  QMessageBox::about( this, tr("About GenChemLab"),
		      "General Chemistry Lab Simulator v1.0\n"
		      "http://genchemlab.sourceforge.net/\n\n"
		      "Author: Bryan Herger\n"
		      "bherger@users.sourceforge.net");
}

void ApplicationWindow::switchToWidget1() {
  widget_id = 1;
  t1 = new Titration(this, "Lab1");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();
  menuBar()->removeAction(select_id);
  statusBar()->message( tr("Starting titration lab.") );

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Titration"), adder, -1, 1 );

  adder->insertItem( tr("&Set up"), t1, SLOT(showSetup()) );

  setCaption( tr("GenChemLab - Titration") );
  resize( 500, 500 );
}

void ApplicationWindow::switchToWidget2() {
  widget_id = 20;
  t1 = new Qualitative(this, "Lab2");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);
  statusBar()->message( tr("Starting qualitative analysis lab.") );
  setCaption( tr("GenChemLab - Qualitative analysis") );
}

void ApplicationWindow::switchToWidget3() {
  widget_id = 2;
  t1 = new Calorimetry(this, "Lab3");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Calorimetry"), adder, -1, 1 );

  adder->insertItem( tr("&Find calorimeter constant"), t1, 
		     SLOT(calorimeterConstant()) );
  adder->insertItem( tr("&Set up experiment"), this, SLOT(addToBeaker()) );  

  statusBar()->message( tr("Starting calorimetry lab.") );
  setCaption( tr("GenChemLab - Calorimetry") );
}

void ApplicationWindow::switchToWidget4() {
  widget_id = 3;
  t1 = new Bpfp(this, "Lab4");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Freezing-Point"), adder, -1, 1 );

  adder->insertItem( tr("&Set up experiment"), this, SLOT(addToBeaker()) );  
  adder->insertItem( tr("&Set target temperature"), t1, 
		     SLOT(targetTemperature()) );

  statusBar()->message( tr("Starting freezing point depression lab.") );
  setCaption( tr("GenChemLab - Freezing Point") );
}

void ApplicationWindow::switchToWidget5() {
  widget_id = 4;
  t1 = new Spectrometry(this, "Lab5");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Spectrophotometry"), adder, -1, 1 );

  adder->insertItem( tr("&Set wavelength"), t1, SLOT(setWavelength()) );  
  adder->insertItem( tr("&Wavelength scan"), t1, SLOT(absScan()) );  
  adder->insertItem( tr("&Make sample"), this, 
		     SLOT(addToBeaker()) );

  statusBar()->message( tr("Starting spectrophotometry lab.") );
  setCaption( tr("GenChemLab - Spectrophotometry") );
}

void ApplicationWindow::switchToWidget10() {
  widget_id = 10;
  t1 = new Vapor(this, "Lab10");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Vapor pressure"), adder, -1, 1 );

  adder->insertItem( tr("&Set up"), this, 
		     SLOT(addToBeaker()) );

  statusBar()->message( tr("Starting vapor pressure lab.") );
  setCaption( tr("GenChemLab - Vapor Pressure") );
}

void ApplicationWindow::switchToWidget11() {
  widget_id = 11;
  t1 = new Electro(this, "Lab11");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);

  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("&Electrochemistry"), adder, -1, 1 );

  adder->insertItem( tr("&Set up"), this, 
		     SLOT(addToBeaker()) );

  statusBar()->message( tr("Starting electrochemistry lab.") );
  setCaption( tr("GenChemLab - Electrochemistry") );
}

void ApplicationWindow::switchToWidget12() {
  widget_id = 12;
  t1 = new Extract(this, "Lab12");
  connect(t1, SIGNAL(signalSetStatusBar(QString)), this,
	  SLOT(slotSetStatusBar(QString)) );
  setCentralWidget( t1 );
  t1->show();

  menuBar()->removeAction(select_id);
  QMenu * adder = new QMenu( this );
  menuBar()->insertItem( tr("E&xtraction"), adder, -1, 1 );

  adder->insertItem( tr("E&xtract"), this, 
		     SLOT(addToBeaker()) );

  statusBar()->message( tr("Starting extraction lab.") );
  setCaption( tr("GenChemLab - Extraction") );
}

void ApplicationWindow::slotSetStatusBar(QString m1) {
  statusBar()->message( m1 );
}

void ApplicationWindow::doNothing() { }
