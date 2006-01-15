#!/bin/sh
#############################################################################
# Name:        arctest.sh
# Purpose:     Test script for wxArchive classes
# Author:      Mike Wetherell
# RCS-ID:      $Id: arctest.sh,v 1.5 2006-01-15 17:41:47 mweth Exp $
# Copyright:   (c) 2004 Mike Wetherell
# Licence:     wxWindows licence
#############################################################################


# configure
#
arc=arc                 # the name of the arc program
arcpath=$PATH:.         # places to look for arc
tmpdir=arctest.tmp      # temp files (relative path)


# process the command line
# 
prog=`basename $0`

usage() {
    cat <<EOT
Usage: $prog [OPTIONS]
Tests for wxArchive classes
Options:
    -n, --no-clean  don't clean away temporary files on failure 
    -h, --help      display this help and exit
EOT
    exit $1
}

error() {
    echo "$prog: $1" 1>&2
    usage 1 1>&2
}

while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
            usage 0
            ;;
        -n|--no-clean)
            clean=no
            ;;
        *)
            error "unknown option '$1'"
            ;;
    esac
    shift
done


# set up the context used by the remaining functions below
#
starttest() {
    # the context
    archiver=$1                 # empty (self-test), or external archiver
    type=$2                     # e.g. 'zip' or 'tar'
    testdir=$tmpdir/test
    archive=$testdir.$type
}


# make some test data
#
maketestdata() {
    # make test data
    mkdir $tmpdir || exit
    trap fail_handler 0
    mkdir $testdir || exit

    mkdir $testdir/text || exit
    zerofile 0 0 $testdir/text/empty
    date > $testdir/text/small || exit
    cp $0 $testdir/text/script || exit

    mkdir $testdir/bin || exit
    binfile 0 $testdir/bin/bin0
    binfile 1000 $testdir/bin/bin1
    binfile 4095 $testdir/bin/bin2
    binfile 4096 $testdir/bin/bin3
    binfile 4097 $testdir/bin/bin4
    binfile 16384 $testdir/bin/bin5

    mkdir $testdir/zero || exit
    zerofile 5 0 $testdir/zero/zero1
    zerofile 1024 32 $testdir/zero/zero2
    zerofile 32768 77 $testdir/zero/zero3
    zerofile 16385 87 $testdir/zero/zero4

    find $testdir > $tmpdir/filelist
}


# clean up the temporary files
#
cleanup() {
    trap 0
    rm -rf $tmpdir
}


# handle failed test
#
fail_handler() {
    if [ "$clean" != "no" ]; then
        cleanup
    else
        echo "$prog: leaving temporary directory '$tmpdir' for post mortem"
    fi
    echo "**** FAIL *****"
}


# make a test binary file
#
binfile() {
    echo | awk "{ for (i = 0; i < $1; i++) printf \"%c\", rand()*256 }" |
        (if [ $2 = - ]; then cat; else cat > $2; fi) || exit
}


# make a test file with all bytes the same
#
zerofile() {
    echo | awk "{ for (i = 0; i < $1; i++) printf \"%c\", $2 }" > $3 || exit
}


# a wrapper around 'arc' which pipes the input/output archive if $mode = pipe
# or otherwise just calls it 'normally'
#
runarc() {
    cmd=$1
    filename=$2
    shift 2

    if [ "$mode" != "pipe" ]; then
        PATH=$arcpath $arc $cmd $filename $*
    elif [ $cmd = list -o $cmd = extract ]; then
        cat $filename | PATH=$arcpath $arc $cmd $type $*
    else
        (if [ -f $filename ]; then cat $filename; fi) |
            (PATH=$arcpath $arc $cmd $type $*; echo $? > $filename.err) |
            cat > $filename.tmp
        test `cat $filename.err` -eq 0 && mv $filename.tmp $filename
    fi

    test $? -eq 0 || exit
}


