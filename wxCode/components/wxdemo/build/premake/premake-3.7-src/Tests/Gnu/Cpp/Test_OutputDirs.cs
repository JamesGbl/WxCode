using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
{
	[TestFixture]
	public class Test_OutputDirs
	{
		Script  _script;
		Project _expects;
		Parser  _parser;

		#region Setup and Teardown

		[SetUp]
		public void Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new GnuParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}

		#endregion

		#region BinDir Tests

		[Test]
		public void BinDir_Default()
		{
			_expects.Package[0].Config[0].BinDir = ".";
			_expects.Package[0].Config[1].BinDir = ".";
			Run();
		}

		[Test]
		public void BinDir_SetAtProject()
		{
			_script.Append("project.bindir = 'bin'");
			_expects.Package[0].Config[0].BinDir = "bin";
			_expects.Package[0].Config[1].BinDir = "bin";
			Run();
		}

		[Test]
		public void BinDir_SetAtProjectConfig()
		{
			_script.Append("project.config['Debug'].bindir = 'bin/Debug'");
			_script.Append("project.config['Release'].bindir = 'bin/Release'");
			_expects.Package[0].Config[0].BinDir = "bin/Debug";
			_expects.Package[0].Config[1].BinDir = "bin/Release";
			Run();
		}

		[Test]
		public void BinDir_FromSubDir()
		{
			_script.Append("project.bindir = 'bin'");
			_script.Append("package.path = 'MyPackage'");
			_expects.Package[0].Config[0].BinDir = "../bin";
			_expects.Package[0].Config[1].BinDir = "../bin";
			Run();
		}


		[Test]
		public void BinDir_SetAtPackage()
		{
			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.bindir = 'bin'");
			_expects.Package[0].Config[0].BinDir = "bin";
			_expects.Package[0].Config[1].BinDir = "bin";
			Run();
		}

		#endregion

		#region LibDir Tests
		[Test]
		public void Test_LibDir_Default()
		{
			_expects.Package[0].Config[0].LibDir = ".";
			_expects.Package[0].Config[1].LibDir = ".";
			Run();
		}

		[Test]
		public void Test_LibDir_SetAtProject()
		{
			_script.Append("project.libdir = 'lib'");
			_expects.Package[0].Config[0].LibDir = "lib";
			_expects.Package[0].Config[1].LibDir = "lib";
			Run();
		}

		[Test]
		public void Test_LibDir_SetAtProjectConfig()
		{
			_script.Append("project.config['Debug'].libdir = 'lib/Debug'");
			_script.Append("project.config['Release'].libdir = 'lib/Release'");
			_expects.Package[0].Config[0].LibDir = "lib/Debug";
			_expects.Package[0].Config[1].LibDir = "lib/Release";
			Run();
		}
		#endregion

		#region ObjDir Tests
		[Test]
		public void Test_ObjDir_Default()
		{
			_expects.Package[0].Config[0].ObjDir = "obj/Debug";
			_expects.Package[0].Config[1].ObjDir = "obj/Release";
			Run();
		}

		[Test]
		public void Test_ObjDir_SetAtPackage()
		{
			_script.Append("package.objdir = 'temp'");
			_expects.Package[0].Config[0].ObjDir = "temp/Debug";
			_expects.Package[0].Config[1].ObjDir = "temp/Release";
			Run();
		}
		#endregion
	}
}
