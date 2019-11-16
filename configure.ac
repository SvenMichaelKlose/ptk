dnl Process this file with autoconf to produce a configure script.
AC_INIT([ptk], [0.0.4])
#CXXFLAGS="-Wall -O0 -g -pipe -DEMBED_X11WRAP -DDEBUG"
CXXFLAGS="-Wall -O0 -g -pipe -s -DEMBED_X11WRAP"

AC_SUBST(SUBDIRS)
AC_SUBST(STRIP)
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE
AM_PROG_LIBTOOL

dnl Checks for programs.
AC_CHECK_PROG(SHELL,sh)
AC_CHECK_PROG(PERL,perl5 perl)
AC_CHECK_PROG(STRIP,strip)
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

LT_INIT

dnl Checks for libraries.
dnl Replace `main' with a function in -lc:
AC_CHECK_LIB(c, main)
dnl Replace `main' with a function in -lm:
AC_CHECK_LIB(m, main)
dnl Replace 'main' with a function in -lptk:
AC_CHECK_LIB(ptk, main)

dnl Checks for header files.
AC_PATH_X
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_OUTPUT(src/Makefile src/ptk/Makefile src/ptktoolkit/Makefile Makefile \
	programs/Makefile \
	programs/helloworld/Makefile \
	programs/tests/Makefile \
	programs/scribble/Makefile \
	Documentation/Makefile)
