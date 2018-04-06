# -*- Makefile -*-
#
# Copyright (C) 2000-2006  Wizzer Works
#
# Makefile - for making the release configuration of the Magic Lantern
# Util Library.
#
# $Id$
#
#*************************************************************************

#
# Set which configuration this is...
#
CONFIGURATION = LibRelease

ISM_DEPTH = ../../..
LEAF_DEPTH = ../..

#
# Include local definitions.
#
include localdefs

#
# The actual work of building is in a directory above, in a file
# called "leafdefs".
#
include $(LEAF_DEPTH)/leafdefs

# Turn off STRICT compilation.
WIN_STRICT =

install: $(TARGETS)
	$(INSTALL) -d $(MLE_ROOT)/lib/tools
	$(INSTALL) $^ $(MLE_ROOT)/lib/tools

collect:
	$(IBCOLLECT) -log ../../../build/collect.raw \
                     -srcDir . \
                     -destDir "lib/tools" \
                     -tags LIB \
                     $(TARGETS)
