#!/bin/sh

# Author: Francesco Montorsi
# Purpose: sample shell script to fetch your project HTML
#          (taken from http://sourceforge.net/docman/display_doc.php?docid=1502&group_id=1)
#          modified to generate XHTML code

# where the news.inc.php file will be put
destdir=/home/project-web/wxcode/htdocs

# get the exported HTML
/usr/bin/wget -q -O $destdir/news.inc.html.tmp 'http://sourceforge.net/export/projnews.php?group_id=51305&limit=4&flat=0&show_summaries=1'  > /dev/null

# set group_id in links
sed 's/\/forum\/forum.php?forum_id=0/\/news?group_id=51305/' $destdir/news.inc.html.tmp  > $destdir/news.inc.html


#
# NOTE: since 2006 (I don't know exactly the month/day) SF exports news directly in XHTML,
#       making the following stuff useless:
#
#     # convert from HTML to XHTML: i.e. convert uppercase tags to lowercase
#     # convert A tags
#     sed 's/<A HREF/<a href/g; s/<\/A>/<\/a>/g;' ./projhtml.tmp > ./projhtml2.tmp
#
#     # convert B tags to strong tags
#     sed 's/<B>/<strong>/g; s/<\/B>/<\/strong>/g;' ./projhtml2.tmp > ./projhtml.tmp
#
#     # convert I tags to em tags
#     sed 's/<I>/<em>/g; s/<\/I>/<\/em>/g;' ./projhtml.tmp > ./projhtml2.tmp
#
#     # convert BR and HR tags to br and hr
#     sed 's/<BR>/<br\/>/g; s/<HR/<hr/g; s/noshade>/\/>/g' ./projhtml2.tmp > ./projhtml.tmp
#
#     # remove the align="center", size="1", width="100%" attributes which are not XHTML compliant
#     sed 's/align="center"/class="news"/g; s/width="100%"//g; s/size="1"//g' ./projhtml.tmp > ./projhtml2.tmp
#
#     # cleanup
#     /bin/rm -f ./projhtml.tmp
#     /bin/mv -f ./projhtml2.tmp $destdir/news.inc.html
