using System;
using System.Collections;

namespace Premake.Tests.Framework
{
	public class PackageCollection : CollectionBase
	{
		public void Add(Package package)
		{
			List.Add(package);
		}

		public void Add(int count)
		{
			for (int i = 0; i < count; ++i)
				List.Add(new Package());
		}

		public Package this[int index]
		{
			get { return (Package)List[index]; }
			set { List[index] = value; }
		}

		public void CompareTo(PackageCollection actual)
		{
			if (this.Count != actual.Count)
				throw new FormatException("Expected " + this.Count + " packages but got " + actual.Count);

			for (int i = 0; i < this.Count; ++i)
				this[i].CompareTo(actual[i]);
		}
	}
}
