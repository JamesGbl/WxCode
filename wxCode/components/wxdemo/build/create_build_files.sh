#!/bin/sh
#

# Autodetect wxWidgets settings
if wx-config --unicode >/dev/null 2>/dev/null; then
	unicode="--unicode"
fi
if wx-config --static >/dev/null 2>/dev/null; then
	static="--use-wx-static"
fi

# ========== Code::Blocks project files ==========
premake/premake-linux --target cb-gcc $unicode $static
echo done...
echo 
#
# ========== GNU Makefile ==========
premake/premake-linux --target gnu $unicode $static
echo done...
echo 
#
echo Done generating all project files
#
