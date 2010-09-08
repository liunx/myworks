AC_PREREQ(2.59)
AC_INIT([The Operator Server], [0.01], [liunx163@163.com], [operator])
AM_INIT_AUTOMAKE([1.7.9])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 ])

dnl
dnl Process features.
dnl

AC_ARG_WITH(ssl,
	    [[ --without-ssl		disable SSL autodetection]])
dnl
dnl Checks for system header files that might be missing.
dnl
AC_HEADER_STDBOOL
AC_CHECK_HEADERS(unistd.h sys/time.h)
AC_CHECK_HEADERS(termios.h sys/ioctl.h sys/select.h utime.h sys/utime.h)
AC_CHECK_HEADERS(stdint.h inttypes.h pwd.h wchar.h)

AC_OUTPUT

