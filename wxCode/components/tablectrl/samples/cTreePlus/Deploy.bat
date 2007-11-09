@echo off
echo Configuration %1
if %1 == Debug goto Debug
if %1 == Release goto Release
echo "Error: Specify configuration [Debug|Release]"
goto ok
exit -1

:Debug
echo "%FAIRCOM_ROOT%\lib.mfp\ctreestdd.dll"
copy "%FAIRCOM_ROOT%\lib.mfp\ctreestdd.dll" %1
goto ok

:Release
echo "%FAIRCOM_ROOT%\lib.mfp\ctreestd.dll"
copy "%FAIRCOM_ROOT%\lib.mfp\ctreestd.dll" %1
goto ok
:ok
