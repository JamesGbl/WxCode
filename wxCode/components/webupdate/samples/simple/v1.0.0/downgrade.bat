REM
REM Author: Francesco Montorsi
REM RCS-ID: $Id$
REM Purpose: this script should be used after testing the WebUpdater, and thus
REM          after having upgraded this simple sample from v1.0.0 to v2.0.3,
REM          to "downgrade" it back to version v1.0.0
REM          This is very useful to test the WebUpdater again...


copy /Y simple_1_0_0.xml simple.xml
copy /Y adv_1_0_0.xml adv.xml
copy /Y simpledata_1_0_0.txt simpledata.txt
