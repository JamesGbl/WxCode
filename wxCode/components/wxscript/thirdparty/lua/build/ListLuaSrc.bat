@echo off
REM
REM Author: Francesco Montorsi (frm@users.sourceforge.net)
REM Date: 2/9/2004

REM Runs ObjList.bat with correct args


REM without using CALL, this batch file would end after the end of the first command...
CALL ObjList luasrc.bkl LUA ../src
CALL ObjList lualibsrc.bkl LUALIB ../src/lib
CALL ObjList luaintsrc.bkl LUAINT ../src/lua
CALL ObjList luacsrc.bkl LUAC ../src/luac

