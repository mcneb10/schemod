AC_INIT([schemod], [1.0])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.21.1])
AC_PROG_CC
AC_PROG_CXX

AM_CONDITIONAL([USE_NLS], [test "${USE_NLS}" = "yes"])

# Error for if library isn't found.
# AC_DEFUN([AX_LIB_NOT_FOUND_ERROR], [AC_MSG_ERROR([\
# Library $1 not found. If you are on macos, try to add /opt/homebrew/lib/ to the library search paths \
# and the /opt/homebrew/include/ to the include paths])])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile src/Makefile po/Makefile.in])

dnl AC_LINK_IFELSE([sfml], [glewInit], [], [AX_LIB_NOT_FOUND_ERROR([sfml])])
# Link the sfml libs (the ones written here include the other essential ones)
PKG_CHECK_MODULES([SFML], [sfml-graphics sfml-audio])
PKG_CHECK_MODULES([SPDLOG], [spdlog])

dnl AC_CHECK_HEADERS([SFML/Graphics.hpp], [], [AC_MSG_ERROR([SFML headers not found!])])
dnl AC_CHECK_HEADERS([spdlog/spdlog.h], [], [AC_MSG_ERROR([spdlog headers not found!])])
AM_MISSING_PROG(PERL, perl)

AC_OUTPUT