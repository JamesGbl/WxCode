#!/bin/sh
#

# Autodetect wxWidgets settings
if wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if wx-config --static=yes >/dev/null 2>/dev/null; then
	static="--use-wx-static"
fi

premake/premake-mac --target cb-gcc $unicode $static
echo done...
echo 
#
premake/premake-mac --target gnu $unicode $static
echo done...
echo 
#
echo Done generating all project files
#
