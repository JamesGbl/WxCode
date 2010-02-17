 
 wxAdvTable ReadMe
 -------------
 
 Website: http://wxcode.sourceforge.net/components/advtable
 Author: Moskvichev Andrey V. (email: mab [at] smtp [dot] ru)
 Version: 1.5 
 Description:
 wxAdvTable is advanced table component designed with flexibility in mind.
 It has many advanced features like: 
  - composite rows/columns
  - built-in sorting support
  - clean MCV (Model-Controller-View) design
  - flexible rendering and editing support
  - flexible data access model
  - dynamic row/columns add/removal
  - visual features like: highlight modes, cell attributes, etc
 
 	Composite row/columns.
 wxAdvTable uses composite rows and columns. Composite means one row/column
 contains one of more subrows/columns, like this:
	 _____________________________
	|           Column            |
	|-------------+-------+-------|
	| Sub 1       | Sub 2 | Sub 3 |
	|------+------|       |       |
	| SS 1 | SS 2 |       |       |
	|______|______|_______|_______|
 Where "Sub 1", "Sub 2" and "Sub 3" are subcolumns.
 Every subcolumn can also contain subcolumns ("SS 1", "SS 2" on picture), 
 and so on.
 
 All rows/columns are arranged in layers.
	____________ _____________________________
	 Layer 1    |           Column            |
	------------|-------------+-------+-------|
	 Layer 2    | Sub 1       | Sub 2 | Sub 3 |
	------------|------+------|       |       |
	 Layer 3    | SS 1 | SS 2 |       |       |
	____________|______|______|_______|_______|
 
 There are three layers in our example. 
 Layer 1 contains "Column", 
 Layer 2 - "Sub 1", "Sub 2" and "Sub 3", 
 Layer 3 - "SS 1", "SS 2", "Sub 2" and "Sub 3"
 Note that "Sub 2" and "Sub 3" occupies two layers, "Layer 2" and "Layer 3".
 
 	Decomposition. 
 To determine table cell count, wxAdvTable performs rows/columns decomposition.
 Decomposed row/column count is number of cells on lowest layer, 
 in example above it's "Layer 3", and number of column will be 4. 
 When accessing data from model, it indexed by decomposed count, 
 _not_ by count of row/column definitions.

	Rows/columns definition.
 Rows/columns defined by wxAdvHdrCell class, which contains 
 attributes like: rows/column label, size, sortable or not, resizable or not, 
 horizontal and vertical label text alignment, and etc.
 wxAdvHdrCell also defines subcells for row/column.
 
	Data model.
 wxAdvTable not stores data by itself, it delegate data access to 
 data model. Data model is abstract, so you can use it to access data 
 from various data sources, such as files, database tables, etc.
 Data model defines content, data format, editable/readonly and 
 graphical representation attributes for each cell.
 There are wxAdvStringTableDataModel for string tables, it stores 
 data like strings and all cells has string data format, and
 wxFilterTableDataModel that acts as filter for underlaying model, 
 it stores data in internal storage and updates it when underlaying 
 model data is changed. It can be used as cache for data sources like 
 databases, or data which calculation takes long time.
 
 	Renderers.
 Data in cells rendered by objects called "Renderers", they are 
 derivates of wxAdvTableCellRenderer class. Renderers associates with
 data formats. There are wxAdvStringCellRender for string data,
 wxAdvBoolCellRenderer for boolean data, 
 wxAdvColourCellRenderer for color data,
 wxAdvDateTimeCellRenderer for datetime data, 
 wxAdvChoicesCellRenderer for choices data. 

	Editors.
 Editors are objects that performs data editing. They are activated on 
 user input (keypress on left mouse click) on cell, and deactivated when 
 editing is done or cancelled.
 Editors are associated with data formats. 

	Sorting.
 wxAdvTable has built-in sorting support. Rows or columns can be sorted by object called 
 "Sorter", that is derivate of wxAdvTableSorter class.
 There are two sorting modes: 
 SortRows - in which rows will be rearranged by order specified by sorter,
 and SortCols - in which columns will be rearranged.
 There is sorting index, index of row/column by which will be sorting 
 performed. Sorting index is row index for SortCols mode, and 
 column index for SortRows mode.
 Sorting index is set by mouse click on row/column header, or 
 can be set by calling wxAdvTable::SetSortingIndex.
 Rows/columns by default are not sortable. To enable sorting you can 
 call wxAdvHdrCell::Sortable or set all rows/columns sortable by 
 calling wxAdvTable::SetAllowSortByAnyRos or wxAdvTable::SetAllowSortByAnyCol.
 There are some special sorter for data formats:
 wxAdvTableDateTimeSorter for datetime values, 
 wxAdvTableDoubleSorter for double values.
 
 	Filtering.
 TODO: Filtering is not implemented yet. 
 
    Row/columns resize.
 Rows/columns can be resized if they marked by wxAdvHdrCell::Resizable method 
 call or when all rows/columns resize enabled in wxAdvTable (by calling 
 wxAdvTable::SetAllowResizeAll[Rows|Cols] with true argument).
 There are two resize constraints: minSize and maxSize. They can be set 
 to rows/cols by calling wxAdvHdrCell::MinSize and MaxSize method.
 MaxSize by default is unlimited, MinSize is set to zero.
  
    Events.
 wxAdvTable supports some wxGrid events and have it's own events.
 wxGrid event are cell click, change, select events:
 wxEVT_GRID_CELL_LEFT_CLICK, wxEVT_GRID_CELL_RIGHT_CLICK, 
 wxEVT_GRID_CELL_LEFT_DCLICK, wxEVT_GRID_CELL_RIGHT_DCLICK,
 wxEVT_GRID_CELL_CHANGE, wxEVT_GRID_SELECT_CELL, 
 wxEVT_GRID_EDITOR_SHOWN, wxEVT_GRID_EDITOR_HIDDEN. 
 
 wxAdvTable events are hdrCell click, move, resize events:
 wxEVT_ADVTABLE_HDRCELL_LEFT_CLICK, wxEVT_ADVTABLE_HDRCELL_RIGHT_CLICK,
 wxEVT_ADVTABLE_HDRCELL_LEFT_DCLICK, wxEVT_ADVTABLE_HDRCELL_RIGHT_DCLICK,
 wxEVT_ADVTABLE_HDRCELL_SIZE, wxEVT_ADVTABLE_HDRCELL_MOVE, wxEVT_ADVTABLE_HDRCELL_SORT.

 Because wxAdvTable rows/columns can be composite, wxGrid events for
 rows/columns cannot be used. 
 
  
 Installation - win32
 --------------------
 
 When building on win32, you can use the makefiles in the BUILD folder using the
 WX_DIR, WX_DEBUG, WX_UNICODE and WX_SHARED to match your wxWidgets build.
 Examples:
 
   > nmake -fmakefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
   > make -fmakefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1


     
 Installation - unix
 -------------------

 When building on a Linux/GNU-based system, type

  ./configure
  make

 to build against the default build of wxWidgets (the one listed as 'default' 
 by wx-config --list). Type "./configure --help" for more info.



 ChangeLog
 ---------
 
 1.5:
      What's new:
       - ScrollToCell method was added
       - added gradient header cells drawing mode to wxAdvDefaultHdrCellRenderer
       - added wxEVT_ADVTABLE_HDRCELL_SORT event, fired when sorting row/column changed
       - keys handling and focus bugfixes
       - scrolling bugfixes
       - dll export bugfixes
       - documentation updated
       
 1.4:
      What's new:
       - three state sorting: ascending, descending, nosorting 
       - added datetime renderer/editor, and wxAdvTableDateTimeSorter
       - added choices renderer/editor, and wxAdvTableChoicesSorter
       - some redraw bugfixes
       
 1.3:
      What's new:
       - sorting bugfixes
       - rows/columns resize bugfixes
       - rows/columns remove methods added
       - rows/columns minimal/maximal size added
       - builder application added 
       - demo application updated
 
 1.2: third version of wxAdvTable at wxCode
      What's new:
       - many bugfixes
       - updated demo application
       - wxAdvStringTableDataModel is extended to support different cell
         formats, and renamed to wxAdvDefaultTableDataModel
       - many refactoring changes
 
 1.1: second version of wxAdvTable at wxCode
      What's new:
       - added row/columns drag resize and move, and corresponding events
       - major redesign
       - headers drawing bugfixes
       - editors bugfixes
       - sorting bugfixes 
       - scrolling bugfixes
       - updated documentation
       - many refactoring changes
       	
 1.0 - first version of wxAdvTable at wxCode
 
 Future plans:
       - Perl and Python bindings
       - Rows/columns drag move
       - add filtering support
       - resize code optimization
       - add bitmap data type
	    
 Known issues:
       - choices editor on Windows (tm) looks ugly
       - keys up/down/left/right/page up/page down must move cell focus
         and also scroll table if needed
       - make posible to save wxAdvTable to Excel (tm) or OpenOffice document 
         workbook.
       - tested only on wxMSW-2.8.9 (Windows XP(tm) SP2 mingw) and wxGTK-2.8.8 (Gentoo Linux x86_64 gcc),
         if someone can test on other wxWidgets ports, please let me know
       - bugs with keydown handling on wxGTK

