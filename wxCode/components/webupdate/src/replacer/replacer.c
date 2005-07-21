/////////////////////////////////////////////////////////////////////////////
// Name:        replacer.c
// Purpose:     replaces files 
// Author:      Francesco Montorsi
// Created:     2005/08/21
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void PrintUsage(char *thisname)
{
	printf("Usage: %s --pid 123  --from c:\\temp  --to c:\\programs\\myprogram", thisname);
	printf("       The arguments must be in the specified order and must be space-separed.");
	printf("       See the program source for more info.");
}

int main(char **argv, int argc)
{
	char cmd[256];

	// ARGUMENT CHECKING
	if (argc != 6) {
		PrintUsage(argv[0]);
		return 1;
	}
	
	if (strcmp(argv[1], "pid") != 0 ||
		strcmp(argv[1], "from") != 0 ||
		strcmp(argv[1], "to") != 0) {
		PrintUsage(argv[0]);
		return 1;
	}
	
	
	// WAIT FOR THE PROCESS TO TERMINATE
	_cwait(NULL, argv[2], NULL);
	

	// PROCEED WITH THE COPY
#if defined( _WIN32 ) || defined( WIN32 )
	strcpy(cmd, "copy /Y ");
#else
	strcpy(cmd, "cp -rf ");
#endif
	strcat(cmd, argv[4]);
	strcat(cmd, " ");
	strcat(cmd, argv[6]);

	system(cmd);
	return 0;
}
