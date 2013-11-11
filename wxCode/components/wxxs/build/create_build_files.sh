#!/bin/sh
#

# Parse command line options
wxroot=""
wxpath=""
shared="--shared" 
wxshared="--with-wx-shared"

for args in "$@"
do
    haveroot=`expr "${args}" : '--wx-root=.*'`
    if ( [ ${args} = "--help" ] || [ ${args} = "-h" ] ); then
        echo "Available options:"
        echo
        echo "--wx-root                 Specify the wxWidgets build path,"
        echo "                          useful for wxWidgets builds not installed"
        echo "                          in your system (alternate/custom builds)"
        echo "                          Example: --wx-root=/home/devel/wx/3.0/buildgtk"
        echo "                          Current: $wxpath"
		echo
		echo "--disable-wx-shared		Use static wxWidgets build instead of shared libraries."
		echo
		echo "--disable-shared			Build wxXS as a static library."
		echo
        echo
        exit
    elif ( [ "$haveroot" -gt "0" ] ); then
        wxroot=${args}
		wxpath=${wxroot#-*=}/
		wxroot="--wx-root "${wxpath}
        continue
	elif [ ${args} = "--disable-wx-shared" ]; then
        wxshared=""
        continue
	elif [ ${args} = "--disable-shared" ]; then
        shared=""
        continue
    fi
done

# Autodetect wxWidgets settings
if ${wxpath}wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if ! ${wxpath}/wx-config --debug >/dev/null 2>/dev/null; then
	debug="--disable-wx-debug"
fi
release=`${wxpath}wx-config --release`

#
# ========== CodeLite project files ==========
premake/premake-linux --target cl-gcc --wx-version $release $shared $wxshared $unicode $debug $wxroot $1
echo done...
echo 
#
# ========== Code::Blocks project files ==========
#premake/premake-linux --target cb-gcc --wx-version $release $shared $wxshared $unicode $debug $wxroot $1
#echo done...
#echo 
#
# ========== GNU Makefile ==========
premake/premake-linux --target gnu --wx-version $release $shared $wxshared $unicode $debug $wxroot $1
echo done...
echo 
#
echo Done generating all project files for wxXmlSerializer
#
