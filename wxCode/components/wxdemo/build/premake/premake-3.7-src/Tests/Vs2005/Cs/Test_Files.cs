using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2005.Cs
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
			_script = Script.MakeBasic("exe", "c#");

			_expects = new Project();
			_expects.Package.Add(1);
			_expects.Package[0].Config.Add(2);

			_parser = new Vs2005Parser();
		}

		public void Run()
		{
			TestEnvironment.Run(_script, _parser, _expects, null);
		}
		#endregion

		/* Note: VS.NET *requires* backslash in source file names */

		[Test]
		public void Test_FilesInRoot()
		{
			_script.Replace("'somefile.txt'", "'file1.cs','file2.cs'");
			_expects.Package[0].File.Add("file1.cs");
			_expects.Package[0].File.Add("file2.cs");
			Run();
		}

		[Test]
		public void Test_FilesInSubDirs()
		{
			_script.Replace("'somefile.txt'", "'Src/file1.cs','Src/Base/file2.cs'");
			_expects.Package[0].File.Add("Src\\file1.cs");
			_expects.Package[0].File.Add("Src\\Base\\file2.cs");
			Run();
		}

		[Test]
		public void Test_FilesAboveDir()
		{
			_script.Replace("'somefile.txt'", "'Src/file1.cs','../Help/file2.cs'");
			_expects.Package[0].File.Add("Src\\file1.cs");
			_expects.Package[0].File.Add("..\\Help\\file2.cs");
			Run();
		}
	}
}
