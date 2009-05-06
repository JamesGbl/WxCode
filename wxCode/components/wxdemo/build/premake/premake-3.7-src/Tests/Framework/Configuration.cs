using System;
using System.Collections;
using System.Text;

namespace Premake.Tests.Framework
{
	public class Configuration
	{
		public string Name;
		public string BinDir;
		public string LibDir;
		public string ObjDir;
		public string OutDir;
		public string Target;
		public string OutFile;
		public string Kind;
		public string Pch;
		public string[] IncludePaths;
		public string[] Defines;
		public string[] LibPaths;
		public string[] Links;
		public string[] BuildFlags;
		public string   BuildOptions;
		public string[] LinkFlags;
		public string   LinkOptions;
		public string[] Dependencies;
		public string[] LinkDeps;
		public string   ImportLib;
		public string PchHeader;
		public string PchSource;
		public string[] ResDefines;
		public string[] ResPaths;
		public string ResOptions;

		public Configuration()
		{
		}

		public void CompareTo(Configuration actual)
		{
			Test(this.Name,         actual.Name,         "Configuration name");
			Test(this.BinDir,       actual.BinDir,       "BinDir");
			Test(this.LibDir,       actual.LibDir,       "LibDir");
			Test(this.ObjDir,       actual.ObjDir,       "ObjDir");
			Test(this.OutDir,       actual.OutDir,       "Output directory");
			Test(this.Target,       actual.Target,       "Target");
			Test(this.OutFile,      actual.OutFile,      "Output file");
			Test(this.Kind,         actual.Kind,         "Kind");
			Test(this.BuildOptions, actual.BuildOptions, "Build options");
			Test(this.LinkOptions,  actual.LinkOptions,  "Link options");
			Test(this.ImportLib,    actual.ImportLib,    "Import library");
			Test(this.PchHeader,    actual.PchHeader,    "PCH Header");
			Test(this.PchSource,    actual.PchSource,    "PCH Source");
			Test(this.ResOptions,   actual.ResOptions,   "Resource options");
			Test(this.Pch,          actual.Pch,          "Precompiled headers");

			TestList(this.IncludePaths, actual.IncludePaths, "Include paths");
			TestList(this.Defines,      actual.Defines,      "Defined symbols");
			TestList(this.LibPaths,     actual.LibPaths,     "Lib paths");
			TestList(this.Links,        actual.Links,        "Links");
			TestList(this.BuildFlags,   actual.BuildFlags,   "Build flags");
			TestList(this.LinkFlags,    actual.LinkFlags,    "Link flags");
			TestList(this.Dependencies, actual.Dependencies, "Dependencies");
			TestList(this.LinkDeps,     actual.LinkDeps,     "Linker dependencies");
			TestList(this.ResDefines,   actual.ResDefines,   "Resource defines");
			TestList(this.ResPaths,     actual.ResPaths,     "Resource paths");
		}

		private void Test(string expected, string actual, string description)
		{
			if (expected != null && expected.CompareTo(actual) != 0)
				throw new FormatException(description + " should be '" + expected + "' but is '" + actual + "'");
		}

		private void TestList(string[] expected, string[] actual, string description)
		{
			if (expected == null || (expected.Length == 0 && actual == null))
				return;

			if (expected.Length != actual.Length)
			{
				throw new FormatException(description + " list should contain " + expected.Length + 
					" items but has " + actual.Length + ". List follows:\n" +
					BuildListString(actual));
			}

			foreach (string item in expected)
			{
				if (Array.IndexOf(actual, item) < 0)
				{
					throw new FormatException(description + " list missing item '" + item + 
						"'. List follows:\n" + BuildListString(actual));
				}
			}
		}

		private string BuildListString(string[] list)
		{
			StringBuilder msg = new StringBuilder();
			foreach (string s in list)
				msg.Append(s + "\n");
			return msg.ToString();
		}
	}
}
