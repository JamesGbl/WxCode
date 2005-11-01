name: wxRSSReader
author: Ryan Norton
wxversion: 2.6,cvs
category: application
language: cpp
description: This is a rather simple example of an RSS reader application with wxWidgets,
geared towards reading the RSS stream from the wxForum.

To use this application, simply select Load wxForum Feed from the file menu
to load the RSS feed of the wxForum (wxforum.org). Select Load Custom
Feed to load a feed from any site you want.

The program will put the site in the left list control and the posts
it recieved from the currently selected feed from the left list
control in the right list control. Single-click on a feed in the left
list control to see the posts in it and double-click to update the
posts. Double-click a post in the right list control to see a preview
of it in the lower-right window.
