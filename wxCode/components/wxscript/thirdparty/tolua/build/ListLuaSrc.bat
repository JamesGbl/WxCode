@echo off
REM
REM Author: Francesco Montorsi (frm@users.sourceforge.net)
REM Date: 2/9/2004

REM Runs ObjList.bat with correct args


REM without using CALL, this batch file would end after the end of the first command...
CALL ObjList tolualibsrc.bkl TOLUALIB ../src/lib
CALL ObjList toluaexesrc.bkl TOLUAEXE ../src/bin

