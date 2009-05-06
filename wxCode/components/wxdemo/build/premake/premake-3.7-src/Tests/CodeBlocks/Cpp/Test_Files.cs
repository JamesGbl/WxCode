using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.CodeBlocks.Cpp
{
	[TestFixture]
	public class Test_Files
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

			_parser = new CodeBlocksParser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		[Test]
		public void Test_FilesInRoot()
		{
			_script.Replace("'somefile.txt'", "'file1.cpp','file2.cpp'");
			_expects.Package[0].File.Add("file1.cpp");
			_expects.Package[0].File.Add("file2.cpp");
			Run();
		}

		[Test]
		public void Test_FilesInSubDirs()
		{
			_script.Replace("'somefile.txt'", "'Src/file1.cpp','Src/Base/file2.cpp'");
			_expects.Package[0].File.Add("Src/file1.cpp");
			_expects.Package[0].File.Add("Src/Base/file2.cpp");
			Run();
		}

		[Test]
		public void Test_FilesAboveDir()
		{
			_script.Replace("'somefile.txt'", "'Src/file1.cpp','../Help/file2.cpp'");
			_expects.Package[0].File.Add("Src/file1.cpp");
			_expects.Package[0].File.Add("../Help/file2.cpp");
			Run();
		}

		[Test]
		public void Test_HeaderFiles()
		{
			_script.Replace("'somefile.txt'", "'file1.h','file1.cpp'");
			_expects.Package[0].File.Add("file1.h");
			_expects.Package[0].File.Add("file1.cpp");
			Run();
		}


		[Test]
		public void Test_ResourceFiles()
		{
			_script.Replace("'somefile.txt'", "'../resource1.rc'");
			_expects.Package[0].File.Add("../resource1.rc");
			Run();
		}
	}
}
