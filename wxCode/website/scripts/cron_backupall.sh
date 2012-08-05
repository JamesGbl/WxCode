#!/bin/sh

# Author: Francesco Montorsi
# RCS-ID: $Id: cron_backupall.sh,v 1.1 2009/01/05 14:39:11 frm Exp $
# Purpose: backup all the important things of wxCode project
#          (the website is kept in the CVS so there is no
#           need to backup it)


# CVS backup
# ==========

# really too slow (on SF it hangs forever) because of wxCode CVS tarball size (> 17 mb)

# where backup will be saved
# backupdir=/home/project-web/wxcode/htdocs/backup

# SF makes automatically the "nightly CVS tarballs"; we just
# need to link them in our backup dir
# today="$(date +%Y_%m_%d)"
# /usr/bin/rsync -av rsync://wxcode.cvs.sourceforge.net/cvsroot/wxcode/* $backupdir/wxcodecvs.$today.tar.gz


# wxCode DB backup
# ================

( cd /home/project-web/wxcode/htdocs/scripts && ./helper_backupdb.sh )


# Wiki backup
# ===========

# where the wiki files are stored
# wikidir=/tmp/persistent/wxcode/pmwiki.d/wiki.d
#
# where backup will be saved
# backupdir=/home/project-web/wxcode/htdocs/backup
#
# save the wiki pages
# today="$(date +%Y_%m_%d)"
# ( cd $wikidir && tar -czf $backupdir/wxcodewiki.$today.tar.gz * )
#
# set permissions for the backups
# chmod o=r $backupdir/*      # others can read only
# chmod g=r $backupdir/*      # group can read only
# chmod u=rw $backupdir/*     # user can read and also remove backups



# Old backup removal
# ==================

olds=`find /home/project-web/wxcode/htdocs/backup -type f -mtime +30 -name "*.gz" 2>/dev/null`

# NB: don't check the $? var as find will sometimes set it to != 0 just because
#     e.g. it's unable to walk into a directory...
rm -f $olds
