#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ocher], [0.0.1], [clc@alum.mit.edu])
AM_INIT_AUTOMAKE
AC_CONFIG_SUBDIRS([src/zlib])
AC_CONFIG_SRCDIR([src/ocher.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/Makefile
])

: ${CXXFLAGS="-std=c++11 -O2"}
AC_PROG_CXX
AC_GNU_SOURCE
# I have never been able to get either of these to work, hence the explicit
# -std=c++11 above.
#AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])
#AX_CHECK_COMPILE_FLAG([-std=c++11], [CXXFLAGS="$CXXFLAGS -std=c++11"])

AC_ARG_WITH(cdk-dir,
    [  --with-cdk-dir=PATH     path to the cdk installation],
    [
      CPPFLAGS="${CPPFLAGS} -I${withval}/include -I${withval}/include/cdk"
      LDFLAGS="$LDFLAGS -L${withval}/lib"
      if test -n "${need_dash_r}"; then
        LDFLAGS="$LDFLAGS -R${withval}/lib"
      fi
      WITH_CDK=${withval}
    ]
  )

# Checks for libraries.
#AC_CHECK_HEADER(zlib.h,
#                [AC_CHECK_LIB(z, inflateInit_, ,
#                 AC_MSG_NOTICE([zlib support disabled])])
AC_CHECK_LIB(z, inflateInit_)
AC_CHECK_LIB(mxml, mxmlDelete)
AC_CHECK_LIB(ncurses, delwin)
AC_CHECK_LIB(cdk, initCDKScreen)
if test "${ac_cv_lib_cdk_initCDKScreen}" != "yes"; then
  AC_MSG_ERROR([can not find the cdk library.])
fi

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h sys/ioctl.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_INT32_T
AC_TYPE_UINT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memchr strcasecmp strchr strncasecmp strndup])

AC_OUTPUT
