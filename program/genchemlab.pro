TEMPLATE = app
INCLUDEPATH += .

# Input
HEADERS += application.h \
           bpfp.h \
           buretdialog.h \
           calorimetry.h \
           chem_dialog.h \
           chemicals.h \
           electro.h \
           electrodialog.h \
	   extract.h \
           helpwindow.h \
           indicatordialog.h \
           labwidget.h \
           point3d.h \
           qualitative.h \
	   spec.h \
	   textdialog.h \
           titration.h \
           titrationdialog.h \
	   vapor.h
SOURCES += application.cpp \
           bpfp.cpp \
           buretdialog.cpp \
           calorimetry.cpp \
           chem_dialog.cpp \
           electro.cpp \
           electrodialog.cpp \
	   extract.cpp \
           helpwindow.cpp \
           indicatordialog.cpp \
           labwidget.cpp \
           main.cpp \
           qualitative.cpp \
	   spec.cpp \
	   textdialog.cpp \
           titration.cpp \
           titrationdialog.cpp \
	   vapor.cpp
TRANSLATIONS = genchemlab_en.ts genchemlab_fr.ts genchemlab_de.ts \
        genchemlab_pl.ts genchemlab_nl.ts genchemlab_it.ts genchemlab_es.ts \
        genchemlab_pt.ts genchemlab_pt_BR.ts genchemlab_dk.ts genchemlab_jp.ts

RESOURCES     = application.qrc
TARGET = genchemlab
QT +=  opengl printsupport
LIBS += -lGLU
