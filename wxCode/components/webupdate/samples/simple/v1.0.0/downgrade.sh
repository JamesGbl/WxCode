#!/bin/sh
# Author: Francesco Montorsi
# RCS-ID: $Id$
# Purpose: this script should be used after testing the WebUpdater, and thus
#          after having upgraded this simple sample from v1.0.0 to v2.0.3,
#          to "downgrade" it back to version v1.0.0
#          This is very useful to test the WebUpdater again...


cp -f local_1_0_0.xml local.xml
cp -f simpledata_1_0_0.txt simpledata.txt
