Latest information:
===================

* 2008-07-21 : new release (important fixes & enhancements to: events, selection, delete --upgrade recommended)
* 2008-06-18 : new release (minor changes only)
* 2008-05-26 : change of maintainer from Otto Wyss to Ronan Chartois



Release History:
================

* treelistctrl_2008-07-21.tar.gz
  - Fixed: selection behavior
  - Fixed: make all mouse events accessible to child class [#2018407]
  - Fixed: column now always populated for mouse events [#2018407]
  - Fixed: crash on key navigation after item delete [#2021908]
  - Fixed: crash on edit after item delete [#2021908]
  - Fixed: missing event EVT_TREE_END_LABEL_EDIT when cancelled
  - Fixed: several crash possibilities when there is no root [#2021908]
  - Changed: changing the selection mode (wxTR_MULTIPLE style) will now reset the selection
  - Changed: id of main column is always returned in events (used to be -1 on blank area)
  - Changed: after an item is deleted, its next siblings is selected if it exists
  - Changed: key actions: if no item is selected, root is selected and no further action is taken
  - Changed: use same logic for double-click and return key (i.e. activate event will expand by default)
  - Added: events EVT_TREE_ITEM_MIDDLE_CLICK, EVT_TREE_ITEM_MENU
  - Added: events are also generated when empty area is clicked (with wxTreeEvent::IsOk() returning false) [#2018407]
  - Added: test program now with log window showing events fired
  - Added: test program includes compile option WITH_CHILD_CLASS demonstrating capturing mouse events in child class [#2018407]
  - Doc: added ommited (EVT_TREE_END_DRAG,EVT_TREE_ITEM_RIGHT_CLICK) and new events [#2018407]
  - Doc: mention wxTreeEvent::GetInt() to obtain column [#2018407]
  - Doc: corrected incorrect mention of no event on some delete actions

* treelistctrl_2008-06-18.tar.gz
  - Fixed: dead zone in labels with big fonts
  - Fixed: improved text/frame proportion in label row for big fonts (MSW)
  - Added: support for MinGW+MSYS through Makefile
  - Added: support for CodeBlock+GCC through project file

* treelistctrl_2006-06-21.tar.gz
  - first available release on sf.net



Known problems:
===============

None
