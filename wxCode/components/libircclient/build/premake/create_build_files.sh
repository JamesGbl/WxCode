#!/bin/sh

isbsd=`expr "$unamestr" : '.*BSD'`
platform="unknown"
unamestr=$(uname)

if ( [ "$isbsd" -gt "0" ] || [ "$unamestr" = "Linux" ] ); then
	platform="unix"
elif [ "$unamestr" = "Darwin" ]; then
	platform="macosx"
fi

make CONFIG=Release -C./$platform
#./$platform/bin/release/premake4 --file=workspace.lua codeblocks
./$platform/bin/release/premake4 --file=workspace.lua codelite
#./$platform/bin/release/premake4 --file=workspace.lua gmake

#if [ "$platform" = "macosx" ]; then
#	./macosx/bin/release/premake4 --file=workspace.lua xcode3
#fi

