using System;
using System.IO;

namespace Premake.Tests.Framework
{
	public abstract class Parser
	{
		private string[] _lines;
		private int      _currentLine;

		public void Begin(string filename)
		{
			StreamReader reader = new StreamReader(filename);
			string text = reader.ReadToEnd();
			reader.Close();
			text = text.Replace("\r\n", "\n");
			_lines = text.Split('\n');
			_currentLine = 0;
		}

		public bool Expect(string actual, params string[] expected)
		{
			foreach (string value in expected)
			{
				if (actual == value)
					return true;
			}
			throw new FormatException("Unexpected value '" + actual + "'");
		}

		public string GetLine()
		{
			return _lines[_currentLine++];
		}

		public bool Match(string expected)
		{
			return this.Match(expected, false);
		}

		public bool Match(string expected, bool optional)
		{
			string line = _lines[_currentLine];
			if (expected.CompareTo(line) != 0)
			{
				if (optional)
					return false;
				else
					throw new FormatException("I expected:\n" + expected + "\nBut I got:\n" + line);
			}
			_currentLine++;
			return true;
		}

		public string[] Regex(string expected)
		{
			return this.Regex(expected, false);
		}

		public string[] Regex(string expected, bool optional)
		{
			string line = _lines[_currentLine];
			System.Text.RegularExpressions.Match match = System.Text.RegularExpressions.Regex.Match(line, expected, System.Text.RegularExpressions.RegexOptions.None);
			if (!match.Success)
			{
				if (optional)
					return null;
				else
					throw new FormatException("I expected this pattern:\n" + 
						System.Text.RegularExpressions.Regex.Unescape(expected) + "\nBut I got:\n" + line);
			}
			_currentLine++;

			string[] results = new string[match.Groups.Count - 1];
			for (int i = 0; i < match.Groups.Count - 1; ++i)
				results[i] = match.Groups[i + 1].ToString();
			return results;
		}

		public abstract string TargetName { get; }
		public abstract void Parse(Project project, string filename);
	}
}
