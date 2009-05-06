using System;
using System.IO;
using System.Text;

namespace Premake.Tests.Framework
{
	public class Script
	{
		private StringBuilder _text;

		public Script()
		{
			_text = new StringBuilder();
		}

		public static Script MakeBasic(string kind, string lang)
		{
			Script script = new Script();

			script.Append("  project.name = 'MyProject'     ");
			script.Append("  package.name = 'MyPackage'     ");
			script.Append("  package.kind = '"+kind+"'      ");
			script.Append("  package.language = '"+lang+"'  ");
			script.Append("  package.files = { 'somefile.txt' }");
			return script;
		}

		public void Append(string line)
		{
			_text.Append(line);
			_text.Append("\n");
		}

		public void Replace(string pattern, string replacement)
		{
			_text.Replace(pattern, replacement);
		}

		public void WriteFile(string filename)
		{
			string path = Path.GetDirectoryName(filename);
			if (path != String.Empty)
				System.IO.Directory.CreateDirectory(path);

			System.IO.StreamWriter writer = new System.IO.StreamWriter(filename);
			writer.Write(_text.ToString());
			writer.Close();
		}

		public override string ToString()
		{
			return _text.ToString();
		}

	}
}

