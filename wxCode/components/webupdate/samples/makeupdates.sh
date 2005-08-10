#!/bin/bash 

# Author: Francesco Montorsi
# RCS-ID: $Id$
# Purpose: creates the ZIP and TAR.GZ archives used as updates for
#          the samples contained in this folder; then move the archives
#          in the ../website folder


# create the updates for the given program name ($1 = name, $2 = version)
function CreateUpdate { 
    cd $1/v$2
	zip -9 ../../$1-$2.zip $1.exe *.txt -x CVS
	tar -c -z --exclude=CVS -f ../../$1-$2.tar.gz $1.exe *.txt
	cd ../..
}

# remove old & create new packages
rm -f *.zip *.tar.gz
CreateUpdate simple 2.0.3
CreateUpdate advanced 1.5.0

# for the advanced sample, create also the "addon" packages
cd advanced/v1.5.0/addon1 && zip -9 ../../../addon1-1.5.0.zip * -i *.txt && cd ../../..
cd advanced/v1.5.0/addon2 && zip -9 ../../../addon2-1.5.0.zip * -i *.txt && cd ../../..
cd advanced/v1.5.0/addon3 && zip -9 ../../../addon3-1.5.0.zip * -i *.txt && cd ../../..

# move the packages
mv -f *.zip ../website
mv -f *.tar.gz ../website