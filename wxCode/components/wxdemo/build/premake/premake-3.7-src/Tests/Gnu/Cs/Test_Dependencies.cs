using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cs
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

			_parser = new GnuParser();
		}

		public void Run(params string[] options)
		{
			TestEnvironment.Run(_script, _parser, _expects, options);
		}
		#endregion

		[Test]
		public void Test_ExeAndDll()
		{
			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c#'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "PackageB.dll" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "PackageB.dll" };

			Run("--os linux");
		}


		[Test]
		public void Test_ExeAndDllFromSubDir()
		{
			_script.Append("project.bindir = 'bin'");

			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.links = { 'PackageB' }");

			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c#'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../bin/PackageB.dll" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../bin/PackageB.dll" };

			Run("--os linux");
		}


		[Test]
		public void Test_ExeAndDllWithTargetPath()
		{
			_script.Append("project.bindir = 'bin'");

			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.links = { 'PackageB' }");

			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c#'");
			_script.Append("package.target = 'Modules/PackageB'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../bin/Modules/PackageB.dll" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../bin/Modules/PackageB.dll" };

			Run("--os linux");
		}


		[Test]
		public void Test_ExeAndDllWithBinDir()
		{
			_script.Append("project.config['Debug'].bindir   = 'Bin/Debug'");
			_script.Append("project.config['Release'].bindir = 'Bin/Release'");

			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.links = { 'PackageB' }");

			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c#'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../Bin/Debug/PackageB.dll" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../Bin/Release/PackageB.dll" };

			Run("--os linux");
		}
	}
}
