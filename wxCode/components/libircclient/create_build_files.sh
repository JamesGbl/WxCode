#!/bin/sh

cd libircclient
echo Configuring libircclient...
echo
./configure && make
echo Building libircclient...
echo
cd ./../build/premake
./create_build_files.sh

