@echo off
REM
REM Author: Francesco Montorsi (frm@users.sourceforge.net)
REM Date: 2/9/2004

REM Regen all include file lists and makefiles


CALL ListLuaSrc
echo.
echo.
bakefile_gen
