##############################################################################
## File:        Makefile
## Purpose:     treelisttest
## Maintainer:  Otto Wyss
## Created:     2004-03-22
## RCS-ID:      $Id: Makefile,v 1.6 2004-11-24 22:20:16 wyo Exp $
## Copyright:   (c) wyoDesktop
## Licence:     wxWidgets licence
##############################################################################

# list of source files
SRC = treelisttest.cpp \
      treelistctrl.cpp

# executable name
PRG = treelisttest

# source directories
vpath %.cpp ../../src

# additional header directories
INC = -I../../include

##############################################################################
# implementation, change only if other settings or other targets are needed

ifneq ($(WXDEVEL),)
  WXDEVEL_Debug = $(WXDEVEL)/Debug/
  WXDEVEL_Univ = $(WXDEVEL)/Univ/
endif

ifeq ($(MAKECMDGOALS),)
  WXCONFIG=wx-config
  GTKCONFIG=gtk-config
endif
ifeq ($(MAKECMDGOALS),release)
  WXCONFIG=wx-config
  GTKCONFIG=gtk-config
endif
ifeq ($(MAKECMDGOALS),debug)
  WXCONFIG=wxgtkd-2.4-config
  GTKCONFIG=gtk-config
endif
ifeq ($(MAKECMDGOALS),develdebug)
  WXCONFIG=$(WXDEVEL_Debug)wx-config --inplace
  GTKCONFIG=pkg-config gtk+-2.0
endif
ifeq ($(MAKECMDGOALS),univdebug)
  WXCONFIG=$(WXDEVEL_Univ)wx-config --inplace
  GTKCONFIG=pkg-config gtk+-2.0
endif

OBJ = $(SRC:.cpp=.o)

.cpp.o:
	g++ -c $(INC) `$(WXCONFIG) --cxxflags` `$(GTKCONFIG) --cflags` -g -o $@ $<

all:	$(PRG)
$(PRG):	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

release:	$(PRG)r
$(PRG)r:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --static --libs`

debug:	$(PRG)d
$(PRG)d:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

develdebug:	$(PRG)dd
$(PRG)dd:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

univdebug:	$(PRG)ud
$(PRG)ud:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

clean:
	rm -f *.o
