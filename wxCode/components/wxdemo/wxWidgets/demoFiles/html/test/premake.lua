--*****************************************************************************
--* Author:  Michal Bližòák
--* Edit:    Ján Chudý
--* Date:    06/03/2009
--* Version: 1.00
--*****************************************************************************

-- wxWidgets version
local wx_ver = "28"
local wx_ver_str = "2.8"

-- Adding additional options to support all the build types.
addoption( "use-wx-static", "Use a static configuration of wxWidgets, otherwise link against wxWidgets as a shared library" )
addoption( "unicode", "Use the Unicode character set" )

-- Project name
package.name = "html_test"
local targetName = package.name

if ( windows or linux ) then package.kind = "winexe"
else package.kind = "exe"
end

-- Setup the package compiler settings.
if ( target == "vs2005" or target == "vs2008" ) then
	package.defines = { "_CRT_SECURE_NO_DEPRECATE", "_DISWARNINGS_MSVC" }
	package.buildflags = { "no-main" }
	package.config["Debug"].buildoptions = { "/MTd" }
	package.config["Release"].buildoptions = { "/MT" }
end

if ( target == "cb-gcc" or target == "gnu" ) then
	table.insert( package.config["Debug"].buildoptions, "-O0" )
	table.insert( package.config["Release"].buildoptions, "-fno-strict-aliasing" )
end

-- Set the files to include.
package.files = { matchfiles( "*.cpp", "*.h" ) }
if ( windows ) then
	table.insert(package.files, "../../sample.rc" )
end

-- Common setup
package.language = "c++"

-- Set obj/bin output directory.
package.objdir = "obj"
package.bindir = "."

-- Set the targets.
package.config["Release"].target = targetName
package.config["Debug"].target = targetName

-- Set the build options.
if ( options["use-wx-static"] ) then table.insert( package.buildflags, "static-runtime" ) end

package.config["Release"].buildflags = { "no-symbols", "optimize-speed" }

if ( options["unicode"] ) then table.insert( package.buildflags, "unicode" ) end

-- Set the defines.
if ( not options["use-wx-static"] ) then table.insert( package.defines, "WXUSINGDLL" ) end
if ( options["unicode"] ) then table.insert( package.defines, { "UNICODE", "_UNICODE" } ) end

table.insert( package.defines, "__WX__" )
table.insert( package.config["Debug"].defines, { "DEBUG", "_DEBUG", "__WXDEBUG__" } )
table.insert( package.config["Release"].defines, "NDEBUG" )

