#
# File:		Makefile for samples
# Author:	Robert Roebling
# Created:	1999
# Updated:	
# Copyright:	(c) 1998 Robert Roebling
#
# This makefile requires a Unix version of wxWindows
# to be installed on your system. This is most often
# done typing "make install" when using the complete
# sources of wxWindows or by installing the two
# RPM packages wxGTK.XXX.rpm and wxGTK-devel.XXX.rpm
# under Linux.
#



WX_CONFIG = wx-config
CXX = $(shell $(WX_CONFIG) --cxx)

PROGRAM = sample

OBJECTS = ../src/Shortcut.o ../src/ShortcutGroup.o ../src/ShortcutPanel.o $(PROGRAM).o

# implementation

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c `$(WX_CONFIG) --cxxflags` -I../include -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) `$(WX_CONFIG) --libs`

clean: 
	rm -f *.o
	rm -f $(PROGRAM)
	rm -f ../src/*.o
