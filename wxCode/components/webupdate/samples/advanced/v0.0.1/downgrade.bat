REM
REM Author: Francesco Montorsi
REM RCS-ID: $Id$
REM Purpose: this script should be used after testing the WebUpdater, and thus
REM          after having upgraded this ADVANCED sample from v0.0.1 to v1.5.0,
REM          to "downgrade" it back to version v0.0.1
REM          This is very useful to test the WebUpdater again...


copy /Y online_0_0_1.xml online.xml
copy /Y local_0_0_1.xml local.xml
copy /Y simpledata_0_0_1.txt simpledata.txt

REM remove addons
rmdir /S /Q addon1
rmdir /S /Q addon2
rmdir /S /Q addon3