
/*
 * xstc.h
 *
 * this file is licensed under the wxWindows licence
 * just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are
 *
 * visit: http://opensource.org/ to see both
 * the GNU GPL and wxWindows licences.
 *
 * this software has absolutely no warranty, express or implied
 *
 * just so you know, i don't care if you change the code
 * don't email me if you did someting to it.
 *
 * no need to mark changes, you obviously may want to change the color
 * settings, it would only be a real pain if you had to mark them.
 *
 * if you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * you must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (except for any code you add obviously)
 *
 * this notice may not be changed in any way and must remain at the top of every
 * source file.
 *
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2006
 */

#ifndef XSTC_H
#define XSTC_H

#include "util.h"

#ifndef XSTC_NO_CONFIG
  #include <wx/config.h>
#endif //XSTC_NO_CONFIG

#include <wx/version.h>
#include <wx/ffile.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <wx/arrstr.h>
#include <wx/filedlg.h>
#include <wx/utils.h>

#ifndef XSTC_NO_CONFIG
 #include <wx/gdicmn.h>
 #include <wx/colour.h>
#endif //XSTC_NO_CONFIG

#ifndef XSTC_NO_KEYS
/**
\brief holds keywords for the lexers

\detailed each of the string names representing these strings in PurgeKeys()
          are the same as the name of the identifier. identifiers are named thus:
          name
          name1
            and for trailing numbers:
          namex_0
          namex_1

\struct
*/
struct WXDLLIMPEXP_XSTC Keyword_Sets//should hold either default keywords OR a filename to the keywords
      {//if a '.' is found in it, a filename is assumed, KeyCheck checks this and loads the file if the filename is found
//versions where a language support was added is marked above each key set.
//comments in place of language without keyword support
#ifndef XSTC_NO_ABAQUS
//V1.75
         wxString abaqus;//processors
		 wxString abaqus1;//commands
		 wxString abaqus2;//slashommands
		 wxString abaqus3;//starcommands
		 wxString abaqus4;//arguments
		 wxString abaqus5;//functions
#endif //XSTC_NO_ABAQUS
#ifndef XSTC_NO_ADA
//V1.37
		 wxString ada;
#endif //XSTC_NO_ADA
#ifndef XSTC_NO_APDL
//V1.60
         wxString apdl;//preprocessor
         wxString apdl1;//commands
         wxString apdl2;//slashcommands
         wxString apdl3;//starcommands
         wxString apdl4;//arguments
         wxString apdl5;//functions
#endif //XSTC_NO_APDL
#ifndef XSTC_NO_ASM
//V1.56
         wxString asm_0;//cpu instructions
         wxString asm_1;//fpu instructions
         wxString asm_2;//registers
         wxString asm_3;//directives
         wxString asm_4;//directive operands
         wxString asm_5;//extended instructions
#endif //XSTC_NO_ASM
#ifndef XSTC_NO_ASN1
//V1.62
         wxString asn1_0;//keywords
         wxString asn1_1;//attributes
         wxString asn1_2;//descriptors
         wxString asn1_3;//types
#endif //XSTC_NO_ASN1
#ifndef XSTC_NO_ASYMPTOTE
//V1.75
		 wxString asymptote;//key words 1
		 wxString asymptote1;//key words 2
#endif //XSTC_NO_ASYMPTOTE
#ifndef XSTC_NO_AU3
//V1.60
         wxString au3_0;
         wxString au3_1;
         wxString au3_2;
         wxString au3_3;
         wxString au3_4;
         wxString au3_5;
         wxString au3_6;
         wxString au3_7;
#endif //XSTC_NO_AU3
#ifndef XSTC_NO_AVE
//V1.36
         wxString ave;
         wxString ave1;
         wxString ave2;
         wxString ave3;
         wxString ave4;
         wxString ave5;
#endif //XSTC_NO_AVE
#ifndef XSTC_NO_BAAN
//V1.42
         wxString baan;
         wxString baan1;
#endif //XSTC_NO_BAAN
//if all 4 versions of basic are turned off, then the all versions keys are removed too.
#ifndef XSTC_NO_BLITZBASIC
#ifndef XSTC_NO_FREEBASIC
#ifndef XSTC_NO_POWERBASIC
#ifndef XSTC_NO_PUREBASIC
         wxString basic;//these vary on the language
         wxString basic2;
         wxString basic3;
#endif //XSTC_NO_PUREBASIC
#endif //XSTC_NO_POWERBASIC
#endif //XSTC_NO_FREEBASIC
#endif //XSTC_NO_BLITZBASIC

//check beside the dialect to see version it was added.
#ifndef XSTC_NO_BLITZBASIC
         wxString basic1bl;//blitz //V1.63
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_FREEBASIC
         wxString basic1fr;//free //V1.65
#endif //XSTC_NO_FREEBASIC

#ifndef XSTC_NO_POWERBASIC
         wxString basic1po;//power //V1.56
#endif //XSTC_NO_POWERBASIC

#ifndef XSTC_NO_PUREBASIC
         wxString basic1pu;//pure //V1.63
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
//V1.60
         wxString bash;//keywords
#endif //XSTC_NO_BASH
#ifndef XSTC_NO_BATCH
//initial release
         wxString batch;
#endif //XSTC_NO_BATCH
#ifndef XSTC_NO_BULLANT
//V1.41
         wxString bullant;//keywords
#endif //XSTC_NO_BULLANT
#ifndef XSTC_NO_CAML
//V1.63
         wxString caml;//primary keywords
         wxString caml1;
         wxString caml2;
#endif //XSTC_NO_CAML
#ifndef XSTC_NO_CLW
//V1.55
         wxString clw;//clarion keywords
         wxString clw1;//compiler directives
         wxString clw2;//built-in functions
         wxString clw3;//runtime expressions
         wxString clw4;//structures data types
         wxString clw5;//attributes
         wxString clw6;//standard equates
         wxString clw7;//reserved words labels
         wxString clw8;//reserved words procedure labels
#endif //XSTC_NO_CLW
#ifndef XSTC_NO_CMAKE
//V1.72
		 wxString cmake;//Commands
		 wxString cmake1;//Parameters
		 wxString cmake2;//UserDefined
#endif //XSTC_NO_CMAKE
#ifndef XSTC_NO_COBOL
//1.78
         wxString cobol;//A keywords
         wxString cobol1;//B keywords
         wxString cobol2;//extended
#endif //XSTC_NO_COBOL
#ifndef XSTC_NO_CONF
//V1.33
         wxString conf;//directives
         wxString conf1;//parameters
#endif //XSTC_NO_CONF
#ifndef XSTC_NO_CPP
//C++ original release IDL added V1.24 C# added V1.31
         wxString cpp;//primary keywords
         wxString cpp1;//secondary keywords [stl]
         wxString cpp2;//documentation
         wxString cpp3;//global classes and typedefs
#endif //XSTC_NO_CPP
#ifndef XSTC_NO_CSOUND
//V1.64
         wxString csound;//opcodes
         wxString csound1;//header statements
         wxString csound2;//user keywords
#endif //XSTC_NO_CSOUND
#ifndef XSTC_NO_CSS
//V1.55
         wxString css;//css1 keywords
         wxString css1;//pseudo classes
         wxString css2;//css2 keywords
#endif //XSTC_NO_CSS
#ifndef XSTC_NO_D
//V1.72
		 wxString d;
		 wxString d1;
		 wxString d2;
		 wxString d3;
#endif //XSTC_NO_D
//Diff V1.31
#ifndef XSTC_NO_EIFFEL
//V1.38
         wxString eiffel;//keywords
#endif //XSTC_NO_EIFFEL
#ifndef XSTC_NO_EIFFEL
//V1.38
         wxString eiffelkw;
#endif //XSTC_NO_EIFFEL
#ifndef XSTC_NO_ERLANG
//V1.57
         wxString erlang;//keywords
#endif //XSTC_NO_ERLANG
//errorlist initial release
#ifndef XSTC_NO_ESCRIPT
//V1.53
         wxString escript;
         wxString escript1;
         wxString escript2;
#endif //XSTC_NO_ESCRIPT
#ifndef XSTC_NO_F77
//V1.49
         wxString f77_0;
         wxString f77_1;
         wxString f77_2;
#endif //XSTC_NO_F77
#ifndef XSTC_NO_FLAGSHIP
//V1.64
         wxString flagship;//keywords
         wxString flagship1;//functions
         wxString flagship2;//user 1
         wxString flagship3;//user2
#endif //XSTC_NO_FLAGSHIP
#ifndef XSTC_NO_FORTH
//V1.56
         wxString forth;
         wxString forth1;
         wxString forth2;
         wxString forth3;
         wxString forth4;
         wxString forth5;
#endif //XSTC_NO_FORTH
#ifndef XSTC_NO_FORTRAN
//V1.49
         wxString fortran;
         wxString fortran1;
         wxString fortran2;
#endif //XSTC_NO_FORTRAN
#ifndef XSTC_NO_GAP
//V1.74
		 wxString gap;
		 wxString gap1;
		 wxString gap2;
		 wxString gap3;
#endif //XSTC_NO_GAP
#ifndef XSTC_NO_GUI4CLI
//V1.60
         wxString gui4cli;//globals
         wxString gui4cli1;//events
         wxString gui4cli2;//sttributes
         wxString gui4cli3;//contorl
         wxString gui4cli4;//commands
#endif //XSTC_NO_GUI4CLI
#ifndef XSTC_NO_HASKELL
//V1.63
         wxString haskell;
#endif //XSTC_NO_HASKELL
#ifndef XSTC_NO_HTM
//initial release php added V1.23 later merged V1.30 sgml V1.40 asp 1.40 i guess
         wxString html;//html elements attributes
         wxString html1;//javascript keywords
         wxString html2;//vbscript keywords
         wxString html3;//python keywords;
         wxString html4;//php keywords
         wxString html5;//sgml dtd keywords
         wxString html6;
         wxString html7;//phpscript
#endif //XSTC_NO_HTM
#ifndef XSTC_NO_INNOSETUP
//V1.68
         wxString inno;//sectoins
         wxString inno1;//keywords
         wxString inno2;//parameters
         wxString inno3;//preprocessor directives
         wxString inno4;//pascal keywords
         wxString inno5;//user defined keywords
#endif //XSTC_NO_INNOSETUP
#ifndef XSTC_NO_JS
//initial release, later merged into C++ lexer V1.22
         wxString js;//same as cpp lexer unless i can get html sublexer to work right.
         wxString js1;//i may not use that for primary javascript files anyway
         wxString js2;
#endif //XSTC_NO_JS
#ifndef XSTC_NO_JAVA
//initial release, later merged into C++ lexer V1.22
         wxString java;//same as cpp lexer
         wxString java1;
         wxString java2;
         wxString java3;
#endif //XSTC_NO_JAVA
#ifndef XSTC_NO_KIX
//V1.60
         wxString kix;
         wxString kix1;
         wxString kix2;
#endif //XSTC_NO_KIX
//latex V1.25
#ifndef XSTC_NO_LISP
//V1.38
         wxString lisp;//functions and secial operators
         wxString lisp1;//keywords
#endif //XSTC_NO_LISP
#ifndef XSTC_NO_LOUT
//V1.53
         wxString lout;
         wxString lout1;
         wxString lout2;
#endif //XSTC_NO_LOUT
#ifndef XSTC_NO_LUA
//V1.26
         wxString lua;//keywords
         wxString lua1;//basic functions
         wxString lua2;//string math functions
         wxString lua3;//coroutines, I/O system facilities
         wxString lua4;//user1
         wxString lua5;//user2
         wxString lua6;//user3
         wxString lua7;//user4
#endif //XSTC_NO_LUA
//makefile V1.35 ??
#ifndef XSTC_NO_MAGIK
//V1.76
		 wxString magik;//keywords
		 wxString magik1;//pragmatics
		 wxString magik2;//containers
		 wxString magik3;//flow
		 wxString magik4;//characters
#endif //XSTC_NO_MAGIK
#ifndef XSTC_NO_MATLAB
//V1.45
         wxString matlab;//keywords
#endif //XSTC_NO_MATLAB
#ifndef XSTC_NO_METAPOST
//V1.56
         wxString metapost;//metapost
         wxString metapost1;//metafun
#endif //XSTC_NO_METAPOST
#ifndef XSTC_NO_MMIXAL
//V1.54
         wxString mmixal;//operator codes
         wxString mmixal1;//special registers
         wxString mmixal2;//predefined symbols
#endif //XSTC_NO_MMIXAL
#ifndef XSTC_NO_NIMROD
//1.78
         wxString nimrod;
#endif //XSTC_NO_NIMROD
#ifndef XSTC_NO_NNCRONTAB
//V1.40
         wxString nncrontab;//section forth keywords
         wxString nncrontab1;//nncrontab keywords
         wxString nncrontab2;//modifiers
#endif //XSTC_NO_NNCRONTAB
#ifndef XSTC_NO_NSIS
//V1.54
         wxString nsis;//functions
         wxString nsis1;//variables
         wxString nsis2;//lables
         wxString nsis3;//user defined
#endif //XSTC_NO_NSIS
#ifndef XSTC_NO_OCTAVE
//V1.58
         wxString octave;
#endif //XSTC_NO_OCTAVE
#ifndef XSTC_NO_OPAL
//V1.68
         wxString opal;//keywords
         wxString opal1;//sorts
#endif //XSTC_NO_OPAL
#ifndef XSTC_NO_PASCAL
//V1.34
         wxString pas;//keywords
         wxString pas1;//classwords
#endif //XSTC_NO_PASCAL
#ifndef XSTC_NO_PERL
//V1.21
         wxString perl;//keywords
#endif //XSTC_NO_PERL
#ifndef XSTC_NO_PLM
//V1.74
		 wxString plm;
#endif //XSTC_NO_PLM
//PO V1.77
#ifndef XSTC_NO_POV
//V1.52
         wxString pov;
         wxString pov1;
         wxString pov2;
         wxString pov3;
         wxString pov4;
         wxString pov5;
         wxString pov6;
         wxString pov7;
#endif //XSTC_NO_POV
#ifndef XSTC_NO_POWERPRO
//1.78
         wxString powerpro;
         wxString powerpro1;
         wxString powerpro2;
         wxString powerpro3;
#endif //XSTC_NO_POWERPRO
#ifndef XSTC_NO_POWERSHELL
//V1.76
		 wxString powershell;
		 wxString powershell1;
		 wxString powershell2;
#endif //XSTC_NO_POWERSHELL
#ifndef XSTC_NO_PROGRESS
//V1.74
		 wxString progress;//primary keywords
		 wxString progress1;//secondary keywords
		 wxString progress2;//documentation
#endif //XSTC_NO_PROGRESS
//properties initial release
#ifndef XSTC_NO_PS
//V1.54
         wxString ps;//L1 operators
         wxString ps1;//L2 operators
         wxString ps2;//L3 operators
         wxString ps3;//RIP-specific operators
         wxString ps4;//user defined operators
#endif //XSTC_NO_PS
#ifndef XSTC_NO_PYTHON
//initial release
         wxString python;//keywords
         wxString python1;//highlighted identifiers
#endif //XSTC_NO_PYTHON
#ifndef XSTC_NO_R
//V1.75
		 wxString r;
		 wxString r1;
		 wxString r2;
#endif //XSTC_NO_R
#ifndef XSTC_NO_REBOL
//V1.64
         wxString rebol;
         wxString rebol1;
         wxString rebol2;
#endif //XSTC_NO_REBOL
#ifndef XSTC_NO_RUBY
//V1.38
         wxString ruby;
#endif //XSTC_NO_RUBY
#ifndef XSTC_NO_SCRIPTOL
//V1.54
         wxString scriptol;
#endif //XSTC_NO_SCRIPTOL
#ifndef XSTC_NO_SMALLTALK
//V1.64
         wxString smalltalk;//special selectors
#endif //XSTC_NO_SMALLTALK
#ifndef XSTC_NO_SML
//1.78
         wxString sml;
         wxString sml1;
         wxString sml2;
#endif //XSTC_NO_SML
#ifndef XSTC_NO_SORCUS
//1.78
         wxString sorcus;//command
         wxString sorcus1;//parameter
         wxString sorcus2;//constants
#endif //XSTC_NO_SORCUS
#ifndef XSTC_NO_SPECMAN
//V1.60
         wxString specman;
         wxString specman1;
         wxString specman2;
         wxString specman3;
#endif //XSTC_NO_SPECMAN
#ifndef XSTC_NO_SPICE
//V1.69
         wxString spice;//command
         wxString spice1;//functions
         wxString spice2;//params
#endif //XSTC_NO_SPICE
#ifndef XSTC_NO_MSSQL
//V1.59
         wxString mssql;//statements
         wxString mssql1;//data tables
         wxString mssql2;//system tables
         wxString mssql3;//global variables
         wxString mssql4;//functions
         wxString mssql5;//system stored procedures
         wxString mssql6;//operators
#endif //XSTC_NO_MSSQL
#ifndef XSTC_NO_MYSQL
//V1.77
		 wxString mysql;//major keywords
		 wxString mysql1;//keywords
		 wxString mysql2;//database objects
		 wxString mysql3;//functions
		 wxString mysql4;//system variables
		 wxString mysql5;//procedure keywords
		 wxString mysql6;//user1
		 wxString mysql7;//user2
		 wxString mysql8;//user3
#endif //XSTC_NO_MYSQL
#ifndef XSTC_NO_SQL
//V1.22
         wxString sql;
         wxString sql1;
         wxString sql2;
         wxString sql3;
         wxString sql4;
         wxString sql5;
         wxString sql6;
         wxString sql7;
#endif //XSTC_NO_SQL
#ifndef XSTC_NO_TACL
//1.78
         wxString tacl;//builtin
         wxString tacl1;//labels
         wxString tacl2;//commands
#endif //XSTC_NO_TACL
#ifndef XSTC_NO_TADS3
//V1.64
         wxString tads3_0;
         wxString tads3_1;
         wxString tads3_2;
         wxString tads3_3;
#endif //XSTC_NO_TADS3
#ifndef XSTC_NO_TAL
//1.78
         wxString tal;//keywords
         wxString tal1;//builtin
#endif //XSTC_NO_TAL
#ifndef XSTC_NO_TCL
//V1.39
         wxString tcl;
         wxString tcl1;
         wxString tcl2;
         wxString tcl3;
         wxString tcl4;
         wxString tcl5;
         wxString tcl6;
         wxString tcl7;
         wxString tcl8;
#endif //XSTC_NO_TCL
#ifndef XSTC_NO_TEX
//V1.25
         wxString tex;
#endif //XSTC_NO_TEX
#ifndef XSTC_NO_VB
//initial release
         wxString vb;
         wxString vb1;
         wxString vb2;
         wxString vb3;
#endif //XSTC_NO_VB
#ifndef XSTC_NO_VBSCRIPT
//initial release
         wxString vbscript;
         wxString vbscript1;
         wxString vbscript2;
         wxString vbscript3;
#endif //XSTC_NO_VBSCRIPT
#ifndef XSTC_NO_VERILOG
//V1.59
         wxString verilog;
         wxString verilog1;
         wxString verilog2;
         wxString verilog3;
#endif //XSTC_NO_VERILOG
#ifndef XSTC_NO_VHDL
//V1.62
         wxString vhdl;//keywords
         wxString vhdl1;//operators
         wxString vhdl2;//attributes
         wxString vhdl3;//functions
         wxString vhdl4;//packages
         wxString vhdl5;//types
         wxString vhdl6;//user
#endif //XSTC_NO_VHDL
//xcode dunno
//xml V1.21
#ifndef XSTC_NO_YAML
//V1.55
         wxString yaml;
#endif //XSTC_NO_YAML
      };
