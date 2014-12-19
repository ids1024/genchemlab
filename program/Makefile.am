bin_PROGRAMS = genchemlab
genchemlab_SOURCES = moc-application.cpp moc-titration.cpp moc-chem_dialog.cpp moc-labwidget.cpp moc-indicatordialog.cpp moc-buretdialog.cpp moc-qualitative.cpp moc-calorimetry.cpp moc-helpwindow.cpp moc-bpfp.cpp moc-spec.cpp moc-textdialog.cpp moc-vapor.cpp moc-electro.cpp moc-electrodialog.cpp  moc-kinetic.cpp moc-titrationdialog.cpp moc-extract.cpp application.cpp main.cpp titration.cpp chem_dialog.cpp labwidget.cpp indicatordialog.cpp buretdialog.cpp qualitative.cpp calorimetry.cpp helpwindow.cpp bpfp.cpp spec.cpp textdialog.cpp vapor.cpp electro.cpp electrodialog.cpp  kinetic.cpp titrationdialog.cpp extract.cpp

genchemlab_LDADD = -L/usr/lib -L/usr/X11R6/lib -lQt3Support -lQtOpenGL -lQtGui -lQtCore -lGL -lGLU -lpthread 
EXTRA_DIST = . application.h titration.h chem_dialog.h labwidget.h indicatordialog.h buretdialog.h qualitative.h calorimetry.h helpwindow.h bpfp.h spec.h textdialog.h vapor.h electro.h electrodialog.h kinetic.h titrationdialog.h extract.h
genchemlab_METASOURCES = USE_AUTOMOC
AM_CPPFLAGS = -I/usr/X11R6/include -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4/Qt3Support -I/usr/include/qt4 -DUNIX -DRINGHOME="\"$(pkgdatadir)/\"" -DQT_NO_DEBUG -DQT_QT3SUPPORT_LIB -DQT3_SUPPORT -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
AM_CXXFLAGS = -g
MOC = moc-qt4

Developmentdir = /usr/share/applications
Development_DATA = genchemlab.desktop

pixmapsdir = /usr/share/pixmaps
pixmaps_DATA = genchemlab.png


moc-%.cpp: %.h
	$(MOC) -o$@ $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(MOC_CPPFLAGS) $<
