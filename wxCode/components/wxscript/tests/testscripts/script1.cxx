// Script1.cxx - a sample C++ script which can be loaded
//               through the UnderC and CINT engines...
//////////////////////////////////////////////////////////


int cint_func1(const char *str, int n)
{
	int i, res;
	for (i=0; i < n; i++)
		res += str[i];
	return res;
}

bool cint_func2(bool input)
{
	return !input;
}

