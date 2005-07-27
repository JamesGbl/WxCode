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


#if defined( _WIN32 ) || defined( WIN32 )

	#include <windows.h>
	#define THISPROGNAME		"replacer"
	#define TIMEOUT				15000

	void ShowMsg(const char *msg)
	{
		MessageBox(NULL, msg, "Replacer", MB_OK);
	}

	void PrintUsage()
	{
		char msg[1024];
		sprintf(msg, "Usage: %s --pid 123  --from c:\\temp  --to c:\\programs\\myprogram "
					"[--restart c:\\programfolder\\program.exe]\n", THISPROGNAME);
		strcat(msg, "\tThe arguments must be in the specified order and must be space-separed.\n");
		strcat(msg, "\tSee the program source for more info.\n");

		ShowMsg(msg);
	}

	int APIENTRY WinMain(HINSTANCE hInstance,
						 HINSTANCE hPrevInstance,
						 LPSTR     lpCmdLine,
						 int       nCmdShow)
	{
		char from[1024], to[1024], cmd[1024], restartexe[1024], *tmp;		
		HANDLE hProcess;
		DWORD res, pid;
		int restart;

		// ugly but working way to parse the command line...
		if (sscanf(lpCmdLine, "--pid %i --from %s --to %s", &pid, &from, &to) != 3) {
			PrintUsage();
			return 1;
		}

		// the restart option is optional
		tmp = strstr(lpCmdLine, "--restart");
		if (tmp)
			restart = (sscanf(tmp, "--restart %s", restartexe) == 1);
		else
			restart = 0;

		// check if the given PID is valid
		hProcess = OpenProcess(SYNCHRONIZE |
								PROCESS_TERMINATE |
								PROCESS_QUERY_INFORMATION,
								FALSE, // not inheritable
								(DWORD)pid);
		if (hProcess == NULL) {
			ShowMsg("Error: cannot find such Process ID !\n");
			return -1;
		}

		// we'll wait for max TIMEOUT millisecs
		res = WaitForSingleObject(hProcess, TIMEOUT);	
		CloseHandle(hProcess);

		if (res == WAIT_TIMEOUT) {
			char tmp[128];
			sprintf(tmp, "Error: I waited for %d seconds and that process did not terminate !\n", TIMEOUT/1000);
			ShowMsg(tmp);
			return -1;
		}

		// exec the command
		strcpy(cmd, "copy /Y ");
		strcat(cmd, from);
		strcat(cmd, " ");
		strcat(cmd, to);

		// avoid the system() command which shows an ugly DOS window
		ShellExecute(NULL, NULL, cmd, NULL, NULL, SW_HIDE);

		// do we have to restart our app ?
		if (restart)
			ShellExecute(NULL, "open", restartexe, NULL, NULL, SW_SHOWNORMAL);

		return 1;
	}


#else		// not defined( _WIN32 ) || defined( WIN32 )

	/* NOTE: on unix/linux we can create a console process because wxExecute won't
	         show an ugly prompt window when we call it... (while it does on win32)
			 note also that we rely on the GNU libc's waitpid function
    */


	void PrintUsage()
	{
		printf("Usage: %s --pid 123  --from /usr/temp  --to /usr/bin/myprogram  [--restart /usr/bin/myapp]\n", THISPROGNAME);
		printf("       The arguments must be in the specified order and must be space-separed.\n");
		printf("       See the program source for more info.\n");
	}

	int main(int argc, char **argv)
	{
		char from[256], to[256], restartexe[1024], cmd[1024];
		long i, pid, restart=0;


		// ARGUMENT CHECKING
		if (argc < 7) {
			PrintUsage();
			return 1;
		}
		
		for (i=0; i <  argc; i++)
			printf("%s\n", argv[i]);
		if (strcmp(argv[1], "--pid") != 0 ||
			strcmp(argv[3], "--from") != 0 ||
			strcmp(argv[5], "--to") != 0) {
			PrintUsage();
			return 1;
		}

		if (argc == 9 && strcmp(argv[7], "--restart") == 0) {
			strcpy(restartexe, argv[8]);
			restart = 1;
		}

		pid = atoi(argv[2]);
		strcpy(from, argv[4]);
		strcpy(to, argv[6]);
		
		
		// WAIT FOR THE PROCESS TO TERMINATE
		while (kill(pid, 0) == 0)
			sleep(1);


		// PROCEED WITH THE COPY
		strcpy(cmd, "cp -rf ");
		strcat(cmd, argv[4]);
		strcat(cmd, " ");
		strcat(cmd, argv[6]);
		system(cmd);


		// RESTART IF REQUIRED
		if (restart)
			system(restartexe);
		
		return 0;
	}

#endif		// defined( _WIN32 ) || defined( WIN32 )