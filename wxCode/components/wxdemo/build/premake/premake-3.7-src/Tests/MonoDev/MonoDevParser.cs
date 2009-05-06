using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.MonoDev
{
	public class MonoDevParser : Parser
	{
		#region Parser Methods
		public override string TargetName
		{
			get { return "monodev"; }
		}
		#endregion

		#region Solution Parsing
		public override void Parse(Project project, string filename)
		{
			Begin(filename + ".mds");

			string[] matches = Regex("<Combine name=\"(.+)\" fileversion=\"2.0\">");
			project.Name = matches[0];

			matches = Regex("  <Configurations active=\"(.+)\">");
			string active = matches[0];

			// Parse configurations
			while (!Match("  </Configurations>", true))
			{
				matches = Regex("    <Configuration name=\"(.+)\" ctype=\"CombineConfiguration\">");
				Configuration config = new Configuration();
				config.Name = matches[0];
				project.Configuration.Add(matches[0]);

				int i = 0;
				while (!Match("    </Configuration>", true))
				{
					matches = Regex("      <Entry build=\"True\" name=\"(.+?)\" configuration=\"" + active + "\" />");
					if (i == project.Package.Count)
					{
						Package package = new Package();
						package.Name = matches[0];
						project.Package.Add(package);
					}
					else
					{
						if (project.Package[i].Name != matches[0])
							throw new FormatException("Package name should be " + project.Package[i] + ", is " + matches[0]);
					}
				}
			}

			foreach (Package package in project.Package)
			{
				foreach (string name in project.Configuration)
				{
					Configuration config = new Configuration();
					config.Name = name;
					package.Config.Add(config);
				}
			}


			// Parse startup entries
			Match("  <StartMode startupentry=\"" + project.Package[0].Name + "\" single=\"True\">");
			foreach (Package package in project.Package)
			{
				Match("    <Execute type=\"None\" entry=\"" + package.Name + "\" />");
			}
			Match("  </StartMode>");

			// Parse project entries
			Match("  <Entries>");
			foreach (Package package in project.Package)
			{
				matches = Regex("    <Entry filename=\"(.+)\" />");
				/* Pull out directory, but keep the path separators intact (no translation) */
				package.Path = matches[0].Substring(0, Path.GetDirectoryName(matches[0]).Length);
				package.ScriptName = Path.GetFileName(matches[0]);
			}
			Match("  </Entries>");

			Match("</Combine>");

			foreach (Package package in project.Package)
			{
				string pkgfilename = Path.Combine(Path.Combine(project.Path, package.Path), package.ScriptName);
				ParsePackage(project, package, pkgfilename);

				/* SHARPDEV_DEPENDENCY_BUG: Dependencies are set correctly here! */
#if SHARPDEV_DEPENDENCY_BUG
				Console.WriteLine(package.Name + ": ");
				foreach (Configuration config in package.Config)
				{
					Console.WriteLine("  " + config.Name + ": " + config.Dependencies.Length);
				}
#endif
			}
		}
		#endregion

		#region Generic Package Parsing
		private void ParsePackage(Project project, Package package, string filename)
		{
			Begin(filename);

			string[] matches;
			matches = Regex("<Project name=\"(.+?)\" fileversion=\"2.0\" language=\"(.+?)\" ctype=\"DotNetProject\">");
			package.Name = matches[0];
			package.Language = matches[1].ToLower();

			Match("  <Configurations active=\"" + project.Configuration[0] + "\">");
			foreach (Configuration config in package.Config)
			{
				ArrayList buildFlags = new ArrayList();

				Match("    <Configuration name=\"" + config.Name + "\" ctype=\"DotNetProjectConfiguration\">");

				matches = Regex("      <Output directory=\"(.+)\" assembly=\"(.+)\" />");
				config.BinDir = matches[0];
				config.OutDir = matches[0];
				config.Target = matches[1];

				matches = Regex("      <Build debugmode=\"(True|False)\" target=\"(.+?)\" />");
				if (matches[0] == "False")
					buildFlags.Add("no-symbols");
				switch (matches[1])
				{
				case "Exe": config.Kind = "exe"; break;
				case "WinExe": config.Kind = "winexe"; break;
				case "Library": config.Kind = "dll"; break;
				}

				matches = Regex("      <Execution runwithwarnings=\"(True|False)\" consolepause=\"(True|False)\" runtime=\"MsNet\" />");
				if (matches[0] == "False")
					buildFlags.Add("fatal-warnings");

				matches = Regex("      <CodeGeneration compiler=\"Csc\" warninglevel=\"4\" optimize=\"(True|False)\" unsafecodeallowed=\"(True|False)\" generateoverflowchecks=\"(True|False)\" definesymbols=\"(.*?)\" generatexmldocumentation=\"(True|False)\" ctype=\"CSharpCompilerParameters\" />");
				package.Compiler = "Csc";

				if (matches[0] == "True")
					buildFlags.Add("optimize");
				if (matches[1] == "True")
					buildFlags.Add("unsafe");

				Match("    </Configuration>");
				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
				config.Defines = (matches[3] != "") ? matches[3].Split(';') : new string[] { };
			}
			Match("  </Configurations>");

			Match("  <DeploymentInformation strategy=\"File\">");
			Match("    <excludeFiles />");
			Match("  </DeploymentInformation>");

			Match("  <Contents>");

			Hashtable folders = new Hashtable();
			folders.Add(".", true);
			while (!Match("  </Contents>", true))
			{
				matches = Regex("    <File name=\"(.+?)\" subtype=\"(.*)\" buildaction=\"(.+?)\" />");
				if (matches[1] == "Directory")
				{
					if (matches[2] != "Compile")
						throw new FormatException("Buildaction of folders must be Compile");
					folders.Add(matches[0], true);
				}
				else if (matches[1] == "Code")
				{
					if (matches[2] == "EmbedAsResource")
						matches[2] = "EmbeddedResource";
	
					/* Get path without translating directory separators to \ */
					string path = matches[0].Substring(0, Path.GetDirectoryName(matches[0]).Length);
					if (folders[path] == null)
						throw new FormatException("Missing directory entry in file block for '" + path + "'");
				
					package.File.Add(matches[0], matches[1], matches[2], null);
				}
				else
				{
					throw new FormatException("Unrecognized file subtype '" + matches[1] + "'");
				}
			}

			ArrayList links = new ArrayList();
			ArrayList lddep = new ArrayList();
			if (!Match("  <References />", true))
			{
				Match("  <References>");
				while (!Match("  </References>", true))
				{
					matches = Regex("    <Reference type=\"(Assembly|Gac|Project)\" localcopy=\"(True|False)\" refto=\"(.+)\" />");
					links.Add(matches[2]);

					if (matches[0] == "Project")
						lddep.Add(matches[2]);
				}
			}
			foreach (Configuration config in package.Config)
			{
				config.Links = (string[])links.ToArray(typeof(string));
				config.Dependencies = (string[])lddep.ToArray(typeof(string));
			}

			Match("</Project>");
		}
		#endregion
	}
}