#endif //XSTC_NO_KEYS
/**
\brief holds two string values for the lexer properties

\detailed it is a simple way to pass property strings around as one
          then split the L/R values when the property needs to be
          sent to scintilla

\struct
*/
struct WXDLLIMPEXP_XSTC PropSTR //holds the right and left values of a property string
{//the 'side' is delimited by the '_' underscore character
   wxString L;//left
   wxString R;//right
};

/*
there are two ways to edit coloring of a lexer. first, quite simple find
the source that sets up the lex coloring and manualy edit the entries.
if you want to dynamically allow color changes, you may want to just
make a conf file to take care of it for you. obviously you can change
the code and make a conf file at the same time, to say, change the default
color settings without needing to have a lot of files for your program to
work directly.
*/

/**
\brief this class automates some of scintilla

\detailed the basics of what XSTC does is:
          automatically sets up brace matching
          automatic margin setup
          toggle the fold margin based on lexer support
          automatic set up of lexer colors and keywords from extention

          and, XSTC is a wrapper class, light enough (because scintilla
          does all of the work) that it can be added right into the project
          like any other source file or it can be compiled as a library
          encompassing wxSTC/SCI and scintilla.

          To compile sources in-project
            define XSTC_H_FLAT
          Make sure to define XCLASS
            0=wxSTC 1=wxSCI
*/
class WXDLLIMPEXP_XSTC XSTC: public XSTC_CLASS
{
    public:

