#
# File:		Makefile for samples
# Author:	Robert Roebling
# Created:	1999
# Updated:	Francesco Montorsi
# Copyright:	(c) 1998 Robert Roebling


CXX = $(shell wx-config --cxx)

TEST1 = test1
TEST2 = test2
LIB = libwxscript



LIB_OBJECTS = src/script.o src/sccint.o src/sclua.o src/scunderc.o src/scbasic.o
TEST1_OBJECTS = tests/test1/test.o
TEST2_OBJECTS = tests/test2/test.o

EXTRAFLAGS=-DwxSCRIPT_NO_CINT -DwxSCRIPT_NO_UNDERC -I/home/fre/prog/tolua-5.0/include



# --------------------
# RULES:
# --------------------

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c -g `wx-config --cxxflags` -I./include $(EXTRAFLAGS) -o $@ $<



# --------------------
# TARGETS:
# --------------------

all:    $(LIB).a $(TEST1) $(TEST2)

$(TEST1):	$(TEST1_OBJECTS)
	$(CXX) -o $(TEST1) $(TEST1_OBJECTS) `wx-config --libs` -l$(LIB).a -L./lib $(EXTRALIBS)

$(TEST2):	$(TEST2_OBJECTS)
	$(CXX) -o $(TEST2) $(TEST2_OBJECTS) `wx-config --libs` -l$(LIB).a -L./lib $(EXTRALIBS)

$(LIB).a:	$(LIB_OBJECTS)
	ar r lib/$(LIB).a $(LIB_OBJECTS)

clean: 
	rm -f *.o
	rm -f src/*.o
	rm -f tests/test1/*.o
	rm -f tests/test2/*.o
	rm -f $(TEST1)
	rm -f $(TEST2)
	rm -f lib/*.a

