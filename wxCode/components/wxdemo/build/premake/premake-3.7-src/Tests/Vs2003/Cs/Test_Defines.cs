using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2003.Cs
{
	[TestFixture]
	public class Test_Defines
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
		public void Test_NoDefines()
		{
			_expects.Package[0].Config[0].Defines = new string[] { };
			_expects.Package[0].Config[1].Defines = new string[] { };
			Run();
		}

		[Test]
		public void Test_DefinesOnPackage()
		{
			_script.Append("package.defines = { 'TRACE', 'EXPORT' }");
			_expects.Package[0].Config[0].Defines = new string[] { "TRACE", "EXPORT" };
			_expects.Package[0].Config[1].Defines = new string[] { "TRACE", "EXPORT" };
			Run();
		}

		[Test]
		public void Test_DefinesInPackageConfig()
		{
			_script.Append("package.config['Debug'].defines = { 'DEBUG' }");
			_script.Append("package.config['Release'].defines = { 'NDEBUG' }");
			_expects.Package[0].Config[0].Defines = new string[] { "DEBUG" };
			_expects.Package[0].Config[1].Defines = new string[] { "NDEBUG" };
			Run();
		}
			
		[Test]
		public void Test_DefinesOnPackageAndConfig()
		{
			_script.Append("package.defines = { 'TRACE' }");
			_script.Append("package.config['Debug'].defines = { 'DEBUG' }");
			_script.Append("package.config['Release'].defines = { 'NDEBUG' }");
			_expects.Package[0].Config[0].Defines = new string[] { "TRACE", "DEBUG" };
			_expects.Package[0].Config[1].Defines = new string[] { "TRACE", "NDEBUG" };
			Run();
		}
	}
}
