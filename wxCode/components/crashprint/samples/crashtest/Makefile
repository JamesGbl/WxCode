##############################################################################
## File:        Makefile
## Purpose:     crashtest
## Maintainer:  Otto Wyss
## Created:     2004-10-01
## RCS-ID:      $Id: Makefile,v 1.2 2004-10-04 20:33:06 wyo Exp $
## Copyright:   (c) wxCode
## Licence:     wxWidgets licence
##############################################################################

# list of source files
SRC = app.cpp \
      crashprint.cpp

# executable name
PRG = crashtest

# source directories
vpath %.cpp $(WXCODE)/crashprint/src

# additional header directories
INC = -I$(WXCODE)/crashprint/include

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
  WXCONFIG=$(WXDEVEL_Debug)wx-config --inplace
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
