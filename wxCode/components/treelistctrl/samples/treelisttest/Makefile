##############################################################################
## File:        Makefile
## Purpose:     treelisttest
## Maintainer:  Otto Wyss
## Created:     2004-03-22
## RCS-ID:      $Id: Makefile,v 1.1 2004-07-19 18:47:41 wyo Exp $
## Copyright:   (c) wyoDesktop
## Licence:     wxWidgets licence
##############################################################################

# list of source files
SRC = treelisttest.cpp

# source directories
vpath %.cpp $(WXCODE)/treelistctrl/src

# executable name
PRG = treelisttest

# additional header directories
INC =

##############################################################################
# implementation, change only if other settings or other targets are needed

ifneq ($(WXDEVEL),)
  WXDEVEL_Release = $(WXDEVEL)/Release/
  WXDEVEL_Debug = $(WXDEVEL)/Debug/
  WXDEVEL_Univ = $(WXDEVEL)/Univ/
endif

ifeq ($(MAKECMDGOALS),)
  WXCONFIG=wxgtk-2.4-config --prefix=/usr
endif
ifeq ($(MAKECMDGOALS),debug)
  WXCONFIG=wxgtkd-2.4-config --prefix=/usr
endif
ifeq ($(MAKECMDGOALS),univdebug)
  WXCONFIG=$(WXDEVEL_Debug)wxgtkd-2.5-config --inplace
endif

OBJ = $(SRC:.cpp=.o)

.cpp.o:
	g++ -c $(INC) `$(WXCONFIG) --cxxflags` -g -o $@ $<

all:	$(PRG)
$(PRG):	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --static --libs`

debug:	$(PRG)d
$(PRG)d:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

univdebug:	$(PRG)ud
$(PRG)ud:	$(OBJ)
	g++ -o $@ $^ `$(WXCONFIG) --libs`

clean:
	rm -f *.o
