using System;
using NUnit.Framework;
using Premake.Tests.Framework;

namespace Premake.Tests.Vs2003.Cpp
{
	[TestFixture]
	public class Test_Pch
	{
		Script _script;
		Project _expects;
		Parser _parser;

		#region Setup and Teardown
		[SetUp]
		public void Test_Setup()
		{
			_script = Script.MakeBasic("exe", "c++");

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
		public void PchDefaultsToAuto()
		{
			_expects.Package[0].Config[0].Pch = "auto";
			_expects.Package[0].Config[1].Pch = "auto";
			Run();
		}

		[Test]
		public void NoPchTurnsItOff()
		{
			_script.Append("package.buildflags = {'no-pch'}");
			_expects.Package[0].Config[0].Pch = "off";
			_expects.Package[0].Config[1].Pch = "off";
			Run();
		}

		[Test]
		public void PchOnWhenPchHeaderIsSet()
		{
			_script.Append("package.pchheader='stdafx.h'");
			_expects.Package[0].Config[0].Pch = "on";
			_expects.Package[0].Config[1].Pch = "on";
			Run();
		}

		[Test]
		public void PchHeaderIsSet()
		{
			_script.Append("package.pchheader='stdafx.h'");
			_expects.Package[0].Config[0].PchHeader = "stdafx.h";
			_expects.Package[0].Config[1].PchHeader = "stdafx.h";
			Run();
		}

		[Test]
		public void PchSourceIsSet()
		{
			_script.Replace("'somefile.txt'", "'stdafx.cpp'");
			_script.Append("package.pchheader = 'stdafx.h'");
			_script.Append("package.pchsource = 'stdafx.cpp'");
			_expects.Package[0].Config[0].PchSource = "stdafx.cpp";
			_expects.Package[0].Config[1].PchSource = "stdafx.cpp";
			Run();
		}

		[Test]
		public void PchOffWithFlagAndHeader()
		{
			_script.Append("package.buildflags = {'no-pch'}");
			_script.Append("package.pchheader='stdafx.h'");
			_expects.Package[0].Config[0].Pch = "off";
			_expects.Package[0].Config[1].Pch = "off";
			Run();
		}
	}
}
