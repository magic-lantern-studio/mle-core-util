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

ISM_DEPTH = ..
include $(ISM_DEPTH)/ismdefs

# Descend into the subdirectories for COMMONTARGS:
#
SUBDIRS = \
	util \
	$(NULL)

all $(COMMONTARGS):
	$(SUBDIRS_MAKERULE)

install: $(_FORCE)
	$(SUBDIRS_MAKERULE)

include $(ISMRULES)