    /**
    \brief constructor, obviously sets it all up

    \detailed does what a constructor should
    */
WXDLLIMPEXP_XSTC XSTC(wxWindow *parent, wxWindowID id=wxID_ANY,
         const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize,
         long style = 0,
         const wxString& name = "XSTC");

    /**
    \brief in case you didn't notice it's a destructor

    \detailed probably will always remain empty
    */
WXDLLIMPEXP_XSTC virtual ~XSTC();

//the load and save functions here wrap the scintilla functins and allow for other
//actions to take place when it happens. (so lexers and coloring can be set up via extension.)
    /**
    \brief load file

    \detailed loads a file through scintilla then sets up the lexer
              unless setlexer is false, then the current lexer will
              color the document and the fold margin will stay as
              it was toggled already. also returns a status message.
    */
WXDLLIMPEXP_XSTC long LoadFileX(wxString filename, bool setlexer = false);

    /**
    \brief save file

    \detailed if trimming space, space to tab or any other functionality
              was aded to xstc and set to on for saving, then it will be
              called in this function.

              savemode is a three mode flag. for utilizing a saveas dialog automatically.
              never always or on failed attempt of the used filename

              if no filename is passed then the internal "XFilename" copy is used
              if one is passed that variable is ignored
    */
WXDLLIMPEXP_XSTC long SaveFileX(const wxString filename = wxT(""), long saveas = SAVE_AS_NEVER);

