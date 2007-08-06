#!/bin/sh

# Build the two samples using custom makefile for Win32/Linux
echo Building main sample with combined Win32/Linux makefile
old_path=`pwd`
cd ../samples/main
make main

echo Building minimal sample with combined Win32/Linux makefile
cd ../minimal
make min

cd $old_path
echo Done building
