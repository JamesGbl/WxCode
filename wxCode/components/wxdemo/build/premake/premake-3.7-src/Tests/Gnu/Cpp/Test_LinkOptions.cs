using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
{
	[TestFixture]
	public class Test_LinkOptions
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
		public void Test_SetOptionsOnPackage()
		{
			_script.Append("package.linkoptions = { '--some-opt'  }");
			_expects.Package[0].Config[0].LinkOptions = "--some-opt";
			_expects.Package[0].Config[1].LinkOptions = "--some-opt";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnConfig()
		{
			_script.Append("package.config['Debug'].linkoptions = { '--debug-opt'  }");
			_script.Append("package.config['Release'].linkoptions = { '--release-opt'  }");
			_expects.Package[0].Config[0].LinkOptions = "--debug-opt";
			_expects.Package[0].Config[1].LinkOptions = "--release-opt";
			Run();
		}

		[Test]
		public void Test_SetOptionsOnPackageAndConfig()
		{
			_script.Append("package.linkoptions = { '--some-opt'  }");
			_script.Append("package.config['Release'].linkoptions = { '--release-opt'  }");
			_expects.Package[0].Config[0].LinkOptions = "--some-opt";
			_expects.Package[0].Config[1].LinkOptions = "--some-opt --release-opt";
			Run();
		}
	}
}
