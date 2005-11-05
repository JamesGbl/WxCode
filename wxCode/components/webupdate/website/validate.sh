#!/bin/sh
#
# Author: Francesco Montorsi
# RCS-ID: $Id$
# validate.sh - uses 'xmllint' to validate all XML files found.
#
# where to download XMLLINT ? 
# ----------------------------
# go to ftp://xmlsoft.org/win32/ and then download the following files
# (they are all required since they contain some indispensable DLL files):
# - zlib-X.X.X.win32.zip
# - libxml2-X.X.X.win32.zip
# the last one contains the XMLLINT file...


echo "If you do not see any message below, then all XML files are valid ;-)"
for f in $(ls *.xml) do;
   xmllint --noout --valid $f
done
