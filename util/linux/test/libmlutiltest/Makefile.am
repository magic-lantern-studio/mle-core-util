#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
lib_LTLIBRARIES = libmlutiltest.la

#######################################
# Build information for each library

# Sources for libmlutiltest
libmlutiltest_la_SOURCES = libmlutiltest.cxx \
    testMlDebug.cxx

# Linker options libTestProgram
libmlutiltest_la_LDFLAGS = 

# Compiler options. Here we are adding the include directory
# to be searched for headers included in the source code.
libmlutiltest_la_CPPFLAGS = \
    -DMLE_NOT_DLL \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/../include \
	-I$(top_srcdir)/../../common/include \
	-I/usr/local/include \
	-std=c++11