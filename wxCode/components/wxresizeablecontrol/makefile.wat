#
# File:		makefile.wat
# Author:	Angela Wrobel
# Created:	2004
# Updated:	
# Copyright:
#
# Makefile : Builds sample for OpenWatcom

WXDIR = $(WXWIN)

TARGET=resizecd
PROGRAM=resizecd
EXEDIR=watcom

OBJECTS = &
	$(EXEDIR)/$(TARGET).obj &
	$(EXEDIR)/myframe.obj &
	$(EXEDIR)/ownerrnd.obj &
   $(EXEDIR)/resizec.obj

!include ./makeprog.wat


