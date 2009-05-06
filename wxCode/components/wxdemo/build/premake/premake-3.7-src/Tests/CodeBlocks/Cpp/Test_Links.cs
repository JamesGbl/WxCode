using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.CodeBlocks.Cpp
{
	[TestFixture]
	public class Test_Links
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

			_parser = new CodeBlocksParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_LinksOnPackage()
		{
			_script.Append("package.links = { 'lib1', 'lib2' }");
			_expects.Package[0].Config[0].Links = new string[] { "lib1", "lib2" };
			_expects.Package[0].Config[1].Links = new string[] { "lib1", "lib2" };
			Run();
		}

		[Test]
		public void Test_LinksOnPackageConfig()
		{
			_script.Append("package.config['Debug'].links = { 'lib1-d' }");
			_script.Append("package.config['Release'].links = { 'lib1' }");
			_expects.Package[0].Config[0].Links = new string[] { "lib1-d" };
			_expects.Package[0].Config[1].Links = new string[] { "lib1" };
			Run();
		}

		[Test]
		public void Test_LinksOnPackageAndConfig()
		{
			_script.Append("package.links = { 'pkglib' }");
			_script.Append("package.config['Debug'].links = { 'liba-d' }");
			_script.Append("package.config['Release'].links = { 'liba' }");
			_expects.Package[0].Config[0].Links = new string[] { "pkglib", "liba-d" };
			_expects.Package[0].Config[1].Links = new string[] { "pkglib", "liba" };
			Run();
		}

	}
}
