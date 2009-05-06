using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2003.Cpp
{
	[TestFixture]
	public class Test_ImportLib
	{
		Script  _script;
		Project _expects;
		Parser  _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("dll", "c++");
			_script.Append("project.bindir = 'bin'");
			_script.Append("project.libdir = 'lib'");

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

		[Test]
		public void Test_WithImportLib()
		{
			_expects.Package[0].Config[0].ImportLib = "lib/MyPackage.lib";
			_expects.Package[0].Config[1].ImportLib = "lib/MyPackage.lib";
			Run();
		}

		[Test]
		public void Test_NoImportLib()
		{
			_script.Append("package.buildflags = { 'no-import-lib' }");
			_expects.Package[0].Config[0].ImportLib = "obj/Debug/MyPackage.lib";
			_expects.Package[0].Config[1].ImportLib = "obj/Release/MyPackage.lib";
			Run();
		}
	}
}
