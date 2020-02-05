# -*- Makefile -*-
#
# Copyright (C) 2000-2006  Wizzer Works
#
# Makefile - For making the Win32 Configuration of the Magic Lantern Util
#            Libraries.
#
# $Id$
#
#*************************************************************************

ISM_DEPTH = ../..
include $(ISM_DEPTH)/ismdefs

#
# Set which versions we are making.  There needs to be a subdirectory
# under here for each (for VERSIONS_MAKERULE to work).
#
VERSIONS = Debug Release

all $(COMMONTARGS):
	$(VERSIONS_MAKERULE)

install: $(_FORCE)
	$(VERSIONS_MAKERULE)

include $(ISMRULES)
