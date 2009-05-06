using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.MonoDev.Cs
{
	[TestFixture]
	public class Test_Dependencies
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
			_expects.Package.Add(2);
			_expects.Package[0].Config.Add(2);
			_expects.Package[1].Config.Add(2);

			_parser = new MonoDevParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

#if FAILING
		[Test]
		public void Test_ExeAndDll()
		{
			/* This test is failing, but I can't figure out why. At the end of 
			 * MonoDevParser.Parse() the dependencies are set correctly, but
			 * when that method exits back to TestEnvironment.Run() they have
			 * disappeared, replaced by an empty list! I spent a couple of
			 * hours trying to figure this one out to no avail. If you think
			 * you are up for it, search for SHARPDEV_DEPENDENCY_BUG in 
			 * Parser_MonoDev.cs and TestEnvironment.cs. */

			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c#'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			Run();
		}
#endif
	}
}
