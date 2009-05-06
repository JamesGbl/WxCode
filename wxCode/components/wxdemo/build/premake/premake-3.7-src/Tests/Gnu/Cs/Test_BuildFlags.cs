using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cs
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

			_parser = new GnuParser();
		}

		public void Run(params string[] args)
		{
			TestEnvironment.Run(_script, _parser, _expects, args);
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
			Run("--dotnet ms");
		}

		[Test]
		public void Test_OptimizeMono()
		{
			/* Mono does not support an optimize flag */
			_script.Append("package.buildflags = { 'optimize' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-symbols" };
			Run("--dotnet mono");
		}
	}
}
