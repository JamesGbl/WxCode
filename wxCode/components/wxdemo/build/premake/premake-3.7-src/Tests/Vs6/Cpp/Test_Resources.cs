using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs6.Cpp
{
	[TestFixture]
	public class Test_Resources
	{
		Script _script;
		Project _expects;
		Parser _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new Vs6Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_UsesIncludePaths()
		{
			_script.Append("package.includepaths = { 'include' }");
			_expects.Package[0].Config[0].ResPaths = new string[] { "include" };
			_expects.Package[0].Config[1].ResPaths = new string[] { "include" };
			Run();
		}

		[Test]
		public void Test_UsesResourcePaths()
		{
			_script.Append("package.respaths = { 'resources' }");
			_expects.Package[0].Config[0].ResPaths = new string[] { "resources" };
			_expects.Package[0].Config[1].ResPaths = new string[] { "resources" };
			Run();
		}

		[Test]
		public void Test_MergesIncludeAndResourcePaths()
		{
			_script.Append("package.includepaths = { 'include' }");
			_script.Append("package.respaths = { 'resources' }");
			_expects.Package[0].Config[0].ResPaths = new string[] { "include", "resources" };
			_expects.Package[0].Config[1].ResPaths = new string[] { "include", "resources" };
			Run();
		}

		[Test]
		public void Test_UsesDefines()
		{
			_script.Append("package.defines = { 'MYDEFINE' }");
			_expects.Package[0].Config[0].ResDefines = new string[] { "_DEBUG", "MYDEFINE" };
			_expects.Package[0].Config[1].ResDefines = new string[] { "NDEBUG", "MYDEFINE" };
			Run();
		}

		[Test]
		public void Test_UsesResourceDefines()
		{
			_script.Append("package.resdefines = { 'RESDEFINE' }");
			_expects.Package[0].Config[0].ResDefines = new string[] { "_DEBUG", "RESDEFINE" };
			_expects.Package[0].Config[1].ResDefines = new string[] { "NDEBUG", "RESDEFINE" };
			Run();
		}

		[Test]
		public void Test_UsesDefinesAndResourceDefines()
		{
			_script.Append("package.defines = { 'MYDEFINE' }");
			_script.Append("package.resdefines = { 'RESDEFINE' }");
			_expects.Package[0].Config[0].ResDefines = new string[] { "_DEBUG", "MYDEFINE", "RESDEFINE" };
			_expects.Package[0].Config[1].ResDefines = new string[] { "NDEBUG", "MYDEFINE", "RESDEFINE" };
			Run();
		}

		[Test]
		public void Test_UsesResourceOptions()
		{
			_script.Append("package.resoptions = { 'ABC', 'XYZ' }");
			_expects.Package[0].Config[0].ResOptions = "ABC XYZ";
			_expects.Package[0].Config[1].ResOptions = "ABC XYZ";
			Run();
		}
	}
}
