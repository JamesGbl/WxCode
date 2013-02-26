@echo off

rem ========== Code::Blocks project files ==========
rem premake\premake3.exe --target cb-gcc --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake3.exe --target cb-gcc --wx-version 28 --unicode --static-runtime
rem echo done...
rem echo.

rem ==========CodeLite project files ==========
premake\premake3.exe --target cl-gcc --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake3.exe --target cl-gcc --wx-version 28 --unicode --static-runtime
echo done...
echo.

rem ========== GNU Makefile ==========
premake\premake3.exe --target gnu --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake3.exe --target gnu --wx-version 28 --unicode --static-runtime
echo done...
echo.

rem ========== MS Visual Studio 2005/2008 project files ==========
rem premake\premake3.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared --no-builtin-wchar
rem premake\premake3.exe --target vs2005 --wx-version 28 --unicode --with-wx-shared --shared
rem premake\premake3.exe --target vs2005 --wx-version 28 --unicode --static-runtime --no-builtin-wchar
rem premake\premake3.exe --target vs2005 --wx-version 28 --unicode --static-runtime
echo done...
echo.