# modify the zip and shadow the changes in the temp files
#
domodify() {
    # rename an entry
    oldname=`entryname 'script|SCRIPT'`
    newname=`echo $oldname | sed 's/script/newname/;s/SCRIPT/NEWNAME/'`
    runarc rename $archive $oldname $newname
    oldname=`find $testdir -name script -o -name SCRIPT`
    newname=`echo $oldname | sed 's/script/newname/;s/SCRIPT/NEWNAME/'`
    mv $oldname $newname || exit
    echo

    # remove an entry
    removename=`entryname 'bin4|BIN4'`
    runarc remove $archive $removename
    rm `find $testdir -name bin4 -o -name BIN4` || exit
    echo

    # write a test file and try adding that
    addname=$testdir/newfile
    echo "New file added as a test" > $addname || exit
    runarc add $archive $addname
    echo
}


# get full name of an entry
#
entryname() {
    runarc list $archive | awk "/$1/ { print \$4 }"
}


# extract
#
doextract() {
    mv $testdir $testdir.bak || exit
    runarc extract $archive
    echo
}


# verify the extracted files against the temp files
# todo: verify timestamps
#
doverify() {
    echo verify...

    find $testdir | tr [A-Z] [a-z] | sort > $tmpdir/filelist.out
    find $testdir.bak | sed 's/\.bak//' | tr [A-Z] [a-z] |
        sort > $tmpdir/filelist.bak

    diff $tmpdir/filelist.out $tmpdir/filelist.bak || exit

    for file in `cat $tmpdir/filelist.out`
    do
        test -d $file ||
            cmp $file `echo $file | sed "s|$testdir|$testdir.bak|"` || exit
    done

    echo success
    echo
}


# create an archive modify it and check it
#
selftest() {
    starttest "" $1
    unarchivertest runarc extract $archive
}


# check arc can extract a zip appended to a stub
#
appendedziptest() {
    starttest "" zip
    mode=normal
    echo "---- Extract zip appended to a stub ----"
    maketestdata

    runarc create $archive $testdir
    echo

    echo append to 4k stub
    binfile 4096 - | cat - $archive > $archive.tmp || exit
    mv $archive.tmp $archive || exit
    echo

    doextract
    doverify
    cleanup
}


# check if an external archiver/unarchiver is in the path
#
have() {
    LOCATION=`which $1` && test -x "$LOCATION" && starttest $*
}


# test against an external archiver
#
archivertest() {
    for mode in normal pipe
    do
        echo "---- Compatability test against '$archiver' (Format: $type, Mode: $mode) ----"
        maketestdata

        echo "$*"
        $* || exit
        cp $archive $archive.bak || exit
        echo

        # rename an entry
        oldname=`entryname 'zero3|ZERO3'`
        newname=`echo $oldname | sed 's/zero3/newname/;s/ZERO3/NEWNAME/'`
        runarc rename $archive $oldname $newname
        echo

        # rename it back again
        runarc rename $archive $newname $oldname
        echo

        # compare with the original zip
        if [ "$type" = "zip" ]; then
            echo check full meta-data preserved...
            cmp $archive.bak $archive || exit
            echo yes
            echo
        fi

        domodify
        doextract
        doverify
        cleanup
    done
}


# test against an external unarchiver
# 
unarchivertest() {
    for mode in normal pipe
    do
        if [ "$archiver" = "" ]; then
            echo "---- Self test (Format: $type, Mode: $mode) ----"
        else
            echo "---- Compatability test against '$archiver' (Format: $type, Mode: $mode) ----"
        fi
        maketestdata

        runarc create $archive $testdir
        cp $archive $archive.bak
        echo

        domodify

        mv $testdir $testdir.bak || exit
        test "$1" != "" && echo "$*"
        $* || exit
        echo

        doverify
        cleanup
    done
}


# zip self tests
selftest zip
appendedziptest

## check compatibilty with other zippers
have zip zip            && archivertest   zip -r $archive $testdir
have unzip zip          && unarchivertest unzip $archive

have pkzip zip          && archivertest   pkzip -P $archive \@$tmpdir/filelist
have pkunzip zip        && unarchivertest pkunzip -d $archive

# tar self tests
selftest tar

# compatibilty with tar and pax
have tar tar            && archivertest   tar cvf $archive $testdir
have tar tar            && unarchivertest tar xvf $archive

have pax tar            && archivertest   pax -wf $archive $testdir
have pax tar            && unarchivertest pax -rf $archive

echo "**** SUCCESS *****"
