#
# File:		Makefile for samples
# Author:	Robert Roebling
# Created:	1999
# Updated:	Francesco Montorsi
# Copyright:	(c) 1998 Robert Roebling


CXX = $(shell wx-config --cxx)

SAMPLE = minimal
LIB = libkeybinder



LIB_OBJECTS = src/keybinder.o src/menuutils.o
SAMPLE_OBJECTS = sample/$(SAMPLE).o



# --------------------
# RULES:
# --------------------

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -g `wx-config --cxxflags` -I./include $(EXTRAFLAGS) -o $@ $<



# --------------------
# TARGETS:
# --------------------

all:    $(LIB).a $(SAMPLE)

$(SAMPLE):	$(SAMPLE_OBJECTS)
	$(CXX) -o $(SAMPLE) $(SAMPLE_OBJECTS) `wx-config --libs` -lkeybinder -L./lib $(EXTRALIBS)

$(LIB).a:	$(LIB_OBJECTS)
	ar r lib/$(LIB).a $(LIB_OBJECTS)

clean: 
	rm -f *.o
	rm -f src/*.o
	rm -f sample/*.o
	rm -f $(SAMPLE)
	rm -f lib/*.a
