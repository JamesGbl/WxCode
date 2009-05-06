/**********************************************************************
 * Premake - gnu_cpp.c
 * The GNU C/C++ makefile target
 *
 * Copyright (c) 2002-2005 Jason Perkins and the Premake project
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in the file LICENSE.txt for details.
 **********************************************************************/

#include <stdio.h>
#include <string.h>
#include "premake.h"
#include "gnu.h"
#include "os.h"

static const char* filterLinks(const char* name);
static const char* listCppSources(const char* name);
static const char* listRcSources(const char* name);
static const char* listCppTargets(const char* name);
static const char* listRcTargets(const char* name);
static const char* listLinkerDeps(const char* name);


int gnu_cpp()
{
	int i;

	const char* prefix = (g_verbose) ? "" : "@";

	/* Open package makefile and write the header */
	if (gnu_pkgOwnsPath())
		strcpy(g_buffer, path_join(prj_get_pkgpath(), "Makefile", ""));
	else
		strcpy(g_buffer, path_join(prj_get_pkgpath(), prj_get_pkgname(), DOT_MAKE));
	io_openfile(g_buffer);

	io_print("# %s ", prj_is_lang("c++") ? "C++" : "C");

	if (prj_is_kind("exe"))
		io_print("Console Executable");
	else if (prj_is_kind("winexe"))
		io_print("Windowed Executable");
	else if (prj_is_kind("dll"))
		io_print("Shared Library");
	else if (prj_is_kind("lib"))
		io_print("Static Library");

	io_print(" Makefile autogenerated by premake\n");
	io_print("# Don't edit this file! Instead edit `premake.lua` then rerun `make`\n\n");

	/* Set a default configuration */
	prj_select_config(0);
	io_print("ifndef CONFIG\n");
	io_print("  CONFIG=%s\n", prj_get_cfgname());
	io_print("endif\n\n");

	/* fake support for universal binaries */
	io_print("# if multiple archs are defined turn off automated dependency generation\n");
	io_print("DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)\n\n");

	/* Process the build configurations */
	for (i = 0; i < prj_get_numconfigs(); ++i)
	{
		prj_select_config(i);

		io_print("ifeq ($(CONFIG),%s)\n", prj_get_cfgname());

		io_print("  BINDIR := %s\n", gnu_escapePath(prj_get_bindir()));
		io_print("  LIBDIR := %s\n", gnu_escapePath(prj_get_libdir()));
		io_print("  OBJDIR := %s\n", gnu_escapePath(prj_get_objdir()));
		io_print("  OUTDIR := %s\n", gnu_escapePath(prj_get_outdir()));

		/* Write preprocessor flags - how to generate dependencies for DMC? */
		io_print("  CPPFLAGS :=");
		if (!matches(g_cc, "dmc"))
			io_print(" $(DEPFLAGS)");
		print_list(prj_get_defines(), " -D \"", "\"", "", NULL);
		print_list(prj_get_incpaths(), " -I \"", "\"", "", NULL);
		io_print("\n");

		/* Write C flags */
		io_print("  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH)");
		if (prj_is_kind("dll") && !os_is("windows"))
			io_print(" -fPIC");
		if (!prj_has_flag("no-symbols"))
			io_print(" -g");
		if (prj_has_flag("optimize-size"))
			io_print(" -Os");
		if (prj_has_flag("optimize-speed"))
			io_print(" -O3");
		if (prj_has_flag("optimize") && !prj_has_flag("optimize-size") && !prj_has_flag("optimize-speed"))
			io_print(" -O2");
		if (prj_has_flag("extra-warnings"))
			io_print(" -Wall");
		if (prj_has_flag("fatal-warnings"))
			io_print(" -Werror");
		if (prj_has_flag("no-frame-pointer"))
			io_print(" -fomit-frame-pointer");
		print_list(prj_get_buildoptions(), " ", "", "", NULL);
		io_print("\n");

		/* Write C++ flags */
		io_print("  CXXFLAGS += $(CFLAGS)");
		if (prj_has_flag("no-exceptions"))
			io_print(" --no-exceptions");
		if (prj_has_flag("no-rtti"))
			io_print(" --no-rtti");
		io_print("\n");

		/* Write linker flags */
		io_print("  LDFLAGS += -L$(BINDIR) -L$(LIBDIR)");
		if (prj_is_kind("dll") && (g_cc == NULL || matches(g_cc, "gcc")))
		{
			if (!os_is("macosx"))
				io_print(" -shared");
			
			/* Create import libraries for DLLs if we're using Cygwin or MinGW.
			 * This seems fragile to me, but I'm going to let it sit and see
			 * if anyone complains about it. */
			if (!prj_has_flag("no-import-lib") && os_is("windows"))
			{
				const char* str;

				io_print(" -Wl,--out-implib=\"%s", prj_get_libdir());
				if (prj_get_prefix() == NULL)
					str = "lib";
				else
					str = prj_get_prefix();

				if (prj_has_importlibname())
					io_print("/%s%s.a\"", str, prj_get_importlibname());
				else
					io_print("/%s%s.a\"", str, path_getbasename(prj_get_target()));
			}
		}
		if (prj_is_kind("winexe"))
			io_print(" -mwindows");

		/* OS X has a bug, see http://lists.apple.com/archives/Darwin-dev/2006/Sep/msg00084.html */
		if (prj_has_flag("no-symbols"))
		{
			if (os_is("macosx"))
				io_print(" -Wl,-x");
			else
				io_print(" -s");
		}

		if (os_is("macosx") && prj_has_flag("dylib"))
			io_print(" -dynamiclib -flat_namespace");
		print_list(prj_get_linkoptions(), " ", "", "", NULL);
		print_list(prj_get_libpaths(), " -L\"", "\"", "", NULL);
		print_list(prj_get_links(), " ", "", "", filterLinks);
		io_print("\n");

		/* Build a list of libraries this target depends on */
		io_print("  LDDEPS :=");
		print_list(prj_get_links(), " ", "", "", listLinkerDeps);
		io_print("\n");

		/* Build a list of flags for the resource compiler */
		io_print("  RESFLAGS :=");
		print_list(prj_get_resdefines(), " -D \"", "\"", "", NULL);
		print_list(prj_get_respaths(), " -I \"", "\"", "", NULL);
		print_list(prj_get_resoptions(), " ", "", "", NULL);
		io_print("\n");

		/* Build the target name */
		io_print("  TARGET := %s\n", gnu_escapePath(path_getname(prj_get_target())));
		if (os_is("macosx") && prj_is_kind("winexe"))
			io_print("  MACAPP := %s.app/Contents\n", gnu_escapePath(path_getname(prj_get_target())));


		/* Build command */
		io_print(" BLDCMD = ");
		if (prj_is_kind("lib"))
		{
			io_print("ar -rcs $(OUTDIR)/$(TARGET) $(OBJECTS) $(TARGET_ARCH)");
		}
		else if (prj_is_kind("winexe") && os_is("macosx"))
		{
			io_print("$(%s) -o $(OUTDIR)/$(MACAPP)/MacOS/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)", prj_is_lang("c") ? "CC" : "CXX");
		}
		else
		{
			io_print("$(%s) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)", prj_is_lang("c") ? "CC" : "CXX");
		}
		io_print("\n");

		/* Pre-build commands */
		if (prj_get_numprebuildcommands() > 0)
		{
			io_print("\n  define prebuildcmd\n");
			io_print("\t@echo Running pre-build commands...\n");
			print_list(prj_get_prebuildcommands(), "\t", "\n", "", NULL);
			io_print("  endef\n");
		}

		/* Pre-link commands */
		if (prj_get_numprelinkcommands() > 0)
		{
			io_print("\n  define prelinkcmd\n");
			io_print("\t@echo Running pre-link commands...\n");
			print_list(prj_get_prelinkcommands(), "\t", "\n", "", NULL);
			io_print("  endef\n");
		}	

		/* Post-build commands */
		if (prj_get_numpostbuildcommands() > 0)
		{
			io_print("\n  define postbuildcmd\n");
			io_print("\t@echo Running post-build commands...\n");
			print_list(prj_get_postbuildcommands(), "\t", "\n", "", NULL);
			io_print("  endef\n");
		}

		io_print("endif\n\n");
	}

	/* Write out the list of object file targets for all C/C++ sources */
	io_print("OBJECTS := \\\n");
	print_list(prj_get_files(), "\t$(OBJDIR)/", " \\\n", "", listCppSources);
	io_print("\n");

	/* Write out the list of resource files for windows targets */
	if (os_is("windows"))
	{
		io_print("RESOURCES := \\\n");
		print_list(prj_get_files(), "\t$(OBJDIR)/", " \\\n", "", listRcSources);
		io_print("\n");
	}

	/* Try to figure out how to safely create a directory */
	io_print("MKDIR_TYPE := msdos\n");
	io_print("CMD := $(subst \\,\\\\,$(ComSpec)$(COMSPEC))\n");
	io_print("ifeq (,$(CMD))\n");
	io_print("  MKDIR_TYPE := posix\n");
	io_print("endif\n");
	io_print("ifeq (/bin,$(findstring /bin,$(SHELL)))\n");
	io_print("  MKDIR_TYPE := posix\n");
	io_print("endif\n");
	io_print("ifeq ($(MKDIR_TYPE),posix)\n");
	io_print("  CMD_MKBINDIR := mkdir -p $(BINDIR)\n");
	io_print("  CMD_MKLIBDIR := mkdir -p $(LIBDIR)\n");
	io_print("  CMD_MKOUTDIR := mkdir -p $(OUTDIR)\n");
	io_print("  CMD_MKOBJDIR := mkdir -p $(OBJDIR)\n");
	io_print("else\n");
	io_print("  CMD_MKBINDIR := $(CMD) /c if not exist $(subst /,\\\\,$(BINDIR)) mkdir $(subst /,\\\\,$(BINDIR))\n");
	io_print("  CMD_MKLIBDIR := $(CMD) /c if not exist $(subst /,\\\\,$(LIBDIR)) mkdir $(subst /,\\\\,$(LIBDIR))\n");
	io_print("  CMD_MKOUTDIR := $(CMD) /c if not exist $(subst /,\\\\,$(OUTDIR)) mkdir $(subst /,\\\\,$(OUTDIR))\n");
	io_print("  CMD_MKOBJDIR := $(CMD) /c if not exist $(subst /,\\\\,$(OBJDIR)) mkdir $(subst /,\\\\,$(OBJDIR))\n");
	io_print("endif\n");
	io_print("\n");

	io_print(".PHONY: clean");
	if (prj_get_numprebuildcommands() > 0)
		io_print(" prebuild");
	if (prj_get_numprelinkcommands() > 0)
		io_print(" prelink");
	io_print("\n\n");

	/* Write the main build target */
	if (os_is("macosx") && prj_is_kind("winexe"))
	{
		io_print("all: $(OUTDIR)/$(MACAPP)/PkgInfo $(OUTDIR)/$(MACAPP)/Info.plist $(OUTDIR)/$(MACAPP)/MacOS/$(TARGET)\n\n");
		io_print("$(OUTDIR)/$(MACAPP)/MacOS/$(TARGET)");
	}
	else
	{
		io_print("$(OUTDIR)/$(TARGET)");
	}

	io_print(": ");
	if (prj_get_numprebuildcommands() > 0)
		io_print("prebuild ");
	io_print("$(OBJECTS) $(LDDEPS) $(RESOURCES)");
	if (prj_get_numprelinkcommands() > 0)
		io_print(" prelink");
	io_print("\n");
	if (!g_verbose)
		io_print("\t@echo Linking %s\n", prj_get_pkgname());
	io_print("\t-%s$(CMD_MKBINDIR)\n", prefix);
	io_print("\t-%s$(CMD_MKLIBDIR)\n", prefix);
	io_print("\t-%s$(CMD_MKOUTDIR)\n", prefix);
	if (os_is("macosx") && prj_is_kind("winexe"))
		io_print("\t-%sif [ ! -d $(OUTDIR)/$(MACAPP)/MacOS ]; then mkdir -p $(OUTDIR)/$(MACAPP)/MacOS; fi\n", prefix);
	
	io_print("\t%s$(BLDCMD)\n", prefix);
	
	if (prj_get_numpostbuildcommands() > 0)
		io_print("\t$(postbuildcmd)\n");

	io_print("\n");
	
	if (prj_get_numprebuildcommands() > 0)
	{
		io_print("prebuild:\n");
		io_print("\t$(prebuildcmd)\n\n");
	}
	
	if (prj_get_numprelinkcommands() > 0)
	{
		io_print("prelink:\n");
		io_print("\t$(prelinkcmd)\n\n");
	}	

	if (os_is("macosx") && prj_is_kind("winexe"))
	{
		io_print("$(OUTDIR)/$(MACAPP)/PkgInfo:\n\n");
		io_print("$(OUTDIR)/$(MACAPP)/Info.plist:\n\n");
	}

	/* Write the "clean" target */
	io_print("clean:\n");
	io_print("\t@echo Cleaning %s\n", prj_get_pkgname());
	io_print("ifeq ($(MKDIR_TYPE),posix)\n");
	if (os_is("macosx") && prj_is_kind("winexe"))
		io_print("\t-%srm -rf $(OUTDIR)/$(TARGET).app\n", prefix);
	else
		io_print("\t-%srm -f $(OUTDIR)/$(TARGET)\n", prefix);
	io_print("\t-%srm -rf $(OBJDIR)\n", prefix);
	io_print("else\n");
	io_print("\t-%sif exist $(subst /,\\,$(OUTDIR)/$(TARGET)) del /q $(subst /,\\,$(OUTDIR)/$(TARGET))\n", prefix);
	io_print("\t-%sif exist $(subst /,\\,$(OBJDIR)) del /q $(subst /,\\,$(OBJDIR))\n", prefix);
	io_print("\t-%sif exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))\n", prefix);
	io_print("endif\n\n");

	/* Write static patterns for each source file. Note that in earlier
	 * versions I used pattern rules instead of listing each file. It worked
	 * fine but made it more difficult to test and also required the use of
	 * VPATH which I didn't like. This new approach of listing each file
	 * helps testing and opens the way for per-file configurations */
	print_list(prj_get_files(), "", "\n", "", listCppTargets);
	
	if (os_is("windows"))
		print_list(prj_get_files(), "", "", "", listRcTargets);

	/* Include the automatically generated dependency lists */
	io_print("-include $(OBJECTS:%%.o=%%.d)\n\n");

	io_closefile();
	return 1;
}



