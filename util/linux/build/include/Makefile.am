# These files will end up in the install include directory
# For example, /usr/include
includedir = $(prefix)/include/mle

include_HEADERS = \
	$(top_srcdir)/../../common/include/mle/mlArray.h \
	$(top_srcdir)/../../common/include/mle/mlAssert.h \
	$(top_srcdir)/../../common/include/mle/mlConfig.h \
	$(top_srcdir)/../../common/include/mle/mlDebug.h \
	$(top_srcdir)/../../common/include/mle/MleDsoLoader.h \
	$(top_srcdir)/../../common/include/mle/mle.h \
	$(top_srcdir)/../../common/include/mle/MleMemoryManager.h \
	$(top_srcdir)/../../common/include/mle/MlePath.h \
	$(top_srcdir)/../../common/include/mle/mlErrno.h \
	$(top_srcdir)/../../common/include/mle/MleTemplate.h \
	$(top_srcdir)/../../common/include/mle/MleUtil.h \
	$(top_srcdir)/../../common/include/mle/mlExpandFilename.h \
	$(top_srcdir)/../../common/include/mle/mlFileio.h \
	$(top_srcdir)/../../common/include/mle/mlLogFile.h \
	$(top_srcdir)/../../common/include/mle/mlMacros.h \
	$(top_srcdir)/../../common/include/mle/mlMalloc.h \
	$(top_srcdir)/../../common/include/mle/mlToken.h \
	$(top_srcdir)/../../common/include/mle/mlTrace.h \
	$(top_srcdir)/../../common/include/mle/mlTypes.h \
	$(top_srcdir)/../../common/include/mle/mlUnique.h \
	$(top_srcdir)/../../common/include/mle/mlItoa.h \
	$(top_srcdir)/../../common/include/mle/mlTime.h

if LINUX
include_HEADERS += \
    $(top_srcdir)/../../linux/include/mle/mlPlatformDefs.h \
    $(top_srcdir)/../../linux/include/mle/MleLinuxPath.h
endif