using System;
using System.Collections;

namespace Premake.Tests.Framework
{
	public class Project
	{
		public string      ID;
		public string      Name;
		public string      Path;
		public ArrayList   Configuration;
		public PackageCollection Package;

		public Project()
		{
			Configuration = new ArrayList();
			Package = new PackageCollection();
		}

		public void CompareTo(Project actual)
		{
			Test(this.ID,    actual.ID,    "Project ID");
			Test(this.Name,  actual.Name,  "Project name");
			Test(this.Path,  actual.Path,  "Project path");
			
			this.Package.CompareTo(actual.Package);
		}

		private void Test(string expected, string actual, string description)
		{
			if (expected != null && expected.CompareTo(actual) != 0)
				throw new FormatException(description + " should be '" + expected + "' but is '" + actual + "'");
		}
	}
}