/************************************************************************
 * Checks each entry in the list of package links. If the entry refers
 * to a sibling package, returns the path to that package's output
 ***********************************************************************/

static const char* filterLinks(const char* name)
{
	int i = prj_find_package(name);
	if (i >= 0)
	{
		/* is a sibling */
		const char* kind =  prj_get_kind_for(i);
        const char* lang = prj_get_language_for(i);
        if ((matches(lang, "c++") || matches(lang, "c")) &&
			(matches(kind, "dll") || matches(kind, "lib")))
		{
			const char* dir;
			const char* target;
			target = prj_get_target_for(i);

			strcpy(g_buffer, "");

			/* Add -L if path to sibling libraries, so they don't get
			 * encoded in this linking binary (see bug #1729227) */
			dir = path_getdir(target);
			if (!prj_has_libpath(dir))
			{
				strcat(g_buffer, "-L");
				strcat(g_buffer, dir);
				strcat(g_buffer, " ");
			}

			strcat(g_buffer, "-l");
			strcat(g_buffer, prj_get_targetname_for(i));
			return g_buffer;
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		/* is not a sibling, just output directly */
		strcpy(g_buffer, "-l");
		strcat(g_buffer, name);
		return g_buffer;
	}
}


/************************************************************************
 * Checks each source code file and filters out everything that is
 * not a C or C++ file
 ***********************************************************************/

static const char* listCppSources(const char* name)
{
	if (is_cpp(name))
	{
		strcpy(g_buffer, gnu_escapePath(path_getbasename(name)));
		strcat(g_buffer, ".o");
		return g_buffer;
	}

	return NULL;
}


/************************************************************************
 * Checks each source code file and filters out everything that is
 * not a windows resource file
 ***********************************************************************/

static const char* listRcSources(const char* name)
{
	const char* ext = path_getextension(name);
	if (matches(ext, ".rc"))
	{
		strcpy(g_buffer, gnu_escapePath(path_getbasename(name)));
		strcat(g_buffer, ".res");
		return g_buffer;
	}

	return NULL;
}


/************************************************************************
 * Creates the makefile build rules for all source code files
 ***********************************************************************/

static const char* listCppTargets(const char* name)
{
	if (is_cpp(name))
	{
		const char* ext = path_getextension(name);

		sprintf(g_buffer, "$(OBJDIR)/%s.o: ", gnu_escapePath(path_getbasename(name)));
		strcat(g_buffer, gnu_escapePath(name));
		strcat(g_buffer, "\n\t-");
		if (!g_verbose)
			strcat(g_buffer, "@");
		strcat(g_buffer, "$(CMD_MKOBJDIR)\n");

		if (!g_verbose)
			strcat(g_buffer, "\t@echo $(notdir $<)\n");
      
		strcat(g_buffer, "\t");
		if (!g_verbose)
			strcat(g_buffer, "@");
		if (matches(g_cc, "dmc"))
		{
			/* Digital Mars compiler build step */
			/* FIXME: How to handle assembly files with DMC? */
			if (matches(ext, ".c"))
				strcat(g_buffer, "dmc $(CFLAGS) -o \"$@\" -c \"$<\"\n");
			else if (!matches(ext, ".s"))
				strcat(g_buffer, "dmc -cpp -Ae -Ar -mn $(CXXFLAGS) -o \"$@\" -c \"$<\"\n");
		}
		else
		{
			/* GNU GCC compiler build step */
			if (matches(ext, ".s"))
				strcat(g_buffer, "$(CC) -x assembler-with-cpp $(CPPFLAGS) -o \"$@\" -c \"$<\"\n");
			else if (matches(ext, ".c"))
				strcat(g_buffer, "$(CC) $(CFLAGS) -o \"$@\" -c \"$<\"\n");
			else
				strcat(g_buffer, "$(CXX) $(CXXFLAGS) -o \"$@\" -c \"$<\"\n");
		}

		return g_buffer;
	}
	else
	{
		return NULL;
	}
}


/************************************************************************
 * Creates the makefile build rules for windows resource files
 ***********************************************************************/

static const char* listRcTargets(const char* name)
{
	const char* prefix = (g_verbose) ? "" : "@";
	name = gnu_escapePath(name);
	if (matches(path_getextension(name), ".rc"))
	{
		const char* base = path_getbasename(name);

		io_print("$(OBJDIR)/%s.res: %s\n", base, name);
		io_print("\t-%s$(CMD_MKOBJDIR)\n", prefix);
		if (!g_verbose)
			io_print("\t@echo $(notdir $<)\n");
		if (matches(g_cc, "dmc"))
			io_print("\t%srcc $< -o \"$@\" $(RESFLAGS)\n", prefix);
		else
			io_print("\t%swindres $< -O coff -o \"$@\" $(RESFLAGS)\n", prefix);
		io_print("\n");
	}

	return NULL;
}


/************************************************************************
 * This is called by the code that builds the list of dependencies for 
 * the link step. It looks for sibling projects, and then returns the 
 * full path to that target's output. So if an executable package 
 * depends on a library package, the library filename will be listed 
 * as a dependency
 ***********************************************************************/

static const char* listLinkerDeps(const char* name)
{
	int i = prj_find_package(name);
	if (i >= 0)
	{
		return prj_get_target_for(i);
	}

	return NULL;
}
