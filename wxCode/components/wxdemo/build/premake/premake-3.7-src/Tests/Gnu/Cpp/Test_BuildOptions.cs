using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
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

			_parser = new GnuParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void NoBuildOptions()
		{
			_expects.Package[0].Config[0].BuildOptions = "";
			_expects.Package[0].Config[1].BuildOptions = "";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnPackage()
		{
			_script.Append("package.buildoptions = { '`wx-config --dll`'  }");
			_expects.Package[0].Config[0].BuildOptions = "`wx-config --dll`";
			_expects.Package[0].Config[1].BuildOptions = "`wx-config --dll`";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnConfig()
		{
			_script.Append("package.config['Debug'].buildoptions = { '`wx-config --dll --debug`'  }");
			_script.Append("package.config['Release'].buildoptions = { '`wx-config --dll`'  }");
			_expects.Package[0].Config[0].BuildOptions = "`wx-config --dll --debug`";
			_expects.Package[0].Config[1].BuildOptions = "`wx-config --dll`";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnPackageAndConfig()
		{
			_script.Append("package.buildoptions = { '--some-opt'  }");
			_script.Append("package.config['Release'].buildoptions = { '--release-opt'  }");
			_expects.Package[0].Config[0].BuildOptions = "--some-opt";
			_expects.Package[0].Config[1].BuildOptions = "--some-opt --release-opt";
			Run();
		}
	}
}