    /**
    \brief clears the editor

    \detailed clears the editor sets lexer to NULL and Filename to 'Untitled'
              also returns a status message.
    */
WXDLLIMPEXP_XSTC void CloseFile(); //kindof an extended Clear() function for the same purpose as the file wrappers.

#ifndef XSTC_NO_KEYS
    /**
    \brief sets all keywords to ""
    */
WXDLLIMPEXP_XSTC void PurgeKeys();

    /**
    \brief simply it resets ALL of the keys_set strings to default
    */
WXDLLIMPEXP_XSTC void InitKeys();
#endif //XSTC_NO_KEYS

    /**
    \brief toggles a bookmark

    \detailed toggles a bookamrk at the specified line, internaly this is the
              line where the margin was clicked
    */
WXDLLIMPEXP_XSTC void ToggleBookMark(int line);

    /**
    \brief sets margin number for each margin type

    \detailed sets the margins and sets the withs to those set with SetMgnWidth
    */
WXDLLIMPEXP_XSTC void MapMargins(int line, int symbol, int fold);

    /**
    \brief sets widths of the mapped margins

    \detailed sets the margin widths and stores them so that
              when mapping margins the same settings will be used
    */
WXDLLIMPEXP_XSTC void SetMgnsWidth(int line, int symbol, int fold);

#ifndef XSTC_NO_KEYS
    /**
    \brief load keywords from a file

    \detailed loads a newline delimited keyword set from file and returns it
              as space delimited or a space delimited should just pass as is.
              use: KeyCheck() instead if you might support keywords too.
    */
WXDLLIMPEXP_XSTC wxString LoadKeyWords(wxString filename);
#endif //XSTC_NO_KEYS

    /**
    \brief sets the visual mode to color on black

    \detailed sets up scintilla for color over black, all lexers adjust
              accordingly if this is used to set up the general colors
    */
WXDLLIMPEXP_XSTC void DarkStyle();

    /**
    \brief attempts to emulate visual studio coloring

    \detailed i don't use VS too often and i copied these settings
              with some creativity for readability from Dev-C++
    */
WXDLLIMPEXP_XSTC void VisualStudioStyle();

    /**
    \brief attempts to emulate classic colors (whatever that means)

    \detailed dunno i copied these settings with some
              creativity for readability from Dev-C++
    */
WXDLLIMPEXP_XSTC void ClassicStyle();

    /**
    \brief attempts to emulate the old borland TUI ide

    \detailed yes i have a copy of Turbo C++ and in fact that is what
              i learned c++ on, if i can figure out how to set up the
              default colors again i definatly can get this one right.
    */
WXDLLIMPEXP_XSTC void BorlandStyle();

	   /**
    \brief a style i found on the internet

    \detailed i found it on codinghorror in is you're ide hot or not as i recall
    */
WXDLLIMPEXP_XSTC void JeffStyle();

	   /**
    \brief a style i found on the internet

    \detailed i found it on codinghorror in is you're ide hot or not as i recall
    */
WXDLLIMPEXP_XSTC void ZenburnStyle();

    /**
    \brief a style based on the matrix

    \detailed i saw a matrix based color scheme and decided to make one
    */
WXDLLIMPEXP_XSTC void MatrixStyle();

    /**
    \brief loads a style from the configs

    \detailed will load colorstyle settings from a config file. only needs the style name, will create all
              default of black on white if no settings exsist. also uses styleconf(), but these settings take
              priority.
    */
WXDLLIMPEXP_XSTC void ConfigStyle(wxString style);

    /**
    \brief returns the names of availible config styles

    \detailed returns a wxArrayString of all names that are in the config CStyles string.
    */
WXDLLIMPEXP_XSTC wxString GetConfNames();

    /**
    \brief turns off enviornment colorstyles

    \detailed it changes the colorstyle string to "NULL" and changes
              the margin colors back to button face (or whatever) all
              other settings will keep, so you could call DarkStyle()
              then this and the lexers will ignore the black bg
    */
WXDLLIMPEXP_XSTC void ResetStyle();

#ifndef XSTC_NO_CONFIG
    /**
    \brief sets config based colors for colorstyle functions

    \detailed called by all colorstyle functions since it does the same
              thing for each
    */
WXDLLIMPEXP_XSTC void StyleConf();

    /**
    \brief sets config based colors for foldstyle functions

    \detailed called by all foldstyle functions since it does the same
              thing for each
    */
WXDLLIMPEXP_XSTC void FoldConf();
#endif //XSTC_NO_CONFIG

    /**
    \brief sets a lexer

    \detailed the lang string is optional. if config is turned on and the language was set in the
			  config database then properties will be automatically loaded. otherwise it is ignored. 
			  properties will only be loaded if lang is set. that is the language being used for the
			  document, not the lexer.
    */
WXDLLIMPEXP_XSTC void SetLexerX(int lexer);

