using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs6.Cpp
{
	[TestFixture]
	public class Test_Target
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

			_parser = new Vs6Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		#region Default Target Tests
		
		[Test]
		public void Test_DefaultTarget()
		{
			_expects.Package[0].Config[0].Target = "MyPackage.exe";
			_expects.Package[0].Config[1].Target = "MyPackage.exe";
			Run();
		}

		[Test]
		public void Test_SetOnPackage()
		{
			_script.Append("package.target = 'MyApp'");
			_expects.Package[0].Config[0].Target = "MyApp.exe";
			_expects.Package[0].Config[1].Target = "MyApp.exe";
			Run();
		}

		[Test]
		public void Test_SetOnPackageConfig()
		{
			_script.Append("package.config['Debug'].target = 'MyPackage-d'");
			_expects.Package[0].Config[0].Target = "MyPackage-d.exe";
			_expects.Package[0].Config[1].Target = "MyPackage.exe";
			Run();
		}

		[Test]
		public void Test_TargetIncludesPath()
		{
			_script.Append("package.target = 'MyApp/MyPackage'");
			_expects.Package[0].Config[0].OutDir = "./MyApp";
			_expects.Package[0].Config[0].Target = "MyPackage.exe";
			_expects.Package[0].Config[1].OutDir = "./MyApp";
			_expects.Package[0].Config[1].Target = "MyPackage.exe";
			Run();
		}

		[Test]
		public void TargetAppliedToImportLib()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("project.bindir = 'bin'");
			_script.Append("project.libdir = 'lib'");
			_script.Append("package.target = 'MyApp/MyPackage'");
			_expects.Package[0].Config[0].OutDir = "bin/MyApp";
			_expects.Package[0].Config[0].ImportLib = "lib/MyApp/MyPackage.lib";
			_expects.Package[0].Config[1].OutDir = "bin/MyApp";
			_expects.Package[0].Config[1].ImportLib = "lib/MyApp/MyPackage.lib";
			Run();
		}

		[Test]
		public void TargetAppliedToStaticLib()
		{
			_script.Replace("'exe'", "'lib'");
			_script.Append("package.target = 'MyLib'");
			_expects.Package[0].Config[0	].Target = "MyLib.lib";
			_expects.Package[0].Config[1].Target = "MyLib.lib";
			Run();
		}

		#endregion

		#region Custom Target Tests

		[Test]
		public void Test_CustomTarget()
		{
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "MyPackage.zmf";
			_expects.Package[0].Config[1].Target = "MyPackage.zmf";
			Run();
		}

		[Test]
		public void Test_CustomTarget_SetOnConfig()
		{
			_script.Append("package.config['Debug'].targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "MyPackage.zmf";
			_expects.Package[0].Config[1].Target = "MyPackage.exe";
			Run();
		}

		#endregion

	}
}
