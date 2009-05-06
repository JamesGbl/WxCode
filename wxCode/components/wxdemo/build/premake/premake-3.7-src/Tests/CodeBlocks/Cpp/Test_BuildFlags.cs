using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.CodeBlocks.Cpp
{
	[TestFixture]
	public class Test_BuildFlags
	{
		Script _script;
		Project _expects;
		Parser _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new CodeBlocksParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_SetFlagOnPackage()
		{
			_script.Append("package.buildflags = { 'no-exceptions' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-exceptions" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-exceptions", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_SetFlagOnConfig()
		{
			_script.Append("package.config['Debug'].buildflags = { 'no-exceptions' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-exceptions" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_SetFlagOnPackageAndConfig()
		{
			_script.Append("package.buildflags = { 'no-exceptions' }");
			_script.Append("package.config['Release'].buildflags = { 'no-rtti' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-exceptions" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-exceptions", "no-rtti" };
			Run();
		}

		[Test]
		public void Test_ExtraWarnings()
		{
			_script.Append("package.buildflags = { 'extra-warnings' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "extra-warnings" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "extra-warnings", "optimize", "no-symbols" };
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
		public void Test_NoExceptions()
		{
			_script.Append("package.buildflags = { 'no-exceptions' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-exceptions" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-exceptions", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_NoFramePointer()
		{
			_script.Append("package.buildflags = { 'no-frame-pointer' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-frame-pointer" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-frame-pointer", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_NoImportLib()
		{
			_script.Replace("exe", "dll");
			_script.Append("package.buildflags = { 'no-import-lib' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-import-lib" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-import-lib", "optimize", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_NoRtti()
		{
			_script.Append("package.buildflags = { 'no-rtti' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "no-rtti" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "no-rtti", "optimize", "no-symbols" };
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
		public void Test_OptimizeSize()
		{
			_script.Append("package.buildflags = { 'optimize-size' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "optimize-size" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize-size", "no-symbols" };
			Run();
		}

		[Test]
		public void Test_OptimizeSpeed()
		{
			_script.Append("package.buildflags = { 'optimize-speed' }");
			_expects.Package[0].Config[0].BuildFlags = new string[] { "optimize-speed" };
			_expects.Package[0].Config[1].BuildFlags = new string[] { "optimize-speed", "no-symbols" };
			Run();
		}
	}
}