    /**
    \brief gets property and value from a string

    \detailed parses a property_value string. the '_' underscore is a
              delimiter between them. property is a lexer property
              string value is the setting to use with that property
    */
WXDLLIMPEXP_XSTC PropSTR GetPropStr(wxString property);

#ifndef XSTC_NO_KEYS
    /**
    \brief checks keyname for either being a filename or keyword set

    \detailed checks keyname for "^ISFILE^" (if it is a filename, or coma delimeted
              set of filenames) and either loads the file(s) or
              returns its given value which sould be a set of keywords
    */
WXDLLIMPEXP_XSTC wxString KeyCheck(wxString keyname);
#endif //XSTC_NO_KEYS

#ifndef XSTC_NO_ABAQUS
    /**
    \brief abaqus lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexABAQUS();
#endif //XSTC_NO_ABAQUS

#ifndef XSTC_NO_ADA
    /**
    \brief ada lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexADA();
#endif //XSTC_NO_ADA

#ifndef XSTC_NO_APDL
    /**
    \brief apdl lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexAPDL();
#endif //XSTC_NO_APDL

#ifndef XSTC_NO_ASM
    /**
    \brief assembly lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexASM();
#endif //XSTC_NO_ASM

#ifndef XSTC_NO_ASN1
    /**
    \brief asn1 lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexASN1();
#endif //XSTC_NO_ASN1

#ifndef XSTC_NO_ASYMPTOTE
    /**
    \brief asymptote lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexASYMPTOTE();
#endif //XSTC_NO_ASYMPTOTE

#ifndef XSTC_NO_AU3
    /**
    \brief au3 lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexAU3();
#endif //XSTC_NO_AU3

#ifndef XSTC_NO_AVE
    /**
    \brief avenuelexcolor setup
    */
WXDLLIMPEXP_XSTC void LexAVE();
#endif //XSTC_NO_AVE

#ifndef XSTC_NO_BAAN
    /**
    \brief baan lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexBAAN();
#endif //XSTC_NO_BAAN

#ifndef XSTC_NO_BLITZBASIC
    /**
    \brief blitzbasic lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexBLBASIC();//all four use the same color defs, but doing this makes it easier to customize the coloring
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_FREEBASIC
    /**
    \brief freebasic lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexFRBASIC();
#endif //XSTC_NO_FREEBASIC

#ifndef XSTC_NO_POWERBASIC
    /**
    \brief powerbasic lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPOBASIC();
#endif //XSTC_NO_POWERBASIC

#ifndef XSTC_NO_PUREBASIC
    /**
    \brief purebasic lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPUBASIC();
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
    /**
    \brief bash lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexBASH();
#endif //XSTC_NO_BASH

#ifndef XSTC_NO_BATCH
    /**
    \brief batch lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexBATCH();
#endif //XSTC_NO_BATCH

#ifndef XSTC_NO_BULLANT
/* has no properties to set up
void LexBULLANT();
*/
#endif //XSTC_NO_BULLANT

#ifndef XSTC_NO_CAML
    /**
    \brief caml lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCAML();
#endif //XSTC_NO_CAML

#ifndef XSTC_NO_CLW
    /**
    \brief clw lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCLW();
#endif //XSTC_NO_CLW

#ifndef XSTC_NO_CMAKE
    /**
    \brief cmake lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCMAKE();
#endif //XSTC_NO_CMAKE

#ifndef XSTC_NO_COBOL
    /**
    \brief cobol lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCOBOL();
#endif //XSTC_NO_COBOL

#ifndef XSTC_NO_CONF
    /**
    \brief apache conf lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCONF();
#endif //XSTC_NO_CONF

#ifndef XSTC_NO_CPP
    /**
    \brief c/c++ lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCPP();
#endif //XSTC_NO_CPP

#ifndef XSTC_NO_CSOUND
    /**
    \brief csound lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCSOUND();
#endif //XSTC_NO_CSOUND

#ifndef XSTC_NO_CSS
    /**
    \brief css lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexCSS();
#endif //XSTC_NO_CSS

#ifndef XSTC_NO_D
    /**
    \brief d lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexD();
#endif //XSTC_NO_D

#ifndef XSTC_NO_DIFF
    /**
    \brief diff lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexDIFF();
#endif //XSTC_NO_DIFF

#ifndef XSTC_NO_EIFFEL
    /**
    \brief eiffel lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexEIFFEL();
#endif //XSTC_NO_EIFFEL

#ifndef XSTC_NO_EIFFELKW
    /**
    \brief eiffelkw lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexEIFFELKW();
#endif //XSTC_NO_EIFFELKW

#ifndef XSTC_NO_ERLANG
    /**
    \brief erlang lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexERLANG();
#endif //XSTC_NO_ERLANG

#ifndef XSTC_NO_ERRORLIST
    /**
    \brief errorlist lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexERRORLIST();
#endif //XSTC_NO_ERRORLIST

#ifndef XSTC_NO_ESCRIPT
    /**
    \brief escript lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexESCRIPT();
#endif //XSTC_NO_ESCRIPT

#ifndef XSTC_NO_F77
    /**
    \brief f77 lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexF77();
#endif //XSTC_NO_F77

#ifndef XSTC_NO_FLAGSHIP
    /**
    \brief flagship (clipper) lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexFLAGSHIP();
#endif //XSTC_NO_FLAGSHIP

#ifndef XSTC_NO_FORTH
    /**
    \brief forth lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexFORTH();
#endif //XSTC_NO_FORTH

#ifndef XSTC_NO_FORTRAN
    /**
    \brief fortran lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexFORTRAN();
#endif //XSTC_NO_FORTRAN

#ifndef XSTC_NO_GAP
    /**
    \brief gap lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexGAP();
#endif //XSTC_NO_GAP

#ifndef XSTC_NO_GUI4CLI
    /**
    \brief agui4cli lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexGUI4CLI();
#endif //XSTC_NO_GUI4CLI

#ifndef XSTC_NO_HASKELL
    /**
    \brief haskel lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexHASKELL();
#endif //XSTC_NO_HASKELL

#ifndef XSTC_NO_HTM
    /**
    \brief html php asp embeded javascript lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexHTM();
#endif //XSTC_NO_HTM

#ifndef XSTC_NO_INNOSETUP
    /**
    \brief inno installer lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexINNO();
#endif //XSTC_NO_INNOSETUP

#ifndef XSTC_NO_JS
    /**
    \brief javascript file (cpp lexer) lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexJS();
#endif //XSTC_NO_JS

#ifndef XSTC_NO_JAVA
    /**
    \brief java file (cpp lexer) lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexJAVA();
#endif //XSTC_NO_JAVA

#ifndef XSTC_NO_KIX
    /**
    \brief kix lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexKIX();
#endif //XSTC_NO_KIX

#ifndef XSTC_NO_LATEX
    /**
    \brief latexlexcolor setup
    */
WXDLLIMPEXP_XSTC void LexLATEX();
#endif //XSTC_NO_LATEX

#ifndef XSTC_NO_LISP
    /**
    \brief lisp lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexLISP();
#endif //XSTC_NO_LISP

#ifndef XSTC_NO_LOT
    /**
    \brief lot lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexLOT();
#endif //XSTC_NO_LOT

#ifndef XSTC_NO_LOUT
    /**
    \brief lout lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexLOUT();
#endif //XSTC_NO_LOUT

#ifndef XSTC_NO_LUA
    /**
    \brief lua lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexLUA();
#endif //XSTC_NO_LUA

#ifndef XSTC_NO_MAKEFILE
    /**
    \brief makefile lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMAKEFILE();
#endif //XSTC_NO_MAKEFILE

#ifndef XSTC_NO_MAGIK
    /**
    \brief magik lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMAGIK();
#endif //XSTC_NO_MAGIK

#ifndef XSTC_NO_MATLAB
    /**
    \brief matlab lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMATLAB();
#endif //XSTC_NO_MATLAB

#ifndef XSTC_NO_METAPOST
    /**
    \brief metapost lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMETAPOST();
#endif //XSTC_NO_METAPOST

#ifndef XSTC_NO_MMIXAL
    /**
    \brief mmixal lexcolor setup
    */
void LexMMIXAL();
#endif //XSTC_NO_MMIXAL

#ifndef XSTC_NO_NIMROD
    /**
    \brief nimrod lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexNIMROD();
#endif //XSTC_NO_NIMROD

#ifndef XSTC_NO_NNCRONTAB
    /**
    \brief nncrontab lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexNNCRONTAB();
#endif //XSTC_NO_NNCRONTAB

#ifndef XSTC_NO_NSIS
    /**
    \brief nsis lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexNSIS();
#endif //XSTC_NO_NSIS

#ifndef XSTC_NO_OCTAVE
    /**
    \brief octave lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexOCTAVE();
#endif //XSTC_NO_OCTAVE

#ifndef XSTC_NO_OPAL
    /**
    \brief opal lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexOPAL();
#endif //XSTC_NO_OPAL

#ifndef XSTC_NO_PAS
    /**
    \brief pascal/delphi lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPAS();
#endif //XSTC_NO_PAS

#ifndef XSTC_NO_PERL
    /**
    \brief perl lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPERL();
#endif //XSTC_NO_PERL

#ifndef XSTC_NO_PLM
    /**
    \brief plm lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPLM();
#endif //XSTC_NO_PLM

#ifndef XSTC_NO_PO
    /**
    \brief po lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPO();
#endif //XSTC_NO_PO

#ifndef XSTC_NO_POV
    /**
    \brief pov lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPOV();
#endif //XSTC_NO_POV

#ifndef XSTC_NO_POWERPRO
    /**
    \brief powerpro lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPOWERPRO();
#endif //XSTC_NO_POWERPRO

#ifndef XSTC_NO_POWERSHELL
    /**
    \brief powershell lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPOWERSHELL();
#endif //XSTC_NO_POWERSHELL

#ifndef XSTC_NO_PROGRESS
    /**
    \brief progress lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPROGRESS();
#endif //XSTC_NO_PROGRESS

#ifndef XSTC_NO_PROPERTIES
    /**
    \brief properties lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPROPERTIES();
#endif //XSTC_NO_PROPERTIES

#ifndef XSTC_NO_PS
    /**
    \brief ps lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPS();
#endif //XSTC_NO_PS

#ifndef XSTC_NO_PYTHON
    /**
    \brief python lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexPYTHON();
#endif //XSTC_NO_PYTHON

#ifndef XSTC_NO_R
    /**
    \brief r lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexR();
#endif //XSTC_NO_R

#ifndef XSTC_NO_REBOL
    /**
    \brief rebol lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexREBOL();
#endif //XSTC_NO_REBOL

#ifndef XSTC_NO_RUBY
    /**
    \brief ruby lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexRUBY();
#endif //XSTC_NO_RUBY

#ifndef XSTC_NO_SCRIPTOL
    /**
    \brief scriptol lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSCRIPTOL();
#endif //XSTC_NO_SCRIPTOL

#ifndef XSTC_NO_SMALLTALK
    /**
    \brief smalltalk lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSMALLTALK();
#endif //XSTC_NO_SMALLTALK

#ifndef XSTC_NO_SML
    /**
    \brief sml lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSML();
#endif //XSTC_NO_SML

#ifndef XSTC_NO_SORCUS
    /**
    \brief sorcus lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSORCUS();
#endif //XSTC_NO_SORCUS

#ifndef XSTC_NO_SPECMAN
    /**
    \brief specman lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSPECMAN();
#endif //XSTC_NO_SPECMAN

#ifndef XSTC_NO_SPICE
    /**
    \brief spice lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSPICE();
#endif //XSTC_NO_SPICE

#ifndef XSTC_NO_MSSQL
    /**
    \brief Microsoft sql lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMSSQL();
#endif //XSTC_NO_MSSQL

#ifndef XSTC_NO_MYSQL
    /**
    \brief Mysql lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexMYSQL();
#endif //XSTC_NO_MYSQL

#ifndef XSTC_NO_SQL
    /**
    \brief sql lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexSQL();
#endif //XSTC_NO_SQL

#ifndef XSTC_NO_TACL
    /**
    \brief tacl lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexTACL();
#endif //XSTC_NO_TACL

#ifndef XSTC_NO_TADS3
    /**
    \brief tads3 lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexTADS3();
#endif //XSTC_NO_TADS3

#ifndef XSTC_NO_TAL
    /**
    \brief tal lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexTAL();
#endif //XSTC_NO_TAL

#ifndef XSTC_NO_TCL
    /**
    \brief tcl lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexTCL();
#endif //XSTC_NO_TCL

#ifndef XSTC_NO_TEX
    /**
    \brief tex lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexTEX();
#endif //XSTC_NO_TEX

#ifndef XSTC_NO_VB
    /**
    \brief Visual Basic lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexVB();
#endif //XSTC_NO_VB

#ifndef XSTC_NO_VBSCRIPT
    /**
    \brief vb script lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexVBSCRIPT();
#endif //XSTC_NO_VBSCRIPT

#ifndef XSTC_NO_VERILOG
    /**
    \brief verilong lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexVERILOG();
#endif //XSTC_NO_VERILOG

#ifndef XSTC_NO_VHDL
    /**
    \brief vhdl lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexVHDL();
#endif //XSTC_NO_VHDL

#ifndef XSTC_NO_XCODE
    /**
    \brief xcode lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexXCODE();
#endif //XSTC_NO_XCODE

#ifndef XSTC_NO_XML
    /**
    \brief xml lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexXML();
#endif //XSTC_NO_XML

#ifndef XSTC_NO_YAML
    /**
    \brief yaml lexcolor setup
    */
WXDLLIMPEXP_XSTC void LexYAML();
#endif //XSTC_NO_YAML

#ifndef XSTC_NO_CONFIG
    /**
    \brief pass a configuration object so that XSTC can load external settings
    
    \detailed the XSTC namespace is used in the configuration files.
              XSTC/COLOR/ is used for all coloring/styling options
              XSTC/EXT/ is used for the extension and property options
    */
WXDLLIMPEXP_XSTC void SetColorConf(wxConfigBase* clrconf, wxString root = wxT("//"));
#endif //XSTC_NO_CONFIG

#ifndef XSTC_NO_TRIMTRAIL
    /**
    \brief trims all trailing spaces and tabs in the document

    \detailed if trimspaces is set to true, then this will be called in savefileX.
    */
WXDLLIMPEXP_XSTC void TrimTrailing();
#endif //XSTC_NO_TRIMTRAIL

#ifndef XSTC_NO_TABSPACE
    /**
    \brief converts tabs to spaces or vice verca

    \detailed tabs=true tabs to spaces else spaces to tabs
              int spaces, is the number of spaces to equal one tab
    */
WXDLLIMPEXP_XSTC void TabSpace(bool tabs, int spaces);
#endif //XSTC_NO_TABSPACE

