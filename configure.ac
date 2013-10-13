dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([elnetw-cpp], [0.1], [https://github.com/turenar/elnetw-cpp])


AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE()
dnl m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_LANG([C])
AC_PROG_CC

AC_HEADER_STDC
AC_FUNC_MALLOC
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([stdlib.h])
AC_CHECK_HEADER([stdio.h])
AC_CHECK_HEADER([unistd.h])
AC_CHECK_HEADER([stddef.h])
AC_CHECK_HEADER([limits.h])
AC_CHECK_HEADER([libgen.h])
AC_CHECK_HEADER([dirent.h])
AC_CHECK_FUNCS([realpath _fullpath], [break])
dnl AC_CHECK_FUNC([dirname])
AC_CHECK_FUNC([_tdirname])
AC_CHECK_FUNC([opendir])

case $host_os in
  *mingw32*)
    CFLAGS="$CFLAGS -mwindows"
esac
AC_DEFINE([UNICODE], [], [Use Unicode chars])
AC_DEFINE([_UNICODE], [], [Use Unicode chars])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

