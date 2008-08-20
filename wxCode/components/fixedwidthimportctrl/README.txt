
wxFixWidthImportCtrl
--------------------

Author: Manuel Martin
Version: 1.0

A control for defining columns-widths in a file or block of text.
The control displays the text and the user can set delimiters (marks).
The widths these marks represent will be used to break each line into pieces.

User can:
- Scroll the text shown (scrolling or using mouse wheel).
- Select a line of text (clicking on it). The line changes its colour.
- Set a 'delimiter' by clicking on the rule.
- Move a 'delimiter' by clicking on it (on the rule) and drag it.
- Remove a 'delimiter' by clicking on it (on the rule) and drag it out
  of the control.


Coding with wxFixWidthImportCtrl
--------------------------------

Just add fiximp.h and fiximp.cpp to your project.

The class 'wxFixWidthImportCtrl' has this members:


size_t GetValCount()
  Returns the number of 'delimiters'.


const wxArrayInt& GetValues()
  Return the 'delimiters' positions.


void SetValues(const wxArrayInt& values)
  Set the 'delimiters' positions.


size_t GetLinesCount()
  Returns the number of lines of file/text.


size_t GetSelectedLineNu()
  Returns the number of the line currently selected.


const wxString& GetSelectedLine()
  Returns currently selected line.


void SelectLine(size_t nline)
  Selects (changes colour) line nline.


const wxString& GetLine(size_t nline)
  Returns line nline of file/text.


wxColour GetDelimColor()
  Returns the colour used to draw 'delimiters'.


wxColour GetSelecColor()
  Returns the colour used to draw the selected line.


void SetDelimColor(unsigned char red, unsigned char green, unsigned char blue)
  Sets the colour used to draw 'delimiters'.


void SetSelecColor(unsigned char red, unsigned char green, unsigned char blue)
  Sets the colour used to draw the selected line.


bool LoadFile(const wxString& filename, const wxMBConv& conv = wxConvLocal)
  Open 'filename', reads it using 'conv' and fills the control.
  Retuns false if something went wrong (opening, converting to Unicode, etc.)


void LoadText(const wxString& textInside)
  Fills the control with a text. End of line (EOL) can be any of Mac, Unix, Win.


void SetTabSize(int tabSize)
  TABs in each line may be replaced with at most 'tabSize' spaces, so the line
  becomes 'tabulated' in columns.
  The default value (-1) means no tab->spaces conversion.
  A value of 0 means 'erase all TABs'
  The conversion is done at LoadFile/LoadText. You can also
  call ConvertTabsToSpaces() later on.


int GetTabSize()
  Returns the number of spaces to be used for each TAB.


void ConvertTabsToSpaces()
  You can use this function after LoadFile/LoadText.
  Because once TABs are converted there are no more TABS, using
  this fucntion again is a waste of time.


void EnableDelim(bool enable = TRUE)
  If you just want to see the text and select a line, but you don't want
  the 'delimiters' to be drawn.


virtual bool Enable(bool enable = true)
virtual bool Show(bool show = true)
  wxWindow overrides.


Event generated
----------------

This control fires an event (of type wxCommandEvent).
when:
- file/string loaded
- selected line changed
- values set/changed


EVT_COMMAND(id, wxEVT_COMMAND_FIXW_UPDATED, funct)
  Use this event macro in your code to do something when the
  control changes. For example when the user selects a line,
  you can read that line and the delimiters values and show a
  preview of the data after breaking it into tokens.


Questions?
----------
E-mail me: mmartin@ceyd.es

