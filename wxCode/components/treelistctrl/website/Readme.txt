Latest information:
===================

* 2012-08-01 : new release 1208 (support for wxWidgets 2.9, minor improvements)
* 2011-04-04 : new release 1104 (eye candies: colors, fonts & more!)
* 2010-06-26 : new release (tooltips support, page-up/down, minor changes & bug fixes)
* 2009-07-19 : new release (maintenance release)
* 2008-12-05 : new release (fixed several crashes)
* 2008-07-21 : new release (important fixes & enhancements to: events, selection, delete)
* 2008-06-18 : new release (minor changes only)
* 2008-05-26 : change of maintainer from Otto Wyss to Ronan Chartois

(see release details further down)


Known problems:
===============

- end of drag triggers a menu event (wrong) on the item at the new position (even more wrong)


Release History:
================
* treelistctrl_1208.zip
  - Fixed: HitTest() was not working [#3093531 --thanks Yogesh]
  - Fixed: click events now return position (Point) in all cases
  - Fixed: include item in event EVT_COMMAND_TREE_ITEM_MENU [#3522228 --thanks T. Modes]
  - Fixed: take hidden columns into account for total width [#3522074 --thanks T. Modes]
  - Changed: by default FindItem() searches all columns (instead of main) [#3246444]
  - Added: resource files / XRC support [#3103099 --thanks Pascal Potvin]
  - Added: sample program now shows HitTest() output on middle-click
  - Added: can now change parent with SetItemParent(), sample demonstrates this with drag&drop [feature#3429784]
  - Added: support for wxWidgets 2.9 with separate 'wxCode' namespace

* treelistctrl_1104.zip
  - Fixed: position of editor when a column is hidden [wxPack]
  - Fixed: extra check to prevent painting of zero-width columns under GTK [#3061215]
  - Fixed: improved computing of BestSize, non-expanded controls in a sizer keep their initial size [#3061207]
  - Fixed: Edit control does not move with columns (stop editing on column move) [#2815443]
  - Changed: background color is applied to entire column width. Same for cursor and selection highlight excepted for main col [#2823804]
  - Added: make scroll events accessible to child class [#3082693]
  - Added: test program includes demonstrates capturing mouse events in child class (compile option WITH_CHILD_CLASS) [#3082693]
  - Added: SetCurrentItem() allows to set/reset the current item (cursor indicated by the black rectangle) [Remigius]
  - Added: EndEdit() to match EditLabel() [#2815443]
  - Added: multi-column support for various attribute (colors, font, ...) [#2823804 --thanks Chris Klecker]
  - Added: extented Sort() method [--thanks Chris Klecker]
  - Added: test program now has a 'Item' and 'Cell' menu to better play with attributes
  NOTE for DLL builders: some methods were removed or changed

* treelistctrl_2010-06.26.tar.gz
  - Fixed: improved support for GCC 4.x & compiling under Linux/GTK [#2858731]
  - Fixed: crash on start with GTK (call to SetBackgroundStyle)
  - Fixed: Unicode support [#2911015]
  - Fixed: spurious warning, unicode support with VC8.0 [#2962879]
  - Fixed: keep multiple selection when right-clicking on any selected (included last selected) item
  - Fixed: rectangle on non-current item after multiple unselection
  - Fixed: explicit conversions around size_t
  - Changed: made GetHeaderWindow() and GetMainWindow() protected (useless outside treelistctrl.cpp)
  - Changed: IsVisible now with 'within' option, and will only return true for fully visible items
  - Added: test program now reports selection content
  - Added: tooltip support, also for individual items [feature#2858734]
  - Added: support for page-up/page-down, improved 'end' key [feature#2989510]

* treelistctrl_2009-07-19.tar.gz
  - Fixed: GetPrev() not working at all [#2691182]
  - Changed: reduced flickering [feature #2460180]

* treelistctrl_2008-12-05.tar.gz
  - Fixed: non-compliant declarations in class wxTreeListMainWindow [#2051640]
  - Fixed: edit changes commited to wrong column [#2342061]
  - Fixed: crash after rename / cell edit [#2342054]
  - Fixed: crash on deletion of selected item (seen with multiple selection enabled on shutdown); bug introduced in v2008-07-21
  - Fixed: empty item text (in main & other columns)
  - Fixed: Drag from treelistctrl to another works improperly (drag origin not remembered accurately) [#1811335]
  - Fixed: allow multiple selection in right-click & dragging [#1488235]
  - Fixed: assertion in DoNotifyWindowAboutCaptureLost in wxMSW 2.7.1 [#1583135]
  - Changed: better positioning and size of editing cell

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
