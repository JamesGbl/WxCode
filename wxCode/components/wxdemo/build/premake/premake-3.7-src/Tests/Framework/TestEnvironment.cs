using System;
using System.Collections;
using System.Diagnostics;
using System.IO;
using System.Text.RegularExpressions;

namespace Premake.Tests.Framework
{
	public class TestEnvironment
	{
		private static ArrayList _files;
		private static Hashtable _scripts;

		public static string    Errors;
		public static string    Output;

		static TestEnvironment()
		{
			_files = new ArrayList();
			_scripts = new Hashtable();
		}

		public static void AddFile(string filename)
		{
			_files.Add(filename);
		}

		public static void AddScript(Script script)
		{
			_scripts["premake.lua"] = script;
		}

		public static void Run(Script script, Parser parser, Project expected, string[] options)
		{
			/* Remember where Premake is located */
			string executable = Directory.GetCurrentDirectory() + Path.DirectorySeparatorChar + "premake";

			/* Create a temporary directory for this run */
			string temp = Path.GetTempPath() + Guid.NewGuid().ToString() + Path.DirectorySeparatorChar;
			Directory.CreateDirectory(temp);
			Directory.SetCurrentDirectory(temp);

			/* Create any required files */
			foreach (string filename in _files)
			{
				string dirname = Path.GetDirectoryName(filename);
				if (dirname != String.Empty && !Directory.Exists(dirname))
					Directory.CreateDirectory(dirname);
				File.Create(filename).Close();
			}

			try
			{
				/* Write out the script */
				script.WriteFile("premake.lua");

				/* Run premake */
				string args = (options != null) ? String.Join(" ", options) : String.Empty;

				Process process = new Process();
				process.StartInfo.FileName = executable;
				process.StartInfo.Arguments = args + " --target " + parser.TargetName;
				process.StartInfo.CreateNoWindow = true;
				process.StartInfo.WorkingDirectory = Directory.GetCurrentDirectory();
				process.StartInfo.RedirectStandardOutput = true;
				process.StartInfo.RedirectStandardError = true;
				process.StartInfo.UseShellExecute = false;
				process.Start();
				process.WaitForExit();
				Errors = process.StandardError.ReadToEnd();
				Output = process.StandardOutput.ReadToEnd();
				if (process.ExitCode != 0)
				{
					string message = (Errors != String.Empty) ? Errors : Output;
					throw new InvalidOperationException("Premake aborted with code " + process.ExitCode + ": \n" + message);
				}


				/* Start building the results right here */
				Project actual = new Project();

				/* Locate the name of the just-written project file */
				MatchCollection matches = Regex.Matches(script.ToString(), "project.name = '(.+)'");
				actual.Name = matches[0].Groups[1].ToString();

				matches = Regex.Matches(script.ToString(), "project.path = '(.+)'");
				actual.Path = (matches.Count > 0) ? matches[0].Groups[1].ToString() : "";

				/* Parse the results */
				parser.Parse(actual, Path.Combine(actual.Path, actual.Name));

				/* SHARPDEV_DEPENDENCY_BUG: Dependencies have disappeared! */
#if SHARPDEV_DEPENDENCY_BUG
				foreach (Package p in actual.Package)
				{
					Console.WriteLine(p.Name + ":");
					foreach (Configuration config in p.Config)
					{
						Console.WriteLine("  " + config.Name + ": " + config.Dependencies.Length);
					}
				}
#endif
				expected.CompareTo(actual);
			}
			finally
			{
				Directory.SetCurrentDirectory(Path.GetDirectoryName(executable));
				Directory.Delete(temp, true);
				_files.Clear();
				_scripts.Clear();
			}
		}
	}
}
