#Created by Germán Carrera 2005
#Revised by René Berber
#Under the GNU license

#wxwindows path installed
WXDIR := ${shell wx-config --prefix}

CPLUS := ${shell wx-config --cxx}

#flags in a Linux system (I will put other examples)-c 

WXCFLAGS := -c ${shell wx-config --static=yes --cxxflags}

WXLFLAGS := -lplot ${shell wx-config --static=yes --libs}

OFILES = demowxplot.o 

demowxplot: demowxplot.o
	$(CPLUS) -o demowxplot demowxplot.o $(WXLFLAGS)
demowxplot.o: demowxplot.cpp
	$(CPLUS) $(WXCFLAGS) demowxplot.cpp    

clean:
	rm -f *.o demowxplot
