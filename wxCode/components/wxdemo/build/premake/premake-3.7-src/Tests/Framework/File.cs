using System;

namespace Premake.Tests.Framework
{
	public class SourceFile
	{
		public string Name;
		public string Subtype;
		public string BuildAction;
		public string DependsOn;

		public void CompareTo(SourceFile actual)
		{
			Test(this.Name,        actual.Name,        "Name");
			Test(this.Subtype,     actual.Subtype,     "Subtype");
			Test(this.BuildAction, actual.BuildAction, "Build action");
			Test(this.DependsOn,   actual.DependsOn,   "Dependency");
		}
	
		private void Test(string expected, string actual, string description)
		{
			if (expected != null && expected.CompareTo(actual) != 0)
				throw new FormatException(description + " on '" + this.Name + "' should be '" + expected + "' but is '" + actual + "'");
		}
	}
}
