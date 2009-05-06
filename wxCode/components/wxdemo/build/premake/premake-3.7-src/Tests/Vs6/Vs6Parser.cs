using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs6
{
	public class Vs6Parser : Parser
	{
		#region Parser Methods
		public override string TargetName
		{
			get { return "vs6"; }
		}
		#endregion

		#region Workspace Parsing

		public override void Parse(Project project, string filename)
		{
			/* File header */
			Begin(filename + ".dsw");

			Match("Microsoft Developer Studio Workspace File, Format Version 6.00");
			Match("# WARNING: DO NOT EDIT OR DELETE THIS WORKSPACE FILE!");
			Match("");
			Match("###############################################################################");
			Match("");

			Hashtable dependencies = new Hashtable();
			string[] matches;
			while (!Match("Global:", true))
			{
				Package package = new Package();
				project.Package.Add(package);

				matches = Regex("Project: \"(.+)\"=(.+) - Package Owner=<4>");
				package.Name       = matches[0];
				package.Path       = Path.GetDirectoryName(matches[1]);
				package.ScriptName = Path.GetFileName(matches[1]);
				package.Language   = "c++";
				Match("");

				Match("Package=<5>");
				Match("{{{");
				Match("}}}");
				Match("");
				Match("Package=<4>");
				Match("{{{");

				/* Get the package dependencies, cache until I have configs */
				ArrayList deps = new ArrayList();
				while (Match("    Begin Project Dependency", true))
				{
					matches = Regex("    Project_Dep_Name (.+)");
					deps.Add(matches[0]);
					Match("    End Project Dependency");
				}
				dependencies[package] = (string[])deps.ToArray(typeof(string));

				Match("}}}");
				Match("");
				Match("###############################################################################");
				Match("");
			}

			Match("");
			Match("Package=<5>");
			Match("{{{");
			Match("}}}");
			Match("");
			Match("Package=<3>");
			Match("{{{");
			Match("}}}");
			Match("");
			Match("###############################################################################");
			Match("");

			foreach (Package package in project.Package)
			{
				filename = Path.Combine(Path.Combine(project.Path, package.Path), package.ScriptName);
				switch (package.Language)
				{
				case "c++":
					ParseCpp(project, package, filename);
					break;
				default:
					throw new NotImplementedException("Loading of " + package.Language + " packages not implemented");
				}

				ArrayList temp = new ArrayList();
				foreach (Configuration config in package.Config)
				{
					config.Dependencies = (string[])dependencies[package];
					temp.Add(config);
				}

				/* VS6 stores configs in reverse order */
				temp.Reverse();
				for (int i = 0; i < package.Config.Count; ++i)
					package.Config[i] = (Configuration)temp[i];
			}
		}

		#endregion

		#region C++ Parsing
		
		private void ParseCpp(Project project, Package package, string filename)
		{
			Begin(filename);
			Match("# Microsoft Developer Studio Project File - Name=\"" + package.Name + "\" - Package Owner=<4>");
			Match("# Microsoft Developer Studio Generated Build File, Format Version 6.00");
			Match("# ** DO NOT EDIT **");
			Match("");

			string[] matches = Regex("# TARGTYPE (.+)");
			switch (matches[0])
			{
			case "\"Win32 (x86) Application\" 0x0101":
				package.Kind = "winexe";
				break;
			case "\"Win32 (x86) Console Application\" 0x0103":
				package.Kind = "exe";
				break;
			case "\"Win32 (x86) Dynamic-Link Library\" 0x0102":
				package.Kind = "dll";
				break;
			case "\"Win32 (x86) Static Library\" 0x0104":
				package.Kind = "lib";
				break;
			default:
				throw new FormatException("Unrecognized target type: '" + matches[0] + "'");
			}
			string baseKind = matches[0].Substring(0, matches[0].Length - 7);
			Match("");
			
			matches = Regex("CFG=" + package.Name + " - (.+)");
			string defaultConfig = matches[0];

			Match("!MESSAGE This is not a valid makefile. To build this project using NMAKE,");
			Match("!MESSAGE use the Export Makefile command and run");
			Match("!MESSAGE ");
			Match("!MESSAGE NMAKE /f \"" + package.Name + ".mak\".");
			Match("!MESSAGE ");
			Match("!MESSAGE You can specify a configuration when running NMAKE");
			Match("!MESSAGE by defining the macro CFG on the command line. For example:");
			Match("!MESSAGE ");
			Match("!MESSAGE NMAKE /f \"" + package.Name + ".mak\" CFG=\"" + package.Name + " - " + defaultConfig + "\"");
			Match("!MESSAGE ");
			Match("!MESSAGE Possible choices for configuration are:");
			Match("!MESSAGE ");

			baseKind = baseKind.Replace("(", "\\(");
			baseKind = baseKind.Replace(")", "\\)");
			while (!Match("!MESSAGE ", true))
			{
				matches = Regex("!MESSAGE \"" + package.Name + " - ([^\"]+)\" \\(based on " + baseKind + "\\)");

				Configuration config = new Configuration();
				config.Name = matches[0];
				config.Dependencies = new string[0];
				package.Config.Add(config);
			}
			Match("");

			/* Verify configurations */
			if (project.Configuration.Count == 0)
			{
				foreach (Configuration config in package.Config)
					project.Configuration.Add(config.Name);
			}
			else
			{
				foreach (Configuration config in package.Config)
					if (!project.Configuration.Contains(config.Name))
						throw new FormatException("Config '" + config.Name + "' not found in project");
			}

			Match("# Begin Project");
			Match("# PROP AllowPerConfigDependencies 0");
			Match("# PROP Scc_ProjName \"\"");
			Match("# PROP Scc_LocalPath \"\"");
			Match("CPP=cl.exe");
			if (package.Kind != "lib")
				Match("MTL=midl.exe");
			Match("RSC=rc.exe");
			Match("");

			bool first = true;
			foreach (Configuration config in package.Config)
			{
				ArrayList bldflags = new ArrayList();
				ArrayList lnkflags = new ArrayList();
				ArrayList defines  = new ArrayList();
				ArrayList incpaths = new ArrayList();
				ArrayList links    = new ArrayList();
				ArrayList libpaths = new ArrayList();

				string cond = (first) ? "!IF" : "!ELSEIF";
				first = false;

				Match(cond + "  \"$(CFG)\" == \"" + package.Name + " - " + config.Name + "\"");
				Match("");

				Match("# PROP BASE Use_MFC 0");
				
				matches = Regex("# PROP BASE Use_Debug_Libraries ([0-1])");
				string useDebugLib = matches[0];

				matches = Regex("# PROP BASE Output_Dir \"(.+)\"");
				config.OutDir = matches[0];
				config.BinDir = matches[0];
				config.LibDir = matches[0];

				matches = Regex("# PROP BASE Intermediate_Dir \"(.+)\"");
				config.ObjDir = matches[0];

				Match("# PROP BASE Target_Dir \"\"");
				Match("# PROP Use_MFC 0");
				Match("# PROP Use_Debug_Libraries " + useDebugLib);
				Match("# PROP Output_Dir \"" + config.OutDir + "\"");
				Match("# PROP Intermediate_Dir \"" + config.ObjDir + "\"");

				if (Match("# PROP Ignore_Export_Lib 1", true))
					bldflags.Add("no-import-lib");

				Match("# PROP Target_Dir \"\"");

				matches = Regex("# ADD BASE CPP /nologo (.+)");
				string flags = matches[0];
				matches = flags.Split(' ');

				int i = 0;
				if (matches[i] == "/MTd" || matches[i] == "/MT")
				{
					lnkflags.Add("static-runtime");
				}
				else 
				{
					Expect(matches[i], "/MDd", "/MD");
				}
				++i;

				if (matches[i] == "/W4")
				{
					bldflags.Add("extra-warnings");
				}
				else
				{
					Expect(matches[i], "/W3");
				}
				++i;

				if (matches[i] == "/WX")
				{
					bldflags.Add("fatal-warnings");
					++i;
				}

				if (matches[i] == "/Gm")
				{
					++i;
				}

				if (matches[i] == "/GR")
				{
					++i;
				}
				else
				{
					bldflags.Add("no-rtti");
				}

				if (matches[i] == "/GX")
				{
					++i;
				}
				else
				{
					bldflags.Add("no-exceptions");
				}

				if (matches[i] == "/ZI")
				{
					++i;
				}
				else
				{
					bldflags.Add("no-symbols");
				}

				if (matches[i] == "/O1")
				{
					bldflags.Add("optimize-size");
				}
				else if (matches[i] == "/O2")
				{
					bldflags.Add("optimize");
				}
				else 
				{
					Expect(matches[i], "/Od");
				}
				++i;

				if (matches[i] == "/Oy")
				{
					bldflags.Add("no-frame-pointer");
					++i;
				}

				while (matches[i] == "/I")
				{
					incpaths.Add(matches[++i].Trim('\"'));
					++i;
				}

				while (matches[i] == "/D")
				{
					defines.Add(matches[++i].Trim('\"'));
					++i;
				}

				Expect(matches[i], "/YX");
				Expect(matches[++i], "/FD");
				if (matches[++i] == "/GZ")
					++i;
				Expect(matches[i++], "/c");

				config.BuildOptions = String.Empty;
				while (i < matches.Length)
					config.BuildOptions += " " + matches[i++];
				config.BuildOptions = config.BuildOptions.Trim();

				Match("# ADD CPP /nologo " + flags);

				string debugSymbol = bldflags.Contains("no-symbols") ? "NDEBUG" : "_DEBUG";

				bool hasMtl = Match("# ADD BASE MTL /nologo /D \"" + debugSymbol + "\" /mktyplib203 /win32", true);
				if (hasMtl)
					Match("# ADD MTL /nologo /D \"" + debugSymbol + "\" /mktyplib203 /win32");

				ArrayList resDefines = new ArrayList();
				ArrayList resPaths = new ArrayList();
				matches = Regex("# ADD BASE RSC /l 0x409 (.+)");
				while (matches[0].Length > 0)
				{
					if (matches[0].StartsWith("/d") || matches[0].StartsWith("/i"))
					{
						int split = matches[0].IndexOf('"', 4);
						string value = matches[0].Substring(4, split - 4);
						if (matches[0].StartsWith("/d"))
							resDefines.Add(value);
						else
							resPaths.Add(value);
						matches[0] = matches[0].Substring(split + 1).TrimStart();
					}
					else
					{
						config.ResOptions = matches[0];
						matches[0] = "";
					}
				}

				config.ResDefines = (string[])resDefines.ToArray(typeof(string));
				config.ResPaths = (string[])resPaths.ToArray(typeof(string));

				string rsc = "# ADD RSC /l 0x409";
				foreach (string symbol in resDefines)
					rsc += " /d \"" + symbol + "\"";
				foreach (string symbol in resPaths)
					rsc += " /i \"" + symbol + "\"";
				if (config.ResOptions != null)
					rsc += " " + config.ResOptions;
				Match(rsc);
				
				Match("BSC32=bscmake.exe");
				Match("# ADD BASE BSC32 /nologo");
				Match("# ADD BSC32 /nologo");

				if (Match("LINK32=link.exe -lib", true))
				{
					config.Kind = "lib";
					Match("# ADD BASE LIB32 /nologo");
					matches = Regex("# ADD LIB32 /nologo /out:\"(.+)\"", true);
					if (matches != null)
					{
						config.Target = matches[0];
					}
					else
					{
						Match("# ADD LIB32 /nologo");
					}
				}
				else
				{
					Match("LINK32=link.exe");
					matches = Regex("# ADD BASE LINK32 (.+)");
					flags = matches[0];
					matches = flags.Split(' ');

					i = 0;
					while (i < matches.Length && matches[i][0] != '/')
						links.Add(matches[i++]);

					Expect(matches[i++], "/nologo");

					bool noMain = true;
					if (matches[i] == "/entry:\"mainCRTStartup\"")
					{
						noMain = false;
						++i;
					}

					if (matches[i] == "/subsystem:windows")
					{
						config.Kind = "winexe";
					}
					else if (matches[i] == "/subsystem:console")
					{
						config.Kind = "exe";
					}
					else if (matches[i] == "/dll")
					{
						config.Kind = "dll";
					}
					else
					{
						throw new FormatException("Unexpected subsystem flag: " + matches[i]);
					}
					++i;

					if (hasMtl && config.Kind != "winexe" && config.Kind != "dll")
						throw new FormatException("Found unexpected MTL block");

					if ((config.Kind == "winexe" || config.Kind == "exe") && noMain)
						bldflags.Add("no-main");

					if (!bldflags.Contains("no-symbols"))
					{
						Expect(matches[i++], "/incremental:yes");
						Expect(matches[i++], "/debug");
					}

					Expect(matches[i++], "/machine:I386");

					if (config.Kind == "dll")
					{
						config.ImportLib = matches[i].Substring(9, matches[i].Length - 10);
						++i;
					}

					config.Target = matches[i].Substring(6, matches[i].Length - 7);
					if (config.Target.StartsWith(config.OutDir))
						config.Target = config.Target.Substring(config.OutDir.Length + 1);
					++i;

					if (!bldflags.Contains("no-symbols"))
						Expect(matches[i++], "/pdbtype:sept");

					config.LibDir = matches[i].Substring(10, matches[i].Length - 11);
					++i;

					while (i < matches.Length && matches[i].StartsWith("/libpath:"))
					{
						libpaths.Add(matches[i].Substring(10, matches[i].Length - 11));
						++i;
					}

					Match("# ADD LINK32 " + flags);
				}

				config.BuildFlags = (string[])bldflags.ToArray(typeof(string));
				config.LinkFlags  = (string[])lnkflags.ToArray(typeof(string));
				config.Defines    = (string[])defines.ToArray(typeof(string));
				config.IncludePaths = (string[])incpaths.ToArray(typeof(string));
				config.Links      = (string[])links.ToArray(typeof(string));
				config.LibPaths   = (string[])libpaths.ToArray(typeof(string));

				Match("");
			}

			Match("!ENDIF");
			Match("");
			Match("# Begin Target");
			Match("");

			foreach (Configuration config in package.Config)
				Match("# Name \"" + package.Name + " - " + config.Name + "\"");

			string folder = "";
			while (!Match("# End Target", true))
			{
				matches = Regex("# Begin Group \"(.+)\"", true);
				if (matches != null)
				{
					folder = Path.Combine(folder, matches[0]);
					Match("");
					Match("# PROP Default_Filter \"\"");
					continue;
				}

				matches = Regex("# End Group", true);
				if (matches != null)
				{
					folder = Path.GetDirectoryName(folder);
					continue;
				}

				Match("# Begin Source File");
				Match("");
				matches = Regex("SOURCE=(.+)");

				filename = matches[0];
				package.File.Add(filename);

				if (filename.StartsWith("./"))
					filename = filename.Substring(2);
				while (filename.StartsWith("../"))
					filename = filename.Substring(3);
				if (Path.GetDirectoryName(filename) != folder)
					throw new FormatException("File '" + matches[0] + "' is in folder '" + folder + "'");

				Match("# End Source File");
			}
		}
			
		#endregion
	}
}
