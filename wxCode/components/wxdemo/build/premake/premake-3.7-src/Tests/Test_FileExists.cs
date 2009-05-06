using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests
{
	[TestFixture]
	public class Test_FileExists
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
		public void FindExistingFile()
		{
			_script.Append("print(fileexists('premake.lua'))");
			Run();
			string result = TestEnvironment.Output;
			Assert.IsTrue(result.StartsWith("true"), "Did not find existing file");
		}

		[Test]
		public void FailOnMissingFIle()
		{
			_script.Append("print(fileexists('nosuchfile.lua'))");
			Run();
			string result = TestEnvironment.Output;
			Assert.IsTrue(result.StartsWith("false"), "Returned false positive");
		}

	}
}
