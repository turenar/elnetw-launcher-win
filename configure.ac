dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([elnetw-launcher-win],[0.1],[https://github.com/turenar/elnetw-launcher-win])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([tools])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

AC_LANG([C])
AC_PROG_CC
AM_PROG_CC_C_O

AC_HEADER_STDC
dnl AC_FUNC_MALLOC
AC_CHECK_HEADER([string.h])
AC_CHECK_HEADER([stdlib.h])
AC_CHECK_HEADER([stdio.h])
AC_CHECK_HEADER([unistd.h])
AC_CHECK_HEADER([stddef.h])
AC_CHECK_HEADER([limits.h])
AC_CHECK_HEADER([libgen.h])
AC_CHECK_HEADER([dirent.h])
AC_CHECK_HEADER([windows.h])
AC_CHECK_HEADER([tchar.h])
AC_CHECK_HEADER([locale.h])
AC_CHECK_HEADER([process.h])

case $host_os in
  *mingw32*)
    CFLAGS="$CFLAGS -mwindows -O0"
	AC_DEFINE([WINVER], [0x502], [Windows Version])
	;;
esac

AC_CHECK_FUNCS([realpath _fullpath], [break])
dnl AC_CHECK_FUNC([dirname])
AC_CHECK_FUNC([_tdirname])
AC_CHECK_FUNC([opendir])
AC_CHECK_FUNC([WinMain])
AC_CHECK_FUNCS([Wow64DisableWow64FsRedirection], [break])

AC_CHECK_LIB([msvcr100], [_stat])
if test x$ac_cv_lib_msvcr100__stat = xyes; then
	AC_DEFINE([__MSVCRT_VERSION__], [0x1000], [for __stat64])
fi
AC_CHECK_DECL([_tstat64], [], [], [[
#include <windows.h>
#include <tchar.h>
]])
AC_CHECK_FUNC([_wstat64])

AC_CHECK_TYPE([struct __stat64])


AC_DEFINE([UNICODE], [], [Use Unicode chars])
AC_DEFINE([_UNICODE], [], [Use Unicode chars])

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES(Makefile src/Makefile)
AC_OUTPUT

