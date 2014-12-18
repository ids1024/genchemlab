bin_PROGRAMS = genchemlab
genchemlab_SOURCES = application.cpp main.cpp titration.cpp chem_dialog.cpp labwidget.cpp indicatordialog.cpp buretdialog.cpp qualitative.cpp calorimetry.cpp helpwindow.cpp bpfp.cpp spec.cpp textdialog.cpp vapor.cpp electro.cpp electrodialog.cpp  kinetic.cpp titrationdialog.cpp extract.cpp
genchemlab_LDADD = -L$(QTDIR)/lib -L/usr/X11R6/lib/qt -L/usr/lib/qt -lGL -lGLU -l$(qtlibname) -lm
EXTRA_DIST = . application.h titration.h chem_dialog.h labwidget.h indicatordialog.h buretdialog.h qualitative.h calorimetry.h helpwindow.h bpfp.h spec.h textdialog.h vapor.h electro.h electrodialog.h kinetic.h titrationdialog.h extract.h
genchemlab_METASOURCES = USE_AUTOMOC
AM_CPPFLAGS = -I/usr/X11R6/include -I$(QTDIR)/include -I/usr/include/qt -I/usr/X11R6/include/qt -DUNIX -DRINGHOME="\"$(pkgdatadir)/\""
AM_CXXFLAGS = -g
MOC = moc

Developmentdir = /usr/share/applications
Development_DATA = genchemlab.desktop

pixmapsdir = /usr/share/pixmaps
pixmaps_DATA = genchemlab.png

