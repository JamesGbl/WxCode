using System;
using System.Collections;
using System.IO;
using Premake.Tests.Framework;

namespace Premake.Tests.SharpDev
{
	public class SharpDevParser : Parser
	{
		#region Parser Methods
		public override string TargetName
		{
			get { return "sharpdev"; }
		}
		#endregion

		#region Combine Parsing
		public override void Parse(Project project, string filename)
		{
			Begin(filename + ".cmbx");

			string[] matches = Regex("<Combine fileversion=\"(.+)\" name=\"(.+)\" description=\"(.*)\">");
			project.Name = matches[1];

			matches = Regex("  <StartMode startupentry=\"\" single=\"True\">");

			while (!Match("  </StartMode>", true))
			{
				matches = Regex("    <Execute entry=\"(.+)\" type=\"None\" />");
				Package package = new Package();
				package.Name = matches[0];
				project.Package.Add(package);
			}

			Match("  <Entries>");
			foreach (Package package in project.Package)
			{
				matches = Regex("    <Entry filename=\"(.+)\" />");
				/* Pull out directory, but keep the path separators intact (no translation) */
				package.Path = matches[0].Substring(0, Path.GetDirectoryName(matches[0]).Length);
				package.ScriptName = Path.GetFileName(matches[0]);
			}
			Match("  </Entries>");

			matches = Regex("  <Configurations active=\"(.+)\">");
			string active = matches[0];

			while (!Match("  </Configurations>", true))
			{
				matches = Regex("    <Configuration name=\"(.+)\">");
				Configuration config = new Configuration();
				config.Name = matches[0];
				project.Configuration.Add(matches[0]);

				foreach (Package package in project.Package)
				{
					package.Config.Add(config);
					Match("      <Entry name=\"" + package.Name + "\" configurationname=\"" + active + "\" build=\"False\" />");
				}

				Match("    </Configuration>");
			}

			if (active != (string)project.Configuration[0])
				throw new FormatException("Active configuration should be '" + project.Configuration[0] + "' but is '" + active + "'");

			Match("</Combine>");

			foreach (Package package in project.Package)
			{
				filename = Path.Combine(Path.Combine(project.Path, package.Path), package.ScriptName);
				ParsePackage(project, package, filename);

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
			
			string[] matches = Regex("<Project name=\"(.+)\" standardNamespace=\"(.+)\" description=\"(.*)\" newfilesearch=\"None\" enableviewstate=\"True\" version=\"1.1\" projecttype=\"(.+)\">");
			package.Name = matches[0];
			package.Language = matches[3].ToLower();

			if (package.Name != matches[1])
				throw new FormatException("Namespace should be '" + package.Name + "' but is '" + matches[1] + "'");

			Match("  <Contents>");

			Hashtable folders = new Hashtable();
			folders.Add(".", true);
			while (!Match("  </Contents>", true))
			{
				matches = Regex("    <File name=\"(.+)\" subtype=\"(.*)\" buildaction=\"(.+)\" dependson=\"(.*)\" data=\"(.*)\" />");
				if (matches[1] == "Directory")
				{
					if (matches[2] != "Compile")
						throw new FormatException("Buildaction of folders must be Compile");
					folders.Add(matches[0].Replace("/", "\\"), true);
				}
				else if (matches[1] == "Code")
				{
					if (matches[2] == "EmbedAsResource")
						matches[2] = "EmbeddedResource";
	
					string path = Path.GetDirectoryName(matches[0]);
					if (folders[path] == null)
						throw new FormatException("Missing directory entry in file block for '" + path + "'");
				
					package.File.Add(matches[0], matches[1], matches[2], matches[3]);
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
					matches = Regex("    <Reference type=\"(Assembly|Gac|Project)\" refto=\"(.+)\" localcopy=\"(True|False)\" />");
					links.Add(matches[1]);

					if (matches[0] == "Project")
						lddep.Add(matches[1]);
				}
			}
			foreach (Configuration config in package.Config)
			{
				config.Links = (string[])links.ToArray(typeof(string));
				config.Dependencies = (string[])lddep.ToArray(typeof(string));
			}

			Match("  <DeploymentInformation target=\"\" script=\"\" strategy=\"File\" />");

			Match("  <Configurations active=\"" + project.Configuration[0] + "\">");
			foreach (Configuration config in package.Config)
			{
				ArrayList buildFlags = new ArrayList();

				matches = Regex("    <Configuration runwithwarnings=\"(True|False)\" name=\"" + config.Name + "\">");
				if (matches[0] == "False")
					buildFlags.Add("fatal-warnings");

				matches = Regex("      <CodeGeneration runtime=\"(.+)\" compiler=\"(.+)\" compilerversion=\"(.*)\" warninglevel=\"4\" nowarn=\"(.*)\" includedebuginformation=\"(True|False)\" optimize=\"(True|False)\" unsafecodeallowed=\"(True|False)\" generateoverflowchecks=\"(True|False)\" mainclass=\"(.*)\" target=\"(.+)\" definesymbols=\"(.*)\" generatexmldocumentation=\"(.+)\" win32Icon=\"(.*)\" noconfig=\"False\" nostdlib=\"(True|False)\" />");
				package.Compiler = matches[1];

				switch (matches[9])
				{
				case "Exe":     config.Kind = "exe";    break;
				case "WinExe":  config.Kind = "winexe"; break;
				case "Library": config.Kind = "dll";    break;
				}

				if (matches[4] == "False")
					buildFlags.Add("no-symbols");

				if (matches[5] == "True")
					buildFlags.Add("optimize");

				if (matches[6] == "True")
					buildFlags.Add("unsafe");

				config.Defines = (matches[10] != "") ? matches[10].Split(';') : new string[]{};

				Match("      <Execution commandlineparameters=\"\" consolepause=\"True\" />");
				
				matches = Regex("      <Output directory=\"(.+)\" assembly=\"(.+)\" executeScript=\"(.*)\" executeBeforeBuild=\"(.*)\" executeAfterBuild=\"(.*)\" executeBeforeBuildArguments=\"(.*)\" executeAfterBuildArguments=\"(.*)\" />");
				config.BinDir = matches[0];
				config.OutDir = matches[0];
				config.Target = matches[1];

				Match("    </Configuration>");

				config.BuildFlags = (string[])buildFlags.ToArray(typeof(string));
			}

			Match("  </Configurations>");
			Match("</Project>");
		}
		#endregion
	}
}