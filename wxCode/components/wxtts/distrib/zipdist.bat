@echo off
set src=n:\projects\wxtts
set dest=%src\deliver
rem Zip up an external source distribution of wxTTS
if "%src" == "" goto usage
if "%dest" == "" goto usage
echo About to archive an external wxTTS distribution:
echo   From   %src
echo   To     %dest\wxtts.zip
echo CTRL-C if this is not correct.
inkey /W4 `Press any key to continue...` %%input

erase %dest\wxtts.zip
cd %src
zip32 -@ %dest\wxtts.zip < %src\distrib\wxtts.rsp


echo wxTTS archived.
goto end

:usage
echo DOS wxTTS distribution.
echo Usage: zipdist

:end


