TEMPLATE	= app
CONFIG		+= qt warn_on release
HEADERS		= application.h
SOURCES		= application.cpp \
		  main.cpp
TARGET		= application
DEPENDPATH=../../include
RESOURCES     = application.qrc
#REQUIRES=full-config
QT +=  opengl printsupport