    /**
    \brief sets up the folding matgin mask for square markers
    */
WXDLLIMPEXP_XSTC void FoldBox();

    /**
    \brief sets up the folding margin mask for circle markers
    */
WXDLLIMPEXP_XSTC void FoldCircle();

    /**
    \brief sets the folding margin mask for arrow markers
    */
WXDLLIMPEXP_XSTC void FoldArrow(bool lines = true);

    /**
    \brief sets the foldimg margin mask for +- markers
    */
WXDLLIMPEXP_XSTC void FoldSimple(bool lines = true);

    /**
    \brief sets the foldimg marker colors
    */
WXDLLIMPEXP_XSTC void FoldColors();

    /**
    \brief converts a string representing a lexer to an int

    \detailed the string can represent a number or lexer keyword, numbers are converted
           and passed verbatim, the keyword is converted to its respective lexer
    */
WXDLLIMPEXP_XSTC int GetEXTLexer(wxString extparam);


    /**
    \brief checks if the entry has any characters that are invalid for use in a filename

    \detailed doens't do any well formed checks, just looks for characters
    */
WXDLLIMPEXP_XSTC bool FilevalidCheck(wxString entry);

    /**
    \brief resets all markers to default

    \detailed this is an easy way to clear the slate, it only resets markers managed my XSTC
    */
WXDLLIMPEXP_XSTC void ResetMarkers();

#ifndef XSTC_NO_CONFIG
    /**
    \brief converts the input string to a color. this is used in the configuration code for loading in colors.

    \detailed takes a valid color string and converts it to a color. if the color database was set a name can
              be used if it is in the database. otherwize an html or rgb string can be used #RRGGBB or rgb(r, g, b)
              the rgb string can be used with or without the rgb prefix, but that is how the wxColour class takes it.
    */
WXDLLIMPEXP_XSTC bool Ccolor(wxString configvalue, wxColour& color);

