@echo off
REM
REM Author: Francesco Montorsi
REM RCS-ID: $Id$
REM validate.bat - uses 'xmllint' to validate all XML files found.
REM
REM where to download XMLLINT ? 
REM ----------------------------
REM go to ftp://xmlsoft.org/win32/ and then download the following files
REM (they are all required since they contain some indispensable DLL files):
REM - zlib-X.X.X.win32.zip
REM - libxml2-X.X.X.win32.zip
REM the last one contains the XMLLINT file...


echo If you do not see any message below, then all XML files are valid ;-)
FOR %%F IN (*.xml) DO xmllint --noout --valid %%F
pause