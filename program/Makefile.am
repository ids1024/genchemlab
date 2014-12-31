bin_PROGRAMS = genchemlab
genchemlab_SOURCES = moc-application.cpp moc-titration.cpp moc-chem_dialog.cpp moc-labwidget.cpp moc-indicatordialog.cpp moc-buretdialog.cpp moc-qualitative.cpp moc-calorimetry.cpp moc-helpwindow.cpp moc-bpfp.cpp moc-spec.cpp moc-textdialog.cpp moc-vapor.cpp moc-electro.cpp moc-electrodialog.cpp  moc-kinetic.cpp moc-titrationdialog.cpp moc-extract.cpp application.cpp main.cpp titration.cpp chem_dialog.cpp labwidget.cpp indicatordialog.cpp buretdialog.cpp qualitative.cpp calorimetry.cpp helpwindow.cpp bpfp.cpp spec.cpp textdialog.cpp vapor.cpp electro.cpp electrodialog.cpp  kinetic.cpp titrationdialog.cpp extract.cpp qrc_application.cpp

genchemlab_LDADD = -L/usr/lib -L/usr/X11R6/lib -lQt5OpenGL -lQt5PrintSupport -lQt5Widgets -lQt5Gui -lQt5Core -lGL -lGLU -lpthread 
EXTRA_DIST = . application.h titration.h chem_dialog.h labwidget.h indicatordialog.h buretdialog.h qualitative.h calorimetry.h helpwindow.h bpfp.h spec.h textdialog.h vapor.h electro.h electrodialog.h kinetic.h titrationdialog.h extract.h
genchemlab_METASOURCES = USE_AUTOMOC
AM_CPPFLAGS = -I/usr/X11R6/include -I. -I/usr/include/qt/QtCore -I/usr/include/qt/QtGui -I/usr/include/qt/QtWidgets -I/usr/include/qt/QtOpenGL -I/usr/include/qt/QtPrintSupport -I/usr/include/qt -DUNIX -DRINGHOME="\"$(pkgdatadir)/\"" -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
AM_CXXFLAGS = -g -fPIC
MOC = moc-qt5

Developmentdir = /usr/share/applications
Development_DATA = genchemlab.desktop

pixmapsdir = /usr/share/pixmaps
pixmaps_DATA = genchemlab.png


moc-%.cpp: %.h
	$(MOC) -o$@ $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(MOC_CPPFLAGS) $<

qrc_application.cpp: application.qrc
	rcc-qt4 application.qrc -o qrc_application.cpp