    /**
    \brief checks the value of a config color setting and converts it to a string defaults to #RRGGBB

    \detailed converts the input value to a specified output value. it uses Ccolor() and then gets
              the appropriate type string. defaults to html color string.
              type used the wxColour flags which are:
              wxC2S_NAME          red
              wxC2S_CSS_SYNTAX    rgb(255, 0, 0)
              wxC2S_HTML_SYNTAX   #FF0000
    */
WXDLLIMPEXP_XSTC bool CcolorS(wxString configvalue, wxString& color, long type = wxC2S_HTML_SYNTAX);

    /**
    \brief gives the user a pointer to the color database, XSTC still destroys it internally.

    \detailed the color string option for Ccolor and CcolorS are not availible till this is called, because
              no database has been created. it will send a new pointer if the database was already created.
              (this function was already called) the database agrument may be NULL if pass is false. this lets
              you just create and get the pointer later if you need it.
    */
WXDLLIMPEXP_XSTC void SetColorDbase(wxColourDatabase& dbase);

#endif //XSTC_NO_CONFIG

/**********************************************************************************************************************
***********************************************Variables here***********************************************************
***********************************************************************************************************************/

    /**
    \brief a status result for file load and save functions
    */
WXDLLIMPEXP_XSTC enum //filestatus
	{
	FILE_OK = 0,
	FILE_NOT_FOUND = 1,
	FILE_SAVE_ERROR = 2
	};

    /**
    \brief a status result for file load and save functions
    */
WXDLLIMPEXP_XSTC enum //savemode
	{
	SAVE_AS_NEVER = 0,
	SAVE_AS_ALWAYS = 1,
	SAVE_AS_ON_FAIL = 3
	};

#ifndef XSTC_NO_CONFIG
    /**
    \brief for external extention conf

    \detailed this when set to true tells the class to search an external
              configuration file for extentions and which lexer it should be assigned
              if no assignment is found for the current extention, the default is used
              or null_lexer when there is no assignment also you can use a strings
              rather than a file should that be nessicary
    */
WXDLLIMPEXP_XSTC bool useext;

    /**
    \brief for external coloring conf

    \detailed this when set to true tells the class to use an external conf
              for colorization or a string
    */
WXDLLIMPEXP_XSTC bool usecolor;
	
	/**
	\brief whether to set properties from config file
	*/
WXDLLIMPEXP_XSTC bool useprops;
#endif //XSTC_NO_CONFIG

    /**
    \brief for fold margin functions

    \detailed set to true and the marker will be styled as outline, if you
              use a config color setting to overwrite the colors, this may
              have no effect. (the foreground color makes the outline)
    */
WXDLLIMPEXP_XSTC bool markoutline;

    /**
    \brief for fold margin functions

    \detailed sets the shape of the marker when using foldmargin functions
              use the same value as the #defines wxSTC_MARK_CIRCLE
              if using char, you must add the char value to the marker
              1000+(ascii)char value defaults to circle
    */
WXDLLIMPEXP_XSTC int markshape;

    /**
    \brief sets the fold line after/before contracted/expanded

    \detailed defaults to line after contracted
    */
WXDLLIMPEXP_XSTC int foldline;

#ifndef XSTC_NO_TRIMTRAIL
    /**
    \brief trim trailing spaces/tabs on save

    \detailed when calling saveX and this is set to true, TrimTrailing() will
              called. for resources and effectiveness this seems to be the
              best approach to trim the end spaces.
    */
WXDLLIMPEXP_XSTC bool trimspaces;
#endif //XSTC_NO_TRIMTRAIL

#ifndef XSTC_NO_TABSPACE
    /**
    \brief covert spaces/tabs to settings on save

    \detailed when calling saveX and this is set to an active mode the whitespace
	          will be converted accordingly.
    */
WXDLLIMPEXP_XSTC int spaceconv; //0 off, 1 space to tab, 2 tab to space, >2 off

    /**
    \brief this is the # of spaces in a tab for TabSpace()
    */
WXDLLIMPEXP_XSTC int spaces;
#endif //XSTC_NO_TABSPACE

#ifndef XSTC_NO_KEYS
    /**
    \brief for loading keywords

    \detailed if true keywords will be loaded, default is keys_set strings, but
              you can either pass your own keywords to keycheck() or send it a
              filename. alternatly you can use loadkeywords() specifically if you
              KNOW that a file will alwayse be used. internally keycheck calls
              loadkeywords if a '.' dot is present in the string
    */
WXDLLIMPEXP_XSTC bool usekeys;
#endif

    /**
    \brief sets the default coloring for c/c++

    \detailed holds a specific string if a colorstyle function was called
              dark classic visualstudio borland zenburn jeff matrix
              it defaults to 'NULL' that is basically just ignore the colors
              that are set, make defaults. or a property string that can be
              sent to GetPropString() can be used for the fg_bg colors. this
              gives a small degree of flexability to the lexcolor functions
              as that property is seen as the whitespace color and they will
              try to adjust the color so any direct conflicts are avoided.
              this won't work too well cus the program doesn't have eyes, but
              its workth a try.
    */
WXDLLIMPEXP_XSTC wxString colorstyle; //this is for the fold marker coloring and to programmically determing
                         //if/what the setting is
    /**
    \brief color of the edge column

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC wxColour edgcolr;

    /**
    \brief column number for the edge column

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC int edgcolm;

    /**
    \brief mode of the edge column

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC int edgmod;

    /**
    \brief mode for text wrap

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC int wrapmod;

    /**
    \brief visual flags for text wrap

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC int wrapvf;

    /**
    \brief cache mode for the document

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC int cachemod;

    /**
    \brief visibility fot the cater background highlight

    \detailed used in the colorstyle functions
    */
WXDLLIMPEXP_XSTC bool caretlv;

