#!/bin/sh

# Author: Otto Wyss, Francesco Montorsi
# RCS-ID: $Id: maint_updatesite.sh,v 1.3 2009/01/06 17:52:08 frm Exp $
# Purpose: synchronizes the site in the CVS repository with the
#          site which is actually accessible from WWW; also
#          updates all the websites of the single components.

trap 'exit 1' SIGINT
trap 'exit 1' SIGQUIT

#export CVSROOT=:pserver:anonymous@wxcode.cvs:/cvsroot/wxcode

function restorepermissions
{
    # after all the updates, restore the permissions for the GROUP
    echo "Restoring permissions..."
    cd /home/project-web/wxcode/htdocs

    # note: the user permissions must be set, too otherwise the owner
    #       won't be able to run the files. To suppress errors which
    #       users != owner would get trying to run this script
    #       we redirect everything to /dev/null
    chmod -R ug+rw *  >/dev/null 2>&1
    chmod ug+rwx scripts/*.sh >/dev/null 2>&1
}


# little fix for a dummy error msg that otherwise occurs from time to time
# (even if it's not clear to me why it does)
#chmod a+x /home/project-web/wxcode/htdocs/backup/CVS

# update the global site
echo "Updating wxCode website..."
cd /home/project-web/wxcode/htdocs
svn -q update

# if user called this with a non-empty argument, limit the
# update to the main website only
if [ ! -z "$1" ]; then
    echo "Not updating component websites as per user request."
    restorepermissions
    exit 0
fi

# update project news
cd /home/project-web/wxcode/htdocs/scripts
./helper_exportnews.sh

# update single component websites
./helper_updatecompwebsites.sh

restorepermissions
