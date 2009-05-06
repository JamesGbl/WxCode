using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.MonoDev
{
	[TestFixture]
	public class Test_Packages
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

			_parser = new MonoDevParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_PackageName()
		{
			_expects.Package[0].Name = "MyPackage";
			Run();
		}

		[Test]
		public void Test_Cs()
		{
			_expects.Package[0].Language = "c#";
			Run();
		}

	}
}
