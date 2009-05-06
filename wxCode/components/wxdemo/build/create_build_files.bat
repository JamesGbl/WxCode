@echo off

rem ==========Code::Blocks project files ==========
rem premake\premake-win32.exe --target cb-gcc --unicode --use-wx-static
premake\premake-win32.exe --target cb-gcc --unicode --use-wx-static
echo done...
echo.

rem ========== GNU Makefile ==========
rem premake\premake-win32.exe --target gnu --unicode --use-wx-static
premake\premake-win32.exe --target gnu --unicode --use-wx-static
echo done...
echo.

rem ========== MS Visual Studio 2008 project files ==========
rem premake\premake-win32.exe --target vs2008 --unicode --use-wx-static
premake\premake-win32.exe --target vs2008 --unicode --use-wx-static
echo done...
echo.

echo Done generating all project files