using System;
using System.Collections;

namespace Premake.Tests.Framework
{
	public class Package
	{
		public string Name;
		public string ID;
		public string Kind;
		public string Path;
		public string Language;
		public string ScriptName;
		public string Compiler;
		public string DefFile;
		public ConfigCollection Config;
		public FileCollection File;
		public ArrayList CopyLocal;
		public string HasAppConfigRule;

		public Package()
		{
			Config = new ConfigCollection();
			File = new FileCollection();
		}

		public Package(Project project)
			: this()
		{
			Config.Add(project);
		}

		public void CompareTo(Package actual)
		{
			Test(this.Name,       actual.Name,       "Package name");
			Test(this.ID,         actual.ID,         "Package ID");
			Test(this.Kind,       actual.Kind,       "Package kind");
			Test(this.Path,       actual.Path,       "Package path");
			Test(this.Language,   actual.Language,   "Package language");
			Test(this.ScriptName, actual.ScriptName, "Package script name");
			Test(this.Compiler,   actual.Compiler,   "Package compiler");
			Test(this.DefFile,    actual.DefFile,    "Exports .def file");
			Test(this.HasAppConfigRule, actual.HasAppConfigRule, "App.config build rule");

			this.Config.CompareTo(actual.Config);
			this.File.CompareTo(actual.File);
		}

		private void Test(string expected, string actual, string description)
		{
			if (expected != null && expected.CompareTo(actual) != 0)
				throw new FormatException(description + " should be '" + expected + "' but is '" + actual + "'");
		}
	}
}
