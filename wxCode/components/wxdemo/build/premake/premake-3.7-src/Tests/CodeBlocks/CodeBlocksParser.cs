using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.CodeBlocks
{
	public class CodeBlocksParser : Parser
	{
		#region Parser Methods
		public override string TargetName
		{
			get { return "cb-gcc"; }
		}
		#endregion

		#region Workspace Parsing
		public override void Parse(Project project, string filename)
		{
			/* File header */
			Begin(filename + ".workspace");
			Match("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
			Match("<CodeBlocks_workspace_file>");
			Match("\t<Workspace title=\"" + project.Name + "\">");

			Hashtable packageDependencies = new Hashtable();
			while (!Match("\t</Workspace>", true))
			{
				string[] matches = Regex("\t\t<Project filename=\"(.+?)\"(.*)>");

				Package package = new Package();
				project.Package.Add(package);

				package.Name = Path.GetFileNameWithoutExtension(matches[0]);
				package.Path = Path.GetDirectoryName(matches[0]);
				package.ScriptName = Path.GetFileName(matches[0]);

				ArrayList deps = new ArrayList();
				while (!Match("\t\t</Project>", true))
				{
					matches = Regex("\t\t\t<Depends filename=\"(.+?)\" />");
					deps.Add(Path.GetFileNameWithoutExtension(matches[0]));
				}
				packageDependencies.Add(package, deps.ToArray(typeof(string)));
			}

			Match("</CodeBlocks_workspace_file>");

			foreach (Package package in project.Package)
			{
				filename = Path.Combine(Path.Combine(project.Path, package.Path), package.ScriptName);
				ParseCpp(project, package, filename);

				foreach (Configuration cfg in package.Config)
					cfg.Dependencies = (string[])packageDependencies[package];
			}
		}
		#endregion

		#region C++ Parsing
		private void ParseCpp(Project project, Package package, string filename)
		{
			Begin(filename);

			Match("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>");
			Match("<CodeBlocks_project_file>");
			Match("\t<FileVersion major=\"1\" minor=\"6\" />");
			Match("\t<Project>");
			Match("\t\t<Option title=\"" + package.Name + "\" />");
			Match("\t\t<Option pch_mode=\"2\" />", true);
			Match("\t\t<Option compiler=\"gcc\" />");
			Match("\t\t<Build>");

			package.Language = "c++";

			while (!Match("\t\t</Build>", true))
			{
				Configuration config = new Configuration();
				package.Config.Add(config);

				string[] matches = Regex("\t\t\t<Target title=\"(.+?)\">");
				config.Name = matches[0];

				while (!Match("\t\t\t</Target>", true))
				{
					ArrayList buildFlags = new ArrayList();
					ArrayList defines = new ArrayList();
					ArrayList incPaths = new ArrayList();
					ArrayList libPaths = new ArrayList();
					ArrayList libs = new ArrayList();
					config.BuildOptions = "";
					config.LinkOptions = "";

					matches = Regex("\t\t\t\t<Option output=\"(.+?)\" prefix_auto=\"0\" extension_auto=\"0\" />");
					config.Target = Path.GetFileName(matches[0]).Replace('\\', '/');
					config.OutDir = Path.GetDirectoryName(matches[0]).Replace('\\', '/'); ;
					config.OutFile = Path.GetFileName(matches[0]).Replace('\\', '/'); ;

					matches = Regex("\t\t\t\t<Option object_output=\"(.+?)\" />");
					config.ObjDir = matches[0];

					matches = Regex("\t\t\t\t<Option type=\"([0-9])\" />");
					switch (matches[0])
					{
					case "0":
						config.Kind = "winexe";
						break;
					case "1":
						config.Kind = "exe";
						break;
					case "2":
						config.Kind = "lib";
						break;
					case "3":
						config.Kind = "dll";
						break;
					default:
						throw new FormatException("Unknown configuration type " + matches[0]);
					}

					if (config.Kind == "lib")
					{
						config.BinDir = "";
						config.LibDir = config.OutDir;
					}
					else
					{
						config.BinDir = config.OutDir;
						config.LibDir = "";
					}

					Match("\t\t\t\t<Option compiler=\"gcc\" />");

					if (config.Kind == "dll")
					{
						Match("\t\t\t\t<Option createDefFile=\"0\" />");

						matches = Regex("\t\t\t\t<Option createStaticLib=\"([0-1])\" />");
						if (matches[0] == "0")
							buildFlags.Add("no-import-lib");
					}

					Match("\t\t\t\t<Compiler>");

					while (!Match("\t\t\t\t</Compiler>", true))
					{
						matches = Regex("\t\t\t\t\t<Add option=\"(.+?)\" />", true);
						if (matches != null)
						{
							switch (matches[0])
							{
							case "-fomit-frame-pointer":
								buildFlags.Add("no-frame-pointer");
								break;
							case "--no-exceptions":
								buildFlags.Add("no-exceptions");
								break;
							case "--no-rtti":
								buildFlags.Add("no-rtti");
								break;
							case "-O":
								buildFlags.Add("optimize");
								break;
							case "-O3":
								buildFlags.Add("optimize-speed");
								break;
							case "-Os":
								buildFlags.Add("optimize-size");
								break;
							case "-g":
								break;
							case "-Wall":
								buildFlags.Add("extra-warnings");
								break;
							case "-Werror":
								buildFlags.Add("fatal-warnings");
								break;
							default:
								if (matches[0].StartsWith("-D"))
								{
									defines.Add(matches[0].Substring(2));
								}
								else
								{
									config.BuildOptions += matches[0] + " ";
								}
								break;
							}
						}

						matches = Regex("\t\t\t\t\t<Add directory=\"(.+?)\" />", true);
						if (matches != null)
						{
							incPaths.Add(matches[0]);
						}
					}

					Match("\t\t\t\t<Linker>");
					while (!Match("\t\t\t\t</Linker>", true))
					{
						matches = Regex("\t\t\t\t\t<Add option=\"(.+?)\" />", true);
						if (matches != null)
						{
							switch (matches[0])
							{
							case "-s":
								buildFlags.Add("no-symbols");
								break;
							default:
								config.LinkOptions += matches[0] + " ";
								break;
							}
						}

						matches = Regex("\t\t\t\t\t<Add directory=\"(.+?)\" />", true);
						if (matches != null)
						{
							libPaths.Add(matches[0]);
						}

						matches = Regex("\t\t\t\t\t<Add library=\"(.+?)\" />", true);
						if (matches != null)
						{
							libs.Add(matches[0]);
						}
					}

					ArrayList resPaths = new ArrayList();
					if (Match("\t\t\t\t<ResourceCompiler>", true))
					{
						while (!Match("\t\t\t\t</ResourceCompiler>", true))
						{
							matches = Regex("\t\t\t\t\t<Add directory=\"(.+?)\" />");
							resPaths.Add(matches[0]);
						}
					}

					config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
					config.Defines = (string[])defines.ToArray(typeof(string));
					config.IncludePaths = (string[])incPaths.ToArray(typeof(string));
					config.LibPaths = (string[])libPaths.ToArray(typeof(string));
					config.Links = (string[])libs.ToArray(typeof(string));
					config.BuildOptions = config.BuildOptions.Trim(' ');
					config.LinkOptions = config.LinkOptions.Trim(' ');
					config.ResPaths = (string[])resPaths.ToArray(typeof(string));
				}
			}

			if (project.Configuration.Count == 0)
			{
				foreach (Configuration config in package.Config)
					project.Configuration.Add(config.Name);
			}

			while (!Match("\t\t<Extensions />", true))
			{
				string[] matches = Regex("\t\t<Unit filename=\"(.+?)\">");
				string name = matches[0];
				package.File.Add(name);

				if (Path.GetExtension(name) == ".rc")
				{
					string resname = name.Replace("../", "");
					resname = resname.Replace(".rc", ".res");

					Match("\t\t\t<Option compilerVar=\"WINDRES\" />");
				}
				else
				{
					if (Match("\t\t\t<Option compilerVar=\"CC\" />", true))
						package.Language = "c";
					else
						Match("\t\t\t<Option compilerVar=\"CPP\" />", true);

					if (!name.EndsWith(".c") && !name.EndsWith(".cpp"))
					{
//						Match("\t\t\t<Option compile=\"0\" />");
//						Match("\t\t\t<Option link=\"0\" />");
					}
				}

				foreach (Configuration cfg in package.Config)
					Match("\t\t\t<Option target=\"" + cfg.Name + "\" />");
				Match("\t\t</Unit>");

			}

			Match("\t</Project>");
			Match("</CodeBlocks_project_file>");
		}
		#endregion
	}
}
