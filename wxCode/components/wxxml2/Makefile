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



CXX = $(shell wx-config --cxx)

PROGRAM = minimal

OBJECTS = src/xml2.o src/dtd.o sample/$(PROGRAM).o

# implementation

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -g `wx-config --cxxflags` -I./include -I/usr/local/include/libxml2 -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) -L/usr/local/lib/ -lxml2 `wx-config --libs`

clean: 
	rm -f sample/*.o
	rm -f $(PROGRAM)
	rm -f src/*.o
