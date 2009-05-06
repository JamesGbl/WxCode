using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2002
{
	public class Vs2002Parser : Parser
	{
		#region Parser Methods
		public override string TargetName
		{
			get { return "vs2002"; }
		}
		#endregion

		#region Solution Parsing
		public override void Parse(Project project, string filename)
		{
			/* File header */
			Begin(filename + ".sln");
			Match("Microsoft Visual Studio Solution File, Format Version 7.00");

			/* Package entries - VS "projects" */
			string[] matches;
			do
			{
				matches = Regex("Project\\(\"{([0-9A-F-]+)}\"\\) = \"(.+)\", \"(.+)\", \"{([0-9A-F-]+)}\"", true);
				if (matches != null)
				{
					Package package = new Package();
					project.Package.Add(package);

					package.Name = matches[1];
					package.ID   = matches[3];
					package.Path = Path.GetDirectoryName(matches[2]);
					package.ScriptName = Path.GetFileName(matches[2]);

					if (package.Path != String.Empty)
						package.Path += '\\';

					switch (matches[0])
					{
					case "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942":
						package.Language = "c++";
						break;

					case "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC":
						package.Language = "c#";
						break;
					}
					Match("EndProject");
				}
			} while (matches != null);

			Match("Global");
			Match("\tGlobalSection(SolutionConfiguration) = preSolution");

			/* Read the list of configurations */
			int i = 0;
			do
			{
				matches = Regex("\t\tConfigName[.]" + i + "[ ]=[ ](.+)", true);
				if (matches != null)
					project.Configuration.Add(matches[0]);
				++i;
			} while (matches != null);

			Hashtable packageDependencies = new Hashtable();
			foreach (Package package in project.Package)
			{
				package.Config.Add(project);
				packageDependencies[package] = new ArrayList();
			}

			Match("\tEndGlobalSection");

			/* Collect package dependencies, I'll sort them out after I finish 
			 * parsing the scripts below */
			Match("\tGlobalSection(ProjectDependencies) = postSolution");
			while (!Match("\tEndGlobalSection", true))
			{
				matches = Regex("\t\t{([0-9A-F-]+)}[.]([0-9]+) = {([0-9A-F-]+)}");
				foreach (Package p2 in project.Package)
				{
					if (p2.ID == matches[0])
					{
						ArrayList pkgdeps = (ArrayList)packageDependencies[p2];
						if (int.Parse(matches[1]) != pkgdeps.Count)
							throw new FormatException("Package dependency index should be " + pkgdeps.Count + ", is " + matches[1]);
						pkgdeps.Add(matches[2]);
					}
				}
			}
			
			Match("\tGlobalSection(ProjectConfiguration) = postSolution");

			/* Read the list of package configurations */
			foreach (Package package in project.Package)
			{
				string arch = (package.Language == "c++") ? "Win32" : ".NET";

				foreach (string config in project.Configuration)
				{
					string pattern = "\t\t{" + package.ID + "}." + config + ".ActiveCfg = " + config + "|" + arch;
					Match(pattern);
					pattern = "\t\t{" + package.ID + "}." + config + ".Build.0 = " + config + "|" + arch;
					Match(pattern);
				}
			}

			Match("\tEndGlobalSection");
			Match("\tGlobalSection(ExtensibilityGlobals) = postSolution");
			Match("\tEndGlobalSection");
			Match("\tGlobalSection(ExtensibilityAddIns) = postSolution");
			Match("\tEndGlobalSection");
			Match("EndGlobal");

			foreach (Package package in project.Package)
			{
				filename = Path.Combine(Path.Combine(project.Path, package.Path), package.ScriptName);
				switch (package.Language)
				{
				case "c++":
					ParseCpp(project, package, filename);
					break;
				case "c#":
					ParseCs(project, package, filename);
					break;
				default:
					throw new NotImplementedException("Loading of " + package.Language + " packages not implemented");
				}
			}

			/* Now sort out the inter-package dependencies */
			foreach (Package package in project.Package)
			{
				ArrayList deps = (ArrayList)packageDependencies[package];
				string[] deplist = new string[deps.Count];

				for (i = 0; i < deps.Count; ++i)
				{
					foreach (Package p2 in project.Package)
					{
						if (p2.ID == (string)deps[i])
							deplist[i] = p2.Name;
					}
				}

				foreach (Configuration config in package.Config)
					config.Dependencies = deplist;
			}
		}
		#endregion

		#region C++ Parsing
		private void ParseCpp(Project project, Package package, string filename)
		{
			Begin(filename);

			Match("<?xml version=\"1.0\" encoding=\"Windows-1252\"?>");
			Match("<VisualStudioProject");
			Match("\tProjectType=\"Visual C++\"");
			Match("\tVersion=\"7.00\"");
			Match("\tName=\"" + package.Name + "\"");

			string[] matches = Regex("\tProjectGUID=\"{([A-F0-9-]+)}\"");
			if (matches[0] != package.ID)
				throw new FormatException("Solution (" + package.ID + ") and project (" + matches[0] + ") GUIDs don't match");

			Match("\tKeyword=\"Win32Proj\">");
		
			Match("\t<Platforms>");
			Match("\t\t<Platform");
			Match("\t\t\tName=\"Win32\"/>");
			Match("\t</Platforms>");

			Match("\t<Configurations>");
			
			foreach (Configuration config in package.Config)
			{
				ArrayList buildFlags = new ArrayList();

				Match("\t\t<Configuration");
				Match("\t\t\tName=\"" + config.Name + "|Win32\"");
				
				matches = Regex("\t\t\tOutputDirectory=\"(.+)\"");
				config.OutDir = matches[0];

				matches = Regex("\t\t\tIntermediateDirectory=\"(.+)\"");
				config.ObjDir = matches[0];

				matches = Regex("\t\t\tConfigurationType=\"([0-9])\"");
				switch (int.Parse(matches[0]))
				{
				case 1:  config.Kind = "exe";  break;
				case 2:  config.Kind = "dll";  break;
				case 4:  config.Kind = "lib";  break;
				default:
					throw new FormatException("Unrecognized value: ConfigurationType=\"" + matches[0] + "\"");
				}

				if (config.Kind == "lib")
					config.LibDir = config.OutDir;
				else
					config.BinDir = config.OutDir;

				matches = Regex("\t\t\tCharacterSet=\"([1-2])\">");
				if (matches[0] == "1")
					buildFlags.Add("unicode");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCCLCompilerTool\"");

				matches = Regex("\t\t\t\tAdditionalOptions=\"(.+)\"", true);
				config.BuildOptions = (matches != null) ? matches[0] : String.Empty;

				matches = Regex("\t\t\t\tOptimization=\"([0-9])\"");
				int optimization = int.Parse(matches[0]);
				switch (optimization)
				{
				case 0:  break;
				case 1:  buildFlags.Add("optimize-size");  break;
				case 2:  buildFlags.Add("optimize-speed"); break;
				case 3:  buildFlags.Add("optimize"); break;
				default:
					throw new FormatException("Unrecognized value: Optimization=\"" + matches[0] + "\"");
				}

				if (Match("\t\t\t\tOmitFramePointers=\"TRUE\"", true))
					buildFlags.Add("no-frame-pointer");

				matches = Regex("\t\t\t\tAdditionalIncludeDirectories=\"(.+)\"", true);
				config.IncludePaths =  (matches != null) ? matches[0].Split(';') : new string[]{};

				matches = Regex("\t\t\t\tPreprocessorDefinitions=\"(.+)\"", true);
				config.Defines = (matches != null) ? matches[0].Split(';') : new string[]{};

				if (optimization == 0)
					Match("\t\t\t\tMinimalRebuild=\"TRUE\"");
				
				if (Match("\t\t\t\tExceptionHandling=\"FALSE\"", true))
					buildFlags.Add("no-exceptions");

				if (optimization > 0)
					Match("\t\t\t\tStringPooling=\"TRUE\"", true);

				if (optimization == 0)
					Match("\t\t\t\tBasicRuntimeChecks=\"3\"", true);

				if (optimization == 0)
					matches = Regex("\t\t\t\tRuntimeLibrary=\"(1|3)\"");
				else
					matches = Regex("\t\t\t\tRuntimeLibrary=\"(0|2)\"");

				if (matches[0] == "0" || matches[0] == "1")
					config.LinkFlags = new string[] { "static-runtime" };
				
				Match("\t\t\t\tEnableFunctionLevelLinking=\"TRUE\"");

				if (!Match("\t\t\t\tRuntimeTypeInfo=\"TRUE\"", true))
					buildFlags.Add("no-rtti");

				matches = Regex("\t\t\t\tTreatWChar_tAsBuiltInType=\"(TRUE|FALSE)\"", true);
				if (matches != null)
					buildFlags.Add(matches[0] == "TRUE" ? "native-wchar" : "no-native-wchar");

				config.Pch = "auto";
				matches = Regex("\t\t\t\tUsePrecompiledHeader=\"([0-9])\"");
				if (matches[0] == "3")
				{
					config.Pch = "on";
					matches = Regex("\t\t\t\tPrecompiledHeaderThrough=\"(.+?)\"");
					config.PchHeader = matches[0];
				}
				else if (matches[0] == "0")
				{
					config.Pch = "off";
				}
				else if (matches[0] != "2")
				{
					throw new FormatException("Expected UsePrecompiledHeader to be 2, got " + matches[0]);
				}

				matches = Regex("\t\t\t\tWarningLevel=\"([3-4])\"");
				if (matches[0] == "4")
					buildFlags.Add("extra-warnings");
				
				if (Match("\t\t\t\tWarnAsError=\"TRUE\"", true))
					buildFlags.Add("fatal-warnings");
				
				matches = Regex("\t\t\t\tDetect64BitPortabilityProblems=\"(TRUE|FALSE)\"");
				if (matches[0] == "FALSE")
					buildFlags.Add("no-64bit-checks");
					
				matches = Regex("\t\t\t\tDebugInformationFormat=\"([0-9])\"/>");
				if (matches[0] == "0")
					buildFlags.Add("no-symbols");
				else if (matches[0] == "3")
					buildFlags.Add("no-edit-and-continue");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCCustomBuildTool\"/>");

				Match("\t\t\t<Tool");
				if (config.Kind == "lib")
				{
					Match("\t\t\t\tName=\"VCLibrarianTool\"");
					matches = Regex("\t\t\t\tOutputFile=\"\\$\\(OutDir\\)/(.+)\"/>");
					config.Target = matches[0];
				}
				else
				{
					Match("\t\t\t\tName=\"VCLinkerTool\"");
            
					matches = Regex("\t\t\t\tAdditionalDependencies=\"(.+)\"", true);
					if (matches != null)
						config.Links = matches[0].Split(' ');

					if (Match("\t\t\t\tIgnoreImportLibrary=\"TRUE\"", true))
						buildFlags.Add("no-import-lib");

					matches = Regex("\t\t\t\tOutputFile=\"\\$\\(OutDir\\)/(.+)\"");
					config.Target = matches[0];

					Match("\t\t\t\tLinkIncremental=\"" + (optimization == 0 ? 2 : 1) + "\"");

					matches = Regex("\t\t\t\tAdditionalLibraryDirectories=\"(.+)\"");
					matches = matches[0].Split(';');
					config.LibDir = matches[0];
					if (config.LibDir != config.OutDir)
						config.BinDir = config.OutDir;

					config.LibPaths = new string[matches.Length - 1];
					for (int i = 0; i < matches.Length - 1; ++i)
						config.LibPaths[i] = matches[i + 1];

					matches = Regex("\t\t\t\tModuleDefinitionFile=\"(.+)\"", true);
					if (matches != null)
						package.DefFile = matches[0];

					if (Match("\t\t\t\tGenerateManifest=\"FALSE\"", true))
						buildFlags.Add("no-manifest");

					string expected = buildFlags.Contains("no-symbols") ? "FALSE" : "TRUE";
					Match("\t\t\t\tGenerateDebugInformation=\"" + expected + "\"");

					if (!buildFlags.Contains("no-symbols"))
						Match("\t\t\t\tProgramDatabaseFile=\"$(OutDir)/" + Path.GetFileNameWithoutExtension(config.Target) + ".pdb\"");

					matches = Regex("\t\t\t\tSubSystem=\"([0-9])\"");
					if (config.Kind == "exe" && matches[0] == "2")
						config.Kind = "winexe";

					if (optimization > 0)
					{
						Match("\t\t\t\tOptimizeReferences=\"2\"");
						Match("\t\t\t\tEnableCOMDATFolding=\"2\"");
					}

					
					if (config.Kind == "exe" || config.Kind == "winexe")
					{
						if (!Match("\t\t\t\tEntryPointSymbol=\"mainCRTStartup\"", true))
							buildFlags.Add("no-main");
					}
					else
					{
						matches = Regex("\t\t\t\tImportLibrary=\"(.+)\"");
						config.ImportLib = matches[0];
					}

					Match("\t\t\t\tTargetMachine=\"1\"/>");

				}

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCMIDLTool\"/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPostBuildEventTool\"/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPreBuildEventTool\"/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPreLinkEventTool\"/>");

				Match("\t\t\t<Tool");
				if (!Match("\t\t\t\tName=\"VCResourceCompilerTool\"/>", true))
				{
					Match("\t\t\t\tName=\"VCResourceCompilerTool\"");
					matches = Regex("\t\t\t\tAdditionalOptions=\"(.+)\"(.*)", true);
					config.ResOptions = (matches != null) ? matches[0] : "";
					matches = Regex("\t\t\t\tPreprocessorDefinitions=\"(.+)\"(.*)", true);
					config.ResDefines = (matches != null) ? matches[0].Split(';') : new string[] { };
					matches = Regex("\t\t\t\tAdditionalIncludeDirectories=\"(.+)\"(.*)", true);
					config.ResPaths = (matches != null) ? matches[0].Split(';') : new string[] { };
				}

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCWebDeploymentTool\"/>");
		
				Match("\t\t</Configuration>");

				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
			}

			Match("\t</Configurations>");
			Match("\t<Files>");

			string indent = "\t";
			string folder = "";
			while (!Match("\t</Files>", true))
			{
				if (Match(indent + "\t<Filter", true))
				{
					indent += "\t";
					matches = Regex(indent + "\tName=\"(.+)\"");
					folder = Path.Combine(folder, matches[0]);
					Match(indent + "\tFilter=\"\">");
				}
				else if (Match(indent + "</Filter>", true))
				{
					indent = indent.Substring(0, indent.Length - 1);
					folder = Path.GetDirectoryName(folder);
				}
				else
				{
					Match(indent + "\t<File");
					matches = Regex(indent + "\t\tRelativePath=\"(.+)\">");
					package.File.Add(matches[0]);

					/* Make sure file appears in the correct folder */
					filename = matches[0];
					if (filename.StartsWith(".\\"))
						filename = filename.Substring(2);
					while (filename.StartsWith("..\\"))
						filename = filename.Substring(3);
					if (Path.GetDirectoryName(filename) != folder)
						throw new FormatException("File '" + matches[0] + "' is in folder '" + folder + "'");

					/* Check for file configuration section */
					foreach (Configuration config in package.Config)
					{
						if (Match(indent + "\t\t<FileConfiguration", true))
						{
							config.PchSource = filename;
							Match(indent + "\t\t\tName=\"" + config.Name + "|Win32\">");
							Match(indent + "\t\t\t<Tool");
							Match(indent + "\t\t\t\tName=\"VCCLCompilerTool\"");
							Match(indent + "\t\t\t\tUsePrecompiledHeader=\"1\"/>");
							Match(indent + "\t\t</FileConfiguration>");
						}
					}

					Match(indent + "\t</File>");

				}
			}

			if (indent != "\t")
				throw new FormatException("Unclosed entity in <Files> block");

			Match("\t<Globals>");
			Match("\t</Globals>");
			Match("</VisualStudioProject>");
		}
		#endregion

		#region C# Parsing
		private void ParseCs(Project project, Package package, string filename)
		{
			string kind = null;

			Begin(filename);

			Match("<VisualStudioProject>");
			Match("\t<CSHARP");
			
			string[] matches = Regex("\t\tProjectType = \"([A-Za-z]+)\"");
			if (matches[0] == "Web")
				kind = "aspnet";

			Match("\t\tProductVersion = \"7.0.9254\"");
			Match("\t\tSchemaVersion = \"1.0\"");
			
			matches = Regex("\t\tProjectGuid = \"{([0-9A-F-]+)}\"");
			if (matches[0] != package.ID)
				throw new FormatException("Solution and project GUIDs don't match");

			Match("\t>");

			Match("\t\t<Build>");
			Match("\t\t\t<Settings");
			Match("\t\t\t\tApplicationIcon = \"\"");
			Match("\t\t\t\tAssemblyKeyContainerName = \"\"");
			
			matches = Regex("\t\t\t\tAssemblyName = \"(.+)\"");
			string target = matches[0];

			Match("\t\t\t\tAssemblyOriginatorKeyFile = \"\"");
			Match("\t\t\t\tDefaultClientScript = \"JScript\"");
			Match("\t\t\t\tDefaultHTMLPageLayout = \"Grid\"");
			Match("\t\t\t\tDefaultTargetSchema = \"IE50\"");
			Match("\t\t\t\tDelaySign = \"false\"");
			Match("\t\t\t\tNoStandardLibraries = \"false\"");

			matches = Regex("\t\t\t\tOutputType = \"([A-Za-z]+)\"");
			if (kind == null)
			{
				switch (matches[0])
				{
				case "Exe":     kind = "exe";    break;
				case "WinExe":  kind = "winexe"; break;
				case "Library": kind = "dll";    break;
				default:
					throw new FormatException("Unexpected value: OutputType = \"" + matches[0] + "\"");
				}
			}
			else if (matches[0] != "Library")
			{
				throw new FormatException("Output type must be 'library' for ASP.NET applications");
			}

			Match("\t\t\t\tRootNamespace = \"" + target + "\"");
			Match("\t\t\t\tStartupObject = \"\"");
			Match("\t\t\t>");

			foreach (Configuration config in package.Config)
			{
				ArrayList buildFlags = new ArrayList();

				config.Target = target;

				Match("\t\t\t\t<Config");
				Match("\t\t\t\t\tName = \"" + config.Name + "\"");
				
				matches = Regex("\t\t\t\t\tAllowUnsafeBlocks = \"(true|false)\"");
				if (matches[0] == "true")
					buildFlags.Add("unsafe");

				Match("\t\t\t\t\tBaseAddress = \"285212672\"");
				Match("\t\t\t\t\tCheckForOverflowUnderflow = \"false\"");
				Match("\t\t\t\t\tConfigurationOverrideFile = \"\"");
				
				matches = Regex("\t\t\t\t\tDefineConstants = \"(.*)\"");
				config.Defines = matches[0].Split(';');
				if (config.Defines[0] == String.Empty)
					config.Defines = new string[]{};

				Match("\t\t\t\t\tDocumentationFile = \"\"");
				
				matches = Regex("\t\t\t\t\tDebugSymbols = \"(true|false)\"");
				if (matches[0] == "false")
					buildFlags.Add("no-symbols");

				Match("\t\t\t\t\tFileAlignment = \"4096\"");
				Match("\t\t\t\t\tIncrementalBuild = \"false\"");
				
				matches = Regex("\t\t\t\t\tOptimize = \"([a-z]+)\"");
				if (matches[0] == "true")
					buildFlags.Add("optimize");
				
				matches = Regex("\t\t\t\t\tOutputPath = \"(.+)\"");
				config.BinDir = matches[0];
				config.OutDir = matches[0];

				Match("\t\t\t\t\tRegisterForComInterop = \"false\"");
				Match("\t\t\t\t\tRemoveIntegerChecks = \"false\"");
				
				matches = Regex("\t\t\t\t\tTreatWarningsAsErrors = \"(true|false)\"");
				if (matches[0] == "true")
					buildFlags.Add("fatal-warnings");

				Match("\t\t\t\t\tWarningLevel = \"4\"");
				Match("\t\t\t\t/>");
			
				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
			}

			Match("\t\t\t</Settings>");

			ArrayList links = new ArrayList();
			ArrayList lddep = new ArrayList();
			Match("\t\t\t<References>");
			while (!Match("\t\t\t</References>", true))
			{
				Match("\t\t\t\t<Reference");

				matches = Regex("\t\t\t\t\tName = \"(.+)\"");
				string name = matches[0];
				links.Add(name);

				matches = Regex("\t\t\t\t\tProject = \"(.+)\"", true);
				if (matches != null)
				{
					string dep = matches[0];
					matches = Regex("\t\t\t\t\tPackage = \"(.+)\"");
					dep += matches[0];
					lddep.Add(dep);
				}
				else
				{
					Match("\t\t\t\t\tAssemblyName = \"" + name + "\"");
				}
				Match("\t\t\t\t/>");
			}
			foreach (Configuration config in package.Config)
			{
				config.Kind = kind;
				config.Links = (string[])links.ToArray(typeof(string));
				config.LinkDeps = (string[])lddep.ToArray(typeof(string));
			}

			Match("\t\t</Build>");
			Match("\t\t<Files>");
			Match("\t\t\t<Include>");

			while (!Match("\t\t\t</Include>", true))
			{
				Match("\t\t\t\t<File");

				matches = Regex("\t\t\t\t\tRelPath = \"(.+)\"");
				string name = matches[0];
				
				matches = Regex("\t\t\t\t\tDependentUpon = \"(.+)\"", true);
				string depends = (matches != null) ? matches[0] : null;

				matches = Regex("\t\t\t\t\tSubType = \"(.+)\"", true);
				string subtype = (matches != null) ? matches[0].ToLower() : null;

				matches = Regex("\t\t\t\t\tBuildAction = \"(.+)\"");
				package.File.Add(name, subtype, matches[0], depends);

				Match("\t\t\t\t/>");
			}

			Match("\t\t</Files>");
			Match("\t</CSHARP>");
			Match("</VisualStudioProject>");

			if (kind != "aspnet")
				ParseUserFile(project, package, filename);
		}

		private void ParseUserFile(Project project, Package package, string filename)
		{
			Begin(filename + ".user");

			Match("<VisualStudioProject>");
			Match("\t<CSHARP>");
			Match("\t\t<Build>");
			
			string[] matches = Regex("\t\t\t<Settings ReferencePath = \"(.+)\" >");
			matches = matches[0].Split(';');
			string[] libpaths = new string[matches.Length - 1];

			/* VS.NET stores reference directories as absolute paths, so I need
			 * to do some ugly trickery here */
			string[] bindir = matches[matches.Length - 1].Split('\\');
			for (int i = 0; i < matches.Length - 1; ++i)
			{
				string[] thisdir = matches[i].Split('\\');
				int j = 0;
				while (j < bindir.Length && j < thisdir.Length && bindir[j] == thisdir[j])
					++j;

				string path = "";
				for (int k = j + 1; k < bindir.Length; ++k)
					path += "..\\";

				for (int k = j; k < thisdir.Length; ++k)
					path += thisdir[k] + '\\';

				libpaths[i] = path.Substring(0, path.Length - 1);
				libpaths[i] = libpaths[i].Replace("/", "\\");
			}
			
			foreach (Configuration config in package.Config)
			{
				config.LibPaths = libpaths;

				Match("\t\t\t\t<Config");
				Match("\t\t\t\t\tName = \"" + config.Name + "\"");
				Match("\t\t\t\t\tEnableASPDebugging = \"false\"");
				Match("\t\t\t\t\tEnableASPXDebugging = \"false\"");
				Match("\t\t\t\t\tEnableUnmanagedDebugging = \"false\"");
				Match("\t\t\t\t\tEnableSQLServerDebugging = \"false\"");
				Match("\t\t\t\t\tRemoteDebugEnabled = \"false\"");
				Match("\t\t\t\t\tRemoteDebugMachine = \"\"");
				Match("\t\t\t\t\tStartAction = \"Project\"");
				Match("\t\t\t\t\tStartArguments = \"\"");
				Match("\t\t\t\t\tStartPage = \"\"");
				Match("\t\t\t\t\tStartProgram = \"\"");
				Match("\t\t\t\t\tStartURL = \"\"");
				Match("\t\t\t\t\tStartWorkingDirectory = \"\"");
				Match("\t\t\t\t\tStartWithIE = \"false\"");
				Match("\t\t\t\t/>");
			}

			Match("\t\t\t</Settings>");
			Match("\t\t</Build>");
			Match("\t\t<OtherProjectSettings");
			Match("\t\t\tCopyProjectDestinationFolder = \"\"");
			Match("\t\t\tCopyProjectUncPath = \"\"");
			Match("\t\t\tCopyProjectOption = \"0\"");
			Match("\t\t\tProjectView = \"ProjectFiles\"");
			Match("\t\t\tProjectTrust = \"0\"");
			Match("\t\t/>");
			Match("\t</CSHARP>");
			Match("</VisualStudioProject>");
		}
		#endregion
	}
}