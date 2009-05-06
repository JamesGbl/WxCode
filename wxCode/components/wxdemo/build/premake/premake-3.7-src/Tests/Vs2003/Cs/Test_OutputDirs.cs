using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2003.Cs
{
	[TestFixture]
	public class Test_OutputDirs
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

			_parser = new Vs2003Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		#region BinDir Tests
		[Test]
		public void Test_BinDir_Default()
		{
			_expects.Package[0].Config[0].BinDir = ".";
			_expects.Package[0].Config[1].BinDir = ".";
			Run();
		}

		[Test]
		public void Test_BinDir_SetAtProject()
		{
			_script.Append("project.bindir = 'bin'");
			_expects.Package[0].Config[0].BinDir = "bin";
			_expects.Package[0].Config[1].BinDir = "bin";
			Run();
		}

		[Test]
		public void Test_BinDir_SetAtProjectConfig()
		{
			_script.Append("project.config['Debug'].bindir = 'bin/Debug'");
			_script.Append("project.config['Release'].bindir = 'bin/Release'");
			_expects.Package[0].Config[0].BinDir = "bin/Debug";
			_expects.Package[0].Config[1].BinDir = "bin/Release";
			Run();
		}
		#endregion
	}
}
