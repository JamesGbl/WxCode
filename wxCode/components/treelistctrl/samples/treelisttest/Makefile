##############################################################################
## File:        Makefile
## Purpose:     treelisttest
## Maintainer:  $Author: pgriddev $
## Created:     2004-10-01
## RCS-ID:      $Id: Makefile,v 1.11 2008-06-18 07:07:50 pgriddev Exp $
## Copyright:   (c) 2004-2008 wxCode
## Licence:     wxWindows
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
endif
ifeq ($(MAKECMDGOALS),release)
  WXCONFIG=wx-config
endif
ifeq ($(MAKECMDGOALS),debug)
  WXCONFIG=wxgtkd-2.4-config
endif
ifeq ($(MAKECMDGOALS),develdebug)
  WXCONFIG=$(WXDEVEL_Debug)wx-config --inplace
endif
ifeq ($(MAKECMDGOALS),univdebug)
  WXCONFIG=$(WXDEVEL_Univ)wx-config --inplace
endif


OBJ = $(SRC:.cpp=.o) treelisttest.res


%.o : %.cpp
	g++ -c $(INC) `$(WXCONFIG) --cxxflags` -g -o $@ $<

%.res : %.rc
	windres `$(WXCONFIG) --cppflags -rescomp` -i $< -J rc -o $@ -O coff


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
	rm -f *.o *.res
