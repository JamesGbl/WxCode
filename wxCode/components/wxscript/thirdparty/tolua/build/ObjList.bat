@echo off
REM
REM Author: Francesco Montorsi (frm@users.sourceforge.net)
REM Date: 2/9/2004

REM This batch file runs only under WinXP.
REM Usage:
REM
REM    ObjList Outputfile.bkl BakefileVarBasename DirectoryToScan
REM



setlocal


REM Prepare to run
set OUTPUT=%CD%\%1
set BASENAME=%2
set DIR=%3
IF EXIST %OUTPUT% del /f %OUTPUT%


REM Let the user know what we are doing
echo.
echo Output file is: %OUTPUT%, the bakefile variable basename is: %BASENAME% and the dir is: %DIR%
echo.


REM Begin the BKL file
echo ^<?xml version="1.0" ?^>     > %OUTPUT%
echo ^<makefile^>                 >> %OUTPUT%
echo ^<set var="%BASENAME%_SRC"^>            >> %OUTPUT%
echo. >> %OUTPUT%


REM **********************************************************************
REM this piece of script requires
REM   DIR - The directory to pass to CD where we must list all CPP files
REM   OUTPUT - The output file where all the listed files will be added
CD %DIR%
FOR %%F IN (*.c*) DO @echo %DIR%/%%~nxF >> %OUTPUT%
REM **********************************************************************


REM Complete our work with the last lines
echo. >> %OUTPUT%
echo ^</set^>                    >> %OUTPUT%
echo ^</makefile^>               >> %OUTPUT%

REM end of the script