if ( windows ) then
--******* WINDOWS SETUP ***********

	-- Set wxWidgets include paths 
	if ( target == "cb-gcc" ) then table.insert( package.includepaths, "$(#WX.include)" )
  else table.insert( package.includepaths, "$(WXWIN)/include" ) end
	
	-- Set the correct 'setup.h' include path.
	if ( options["use-wx-static"] ) then
		if ( options["unicode"] ) then
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_lib/mswu" )
			elseif ( target == "gnu" ) then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_lib/mswu" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_lib/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_lib/mswu" )
			end
		else
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_lib/msw" )
			elseif ( target == "gnu" ) then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_lib/msw" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_lib/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_lib/msw" )
			end
		end
	else
		if ( options["unicode"] ) then
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_dll/mswu" )
			elseif ( target == "gnu" ) then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_dll/mswu" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_dll/mswud" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_dll/mswu" )
			end
		else
			if ( target == "cb-gcc" ) then
				table.insert( package.config["Debug"].includepaths, "$(#WX.lib)/gcc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(#WX.lib)/gcc_dll/msw" )
			elseif ( target == "gnu" ) then
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/gcc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/gcc_dll/msw" )
			else
				table.insert( package.config["Debug"].includepaths, "$(WXWIN)/lib/vc_dll/mswd" )
				table.insert( package.config["Release"].includepaths, "$(WXWIN)/lib/vc_dll/msw" )
			end
		end
	end
	
	-- Set the linker options.
	if ( options["use-wx-static"] ) then
		if ( target == "cb-gcc" ) then
			table.insert( package.libpaths, "$(#WX.lib)/gcc_lib" )
		elseif ( target == "gnu" ) then
			table.insert( package.libpaths, "$(WXWIN)/lib/gcc_lib" )
		else
			table.insert( package.libpaths, "$(WXWIN)/lib/vc_lib" )
		end
	else
		if ( target == "cb-gcc" ) then
			table.insert( package.libpaths, "$(#WX.lib)/gcc_dll" )
		elseif ( target == "gnu" ) then
			table.insert( package.libpaths, "$(WXWIN)/lib/gcc_dll" )
		else
			table.insert( package.libpaths, "$(WXWIN)/lib/vc_dll" )
		end
	end
	
	-- Set wxWidgets libraries to link.
	if ( options["unicode"] ) then
		table.insert( package.config["Release"].links, "wxmsw"..wx_ver.."u" )
		table.insert( package.config["Debug"].links, "wxmsw"..wx_ver.."ud" )
	else
		table.insert( package.config["Release"].links, "wxmsw"..wx_ver )
		table.insert( package.config["Debug"].links, "wxmsw"..wx_ver.."d" )
	end

	if ( options["use-wx-static"] ) then
		if ( options["unicode"] ) then
			table.insert( package.config["Debug"].links, { "wxexpatd", "wxjpegd", "wxpngd", "wxtiffd", "wxzlibd", "wxregexud" } )
			table.insert( package.config["Release"].links, { "wxexpat", "wxjpeg", "wxpng", "wxtiff", "wxzlib", "wxregexu" } )
		else
			table.insert( package.config["Debug"].links, { "wxexpatd", "wxjpegd", "wxpngd", "wxtiffd", "wxzlibd", "wxregexd" } )
			table.insert( package.config["Release"].links, { "wxexpat", "wxjpeg", "wxpng", "wxtiff", "wxzlib", "wxregex" } )
		end

		if ( target == "cb-gcc" or target == "gnu" ) then
			table.insert( package.config["Debug"].links, { "winmm", "rpcrt4", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "comctl32", "wsock32", "odbc32" } )
			table.insert( package.config["Release"].links, { "winmm", "rpcrt4", "kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "comctl32", "wsock32", "odbc32" } )
		else
			table.insert( package.config["Debug"].links, { "rpcrt4", "comctl32", "wsock32" } )
			table.insert( package.config["Release"].links, { "rpcrt4", "comctl32", "wsock32" } )
		end
	end
	
	-- Set the Windows defines.
	table.insert( package.defines, { "__WXMSW__", "WIN32", "_WINDOWS" } )
else

--******* LINUX/MAC SETUP *************

	-- Ignore resource files in Linux/Mac.
	table.insert( package.excludes, matchrecursive( "*.rc" ) )
	
	-- Add buildflag for proper dll building.
	if ( macosx ) then
		table.insert( package.buildflags, "dylib" )
	end
	
	-- Set wxWidgets build options. 

  local wx_config_params = "`wx-config --version="..wx_ver_str.." --static="
  
  if ( options["use-wx-static"] ) then wx_config_params = wx_config_params.."yes"
  else wx_config_params = wx_config_params.."no"
  end
  
  wx_config_params = wx_config_params.." --unicode="
  
  if ( options["unicode"] ) then wx_config_params = wx_config_params.."yes"
  else wx_config_params = wx_config_params.."no"
  end
  
	table.insert( package.config["Debug"].buildoptions, wx_config_params.." --debug=yes --cflags`" )
	table.insert( package.config["Release"].buildoptions, wx_config_params.." --debug=no --cflags`" )

	table.insert( package.config["Debug"].linkoptions, wx_config_params.." --debug=yes --libs`" )
	table.insert( package.config["Release"].linkoptions, wx_config_params.." --debug=no --libs`" )
end
