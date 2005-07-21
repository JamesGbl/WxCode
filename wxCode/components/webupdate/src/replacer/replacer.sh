#!/bin/sh

# Name:        replacer.sh
# Purpose:     takes three arguments:
#              - from = an existing non-empty folder
#              - to = an existing folder
#              - pid = the process ID of some other program
#              This script waits for the process with the given PID to terminate.
#              Then it copies all the files of the "from" folder in the "to" folder,
#              overwriting eventually present old files
# Author:      Francesco Montorsi <frm@users.sourceforge.net>
# Created:     2005/08/21
# RCS-ID:      $Id$
# Copyright:   (c) 2005 Francesco Montorsi
# Licence:     wxWidgets licence


#
# ARGUMENT CHECKING
###########################################

function PrintUsage {
	echo 1>&2 Invalid argument count !
	echo 1>&2 Usage: $0  --pid 123  --from /tmp/tmpdir  --to /usr/local/bin
	echo 1>&2        The arguments must be in the specified order and must be space-separed.
	echo 1>&2        See the script source for more info.
}

if [ $# -ne 6 ]; then		# we need exactly 3 arguments (+3 --option-names) !
    PrintUsage $0
	exit 1
fi

if [ "$1" -ne "--pid" ] || [ "$3" -ne "--from" ] || [ "$5" -ne "--to" ]; then
	PrintUsage $0
	exit 1
fi

$pid = $2
$from = $4
$to = $6



#
# WAIT FOR THE PROCESS TO TERMINATE
###########################################

wait $pid



#
# PROCEED WITH THE COPY
###########################################

cp -rf $from $to
exit 0		# success !
