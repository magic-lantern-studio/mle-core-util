#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlutil.la

#######################################
# Build information for each library

# Sources for libmlutil
libmlutil_la_SOURCES = \
	$(top_srcdir)/../../common/src/mlDebug.c \
	$(top_srcdir)/../../common/src/MleDsoLoader.cxx \
	$(top_srcdir)/../../common/src/MleMemoryManager.cxx \
	$(top_srcdir)/../../common/src/mlErrno.c \
	$(top_srcdir)/../../common/src/MleTemplate.cxx \
	$(top_srcdir)/../../common/src/mlExpandFilename.c \
	$(top_srcdir)/../../common/src/mlLogFile.c \
	$(top_srcdir)/../../common/src/mlUnique.c \
	$(top_srcdir)/../../common/src/mlItoa.c \
	$(top_srcdir)/../../common/src/mlTime.c \
	$(top_srcdir)/../../common/src/mlReadFile.c 

if LINUX
libmlutil_la_SOURCES += \
	$(top_srcdir)/../src/MleLinuxMemoryManager.cxx \
	$(top_srcdir)/../src/MleLinuxPath.cxx
endif

# Linker options for libmlutil
libmlutil_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlutil_la_CPPFLAGS = \
	-DMLE_NOT_UTIL_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../common/include \
	-I$(top_srcdir)/../../linux/include \
	-std=c++11

