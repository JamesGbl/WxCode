using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2005
{
	public class Vs2005Parser : Parser
	{
		private int _numAspNet = 0;

		#region Parser Methods
		public override string TargetName
		{
			get { return "vs2005"; }
		}
		#endregion

		#region Solution Parsing
		public override void Parse(Project project, string filename)
		{
			/* File header */
			Begin(filename + ".sln");
			Match("Microsoft Visual Studio Solution File, Format Version 9.00");
			Match("# Visual Studio 2005");

			/* Package entries - VS "projects" */
			string[] matches;
			Hashtable packageDependencies = new Hashtable();
			while (!Match("Global", true))
			{
				matches = Regex("Project\\(\"{([0-9A-F-]+)}\"\\) = \"(.+)\", \"(.+)\", \"{([0-9A-F-]+)}\"");
				Package package = new Package();
				project.Package.Add(package);

				package.Name = matches[1];
				package.ID = matches[3];
				package.Path = Path.GetDirectoryName(matches[2]);
				package.ScriptName = Path.GetFileName(matches[2]);

				switch (matches[0])
				{
				case "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942":
					package.Language = "c++";
					break;

				case "FAE04EC0-301F-11D3-BF4B-00C04F79EFBC":
					package.Language = "c#";
					break;

				case "E24C65DC-7377-472B-9ABA-BC803B73C61A":
					package.Kind = "aspnet";
					break;
				}

				if (package.Kind == "aspnet")
				{
					Match("\tProjectSection(WebsiteProperties) = preProject");

					ArrayList deps = new ArrayList();
					matches = Regex("\t\tProjectReferences = \"{([0-9A-F-]+)}|(.+).dll;\"", true);
					while (matches != null)
					{
						deps.Add(matches[0]);
						matches = Regex("\t\tProjectReferences = \"{([0-9A-F-]+)}|(.+).dll;\"", true);
					}
					packageDependencies[package.ID] = deps;

					matches = Regex("\t\t(.+).AspNetCompiler.VirtualPath = \"/" + package.Name + "\"", true);
					while (matches != null)
					{
						string cfg = matches[0];
						Match("\t\t" + cfg + ".AspNetCompiler.PhysicalPath = \"" + package.Path + "\\\"");
						Match("\t\t" + cfg + ".AspNetCompiler.TargetPath = \"PrecompiledWeb\\" + package.Name + "\\\"");
						Match("\t\t" + cfg + ".AspNetCompiler.Updateable = \"true\"");
						Match("\t\t" + cfg + ".AspNetCompiler.ForceOverwrite = \"true\"");
						Match("\t\t" + cfg + ".AspNetCompiler.FixedNames = \"false\"");
						matches = Regex("\t\t" + cfg + ".AspNetCompiler.Debug = \"(True|False)\"");

						matches = Regex("\t\t(.+).AspNetCompiler.VirtualPath = \"/" + package.Name + "\"", true);
					}

					int port;
					if (_numAspNet == 0)
						port = 1106;
					else if (_numAspNet == 1)
						port = 1231;
					else
						port = 1251 + 2 * (_numAspNet - 2);
					Match("\t\tVWDPort = \"" + port + "\"");
					_numAspNet++;

					matches = Regex("\t\tDefaultWebSiteLanguage = \"(.+)\"");
					if (matches == null)
						package.Language = "vbnet";
					else if (matches[0] == "Visual C#")
						package.Language = "c#";

					Match("\tEndProjectSection");
				}
				else
				{
					/* Collect package dependencies, I'll sort them out after
					 * I finish parsing the scripts below */
					Match("\tProjectSection(ProjectDependencies) = postProject");
					ArrayList deps = new ArrayList();
					while (!Match("\tEndProjectSection", true))
					{
						matches = Regex("\t\t{([0-9A-F-]+)} = {([0-9A-F-]+)}");
						if (matches[0] != matches[1])
							throw new FormatException("GUID mismatch in ProjectDependencies block, should be the same");
						deps.Add(matches[0]);
					}
					packageDependencies[package.ID] = deps;
				}

				Match("EndProject");
			}

			/* Read the list of configurations...just cache for now, test later */
			ArrayList possibleConfigs = new ArrayList();
			Match("\tGlobalSection(SolutionConfigurationPlatforms) = preSolution");
			while (!Match("\tEndGlobalSection", true))
			{
				matches = Regex("\t\t(.+)[ ]=[ ](.+)", true);

				string cfg = matches[0];
				if (possibleConfigs.Contains(cfg))
					throw new FormatException("Duplicate configuration '" + cfg + "'");
				possibleConfigs.Add(cfg);

				/* Build up my list of scripted package configurations */
				matches = cfg.Split('|');
				if (!project.Configuration.Contains(matches[0]))
					project.Configuration.Add(matches[0]);
			}

			foreach (Package package in project.Package)
				package.Config.Add(project);

			/* Read the list of package configurations...cache and test later */
			ArrayList packageConfigs = new ArrayList();
			Match("\tGlobalSection(ProjectConfigurationPlatforms) = postSolution");
			while (!Match("\tEndGlobalSection", true))
			{
				string cfg = GetLine();
				if (packageConfigs.Contains(cfg))
					throw new FormatException("Duplicate configuration '" + cfg + "'");
				packageConfigs.Add(cfg);
			}

			Match("\tGlobalSection(SolutionProperties) = preSolution");
			Match("\t\tHideSolutionNode = FALSE");
			Match("\tEndGlobalSection");
			Match("EndGlobal");

			foreach (Package package in project.Package)
			{
				if (package.Kind != "aspnet")
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
				else
				{
					foreach (Configuration config in package.Config)
						config.Kind = "aspnet";
				}
			}

			/* Now sort out the inter-package dependencies */
			foreach (Package package in project.Package)
			{
				ArrayList deps = (ArrayList)packageDependencies[package.ID];
				string[] deplist = new string[deps.Count];

				for (int i = 0; i < deps.Count; ++i)
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

			/* Now test configurations */
			bool hasDotNet = false;
			bool hasCpp = false;
			foreach (Package package in project.Package)
			{
				if (package.Language == "c" || package.Language == "c++")
					hasCpp = true;
				else
					hasDotNet = true;
			}

			ArrayList platforms = new ArrayList();
			if (hasDotNet) platforms.Add("Any CPU");
			if (hasDotNet && hasCpp) platforms.Add("Mixed Platforms");
			if (hasCpp) platforms.Add("Win32");

			int c = 0;
			foreach (Package package in project.Package)
			{
				string arch;
				if (package.Language == "c" || package.Language == "c++")
					arch = "Win32";
				else
					arch = "Any CPU";

				foreach (Configuration config in package.Config)
				{
					foreach (string platform in platforms)
					{
						string configPlatform = config.Name + "|" + platform;
						if (!possibleConfigs.Contains(configPlatform))
							throw new FormatException(configPlatform + " is not listed in possible configuration");

						string root = "\t\t{" + package.ID + "}." + configPlatform + ".";

						string match = root + "ActiveCfg = " + config.Name + "|" + arch;
						if (((string)packageConfigs[c]) != match)
							throw new FormatException("Expected:\n  " + match + "\nBut I got:\n  " + packageConfigs[c]);
						c++;

						if (platform == "MixedPlatforms" || platform == arch)
						{
							match = root + "Build.0 = " + config.Name + "|" + arch;
							if (((string)packageConfigs[c]) != match)
								throw new FormatException("Expected:\n  " + match + "\nBut I got:\n  " + packageConfigs[c]);
							c++;
						}
					}
				}
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
			Match("\tVersion=\"8.00\"");
			Match("\tName=\"" + package.Name + "\"");

			string[] matches = Regex("\tProjectGUID=\"{([A-F0-9-]+)}\"");
			if (matches[0] != package.ID)
				throw new FormatException("Solution (" + package.ID + ") and project (" + matches[0] + ") GUIDs don't match");

			Match("\tRootNamespace=\"" + package.Name + "\"");
			Match("\tKeyword=\"Win32Proj\"");
			Match("\t>");

			Match("\t<Platforms>");
			Match("\t\t<Platform");
			Match("\t\t\tName=\"Win32\"");
			Match("\t\t/>");
			Match("\t</Platforms>");

			Match("\t<ToolFiles>");
			Match("\t</ToolFiles>");

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
				case 1: config.Kind = "exe"; break;
				case 2: config.Kind = "dll"; break;
				case 4: config.Kind = "lib"; break;
				default:
					throw new FormatException("Unrecognized value: ConfigurationType=\"" + matches[0] + "\"");
				}

				if (config.Kind == "lib")
					config.LibDir = config.OutDir;
				else
					config.BinDir = config.OutDir;

				matches = Regex("\t\t\tCharacterSet=\"([1-2])\"");
				if (matches[0] == "1")
					buildFlags.Add("unicode");
				Match("\t\t\t>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPreBuildEventTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCCustomBuildTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCXMLDataGeneratorTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCWebServiceProxyGeneratorTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCMIDLTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCCLCompilerTool\"");

				matches = Regex("\t\t\t\tAdditionalOptions=\"(.+)\"", true);
				config.BuildOptions = (matches != null) ? matches[0] : String.Empty;

				matches = Regex("\t\t\t\tOptimization=\"([0-9])\"");
				int optimization = int.Parse(matches[0]);
				switch (optimization)
				{
				case 0: break;
				case 1: buildFlags.Add("optimize-size"); break;
				case 2: buildFlags.Add("optimize-speed"); break;
				case 3: buildFlags.Add("optimize"); break;
				default:
					throw new FormatException("Unrecognized value: Optimization=\"" + matches[0] + "\"");
				}

				if (Match("\t\t\t\tOmitFramePointers=\"true\"", true))
					buildFlags.Add("no-frame-pointer");

				matches = Regex("\t\t\t\tAdditionalIncludeDirectories=\"(.+)\"", true);
				config.IncludePaths = (matches != null) ? matches[0].Split(';') : new string[] { };

				matches = Regex("\t\t\t\tPreprocessorDefinitions=\"(.+)\"", true);
				config.Defines = (matches != null) ? matches[0].Split(';') : new string[] { };

				if (optimization == 0)
					Match("\t\t\t\tMinimalRebuild=\"true\"");

				if (Match("\t\t\t\tExceptionHandling=\"false\"", true))
					buildFlags.Add("no-exceptions");

				if (Match("\t\t\t\tExceptionHandling=\"2\"", true))
					buildFlags.Add("seh-exceptions");

				if (optimization > 0)
					Match("\t\t\t\tStringPooling=\"true\"", true);

				if (optimization == 0)
					Match("\t\t\t\tBasicRuntimeChecks=\"3\"", true);

				if (optimization == 0)
					matches = Regex("\t\t\t\tRuntimeLibrary=\"(1|3)\"");
				else
					matches = Regex("\t\t\t\tRuntimeLibrary=\"(0|2)\"");

				if (matches[0] == "0" || matches[0] == "1")
					config.LinkFlags = new string[] { "static-runtime" };

				Match("\t\t\t\tEnableFunctionLevelLinking=\"true\"");

				if (Match("\t\t\t\tRuntimeTypeInfo=\"false\"", true))
					buildFlags.Add("no-rtti");

				matches = Regex("\t\t\t\tTreatWChar_tAsBuiltInType=\"(true|false)\"", true);
				if (matches != null)
					buildFlags.Add(matches[0] == "true" ? "native-wchar" : "no-native-wchar");

				config.Pch = "off";
				matches = Regex("\t\t\t\tUsePrecompiledHeader=\"([0-9])\"");
				if (matches[0] == "2")
				{
					config.Pch = "on";
					matches = Regex("\t\t\t\tPrecompiledHeaderThrough=\"(.+?)\"");
					config.PchHeader = matches[0];
				}
				else if (matches[0] != "0")
				{
					throw new FormatException("Expected UsePrecompiledHeader to be 2, got " + matches[0]);
				}

				matches = Regex("\t\t\t\tWarningLevel=\"([3-4])\"");
				if (matches[0] == "4")
					buildFlags.Add("extra-warnings");

				if (Match("\t\t\t\tWarnAsError=\"true\"", true))
					buildFlags.Add("fatal-warnings");

				matches = Regex("\t\t\t\tDetect64BitPortabilityProblems=\"(true|false)\"");
				if (matches[0] == "false")
					buildFlags.Add("no-64bit-checks");

				matches = Regex("\t\t\t\tDebugInformationFormat=\"([0-9])\"");
				if (matches[0] == "0")
					buildFlags.Add("no-symbols");
				else if (matches[0] == "3")
					buildFlags.Add("no-edit-and-continue");

				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCManagedResourceCompilerTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCResourceCompilerTool\"");
				matches = Regex("\t\t\t\tAdditionalOptions=\"(.+)\"", true);
				config.ResOptions = (matches != null) ? matches[0] : "";
				matches = Regex("\t\t\t\tPreprocessorDefinitions=\"(.+)\"", true);
				config.ResDefines = (matches != null) ? matches[0].Split(';') : new string[] { };
				matches = Regex("\t\t\t\tAdditionalIncludeDirectories=\"(.+)\"", true);
				config.ResPaths = (matches != null) ? matches[0].Split(';') : new string[] { };
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPreLinkEventTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				if (config.Kind == "lib")
				{
					Match("\t\t\t\tName=\"VCLibrarianTool\"");
					matches = Regex("\t\t\t\tOutputFile=\"\\$\\(OutDir\\)/(.+)\"");
					config.Target = matches[0];
				}
				else
				{
					Match("\t\t\t\tName=\"VCLinkerTool\"");

					matches = Regex("\t\t\t\tAdditionalDependencies=\"(.+)\"", true);
					if (matches != null)
						config.Links = matches[0].Split(' ');

					if (Match("\t\t\t\tIgnoreImportLibrary=\"true\"", true))
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

					if (Match("\t\t\t\tGenerateManifest=\"false\"", true))
						buildFlags.Add("no-manifest");

					string expected = buildFlags.Contains("no-symbols") ? "false" : "true";
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

					Match("\t\t\t\tTargetMachine=\"1\"");
				}
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCALinkTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCManifestTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCXDCMakeTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCBscMakeTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCFxCopTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCAppVerifierTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCWebDeploymentTool\"");
				Match("\t\t\t/>");

				Match("\t\t\t<Tool");
				Match("\t\t\t\tName=\"VCPostBuildEventTool\"");
				Match("\t\t\t/>");

				Match("\t\t</Configuration>");

				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
			}

			Match("\t</Configurations>");

			Match("\t<References>");
			Match("\t</References>");

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
					Match(indent + "\tFilter=\"\"");
					Match(indent + "\t>");
				}
				else if (Match(indent + "</Filter>", true))
				{
					indent = indent.Substring(0, indent.Length - 1);
					folder = Path.GetDirectoryName(folder);
				}
				else
				{
					Match(indent + "\t<File");
					matches = Regex(indent + "\t\tRelativePath=\"(.+)\"([>]?)");
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
					if (matches[1] == "")
					{
						Match(indent + "\t\t>");
						foreach (Configuration config in package.Config)
						{
							config.PchSource = filename;
							Match(indent + "\t\t<FileConfiguration");
							Match(indent + "\t\t\tName=\"" + config.Name + "|Win32\"");
							Match(indent + "\t\t\t>");
							Match(indent + "\t\t\t<Tool");
							Match(indent + "\t\t\t\tName=\"VCCLCompilerTool\"");
							Match(indent + "\t\t\t\tUsePrecompiledHeader=\"1\"");
							Match(indent + "\t\t\t/>");
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
			string kind = package.Kind;
			Begin(filename);

			Match("<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
			Match("  <PropertyGroup>");

			string[] matches;
			matches = Regex("    <Configuration Condition=\" '\\$\\(Configuration\\)' == '' \\\">(.+)</Configuration>");
			string defaultConfigName = matches[0];

			Match("    <Platform Condition=\" '$(Platform)' == '' \">AnyCPU</Platform>");
			Match("    <ProductVersion>8.0.50727</ProductVersion>");
			Match("    <SchemaVersion>2.0</SchemaVersion>");
			Match("    <ProjectGuid>{" + package.ID + "}</ProjectGuid>");

			matches = Regex("    <OutputType>(.+)</OutputType>");
			if (kind == null)
			{
				switch (matches[0])
				{
				case "Exe": kind = "exe"; break;
				case "WinExe": kind = "winexe"; break;
				case "Library": kind = "dll"; break;
				default:
					throw new FormatException("Unexpected value: OutputType = \"" + matches[0] + "\"");
				}
			}
			else if (matches[0] != "Library")
			{
				throw new FormatException("Output type must be 'library' for ASP.NET applications");
			}

			Match("    <AppDesignerFolder>Properties</AppDesignerFolder>");

			matches = Regex("<RootNamespace>(.+)</RootNamespace>");
			string target = matches[0];
			Match("    <AssemblyName>" + target + "</AssemblyName>");
			Match("  </PropertyGroup>");

			foreach (Configuration config in package.Config)
			{
				ArrayList buildFlags = new ArrayList();
				config.Kind = kind;
				config.Target = target;

				Match("  <PropertyGroup Condition=\" '$(Configuration)|$(Platform)' == '" + config.Name + "|AnyCPU' \">");

				if (!Match("    <DebugSymbols>true</DebugSymbols>", true))
				{
					buildFlags.Add("no-symbols");
					Match("    <DebugType>pdbonly</DebugType>");
				}
				else
				{
					Match("    <DebugType>full</DebugType>");
				}

				matches = Regex("    <Optimize>(true|false)</Optimize>");
				if (matches[0] == "true")
					buildFlags.Add("optimize");

				matches = Regex("    <OutputPath>(.*)\\\\</OutputPath>");
				config.BinDir = matches[0];
				config.OutDir = matches[0];

				matches = Regex("    <DefineConstants>(.*)</DefineConstants>");
				config.Defines = matches[0].Split(';');
				if (config.Defines[0] == String.Empty)
					config.Defines = new string[] { };

				Match("    <ErrorReport>prompt</ErrorReport>");
				Match("    <WarningLevel>4</WarningLevel>");

				if (Match("    <AllowUnsafeBlocks>true</AllowUnsafeBlocks>", true))
					buildFlags.Add("unsafe");

				if (Match("    <TreatWarningsAsErrors>true</TreatWarningsAsErrors>", true))
					buildFlags.Add("fatal-warnings");

				Match("  </PropertyGroup>");

				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
			}

			ArrayList links = new ArrayList();
			Match("  <ItemGroup>");
			while (!Match("  </ItemGroup>", true))
			{
				matches = Regex("    <Reference Include=\"(.+)\" />", true);
				if (matches != null)
				{
					string name = matches[0];
					links.Add(name);
				}

				matches = Regex("    <ProjectReference Include=\"(.+)\">", true);
				if (matches != null)
				{
					matches = Regex("      <Project>{([0-9A-F-]+)}</Project>");
					matches = Regex("      <Name>(.+)</Name>");
					Match("    </ProjectReference>");

					string name = matches[0];
					links.Add(name);
				}
			}

			foreach (Configuration config in package.Config)
			{
				config.Links = (string[])links.ToArray(typeof(string));
			}

			Match("  <ItemGroup>");
			while (!Match("  </ItemGroup>", true))
			{
				matches = Regex("    <(.+) Include=\"(.+)\"(>| />)");
				string action = matches[0];
				string name = matches[1];
				string endtag = matches[2];

				string subtype = "";
				string depends = "";

				if (action == "Compile" && endtag == ">")
				{
					subtype = "code";
					while (!Match("    </Compile>", true))
					{
						matches = Regex("      <SubType>(.+)</SubType>", true);
						if (matches != null)
							subtype = matches[0];

						if (Match("      <AutoGen>True</AutoGen>", true))
							subtype = "autogen";

						matches = Regex("      <DependentUpon>(.+)</DependentUpon>", true);
						if (matches != null)
							depends = matches[0];
					}
				}
				else
				{
					subtype = "code";
				}

				if (action == "EmbeddedResource" && endtag == ">")
				{
					while (!Match("    </EmbeddedResource>", true))
					{
						matches = Regex("      <SubType>(.+)</SubType>", true);
						if (matches != null)
							subtype = matches[0];

						matches = Regex("      <DependentUpon>(.+)</DependentUpon>", true);
						if (matches != null)
							depends = matches[0];

						if (Match("      <Generator>ResXFileCodeGenerator</Generator>", true))
						{
							subtype = "autogen";
							Match("      <LastGenOutput>" + Path.GetFileNameWithoutExtension(name) + ".Designer.cs</LastGenOutput>");
						}
					}
				}

				if (action == "Content")
				{
					Match("      <CopyToOutputDirectory>PreserveNewest</CopyToOutputDirectory>");
					Match("    </Content>");
				}

				package.File.Add(name, subtype, action, depends);
			}

			Match("  <Import Project=\"$(MSBuildBinPath)\\Microsoft.CSharp.targets\" />");
			Match("  <!-- To modify your build process, add your task inside one of the targets below and uncomment it. ");
			Match("       Other similar extension points exist, see Microsoft.Common.targets.");
			Match("  <Target Name=\"BeforeBuild\">");
			Match("  </Target>");
			Match("  <Target Name=\"AfterBuild\">");
			Match("  </Target>");
			Match("  -->");
			Match("</Project>");

			if (kind != "aspnet")
				ParseUserFile(project, package, filename);
		}

		private void ParseUserFile(Project project, Package package, string filename)
		{
			Begin(filename + ".user");

			Match("<Project xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">");
			Match("  <PropertyGroup>");

			string[] matches = Regex("    <ReferencePath>(.+)</ReferencePath>");
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
			}

			Match("  </PropertyGroup>");
			Match("</Project>");
		}

		#endregion
	}
}
