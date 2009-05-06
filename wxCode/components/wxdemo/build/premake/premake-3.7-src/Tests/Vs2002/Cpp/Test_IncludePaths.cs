using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2002.Cpp
{
	[TestFixture]
	public class Test_IncludePaths
	{
		Script  _script;
		Project _expects;
		Parser  _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new Vs2002Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_NoIncludePaths()
		{
			_expects.Package[0].Config[0].IncludePaths = new string[] { };
			_expects.Package[0].Config[1].IncludePaths = new string[] { };
			Run();
		}

		[Test]
		public void Test_PathsOnPackage()
		{
			_script.Append("package.includepaths = { '../src', '../include' }");
			_expects.Package[0].Config[0].IncludePaths = new string[] { "../src", "../include" };
			_expects.Package[0].Config[1].IncludePaths = new string[] { "../src", "../include" };
			Run();
		}

		[Test]
		public void Test_PathsInPackageConfig()
		{
			_script.Append("package.config['Debug'].includepaths = { '../debug' }");
			_script.Append("package.config['Release'].includepaths = { '../release' }");
			_expects.Package[0].Config[0].IncludePaths = new string[] { "../debug" };
			_expects.Package[0].Config[1].IncludePaths = new string[] { "../release" };
			Run();
		}
			
		[Test]
		public void Test_PathsOnPackageAndConfig()
		{
			_script.Append("package.includepaths = { '../package' }");
			_script.Append("package.config['Debug'].includepaths = { '../debug' }");
			_script.Append("package.config['Release'].includepaths = { '../release' }");
			_expects.Package[0].Config[0].IncludePaths = new string[] { "../package", "../debug" };
			_expects.Package[0].Config[1].IncludePaths = new string[] { "../package", "../release" };
			Run();
		}
	}
}
