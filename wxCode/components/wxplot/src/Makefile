WXDIR = /home/fhuebner/src/wxWindows
EXTRACPPFLAGS = -g
EXTRALDFLAGS = -g
CPP_DEPENDANCIES=-MM

WXWIN= $(WXDIR)

LIBSOURCES= \
	wxwin.cpp \
	wxPlot.cpp \
	plargs.cpp \
	plbox.cpp \
	plcont.cpp \
	plcore.cpp \
	plctrl.cpp \
	plcvt.cpp \
	pldtik.cpp \
	plfill.cpp \
	plhist.cpp \
	plline.cpp \
	plmap.cpp \
	plot3d.cpp \
	plpage.cpp \
	plsdef.cpp \
	plshade.cpp \
	plsym.cpp \
	pltick.cpp \
	plvpor.cpp \
	plwind.cpp \
	plstripc.cpp \
	pdfutils.cpp \
	plbuf.cpp
LIBOBJECTS= $(LIBSOURCES:.cpp=.o)

include $(WXDIR)/src/makeg95.env

all:    install_lib

demo: 	demo/demo.o
	make -C demo all

install_lib: lib
	cp libplot.a $(WXDIR)/lib
	-mkdir $(WXDIR)/include/wxPlot
	cp wxPlot.h $(WXDIR)/include/wxPlot
	cp plplot.h $(WXDIR)/include/wxPlot

lib: $(LIBOBJECTS) 
	ar $(AROPTIONS) libplot.a  $(LIBOBJECTS)
	$(RANLIB) libplot.a
%.dd: %.cpp
	$(SHELL) -ec '$(CCC) $(CPP_DEPENDANCIES) $(CPPFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'

.PHONY: clean distclean demo
clean:
	-rm -f $(LIBOBJECTS)
	-rm -f core 
	cd demo; make $@
distclean: clean
	-rm -f *.o *.d *.dd libplot.a

include $(LIBSOURCES:.cpp=.dd)



