using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2005.Cs
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
			_script = Script.MakeBasic("exe", "c#");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new Vs2005Parser();
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
			_expects.Package[0].Config[0].Target = "MyPackage";
			_expects.Package[0].Config[1].Target = "MyPackage";
			Run();
		}

		[Test]
		public void Test_SetOnPackage()
		{
			_script.Append("package.target = 'MyApp'");
			_expects.Package[0].Config[0].Target = "MyApp";
			_expects.Package[0].Config[1].Target = "MyApp";
			Run();
		}

		[Test]
		public void Test_SetOnPackageConfig()
		{
			/* Config specific target not supported for C# */
			_script.Append("package.config['Debug'].target = 'MyPackage-d'");
			_expects.Package[0].Config[0].Target = "MyPackage-d";
			_expects.Package[0].Config[1].Target = "MyPackage-d";
			Run();
		}

		[Test]
		public void Test_TargetIncludesPath()
		{
			_script.Append("package.target = 'MyApp/MyPackage'");
			_expects.Package[0].Config[0].OutDir = "./MyApp";
			_expects.Package[0].Config[0].Target = "MyPackage";
			_expects.Package[0].Config[1].OutDir = "./MyApp";
			_expects.Package[0].Config[1].Target = "MyPackage";
			Run();
		}

		[Test]
		public void Test_TargetInPackageDir()
		{
			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.target = package.path .. '/bin/MyTarget'");
			_expects.Package[0].Config[0].OutDir = "../MyPackage/bin";
			_expects.Package[0].Config[0].Target = "MyTarget";
			_expects.Package[0].Config[1].OutDir = "../MyPackage/bin";
			_expects.Package[0].Config[1].Target = "MyTarget";
			Run();
		}


		#endregion
	}
}
