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

PROGRAM = test
TOLUADIR = /home/fre/prog/tolua-5.0

OBJECTS = Test.o ../../src/sccint.o ../../src/scbasic.o ../../src/sclua.o ../../src/scunderc.o ../../src/script.o

# implementation

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -g `wx-config --cxxflags` -DwxSCRIPT_NO_CINT -DwxSCRIPT_NO_UNDERC -I../../include -I$(TOLUADIR)/include -I/usr/local/include -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) -L/usr/local/lib/ -L$(TOLUADIR)/lib -llua -llualib -ltolua `wx-config --libs`

clean: 
	rm -f sample/*.o
	rm -f $(PROGRAM)
	rm -f src/*.o
