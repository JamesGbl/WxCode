using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests
{
	[TestFixture]
	public class Test_MatchFiles
	{
		#region Setup and Teardown
		Script  _script;
		Project _expects;
		Parser  _parser;

		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new Premake.Tests.Gnu.GnuParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_SimplePattern()
		{
			_script.Replace("'somefile.txt'", "matchfiles('*.cpp')");
			TestEnvironment.AddFile("file0.cpp");
			TestEnvironment.AddFile("file1.cpp");
			_expects.Package[0].File.Add("file0.cpp");
			_expects.Package[0].File.Add("file1.cpp");
			Run();
		}

		[Test]
		public void Test_SubdirPattern()
		{
			_script.Replace("'somefile.txt'", "matchfiles('Code/*.cpp')");
			TestEnvironment.AddFile("Code/file0.cpp");
			TestEnvironment.AddFile("Code/file1.cpp");
			_expects.Package[0].File.Add("Code/file0.cpp");
			_expects.Package[0].File.Add("Code/file1.cpp");
			Run();
		}

		[Test]
		public void Test_WithBuildActions()
		{
			_script.Replace("'c++'", "'c#'");
			_script.Replace("'somefile.txt'", "matchfiles('*.cs', '*.bmp')");
			_script.Append("package.config[matchfiles('*.bmp')].buildaction = 'EmbeddedResource'");
			TestEnvironment.AddFile("file0.cs");
			TestEnvironment.AddFile("file1.cs");
			TestEnvironment.AddFile("image0.bmp");
			TestEnvironment.AddFile("image1.bmp");
			_expects.Package[0].File.Add("file0.cs");
			_expects.Package[0].File.Add("file1.cs");
			_expects.Package[0].File.Add("image0.bmp");
			_expects.Package[0].File.Add("image1.bmp");
			Run();
		}

		[Test]
		public void Test_WithPackagePath()
		{
			_script.Replace("package.files", "package.path='Subdir'\n package.files");
			_script.Replace("'somefile.txt'", "matchfiles('*.cpp')");
			TestEnvironment.AddFile("Subdir/aaaa.cpp");
			TestEnvironment.AddFile("Subdir/bbbb.cpp");
			_expects.Package[0].File.Add("aaaa.cpp");
			_expects.Package[0].File.Add("bbbb.cpp");
			Run();
		}

		[Test]
		public void Test_WithPackageDotPath()
		{
			_script.Replace("package.files", "package.path='.'\n package.files");
			_script.Replace("'somefile.txt'", "matchfiles('*.cpp')");
			TestEnvironment.AddFile("aaaa.cpp");
			TestEnvironment.AddFile("bbbb.cpp");
			_expects.Package[0].File.Add("aaaa.cpp");
			_expects.Package[0].File.Add("bbbb.cpp");
			Run();
		}

		[Test]
		public void Test_RecursiveMatch()
		{
			_script.Replace("'somefile.txt'", "matchrecursive('*.cpp')");
			TestEnvironment.AddFile("aaaa.cpp");
			TestEnvironment.AddFile("Sub0/bbbb.cpp");
			TestEnvironment.AddFile("Sub0/Sub1/cccc.cpp");
			TestEnvironment.AddFile("Sub2/dddd.cpp");
			_expects.Package[0].File.Add("aaaa.cpp");
			_expects.Package[0].File.Add("Sub0/bbbb.cpp");
			_expects.Package[0].File.Add("Sub0/Sub1/cccc.cpp");
			_expects.Package[0].File.Add("Sub2/dddd.cpp");
			Run();
		}

		[Test]
		public void Test_RecursiveMatchWithSubdirMask()
		{
			_script.Replace("'somefile.txt'", "matchrecursive('Sub0/*.cpp')");
			TestEnvironment.AddFile("aaaa.cpp");
			TestEnvironment.AddFile("Sub0/bbbb.cpp");
			TestEnvironment.AddFile("Sub0/Sub1/cccc.cpp");
			TestEnvironment.AddFile("Sub2/dddd.cpp");
			_expects.Package[0].File.Add("Sub0/bbbb.cpp");
			_expects.Package[0].File.Add("Sub0/Sub1/cccc.cpp");
			Run();
		}
	}
}
