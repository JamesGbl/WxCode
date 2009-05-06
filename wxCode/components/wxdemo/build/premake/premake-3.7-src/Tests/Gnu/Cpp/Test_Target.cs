using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
{
	[TestFixture]
	public class Test_Target
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

		public void Run(params string[] options)
		{
			TestEnvironment.Run(_script, _parser, _expects, options);
		}
		#endregion

		#region Package/Config Level Tests
		[Test]
		public void Test_DefaultTarget()
		{
			_expects.Package[0].Config[0].Target = "MyPackage";
			_expects.Package[0].Config[1].Target = "MyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_SetOnPackage()
		{
			_script.Append("package.target = 'MyApp'");
			_expects.Package[0].Config[0].Target = "MyApp";
			_expects.Package[0].Config[1].Target = "MyApp";
			Run("--os linux");
		}

		[Test]
		public void Test_SetOnPackageConfig()
		{
			_script.Append("package.config['Debug'].target = 'MyPackage-d'");
			_expects.Package[0].Config[0].Target = "MyPackage-d";
			_expects.Package[0].Config[1].Target = "MyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetIncludesPath()
		{
			_script.Append("package.target = 'MyApp/MyPackage'");
			_expects.Package[0].Config[0].OutDir = "./MyApp";
			_expects.Package[0].Config[0].Target = "MyPackage";
			_expects.Package[0].Config[1].OutDir = "./MyApp";
			_expects.Package[0].Config[1].Target = "MyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_DefaultWithBinDir()
		{
			_script.Append("project.bindir = 'bin'");
			_script.Append("package.path = 'MyPackage'");
			_expects.Package[0].Config[0].OutDir = "../bin";
			_expects.Package[0].Config[0].Target = "MyPackage";
			Run("--os linux");
		}
		#endregion

		#region Prefix/Extension Tests

		[Test]
		public void Test_TargetPrefixOnExe()
		{
			_script.Append("package.targetprefix = 'fi'");
			_expects.Package[0].Config[0].Target = "fiMyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetPrefixOnDll()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.targetprefix = 'fi'");
			_expects.Package[0].Config[0].Target = "fiMyPackage.so";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetPrefixOnLib()
		{
			_script.Replace("'exe'", "'lib'");
			_script.Append("package.targetprefix = 'fi'");
			_expects.Package[0].Config[0].Target = "fiMyPackage.a";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetExtensionOnExe()
		{
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "MyPackage.zmf";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetExtensionOnDll()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			Run("--os linux");
		}

		[Test]
		public void Test_TargetExtensionOnLib()
		{
			_script.Replace("'exe'", "'lib'");
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			Run("--os linux");
		}

		#endregion

		#region Windows Targets

		[Test]
		public void Test_Windows_WinExeTarget()
		{
			_script.Replace("'exe'", "'winexe'");
			_expects.Package[0].Config[0].Target = "MyPackage.exe";
			Run("--os windows");
		}

		[Test]
		public void Test_Windows_ExeTarget()
		{
			_expects.Package[0].Config[0].Target = "MyPackage.exe";
			Run("--os windows");
		}

		[Test]
		public void Test_Windows_DllTarget()
		{
			_script.Replace("'exe'", "'dll'");
			_expects.Package[0].Config[0].Target = "MyPackage.dll";
			Run("--os windows");
		}

		[Test]
		public void Test_Windows_LibTarget()
		{
			_script.Replace("'exe'", "'lib'");
			_expects.Package[0].Config[0].Target = "libMyPackage.a";
			Run("--os windows");
		}

		[Test]
		public void Test_Windows_CustomTarget()
		{
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "MyPackage.zmf";
			_expects.Package[0].Config[1].Target = "MyPackage.zmf";
			Run("--os windows");
		}

		[Test]
		public void Test_Windows_CustomTarget_SetOnConfig()
		{
			_script.Append("package.config['Debug'].targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "MyPackage.zmf";
			_expects.Package[0].Config[1].Target = "MyPackage.exe";
			Run("--os windows");
		}

		#endregion

		#region Linux Targets

		[Test]
		public void Test_Linux_WinExeTarget()
		{
			_script.Replace("'exe'", "'winexe'");
			_expects.Package[0].Config[0].Target = "MyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_Linux_ExeTarget()
		{
			_expects.Package[0].Config[0].Target = "MyPackage";
			Run("--os linux");
		}

		[Test]
		public void Test_Linux_DllTarget()
		{
			_script.Replace("'exe'", "'dll'");
			_expects.Package[0].Config[0].Target = "libMyPackage.so";
			Run("--os linux");
		}

		[Test]
		public void Test_Linux_LibTarget()
		{
			_script.Replace("'exe'", "'lib'");
			_expects.Package[0].Config[0].Target = "libMyPackage.a";
			Run("--os linux");
		}

		[Test]
		public void Test_Linux_CustomTarget()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			_expects.Package[0].Config[1].Target = "libMyPackage.zmf";
			Run("--os linux");
		}

		[Test]
		public void Test_Linux_CustomTarget_SetOnConfig()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.config['Debug'].targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			_expects.Package[0].Config[1].Target = "libMyPackage.so";
			Run("--os linux");
		}

		#endregion

		#region MacOS X Targets

		[Test]
		public void Test_MacOSX_WinExeTarget()
		{
			_script.Replace("'exe'", "'winexe'");
			_expects.Package[0].Config[0].Target = "MyPackage";
			Run("--os macosx");
		}

		[Test]
		public void Test_MacOSX_ExeTarget()
		{
			_expects.Package[0].Config[0].Target = "MyPackage";
			Run("--os macosx");
		}

		[Test]
		public void Test_MacOSX_DllTarget()
		{
			_script.Replace("'exe'", "'dll'");
			_expects.Package[0].Config[0].Target = "libMyPackage.so";
			Run("--os macosx");
		}

		[Test]
		public void Test_MacOSX_LibTarget()
		{
			_script.Replace("'exe'", "'lib'");
			_expects.Package[0].Config[0].Target = "libMyPackage.a";
			Run("--os macosx");
		}

		[Test]
		public void Test_MacOSX_CustomTarget()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			_expects.Package[0].Config[1].Target = "libMyPackage.zmf";
			Run("--os macosx");
		}

		[Test]
		public void Test_MacOSX_CustomTarget_SetOnConfig()
		{
			_script.Replace("'exe'", "'dll'");
			_script.Append("package.config['Debug'].targetextension = 'zmf'");
			_expects.Package[0].Config[0].Target = "libMyPackage.zmf";
			_expects.Package[0].Config[1].Target = "libMyPackage.so";
			Run("--os macosx");
		}

		#endregion
	}
}
