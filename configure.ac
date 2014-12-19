# Process this file with autoconf to produce a configure script.
AC_INIT([genchemlab], [0.1], [herger@chemistry.gatech.edu])
AC_CONFIG_SRCDIR([config.h.in])
AM_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE
top_builddir=`pwd`
AC_SUBST(top_builddir)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
AC_ARG_WITH(qt-thread, AC_HELP_STRING([--with-qt-thread],
            [use Qt threaded library (libqt-mt.so) (default is YES)]),
            package_qt_enabled=$withval, package_qt_enabled=yes)
if test "$package_qt_enabled" = yes; then
        qtlibname=qt-mt
else
        qtlibname=qt
fi
AC_SUBST(qtlibname)

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
MY_SUBDIRS=". program doc"
AC_SUBST(MY_SUBDIRS)
AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 program/Makefile])
AC_OUTPUT
