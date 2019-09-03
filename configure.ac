#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([elastomath], [1.0], [alex.bron@broncosoft.com])
AM_INIT_AUTOMAKE
LT_INIT
AC_OUTPUT(Makefile src/Makefile)
AC_CONFIG_SRCDIR([src/emath/scale.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([malloc.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([sqrt])

AC_OUTPUT
