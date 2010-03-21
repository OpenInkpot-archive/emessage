AC_PREREQ(2.57)
AC_INIT(emessage, 0.1, dottedmag@dottedmag.net)
AM_INIT_AUTOMAKE(foreign)

AC_PROG_CC

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES(EFL, [ecore ecore-evas evas edje])
PKG_CHECK_MODULES(EOI, [libeoi])

AC_OUTPUT([Makefile src/Makefile themes/Makefile])