    /**
    \brief case sensitive cpp lexer

    \detailed used in extention to lexer setting, defaults true
    */
WXDLLIMPEXP_XSTC bool cppcase;

    /**
    \brief case sensitive clw lexer

    \detailed used in extention to lexer setting, defaults true
    */
WXDLLIMPEXP_XSTC bool clwcase;

#ifndef XSTC_NO_ALPHA
//this macro is needed since scintilla didn't support alpha blending till version 1.68
    /**
    \brief alpha blending level for bookmarks, caret backgrounda and text selection

    \detailed this value sets all three to the same alpha setting, use 256 to turn
              blending off (full visible), the config settings set them individually
    */
WXDLLIMPEXP_XSTC int alphalvl;
#endif //XSTC_NO_ALPHA

    /**
    \brief filename of the loaded file

    \detailed if no file is loaded or CloseFile() was called, this is 'Untitled'
              the file name of the loaded file is stored here if loadfileX is
              used, that will contain the full path as passed to it, otherwize you
              set it and it is whatever you make it out to be
    */
WXDLLIMPEXP_XSTC wxString XFilename;

#ifndef XSTC_NO_KEYS
    /**
    \brief a set of strings that hold a default set of keywords

    \detailed this also supports overwriting the keywords using spaces as a delimiter
              or a filename, a '.' dot is used to determine if it is a filename
    */
WXDLLIMPEXP_XSTC Keyword_Sets Keys_Set;
#endif

    /**
    \brief a few strings to represent colors for background markers

    \detailed these strings are a convienient way to store the colors
              for various background markers. you set the marker and
              the color, but these will be updated when a colorstyle
              function is called.
              brkptcol      breakpoint
              actbrkptcol   active breakpoint
              errorcol      error
              more may come, i haven't thought too much of it yet. any suggestions?
    */
WXDLLIMPEXP_XSTC wxString brkptcol, actbrkptcol, errorcol;

 private:
    /**
    \brief handles brace matching

    \detailed matches braces for these symbols {} () [] <>
              also with optional uniqe colors for each brace type
              uniqe per bracetype is under a delayed investigation
              they might need to all be the same, i do hope i can
              figure it out.
    */
    void Bmatch(XSTC_EVENT& event);

    /**
    \brief Handles margin click events

    \detailed calls ToggleBookmark() or togles a fold dependong on wich margin
              was clicked
    */
    void MarginClicked(XSTC_EVENT& event);

    /**
    \brief determins which lexer to use

    \detailed using the file extentions, this returns the correct lexer or
              LEX_NULL if error and prop1 is set to default behavior for that lexer
              as internalt designated the information defined here is used in
              setlexerX. also by loading a set of properties and extentions into
              extset some extentions can be redefined or unsuppoted extentions
              given a lexer other than null
    */
    int AutoEXT(wxString filename);//returns the lexer number

#ifndef XSTC_NO_CONFIG
    /**
    \brief checks if entry (file extention) was loaded into extset (handled in conf file)

    \detailed returns true if it was and sets dest to the handled properties
              if it returns false dest is emptied
    */
    bool IsConfEXT(wxString extension, int &lexer);//was the extention loaded from conf?

	/**
	\brief set properties from config database
	*/
	bool SetProperties(wxString extension);
#endif //XSTC_NO_CONFIG

    /**
    \brief toggles the numbered bookmarks

    \detailed checks the keys and toggles or goto the appropriate bookmark.
    */
    void MarkerToggle(wxKeyEvent& event);
	
	/**
    \brief handles other keypress events than numbered bookmarks
    */
	void KeyParse(wxKeyEvent& event);

/**********************************************************************************************************************
***********************************************Variables here***********************************************************
***********************************************************************************************************************/

    
    /**
    \brief holds the width for the internally mapped margins
    */
    int linemgn, symmgn, fldmgn;//these store the number for the left margins

    /**
    \brief margin width settings

    \detailed widths for settings margins
    */
    int linew, symw, fldw;

#ifndef XSTC_NO_CONFIG
    /**
    \brief the color database that XSTC uses for name to color

    \detailed this is not readily useable and GetColorDbase() must be called first
              that way unneded memory is not allocated. remember is name strings are used
              then color setting will fail on those entries when this is not initialized.
              the user can add entries as they wish, because a pointer is passed with the
              function call.
    */
    wxColourDatabase* XSTCcolorDbase;
    
    /**
    \brief user set location for the XSTC config. defaults to '/'
    */
    wxString confroot;
#endif //XSTC_NO_CONFIG

    /**
    \brief stores the handle to numbered bookmarks

    \detailed stored bookmark handles for the numbered bookmarks so they can be found when the BmarkPos becomes invalid.
    */
    int BmarkHandles[10];

    /**
    \brief all of the names of the config based colorstyles

    \detailed the strings are read from "XSTC/COLOR/MISIC/CSTYLES" a coma delimited list of names.
              the names are used to utilize the config based colorstyle settings. since searching for the styles is not an effective
              means of knowing what ones are present. any undefined settings will be set to the default color of black on white.
    */
    wxString CStyles;

    /**
    \brief c/c++ extention array

    \detailed a space saving way to check the c++ file extentions in AutoEXT()
    */
    wxString c_ext_array[8];

    /**
    \brief these variables are for lexcolor functions, this way there is a fast easy way to chnge default in all functions at once.

    \detailed  these are strings that are used in StyleSetSpec()
    */
    wxString XS_comment,
             XS_comment2,
             XS_comment3,
             XS_s_string,
             XS_d_string,
             XS_number,
             XS_char,
             XS_default,
             XS_key1,
             XS_key2,
             XS_key3,
             XS_key4,
             XS_preproc,
             XS_symbol,
             XS_tag,
             XS_user,
             XS_misic,
             XS_lang,
             XS_global,
             XS_bad, //illegal, error
             XS_instruction, //whatever
             XS_dtype; //datatype

    /**
    \brief fold margin foreground color string

    \detailed to simplify code, the colorstyle function sets this value instead of checking for
              the value of the 'colorstyle' string in each fold setup function
    */
    wxString foldfg;

    /**
    \brief fold margin background color string

    \detailed to simplify code, the colorstyle function sets this value instead of checking for
              the value of the 'colorstyle' string in each fold setup function
    */
    wxString foldbg;

    /**
    \brief holds the id passed to the constructor

    \detailed The window id is needed to dynamically connect events to this
              instance of the editor. its here so you can use it if you need
    */
    wxWindowID WIN_ID; //this way we can use connect to dynamically connect events to our
	                   //component and the user still has the option over what the id
	                   //setting is.
#ifndef XSTC_NO_CONFIG
    /**
    \brief pointer to the color config object

    \detailed XSTC does not write any values to this object, it only reads them
              define color structure!!
    */
     wxConfigBase* colorconf;
#endif //XSTC_NO_CONFIG
};

#endif // XSTC_H

