#!/bin/bash

GPIB_SOURCES=''
GPIB_LIB=''
GPIB_SUPPORT=''

if [ "$1" = "GPIB=1" ]
then
    GPIB_SOURCES='../../src/linux/gpib.cpp ../../src/gpibx.cpp' 
    GPIB_LIB='-lgpib'
    GPIB_SUPPORT='Yes'
else
    echo '============================================================'
    echo 'You run makepy.sh without GPIB support.'
    echo 'If you want to create the python wxctb library with'
    echo 'GPIB support, rerun the command with:'
    echo 'makepy.sh GPIB=1' 
    echo '============================================================'
    GPIB_SUPPORT='No'
fi

echo "swig generates python wrapper files..."
swig -c++ -Wall -nodefault -python -keyword -new_repr -modern wxctb.i

echo "create shared library wxctb with GPIB=$GPIB_SUPPORT for python 2.4..."
g++ -Wall -g -shared -I /usr/include/python2.4/ \
    -I ../../include \
    ../../SWIG/linux/wxctb_wrap.cxx  \
    ../../src/linux/timer.cpp \
    ../../src/linux/serport.cpp \
    ../../src/kbhit.cpp \
    ../../src/iobase.cpp \
    ../../src/fifo.cpp \
    $GPIB_SOURCES \
    $GPIB_LIB \
    -o _wxctb.so

echo "copy wxctb.py and _wxctb.so to the linux/samples folder..."
cp wxctb.py _wxctb.so samples
