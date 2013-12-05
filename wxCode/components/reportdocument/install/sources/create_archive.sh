#!/bin/sh

# test whether wxReportDpcument version is passed to the script
if [ ! -n "$1" ];
then
	echo "Version string must be defined as the script parameter."
	exit 1
fi

# archive name
out="wxreportdocument"

# create temporary dir
echo "Removing old temporary directory..."
rm -rf $out
mkdir $out

# copy program files
echo "Updating build files..."
cd ../../build
bakefile_gen
./acregen.sh
cd ../install/sources

echo "Copying build files..."
mkdir $out/build
cp -R -L ../../build/premake $out/build
cp ../../build/create_build_files*.* $out/build
cp ../../build/premake.lua $out/build
cp ../../build/acregen.sh $out/build
cp ../../build/Bakefiles.bkgen $out/build
cp ../../build/config.guess $out/build
cp ../../build/config.sub $out/build
cp ../../build/configure.ac $out/build
cp ../../build/install.sh $out/build
cp ../../build/wxReportDocument.bkl $out/build
cp ../../configure $out
cp ../../Makefile.in $out

echo "Copying lib files..."
mkdir $out/lib
cp ../../lib/dummy.txt $out/lib

echo "Updating documentation files..."
cd ../../docs
doxygen Doxyfile
cd ../install/sources

echo "Copying documentation files..."
mkdir $out/docs
cp -R -L ../../docs/html $out/docs
cp -R -L ../../docs/img $out/docs
cp -R -L ../../docs/txt $out/docs
cp ../../docs/Doxyfile $out/docs

echo "Copying source files..."
cp -R -L ../../include $out
cp -R -L ../../src $out
cp ../../readme.txt $out

echo "Copying sample projects files..."
cp -R -L ../../samples $out

# create archive
echo "Remove unneeded files and directories..."
rm -rf `find $out -type d -name .svn`
rm -rf `find $out -type d -name .objsu`
rm -rf `find $out -type d -name .objsud`
rm -rf `find $out -type d -name bin`
rm -rf `find $out -type f -name *.project`
rm -rf `find $out -type f -name *.mk`
rm -rf `find $out -type f -name *.gch`
rm -rf `find $out -type f -name *.layout`
rm -rf `find $out -type f -name *.depend`
rm -rf `find $out -type f -name Makefile`

echo "Creating archive..."
tar -cvzf wxreportdocument-$1.tgz $out
zip -r wxreportdocument-$1.zip $out

# remove temporary dir
rm -rf $out

echo "Done."

