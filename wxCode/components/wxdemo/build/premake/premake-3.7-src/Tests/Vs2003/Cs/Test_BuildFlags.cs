using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2003.Cs
{
	[TestFixture]
	public class Test_BuildFlags
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

		[Test]
		public void Test_SetFlagOnPackage()
		{
			_script.Append("package.buildflags = { 'unsafe' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "unsafe" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "unsafe", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_SetFlagOnConfig()
		{
			_script.Append("package.config['Debug'].buildflags = { 'unsafe' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "unsafe" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize", "no-symbols" };
			Run();
		}

		public void Test_SetFlagOnPackageAndConfig()
		{
			_script.Append("package.buildflags = { 'unsafe' }");
			_script.Append("package.config['Release'].buildflags = { 'fatal-warnings' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "unsafe" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "unsafe", "fatal-warnings" };
			Run();
		}

		[Test]
		public void Test_FatalWarnings()
		{
			_script.Append("package.buildflags = { 'fatal-warnings' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "fatal-warnings" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "fatal-warnings", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_NoSymbols()
		{
			_script.Append("package.buildflags = { 'no-symbols' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-symbols" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_Optimize()
		{
			_script.Append("package.buildflags = { 'optimize' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "optimize" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_Unsafe()
		{
			_script.Append("package.buildflags = { 'unsafe' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "unsafe" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "unsafe", "optimize", "no-symbols" };
			Run();
		}
	}
}
