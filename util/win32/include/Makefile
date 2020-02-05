# -*- Makefile -*-
#
# Copyright (C) 2000-2006  Wizzer Works
#
# Makefile for installing header files.
#
# $Id$
#
###########################################################################

ISM_DEPTH = ..
include $(ISM_DEPTH)/ismdefs

VPATH = mle:$(ISM_DEPTH)/../common/include/mle

COMMON_HEADERS = \
	mlArray.h \
	mlAssert.h \
	mlConfig.h \
	mlDebug.h \
	mle.h \
	MleDsoLoader.h \
	MleMemoryManager.h \
	MlePath.h \
	mlErrno.h \
	MleTemplate.h \
	mlExpandFilename.h \
	mlFileio.h \
	mlLogFile.h \
	mlMacros.h \
	mlMalloc.h \
	mlToken.h \
	mlTypes.h \
	mlUnique.h \
	MleUtil.h \
	$(NULL)

WIN32_HEADERS = \
	MleWin32Path.h \
	mlGetopt.h \
	mlPlatformDefs.h \
	$(NULL)

HEADERS = \
	$(COMMON_HEADERS) \
	$(WIN32_HEADERS) \
	$(NULL)

install: $(HEADERS)
	$(INSTALL) -d $(MLE_ROOT)/include/mle
	$(INSTALL) $^ $(MLE_ROOT)/include/mle

include $(ISMRULES)

collect:
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir $(ISM_DEPTH)/../common/include/mle \
                     -destDir "include/mle" \
                     -tags "INCLUDE" \
                     $(COMMON_HEADERS)
	$(IBCOLLECT) -baseDir $(ISM_DEPTH)/.. \
                     -log ../build/collect.raw \
                     -srcDir mle \
                     -destDir "include/mle" \
                     -tags "INCLUDE" \
                     $(WIN32_HEADERS)
