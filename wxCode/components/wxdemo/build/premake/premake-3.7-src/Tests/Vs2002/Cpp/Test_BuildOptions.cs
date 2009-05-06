using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2002.Cpp
{
	[TestFixture]
	public class Test_BuildOptions
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
		public void Test_SetOptionsOnPackage()
		{
			_script.Append("package.buildoptions = { 'pkgopt'  }");
			_expects.Package[0].Config[0].BuildOptions = "pkgopt";
			_expects.Package[0].Config[1].BuildOptions = "pkgopt";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnConfig()
		{
			_script.Append("package.config['Debug'].buildoptions = { 'dbgopt'  }");
			_script.Append("package.config['Release'].buildoptions = { 'relopt'  }");
			_expects.Package[0].Config[0].BuildOptions = "dbgopt";
			_expects.Package[0].Config[1].BuildOptions = "relopt";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnPackageAndConfig()
		{
			_script.Append("package.buildoptions = { 'pkgopt'  }");
			_script.Append("package.config['Release'].buildoptions = { 'relopt'  }");
			_expects.Package[0].Config[0].BuildOptions = "pkgopt";
			_expects.Package[0].Config[1].BuildOptions = "pkgopt relopt";
			Run();
		}
	}
}
