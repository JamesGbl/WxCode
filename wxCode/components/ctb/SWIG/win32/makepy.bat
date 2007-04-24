rem call "c:\Programme\Microsoft Visual C++ Toolkit 2003\vcvars32.bat"

set PYTHON_LIB=t:\python24\libs\python24.lib
set PYTHON_INCLUDE=t:\python24\include
set SWIG=t:\swigwin-1.3.25\swig

echo "swig generates python wrapper files..."
%SWIG% -c++ -Wall -nodefault -python -keyword -new_repr -modern wxctb.i

echo "create shared library wxctb for python 2.4..."
cl /LD /D WIN32 /I %PYTHON_INCLUDE% /I ../../include wxctb_wrap.cxx ../../src/win32/serport.cpp ../../src/win32/timer.cpp ../../src/kbhit.cpp ../../src/iobase.cpp ../../src/gpibx.cpp ../../src/win32/gpib.cpp ../../src/fifo.cpp /link %PYTHON_LIB% winmm.lib ../../lib/gpib32.lib

del wxctb.dll
move wxctb_wrap.dll _wxctb.dll

echo "copy wxctb.py and _wxctb.so to the samples folder..."
copy wxctb.py samples
copy _wxctb.dll samples
