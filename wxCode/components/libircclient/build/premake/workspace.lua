-- A solution contains projects, and defines the available configurations

solution "wxLIBIRCClient"

	language "C++"
	configurations { "Release", "Debug" }
	location ( "../../build/" .. _ACTION )

	local scriptDir = os.getcwd()
	local buildDir 	= solution().location

	os.chdir( buildDir )

	configuration "Debug"
		defines { "DEBUG", "_DEBUG" }
		flags { "ExtraWarnings", "Symbols", "Unicode" }
 
	configuration "Release"
		defines { "NDEBUG" }
		flags { "ExtraWarnings", "OptimizeSpeed", "Unicode" }

	dofile( scriptDir .. "/wxlibircclient.lua" )

