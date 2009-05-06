using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.SharpDev
{
	[TestFixture]
	public class Test_Paths
	{
		Script  _script;
		Project _expects;
		Parser  _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c#");

			_expects = new Project();
			_expects.Package.Add(1);

			_parser = new SharpDevParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_AllInSameDirectory()
		{
			_expects.Package[0].Path = ".";
			Run();
		}

		[Test]
		public void Test_PackageInSubDir()
		{
			_script.Append("package.path = 'MySubDir'");
			_expects.Package[0].Path = "./MySubDir";
			Run();
		}

		[Test]
		public void Test_ProjectInSubDir()
		{
			_script.Append("project.path = 'Build'");
			_expects.Path = "Build";
			_expects.Package[0].Path = "..";
			Run();
		}

		[Test]
		public void Test_BothInSubDirs()
		{
			_script.Append("project.path = 'BuildDir'");
			_script.Append("package.path = 'PkgDir'");
			_expects.Path = "BuildDir";
			_expects.Package[0].Path = "../PkgDir";
			Run();
		}

		[Test]
		public void Test_BothInSameSubDir()
		{
			_script.Append("project.path = 'Build'");
			_script.Append("package.path = 'Build'");
			_expects.Path = "Build";
			_expects.Package[0].Path = ".";
			Run();
		}

	}
}
