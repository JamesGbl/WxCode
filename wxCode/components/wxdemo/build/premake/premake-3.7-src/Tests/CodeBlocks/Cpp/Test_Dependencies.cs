using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.CodeBlocks.Cpp
{
	[TestFixture]
	public class Test_Dependencies
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
			_expects.Package.Add(2);
			_expects.Package[0].Config.Add(2);
			_expects.Package[1].Config.Add(2);

			_parser = new CodeBlocksParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_ExeAndDll()
		{
			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			Run();
		}

		[Test]
		public void Test_ExeAndLib()
		{
			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'lib'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			Run();
		}
	}
}
