using System;
using System.Collections;

namespace Premake.Tests.Framework
{
	public class ConfigCollection : CollectionBase
	{
		public void Add(Configuration config)
		{
			List.Add(config);
		}

		public void Add(int count)
		{
			for (int i = 0; i < count; ++i)
				List.Add(new Configuration());
		}

		public void Add(Project project)
		{
			foreach (string name in project.Configuration)
			{
				Configuration config = new Configuration();
				config.Name = name;
				List.Add(config);
			}
		}

		public Configuration this[int index]
		{
			get { return (Configuration)List[index]; }
			set { List[index] = value; }
		}

		public void CompareTo(ConfigCollection actual)
		{
			if (this.Count == 0)
				return;

			if (this.Count != actual.Count)
				throw new FormatException("Expected " + this.Count + " configurations but got " + actual.Count);

			for (int i = 0; i < this.Count; ++i)
				this[i].CompareTo(actual[i]);
		}
	}
}
