// Script2.uc - a sample C++ script to load with UnderC
//////////////////////////////////////////////////////////


// import all caller's functions
#pragma dlink test.dll
extern class myClass;
#pragma dlink

int uc_func1(const char *str, int n)
{
	int i, res;
	for (i=0; i < n; i++)
		res += str[i];
	return res;
}

bool uc_func2(bool input)
{
	return !input;
}
