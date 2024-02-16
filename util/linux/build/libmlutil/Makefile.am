#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlutil.la

#######################################
# Build information for each library

# Sources for libmlutil
libmlutil_la_SOURCES = \
	../../../common/src/mlDebug.c \
	../../../common/src/MleDsoLoader.cxx \
	../../../common/src/MleMemoryManager.cxx \
	../../../common/src/mlErrno.c \
	../../../common/src/MleTemplate.cxx \
	../../../common/src/mlExpandFilename.c \
	../../../common/src/mlLogFile.c \
	../../../common/src/mlUnique.c \
	../../../common/src/mlItoa.c

if LINUX
libmlutil_la_SOURCES += \
	../../src/MleLinuxMemoryManager.cxx \
	../../src/MleLinuxPath.cxx
endif

# Linker options for libmlutil
libmlutil_la_LDFLAGS = -version-info 1:0:0

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlutil_la_CPPFLAGS = \
	-DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../../common/include \
	-I$(top_srcdir)/../../linux/include \
	-std=c++11

