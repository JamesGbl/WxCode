project "wxLibIRCClient"

    kind         "SharedLib"
    language     "C++"
    linkoptions  { "-mwindows" }

    defines      { "UNICODE", "_UNICODE", "__WX__" }
    flags        { "ExtraWarnings", "Unicode" }

    includedirs  { "../../include", "../../libircclient/include" }
    files        { "../../include/**.h", "../../src/**.cpp" }

    libdirs      { "../../libircclient/src" }
    links        { "ircclient" }

    objdir =     buildDir
    targetdir    ( "../../lib" )
    targetname   ("wxircclient")

    configuration "vs*"
        -- Windows and Visual C++ 2005/2008
        defines			{ "_CRT_SECURE_NO_DEPRECATE" }

        configuration "Debug"
                targetsuffix "d"
                buildoptions { "-O0", "`$(29xDebug)/wx-config --debug=yes --cflags`" }
                linkoptions  { "`$(29xDebug)/wx-config --debug=yes --libs`" }
                flags        { "Symbols" }
                defines      { "DEBUG", "_DEBUG", "__WXDEBUG__" }

        configuration "Release"
                buildoptions { "-fno-strict-aliasing", "`$(29xRelease)/wx-config --debug=no --cflags`" }
                linkoptions  { "`$(29xRelease)/wx-config --debug=no --libs`" }
                flags        { "OptimizeSpeed" }
                defines      { "NDEBUG" }

