#Created by Germ�n Carrera 2005
#Revised by Ren� Berber

#make
#make lib
#make install
#make clean

#wxwindows path installed
WXDIR := ${shell wx-config --prefix}

#Compilers option
C := ${shell wx-config --cc}

CPLUS := ${shell wx-config --cxx}
############################################
CDIR = ./src/
HDIR = ./include/

DRIVERS = ./drivers/
BINDINGS = ./bindings/wxwindows/

#flags in a windows system (I will put other examples)
WXCFLAGS := -c ${shell wx-config --static=yes --cxxflags}   
############################################

GPPFILES = plargs.c plbox.c plcont.c plcore.c plctrl.c plcvt.c pldtik.c plfill.c plhist.c plline.c plmap.c plot3d.c plpage.c plsdef.c plshade.c plsym.c pltick.c plvpor.c plwind.c plstripc.c pdfutils.c plbuf.c plgridd.c plimage.c plvect.c

WXFILES = wxPlot.cpp wxwin.cpp

OFILES =  plargs.o plbox.o plcont.o plcore.o plctrl.o plcvt.o pldtik.o 	plfill.o plhist.o plline.o plmap.o plot3d.o plpage.o plsdef.o plshade.o plsym.o pltick.o plvpor.o plwind.o plstripc.o pdfutils.o plbuf.o plgridd.o plimage.o plvect.o wxwin.o wxPlot.o 


plplot: $(OFILES) 

plargs.o : $(CDIR)plargs.c  
	$(C) -c $(CDIR)plargs.c -I$(HDIR)
plbox.o : $(CDIR)plbox.c 
	$(C) -c $(CDIR)plbox.c -I$(HDIR)	
plcont.o: $(CDIR)plcont.c 
	$(C) -c $(CDIR)plcont.c -I$(HDIR)
plcore.o: $(CDIR)plcore.c 
	$(C) -c $(CDIR)plcore.c -I$(HDIR) 
plctrl.o: $(CDIR)plctrl.c 
	$(C) -c $(CDIR)plctrl.c -I$(HDIR)
plcvt.o: $(CDIR)plcvt.c 
	$(C) -c $(CDIR)plcvt.c -I$(HDIR)
pldtik.o: $(CDIR)pldtik.c 
	$(C) -c $(CDIR)pldtik.c -I$(HDIR)
plfill.o: $(CDIR)plfill.c 
	$(C) -c $(CDIR)plfill.c -I$(HDIR)
plhist.o: $(CDIR)plhist.c 
	$(C) -c $(CDIR)plhist.c -I$(HDIR)
plline.o: $(CDIR)plline.c 
	$(C) -c $(CDIR)plline.c -I$(HDIR)
plmap.o: $(CDIR)plmap.c 
	$(C) -c $(CDIR)plmap.c -I$(HDIR)
plot3d.o: $(CDIR)plot3d.c 
	$(C) -c $(CDIR)plot3d.c -I$(HDIR)
plpage.o: $(CDIR)plpage.c 
	$(C) -c $(CDIR)plpage.c -I$(HDIR)
plsdef.o: $(CDIR)plsdef.c 
	$(C) -c $(CDIR)plsdef.c -I$(HDIR)
plshade.o: $(CDIR)plshade.c 
	$(C) -c $(CDIR)plshade.c -I$(HDIR)
plsym.o: $(CDIR)plsym.c 
	$(C) -c $(CDIR)plsym.c -I$(HDIR)
pltick.o: $(CDIR)pltick.c 
	$(C) -c $(CDIR)pltick.c -I$(HDIR)
plvpor.o: $(CDIR)plvpor.c 
	$(C) -c $(CDIR)plvpor.c -I$(HDIR)
plwind.o: $(CDIR)plwind.c 
	$(C) -c $(CDIR)plwind.c -I$(HDIR) 
plstripc.o: $(CDIR)plstripc.c 
	$(C) -c $(CDIR)plstripc.c -I$(HDIR)
pdfutils.o: $(CDIR)pdfutils.c
	$(C) -c $(CDIR)pdfutils.c -I$(HDIR)
plbuf.o: $(CDIR)plbuf.c
	$(C) -c $(CDIR)plbuf.c -I$(HDIR)
plgridd.o: $(CDIR)plgridd.c
	$(C) -c $(CDIR)plgridd.c -I$(HDIR)
plimage.o: $(CDIR)plimage.c
	$(C) -c $(CDIR)plimage.c -I$(HDIR)
plvect.o: $(CDIR)plvect.c
	$(C) -c $(CDIR)plvect.c -I$(HDIR)
wxwin.o: $(DRIVERS)wxwin.cpp
	$(CPLUS) $(WXCFLAGS) $(DRIVERS)wxwin.cpp -I$(HDIR) 
wxPlot.o: $(BINDINGS)wxPlot.cpp
	$(CPLUS) $(WXCFLAGS) $(BINDINGS)wxPlot.cpp -I$(HDIR)

lib: $(OFILES)
	ar ruv libplot.a  $(OFILES)
        
        
install:
	cp libplot.a $(WXDIR)/lib
	mkdir -p $(WXDIR)/include/wxPlot
	cp $(BINDINGS)wxPlot.h $(WXDIR)/include/wxPlot
	cp $(HDIR)plplot.h $(WXDIR)/include/wxPlot
	cp $(HDIR)plConfig.h $(WXDIR)/include/wxPlot
clean:
	rm -f *.o *.a

