using System;
using System.IO;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2002.Cs
{
	[TestFixture]
	public class Test_LibPaths
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
			_expects.Package[0].Config.Add(2);

			_parser = new Vs2002Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_NoLibPaths()
		{
			_expects.Package[0].Config[0].LibPaths = new string[] { };
			_expects.Package[0].Config[1].LibPaths = new string[] { };
			Run();
		}

		[Test]
		public void Test_PathsOnPackage()
		{
			_script.Append("package.libpaths = { 'src', 'include' }");
			_expects.Package[0].Config[0].LibPaths = new string[] { "src", "include" };
			_expects.Package[0].Config[1].LibPaths = new string[] { "src", "include" };
			Run();
		}

		[Test]
		public void Test_PathsInPackageConfig()
		{
			/* C# doesn't allow per-config lib paths */
			_script.Append("package.config['Debug'].libpaths = { 'debug' }");
			_script.Append("package.config['Release'].libpaths = { 'release' }");
			_expects.Package[0].Config[0].LibPaths = new string[] { "debug" };
			_expects.Package[0].Config[1].LibPaths = new string[] { "debug" };
			Run();
		}
			
		[Test]
		public void Test_PathsOnPackageAndConfig()
		{
			_script.Append("package.libpaths = { 'package' }");
			_script.Append("package.config['Debug'].libpaths = { 'debug' }");
			_script.Append("package.config['Release'].libpaths = { 'release' }");
			_expects.Package[0].Config[0].LibPaths = new string[] { "package", "debug" };
			_expects.Package[0].Config[1].LibPaths = new string[] { "package", "debug" };
			Run();
		}
	}
}
