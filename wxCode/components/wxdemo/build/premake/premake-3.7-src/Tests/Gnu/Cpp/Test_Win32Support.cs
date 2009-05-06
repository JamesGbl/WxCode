using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Gnu.Cpp
{
	[TestFixture]
	public class Test_Win32Support
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

		[Test]
		public void Test_Resources()
		{
			_script.Replace("'somefile.txt'", "'somefile.rc'");
			_expects.Package[0].File.Add("somefile.rc");
			Run("--os windows");
		}
	}
}
