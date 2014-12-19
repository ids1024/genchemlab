TEMPLATE	= app
CONFIG		+= qt warn_on release
HEADERS		= application.h
SOURCES		= application.cpp \
		  main.cpp
TARGET		= application
DEPENDPATH=../../include
#The following line was commented out by qt3to4
#REQUIRES=full-config
#The following line was inserted by qt3to4
QT +=  qt3support opengl
