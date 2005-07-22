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


#define THISPROGNAME		"replacer"
#define TIMEOUT				15000

#if defined( _WIN32 ) || defined( WIN32 )
#include <windows.h>
HANDLE hProcess;
DWORD res;
#endif


void PrintUsage()
{
	printf("Usage: %s --pid 123  --from c:\\temp  --to c:\\programs\\myprogram\n", THISPROGNAME);
	printf("       The arguments must be in the specified order and must be space-separed.\n");
	printf("       See the program source for more info.\n");
}

int main(int argc, char **argv)
{
	char from[256], to[256], cmd[1024];
	long pid;

	// ARGUMENT CHECKING
	if (argc != 7) {
		PrintUsage();
		return 1;
	}
	
	if (strcmp(argv[1], "--pid") != 0 ||
		strcmp(argv[3], "--from") != 0 ||
		strcmp(argv[5], "--to") != 0) {
		PrintUsage();
		return 1;
	}

	pid = atoi(argv[2]);
	strcpy(from, argv[4]);
	strcpy(to, argv[6]);
	
	
	// WAIT FOR THE PROCESS TO TERMINATE
#if defined( _WIN32 ) || defined( WIN32 )

    hProcess = OpenProcess(SYNCHRONIZE |
                                    PROCESS_TERMINATE |
                                    PROCESS_QUERY_INFORMATION,
                                    FALSE, // not inheritable
                                    (DWORD)pid);
    if (hProcess == NULL) {
		printf("Error: cannot find such Process ID !\n");
        return -1;
	}

	res = WaitForSingleObject(hProcess, TIMEOUT);	// we'll wait for max TIMEOUT millisecs
    CloseHandle(hProcess);

	if (res == WAIT_TIMEOUT) {
		printf("Error: I waited for %d seconds and that process did not terminate !\n", TIMEOUT/1000);
		return -1;
	}

#else
	// we need some kind of wait() function
	wait(NULL, argv[2], NULL);
#endif


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
