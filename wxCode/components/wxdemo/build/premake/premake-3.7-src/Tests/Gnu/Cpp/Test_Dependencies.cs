using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
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
			_script = Script.MakeBasic("exe", "c++");

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

		#region DLL Dependencies

		[Test]
		public void Test_ExeAndDll()
		{
			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'dll'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "libPackageB.so" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "libPackageB.so" };

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
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../bin/libPackageB.so" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../bin/libPackageB.so" };

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
			_script.Append("package.language = 'c++'");
			_script.Append("package.target = 'Modules/PackageB'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../bin/Modules/libPackageB.so" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../bin/Modules/libPackageB.so" };

			Run("--os linux");
		}

		#endregion

		#region Static Lib Dependencies

		[Test]
		public void Test_ExeAndLib()
		{
			_script.Append("package.links = { 'PackageB' }");
			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'lib'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "libPackageB.a" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "libPackageB.a" };

			Run("--os linux");
		}

		
		[Test]
		public void Test_ExeAndLibFromSubDir()
		{
			_script.Append("project.libdir = 'lib'");

			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.links = { 'PackageB' }");

			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'lib'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../lib/libPackageB.a" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../lib/libPackageB.a" };

			Run("--os linux");
		}

		[Test]
		public void Test_ExeAndLibWithTargetPath()
		{
			_script.Append("project.libdir = 'lib'");

			_script.Append("package.path = 'MyPackage'");
			_script.Append("package.links = { 'PackageB' }");

			_script.Append("package = newpackage()");
			_script.Append("package.name = 'PackageB'");
			_script.Append("package.kind = 'lib'");
			_script.Append("package.language = 'c++'");
			_script.Append("package.target = 'Modules/PackageB'");
			_script.Append("package.files = matchfiles('*.cpp')");
			_script.Append("package.path = 'PackageB'");

			_expects.Package[0].Config[0].Dependencies = new string[]{ "PackageB" };
			_expects.Package[0].Config[1].Dependencies = new string[]{ "PackageB" };

			_expects.Package[0].Config[0].LinkDeps = new string[]{ "../lib/Modules/libPackageB.a" };
			_expects.Package[0].Config[1].LinkDeps = new string[]{ "../lib/Modules/libPackageB.a" };

			Run("--os linux");
		}

		#endregion
	}
}
