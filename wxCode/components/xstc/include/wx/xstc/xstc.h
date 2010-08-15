
/*
 * xstc.h
 * ______________________________________________________________
 * This software is dual licensed under either the GNU GPL
 * version 2 or at your option any other version
 * ______________________________________________________________
 * or the wxWindows license.
 * Just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are.
 * ______________________________________________________________
 *
 * Visit: http://opensource.org/ to see both-
 * the GNU GPL and wxWindows licences.
 *
 * This software has absolutely no warranty, express or implied.
 *
 * Just so you know, I don't care if you change the code
 * don't email me if you did someting to it.
 *
 * If you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * You must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (Except for any code you add obviously)
 *
 * This notice may not be changed in any way and must remain at the top of every
 * source file.
 *
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2006
 */

#ifndef XSTC_H
#define XSTC_H

#include "util.h"

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
#include <wx/confbase.h>
#endif //XSTC_NO_CONFIG

#ifndef XSTC_NO_KEYS
	/**
	\brief Holds keywords for the lexers

	Each of the string names representing these strings in PurgeKeys()
		are the same as the name of the identifier. Identifiers are named thus:
		name
		name1
			and for names with trailing numbers trailing numbers:
			namex_0
			namex_1

	\struct Keyword_Sets
	*/
//Should hold either default keywords OR a filename listing keywords.
struct WXDLLIMPEXP_XSTC Keyword_Sets
{
/*
 *If "^ISFILE^" is in the string, a filename, or list of filenames is assumed.
 *KeyCheck() checks this and loads the file if the file is found.
 *Versions of Scintilla where a lexer was added is marked above each key set.
 *Comments in place of language without keyword support.
 */
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
//MARKDOWN 2.20
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
//TXT2TAGS 2.20
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
	\brief Holds two string values for lexer properties.

	A simple way to pass property strings around as one unit
		then split the L/R values when the property needs to be
		sent to scintilla.

	\struct PropSTR
	*/
//Holds the right and left values of a property string
struct WXDLLIMPEXP_XSTC PropSTR
{
	//The 'side' is delimited by the '_' underscore character
	wxString L;//left
	wxString R;//right
};

/*
 *There are two ways to edit coloring of a lexer. First find
 *the source that sets up the lexer styles and manualy edit the entries.
 *Otherwise if you want to dynamically allow color changes, you may want to just
 *make a conf file to take care of it for you. Obviously you can change
 *the code and make a conf file at the same time.
 */

	/**
	\class XSTC
	\brief XSTC automates some of Scintilla and adds some other functionality as well.

	XSTC is a wrapper for wxScintilla/wxSTC. Its purpose is to make getting
	started and using wxScintilla easy.

	It has a Scintilla specific configuration
	model already to go, including per lexer style configuration, coloring
	themes that keep all language highlighting looking the same.
	It automatically sets up the margins, setting up the folding margin symbols
	is as simple as a function call. A user bookmark is already to go as well
	as folding for all languages that support it. A set of numbered bookmarks
	0-9 is ready to use. Lexers are set automatically when a file is loaded.

	In addition to the built in theme functions that take care of syntax coloring,
	themes can be loaded from the config database. You can set up a color
	database and use string names for colors. Also some utility functions are
	avaiible to do quick document processing. Optionally keywords can be managed
	automatically and the sets of words can be easily replaced at runtime without
	being overwriten by XSTC.
	
	Aside from much of the repetative stuff XSTC also handles
	setting up some of the more generic or lesser obvious functionalities as well.
	Besides XSTC, there are also several other helper classes availible like
	a macro record/playback module or a document exchanger. (For using 1 Scintilla
	instance with several documents at the same time.)
	See my other work for more details. Nuklear Zelph.
	*/
class WXDLLIMPEXP_XSTC XSTC: public XSTC_CLASS
{
public:

	/**
	\brief initializes XSTC internal variables

	and sets up all the defaults, line ending based on platform, initializes
	the margins, if keyword support is turned on it will initialize all of those
	variables.
	*/
	WXDLLIMPEXP_XSTC XSTC(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, const wxString& name = wxT("XSTC"));

	/**
	\brief probably will always remain empty
	*/
	WXDLLIMPEXP_XSTC virtual ~XSTC();

//The load and save functions here wrap the scintilla functins and allow for other
//actions to take place.
	/**
	\brief Wrapper for wxScintilla file load routine, for automatic coloring.

	loads a file through scintilla then sets up the lexer
		unless setlexer is false, then the current lexer will
		color the document. Also returns a status message.
	*/
	WXDLLIMPEXP_XSTC long LoadFileX(wxString filename, bool setlexer = false);

	/**
	\brief Wrapper for wxScintilla file save routine. Used to invoke any util functions.

	if trimming space, space to tab or any other utility functions are active
		they will be called in this function.

		savemode is a three mode flag, for utilizing a saveas dialog automatically.
		never always or on failed attempt of the used filename.

		If no filename is passed then the internal "XFilename" copy is used
		otherwise it uses the argument variable.
	*/
	WXDLLIMPEXP_XSTC long SaveFileX(const wxString filename = wxT(""), long saveas = SAVE_AS_NEVER);

	/**
	\brief clears the editor

	clears the editor sets lexer to NULL and Filename to 'Untitled'
		also returns a status message.
	*/
	//Kindof an extended Clear() function for the same purpose as the file wrappers.
	WXDLLIMPEXP_XSTC void CloseFile();

#ifndef XSTC_NO_KEYS
	/**
	\brief Sets all keywords to ""
	*/
	WXDLLIMPEXP_XSTC void PurgeKeys();

	/**
	\brief Simply it resets ALL of the keys_set strings to default
	*/
	WXDLLIMPEXP_XSTC void InitKeys();
#endif //XSTC_NO_KEYS

	/**
	\brief Toggles a bookmark

	Toggles a bookamrk at the specified line, internaly this is the
		line where the margin was clicked
	*/
	WXDLLIMPEXP_XSTC void ToggleBookMark(int line);

	/**
	\brief Use this to rearrange the default margin order
	*/
	WXDLLIMPEXP_XSTC void MapMargins(int line, int symbol, int fold);

	/**
	\brief Sets widths of the mapped margins

	If you don't map margins then you don't need to use this to change the width,
	but if you do it is a good idea to use this function instead of doing it yourself.
	*/
	WXDLLIMPEXP_XSTC void SetMgnsWidth(int line, int symbol, int fold);

#ifndef XSTC_NO_KEYS
	/**
	\brief Load keywords from a file

	Will convert newline delimited to space delimited, or pass verbatim in
	it is aleady space delimited.
	*/
	WXDLLIMPEXP_XSTC wxString LoadKeyWords(wxString filename);
#endif //XSTC_NO_KEYS

	/**
	\brief Colors on black background theme.
	*/
	WXDLLIMPEXP_XSTC void DarkStyle();

	/**
	\brief Attempts to emulate visual studio coloring

	I don't use VS so I copied these settings with
	some creativity for readability from Dev-C++.
	*/
	WXDLLIMPEXP_XSTC void VisualStudioStyle();

	/**
	\brief Attempts to emulate classic colors (whatever that means)

	Dunno I copied these settings with some
	creativity for readability from Dev-C++
	*/
	WXDLLIMPEXP_XSTC void ClassicStyle();

	/**
	\brief Attempts to emulate the old borland TUI ide
	*/
	WXDLLIMPEXP_XSTC void BorlandStyle();

	/**
	\brief A style I found on the internet

	I found it on codinghorror in "is you're ide hot or not"
	*/
	WXDLLIMPEXP_XSTC void JeffStyle();

	/**
	\brief a style i found on the internet

	I found it on codinghorror in "is you're ide hot or not"
	*/
	WXDLLIMPEXP_XSTC void ZenburnStyle();

	/**
	\brief A style based on the Matrix

	I saw a matrix based color scheme and decided to make one too.
	*/
	WXDLLIMPEXP_XSTC void MatrixStyle();

	/**
	\brief Loads a theme from the config database.

	Does the same thing that the built in functions do and will set any undefined
	styles to the default black on white. Also for convienience the built in
	theme functions are caled if their name is passed.

	*/
	WXDLLIMPEXP_XSTC void ConfigStyle(wxString style);

	/**
	\brief Look up what names of the themes that reside in the config database.

	Returns a wxArrayString of all names that are in the config CStyles string.
	*/
	WXDLLIMPEXP_XSTC wxString GetConfNames();

	/**
	\brief Sets everything back to default Black on white and clears any extras.

	Changes the colorstyle string to "NULL" and changes
	the margin colors back to button face (or whatever)
	*/
	WXDLLIMPEXP_XSTC void ResetStyle();

#ifndef XSTC_NO_CONFIG
	/**
	\brief Configuration loading for non-language related styles
	*/
	WXDLLIMPEXP_XSTC void StyleConf();

	/**
	\brief Configuration loading for the fold margin symbol styles.
	*/
	WXDLLIMPEXP_XSTC void FoldConf();
#endif //XSTC_NO_CONFIG

	/**
	\brief Sets up the lexer, keywords and properties
	*/
	WXDLLIMPEXP_XSTC void SetLexerX(int lexer);

	/**
	\brief Splits a simple "option" string in two.

	A PropSTR is a small struct that holds a name and value pair.
	*/
	WXDLLIMPEXP_XSTC PropSTR GetPropStr(wxString property);

#ifndef XSTC_NO_KEYS
	/**
	\brief Checks to see if this is a file or a set of keywords.

	If "^ISFILE^" is contained in the string then the file(s) are loaded
	and the lexers approptiate wordlist is updated. The list is coma delimited.
	If that literal is not found then it is assumed to be a wordlist and is
	loaded directly. Do note: if you don't want the overhead of XSTC storing
	all of the keywords in memory, you can set the appropriate strings to be a
	filename and that file will be loaded when appropriate. Keep in mind that
	file loading will always take place in this instance.

	One way to reduce the overhead and still keep automatic keyword support is
	to hand modify the keyword list. (Even if that means you empty them)
	#XSTC_NO_LANGNAME options to turn off languages is another option, but
	that lexer will never auto-load/colorize. You would have to do that yourself.
	*/
	WXDLLIMPEXP_XSTC wxString KeyCheck(wxString keyname);
#endif //XSTC_NO_KEYS

#ifndef XSTC_NO_ABAQUS
	/**
	\brief Abaqus lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexABAQUS();
#endif //XSTC_NO_ABAQUS

#ifndef XSTC_NO_ADA
	/**
	\brief Ada lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexADA();
#endif //XSTC_NO_ADA

#ifndef XSTC_NO_APDL
	/**
	\brief APDL lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexAPDL();
#endif //XSTC_NO_APDL

#ifndef XSTC_NO_ASM
	/**
	\brief Assembly lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexASM();
#endif //XSTC_NO_ASM

#ifndef XSTC_NO_ASN1
	/**
	\brief ASN1 lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexASN1();
#endif //XSTC_NO_ASN1

#ifndef XSTC_NO_ASYMPTOTE
	/**
	\brief Asymptote lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexASYMPTOTE();
#endif //XSTC_NO_ASYMPTOTE

#ifndef XSTC_NO_AU3
	/**
	\brief Au3 lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexAU3();
#endif //XSTC_NO_AU3

#ifndef XSTC_NO_AVE
	/**
	\brief Avenue lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexAVE();
#endif //XSTC_NO_AVE

#ifndef XSTC_NO_BAAN
	/**
	\brief Baan lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexBAAN();
#endif //XSTC_NO_BAAN

#ifndef XSTC_NO_BLITZBASIC
	/**
	\brief Blitzbasic lexcolor setup
	*/
	//All four use the same color defs, but doing this makes it easier to customize
	//the coloring for each dialect.
	WXDLLIMPEXP_XSTC void LexBLBASIC();
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_FREEBASIC
	/**
	\brief Freebasic lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexFRBASIC();
#endif //XSTC_NO_FREEBASIC

#ifndef XSTC_NO_POWERBASIC
	/**
	\brief Powerbasic lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPOBASIC();
#endif //XSTC_NO_POWERBASIC

#ifndef XSTC_NO_PUREBASIC
	/**
	\brief Purebasic lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPUBASIC();
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
	/**
	\brief Bash (Unix shell) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexBASH();
#endif //XSTC_NO_BASH

#ifndef XSTC_NO_BATCH
	/**
	\brief Batch (MS-Dos shell) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexBATCH();
#endif //XSTC_NO_BATCH

#ifndef XSTC_NO_CAML
	/**
	\brief Caml lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCAML();
#endif //XSTC_NO_CAML

#ifndef XSTC_NO_CLW
	/**
	\brief CLW lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCLW();
#endif //XSTC_NO_CLW

#ifndef XSTC_NO_CMAKE
	/**
	\brief Cmake lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCMAKE();
#endif //XSTC_NO_CMAKE

#ifndef XSTC_NO_COBOL
	/**
	\brief Cobol lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCOBOL();
#endif //XSTC_NO_COBOL

#ifndef XSTC_NO_CONF
	/**
	\brief Apache Conf lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCONF();
#endif //XSTC_NO_CONF

#ifndef XSTC_NO_CPP
	/**
	\brief C/C++ lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCPP();
#endif //XSTC_NO_CPP

#ifndef XSTC_NO_CSOUND
	/**
	\brief Csound lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCSOUND();
#endif //XSTC_NO_CSOUND

#ifndef XSTC_NO_CSS
	/**
	\brief CSS lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexCSS();
#endif //XSTC_NO_CSS

#ifndef XSTC_NO_D
	/**
	\brief D lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexD();
#endif //XSTC_NO_D

#ifndef XSTC_NO_DIFF
	/**
	\brief Diff lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexDIFF();
#endif //XSTC_NO_DIFF

#ifndef XSTC_NO_EIFFEL
	/**
	\brief Eiffel lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexEIFFEL();
#endif //XSTC_NO_EIFFEL

#ifndef XSTC_NO_EIFFELKW
	/**
	\brief Eiffelkw lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexEIFFELKW();
#endif //XSTC_NO_EIFFELKW

#ifndef XSTC_NO_ERLANG
	/**
	\brief Erlang lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexERLANG();
#endif //XSTC_NO_ERLANG

#ifndef XSTC_NO_ERRORLIST
	/**
	\brief Errorlist lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexERRORLIST();
#endif //XSTC_NO_ERRORLIST

#ifndef XSTC_NO_ESCRIPT
	/**
	\brief Escript lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexESCRIPT();
#endif //XSTC_NO_ESCRIPT

#ifndef XSTC_NO_F77
	/**
	\brief F77 lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexF77();
#endif //XSTC_NO_F77

#ifndef XSTC_NO_FLAGSHIP
	/**
	\brief Flagship (clipper) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexFLAGSHIP();
#endif //XSTC_NO_FLAGSHIP

#ifndef XSTC_NO_FORTH
	/**
	\brief Forth lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexFORTH();
#endif //XSTC_NO_FORTH

#ifndef XSTC_NO_FORTRAN
	/**
	\brief Fortran lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexFORTRAN();
#endif //XSTC_NO_FORTRAN

#ifndef XSTC_NO_GAP
	/**
	\brief Gap lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexGAP();
#endif //XSTC_NO_GAP

#ifndef XSTC_NO_GUI4CLI
	/**
	\brief Gui4cli lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexGUI4CLI();
#endif //XSTC_NO_GUI4CLI

#ifndef XSTC_NO_HASKELL
	/**
	\brief Haskel lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexHASKELL();
#endif //XSTC_NO_HASKELL

#ifndef XSTC_NO_HTM
	/**
	\brief HTML, PHP, ASP, embeded Javascript lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexHTM();
#endif //XSTC_NO_HTM

#ifndef XSTC_NO_INNOSETUP
	/**
	\brief Inno Installer lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexINNO();
#endif //XSTC_NO_INNOSETUP

#ifndef XSTC_NO_JS
	/**
	\brief Javascript file (cpp lexer) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexJS();
#endif //XSTC_NO_JS

#ifndef XSTC_NO_JAVA
	/**
	\brief Java file (cpp lexer) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexJAVA();
#endif //XSTC_NO_JAVA

#ifndef XSTC_NO_KIX
	/**
	\brief Kix lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexKIX();
#endif //XSTC_NO_KIX

#ifndef XSTC_NO_LATEX
	/**
	\brief Latex lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexLATEX();
#endif //XSTC_NO_LATEX

#ifndef XSTC_NO_LISP
	/**
	\brief Lisp lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexLISP();
#endif //XSTC_NO_LISP

#ifndef XSTC_NO_LOT
	/**
	\brief Lot lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexLOT();
#endif //XSTC_NO_LOT

#ifndef XSTC_NO_LOUT
	/**
	\brief Lout lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexLOUT();
#endif //XSTC_NO_LOUT

#ifndef XSTC_NO_LUA
	/**
	\brief Lua lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexLUA();
#endif //XSTC_NO_LUA

#ifndef XSTC_NO_MAKEFILE
	/**
	\brief Makefile lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMAKEFILE();
#endif //XSTC_NO_MAKEFILE

#ifndef XSTC_NO_MAGIK
	/**
	\brief Magik lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMAGIK();
#endif //XSTC_NO_MAGIK

#ifndef XSTC_NO_MARKDOWN
	/**
	\brief Markdown lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMARKDOWN();
#endif //XSTC_NO_MARKDOWN

#ifndef XSTC_NO_MATLAB
	/**
	\brief Matlab lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMATLAB();
#endif //XSTC_NO_MATLAB

#ifndef XSTC_NO_METAPOST
	/**
	\brief Metapost lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMETAPOST();
#endif //XSTC_NO_METAPOST

#ifndef XSTC_NO_MMIXAL
	/**
	\brief Mmixal lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMMIXAL();
#endif //XSTC_NO_MMIXAL

#ifndef XSTC_NO_NIMROD
	/**
	\brief Nimrod lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexNIMROD();
#endif //XSTC_NO_NIMROD

#ifndef XSTC_NO_NNCRONTAB
	/**
	\brief Nncrontab lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexNNCRONTAB();
#endif //XSTC_NO_NNCRONTAB

#ifndef XSTC_NO_NSIS
	/**
	\brief NSIS lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexNSIS();
#endif //XSTC_NO_NSIS

#ifndef XSTC_NO_OCTAVE
	/**
	\brief Octave lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexOCTAVE();
#endif //XSTC_NO_OCTAVE

#ifndef XSTC_NO_OPAL
	/**
	\brief Opal lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexOPAL();
#endif //XSTC_NO_OPAL

#ifndef XSTC_NO_PAS
	/**
	\brief Pascal/Delphi lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPAS();
#endif //XSTC_NO_PAS

#ifndef XSTC_NO_PERL
	/**
	\brief Perl lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPERL();
#endif //XSTC_NO_PERL

#ifndef XSTC_NO_PLM
	/**
	\brief PLM lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPLM();
#endif //XSTC_NO_PLM

#ifndef XSTC_NO_PO
	/**
	\brief PO lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPO();
#endif //XSTC_NO_PO

#ifndef XSTC_NO_POV
	/**
	\brief POV lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPOV();
#endif //XSTC_NO_POV

#ifndef XSTC_NO_POWERPRO
	/**
	\brief PowerPro lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPOWERPRO();
#endif //XSTC_NO_POWERPRO

#ifndef XSTC_NO_POWERSHELL
	/**
	\brief PowerShell lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPOWERSHELL();
#endif //XSTC_NO_POWERSHELL

#ifndef XSTC_NO_PROGRESS
	/**
	\brief Progress lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPROGRESS();
#endif //XSTC_NO_PROGRESS

#ifndef XSTC_NO_PROPERTIES
	/**
	\brief Properties lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPROPERTIES();
#endif //XSTC_NO_PROPERTIES

#ifndef XSTC_NO_PS
	/**
	\brief PS (Post Script) lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPS();
#endif //XSTC_NO_PS

#ifndef XSTC_NO_PYTHON
	/**
	\brief Python lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexPYTHON();
#endif //XSTC_NO_PYTHON

#ifndef XSTC_NO_R
	/**
	\brief R lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexR();
#endif //XSTC_NO_R

#ifndef XSTC_NO_REBOL
	/**
	\brief Rebol lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexREBOL();
#endif //XSTC_NO_REBOL

#ifndef XSTC_NO_RUBY
	/**
	\brief Ruby lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexRUBY();
#endif //XSTC_NO_RUBY

#ifndef XSTC_NO_SCRIPTOL
	/**
	\brief Scriptol lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSCRIPTOL();
#endif //XSTC_NO_SCRIPTOL

#ifndef XSTC_NO_SMALLTALK
	/**
	\brief Smalltalk lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSMALLTALK();
#endif //XSTC_NO_SMALLTALK

#ifndef XSTC_NO_SML
	/**
	\brief SML lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSML();
#endif //XSTC_NO_SML

#ifndef XSTC_NO_SORCUS
	/**
	\brief Sorcus lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSORCUS();
#endif //XSTC_NO_SORCUS

#ifndef XSTC_NO_SPECMAN
	/**
	\brief Specman lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSPECMAN();
#endif //XSTC_NO_SPECMAN

#ifndef XSTC_NO_SPICE
	/**
	\brief Spice lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSPICE();
#endif //XSTC_NO_SPICE

#ifndef XSTC_NO_MSSQL
	/**
	\brief Microsoft SQL lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMSSQL();
#endif //XSTC_NO_MSSQL

#ifndef XSTC_NO_MYSQL
	/**
	\brief MySQL lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexMYSQL();
#endif //XSTC_NO_MYSQL

#ifndef XSTC_NO_SQL
	/**
	\brief SLQ lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexSQL();
#endif //XSTC_NO_SQL

#ifndef XSTC_NO_TACL
	/**
	\brief Tacl lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTACL();
#endif //XSTC_NO_TACL

#ifndef XSTC_NO_TADS3
	/**
	\brief Tads3 lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTADS3();
#endif //XSTC_NO_TADS3

#ifndef XSTC_NO_TAL
	/**
	\brief Tal lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTAL();
#endif //XSTC_NO_TAL

#ifndef XSTC_NO_TCL
	/**
	\brief Tcl lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTCL();
#endif //XSTC_NO_TCL

#ifndef XSTC_NO_TEX
	/**
	\brief Tex lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTEX();
#endif //XSTC_NO_TEX

#ifndef XSTC_NO_TXT2TAGS
	/**
	\brief Txt2Tags lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexTXT2TAGS();
#endif //XSTC_NO_TXT2TAGS

#ifndef XSTC_NO_VB
	/**
	\brief Visual Basic lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexVB();
#endif //XSTC_NO_VB

#ifndef XSTC_NO_VBSCRIPT
	/**
	\brief VB Script lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexVBSCRIPT();
#endif //XSTC_NO_VBSCRIPT

#ifndef XSTC_NO_VERILOG
	/**
	\brief Verilong lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexVERILOG();
#endif //XSTC_NO_VERILOG

#ifndef XSTC_NO_VHDL
	/**
	\brief VHDL lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexVHDL();
#endif //XSTC_NO_VHDL

#ifndef XSTC_NO_XCODE
	/**
	\brief Xcode lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexXCODE();
#endif //XSTC_NO_XCODE

#ifndef XSTC_NO_XML
	/**
	\brief XML lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexXML();
#endif //XSTC_NO_XML

#ifndef XSTC_NO_YAML
	/**
	\brief Yaml lexcolor setup
	*/
	WXDLLIMPEXP_XSTC void LexYAML();
#endif //XSTC_NO_YAML

#ifndef XSTC_NO_CONFIG
	/**
	\brief Give XSTC a pointer to your config database.

	Without it you wont be ale to use XSTC configuration routines.
	Themes will still work though. root is the sublocation you want the XSTC
	settings stored in. make sure it has a trailing '/' or it wont work.

	See UsingXSTC.html for more information on the configuration paths and settings.
	*/
	WXDLLIMPEXP_XSTC void SetColorConf(wxConfigBase* clrconf, wxString root = wxT("/"));

	/**
	\brief Same as above except if pass == true then the old config database is passed back.
	*/

	WXDLLIMPEXP_XSTC wxConfigBase* SetColorConf(wxConfigBase* clrconf, bool pass = true, wxString root = wxT("/"));

#endif //XSTC_NO_CONFIG

#ifndef XSTC_NO_TRIMTRAIL
	/**
	\brief Trims all trailing spaces and tabs in the document.

	If trimspaces is set to true, then this will be called in savefileX.
	*/
	WXDLLIMPEXP_XSTC void TrimTrailing();
#endif //XSTC_NO_TRIMTRAIL

#ifndef XSTC_NO_TABSPACE
	/**
	\brief Converts tabs to spaces or vice verca

	tabs=true tabs to spaces else spaces to tabs
		int spaces, is the number of spaces to equal one tab
	*/
	WXDLLIMPEXP_XSTC void TabSpace(bool tabs, int spaces);
#endif //XSTC_NO_TABSPACE

	/**
	\brief Sets up the folding margin mask for square markers
	*/
	WXDLLIMPEXP_XSTC void FoldBox();

	/**
	\brief Sets up the folding margin mask for circle markers
	*/
	WXDLLIMPEXP_XSTC void FoldCircle();

	/**
	\brief Sets the folding margin mask for arrow markers.

	If lines is true then connecting lines are drawn, otherwize
	only the head and tail symbold are drawn.
	*/
	WXDLLIMPEXP_XSTC void FoldArrow(bool lines = true);

	/**
	\brief Sets the foldimg margin mask for +- markers.

	If lines is true then connecting lines are drawn, otherwize
	only the head and tail symbold are drawn.
	*/
	WXDLLIMPEXP_XSTC void FoldSimple(bool lines = true);

	/**
	\brief Sets the fold margin styles to the default colors.

	It will do a configuration based setting check if there is a
	config database availible.
	*/
	WXDLLIMPEXP_XSTC void FoldColors();

	/**
	\brief Takes a case insensitive lex-name and returns the appropriate lexer

	If the string is a number then the number is passed back, however if it is
	a string then it is converted to the appropriate number. This will return
	LEX_NULL if no match was found. It uses the names from the #def's in wxScintilla.h
	with the extra stuff removed. "cpp" is a valid string for example.
	*/
	WXDLLIMPEXP_XSTC int GetEXTLexer(wxString extparam);


	/**
	\brief Checks the filename string for invalid characters.

	Does not do any well formed checks.
	*/
	WXDLLIMPEXP_XSTC bool FilevalidCheck(wxString entry);

	/**
	\brief Resets the markers managed my XSTC, user managed markers are left alone.
	*/
	WXDLLIMPEXP_XSTC void ResetMarkers();

#ifndef XSTC_NO_CONFIG
	/**
	\brief Converts a string representing a color to a wxColour. Used by the config code.

	This can convert any css/html string value "#RRGGBB" or a decimal rgb triplet (r,g,b)
	the triplet can be prefixed with rgb hence "rgb(0,0,0)" as wxColour requires but
	is not needed here. (optional) If a color database is set then a string name may be used
	as well to get a color. wxWidgets sets a good number of them up already but you are free
	to modify and add as you please.
	*/
	WXDLLIMPEXP_XSTC bool Ccolor(wxString configvalue, wxColour& color);

	/**
	\brief Converts a wxColour to a string representing the color value.

	Defaults to html color string.
	wxC2S_NAME          red
	wxC2S_CSS_SYNTAX    rgb(255, 0, 0)
	wxC2S_HTML_SYNTAX   #FF0000
	The flag will determine the type of result you get.
	*/
	WXDLLIMPEXP_XSTC bool CcolorS(wxString configvalue, wxString& color, long type = wxC2S_HTML_SYNTAX);

	/**
	\brief Allows XSTC to use string name lookup for colors.

	By passing a null you turn the color-name functionality off.
	This version will internally destroy the old database when you
	Set a new one.
	*/
	WXDLLIMPEXP_XSTC void SetColorDbase(wxColourDatabase& dbase);

	/**
	\brief Same as above but passes back the old database if pass == true
	*/
	WXDLLIMPEXP_XSTC wxColourDatabase* SetColorDbase(wxColourDatabase& dbase, bool pass);

#endif //XSTC_NO_CONFIG

	/**********************************************************************************************************************
	***********************************************Variables here***********************************************************
	***********************************************************************************************************************/

	/**
	\brief Status result for Save and Load
	*/
	WXDLLIMPEXP_XSTC enum //filestatus
	{
		FILE_OK = 0,
		FILE_NOT_FOUND = 1,
		FILE_SAVE_ERROR = 2
	};

	/**
	\brief Used as an argument for SaveFileX(), for showing a SaveAs dialog.
	*/
	WXDLLIMPEXP_XSTC enum //savemode
	{
		SAVE_AS_NEVER = 0,
		SAVE_AS_ALWAYS = 1,
		SAVE_AS_ON_FAIL = 3
	};

#ifndef XSTC_NO_CONFIG
	/**
	\brief Turn on config lookup lexer per file extention in config database.

	If this is false then built in lookup is done which is probably quite poor.
	It is automatically set to true with a call to SetColorConf()
	*/
	WXDLLIMPEXP_XSTC bool useext;

	/**
	\brief Turn on style color lookup in config database

	This is automatically set to true when you call SetColorConf()
	*/
	WXDLLIMPEXP_XSTC bool usecolor;

	/**
	\brief If this is true then properties will be looked up via config database.
	*/
	WXDLLIMPEXP_XSTC bool useprops;
#endif //XSTC_NO_CONFIG

	/**
	\brief If true the marker colors are switched and the bookmark looks outlined.
	*/
	WXDLLIMPEXP_XSTC bool markoutline;

	/**
	\brief Defines the shape of the bookmark marker

	It defaults to a circle but you can change it. The value is based on
	#define wxSCI_MARK_CIRCLE definitions like that.
	Also characters may be used by using 1000+Ascii_val as the argument.
	*/
	WXDLLIMPEXP_XSTC int markshape;

	/**
	\brief Fold line behavior, determines where and if a line shows up when text is collapsed.
	*/
	WXDLLIMPEXP_XSTC int foldline;

#ifndef XSTC_NO_TRIMTRAIL
	/**
	\brief If true then TrimTrailing() will be called on save.
	*/
	WXDLLIMPEXP_XSTC bool trimspaces;
#endif //XSTC_NO_TRIMTRAIL

#ifndef XSTC_NO_TABSPACE
	/**
	\brief Defines how space/tab conversion is handled

	0=none/off
	1=spaces convert to tabs
	2=tabs convert to spaces
	<0||>2 is also off
	*/
	WXDLLIMPEXP_XSTC int spaceconv; //0 off, 1 space to tab, 2 tab to space, >2 off

	/**
	\brief Number of spaces that defines a tab, used in TabSpace() for tab/space conversion.
	*/
	WXDLLIMPEXP_XSTC int spaces;
#endif //XSTC_NO_TABSPACE

#ifndef XSTC_NO_KEYS
	/**
	\brief Defaults to off, however if keyword support is turned on and this is true, wordlists are automatically loaded.
	*/
	WXDLLIMPEXP_XSTC bool usekeys;
#endif

	/**
	\brief Toggles using indent guide to match braces with brace-matching
	*/
	WXDLLIMPEXP_XSTC bool braceline;

	/**
	\brief This is the name of the current theme mode.

	If it is "null", then all settings will be make black on white. (default)

	It is primarily used as a convienience variable for the user to find what if any
	theme is set. This can be a config based theme as well.
	*/
	WXDLLIMPEXP_XSTC wxString colorstyle;
	/**
	\brief Color of the right column.
	*/
	WXDLLIMPEXP_XSTC wxColour edgcolr;

	/**
	\brief Number of characters to put the right column.
	*/
	WXDLLIMPEXP_XSTC int edgcolm;

	/**
	\brief Mode the right column is set to, line or background.
	*/
	WXDLLIMPEXP_XSTC int edgmod;

	/**
	\brief Mode for word wrap.
	*/
	WXDLLIMPEXP_XSTC int wrapmod;

	/**
	\brief Mode for word wrap visual flags. (a small rounded arrow)
	*/
	WXDLLIMPEXP_XSTC int wrapvf;

	/**
	\brief Document caching mode
	*/
	WXDLLIMPEXP_XSTC int cachemod;

	/**
	\brief Whether the caret line gets highlighted
	*/
	WXDLLIMPEXP_XSTC bool caretlv;

	/**
	\brief Whether the case sensitive cpp lexer is used, defaults to off
	*/
	WXDLLIMPEXP_XSTC bool cppcase;

	/**
	\brief Whether the case sensitive clw lexer is used, defaults to off
	*/
	WXDLLIMPEXP_XSTC bool clwcase;

#ifndef XSTC_NO_ALPHA
//This macro is needed since scintilla didn't support alpha blending till version 1.68
	/**
	\brief Generic default trancparentcy level for all alpha supporting styles.
	*/
	WXDLLIMPEXP_XSTC int alphalvl;
#endif //XSTC_NO_ALPHA

	/**
	\brief Defaults to "Untitled". This is the internally stored filename of the document.
	*/
	WXDLLIMPEXP_XSTC wxString XFilename;

#ifndef XSTC_NO_KEYS
	/**
	\brief Sets of strings that hold default wordlists.

	Not all languages have wordlists actually defined, many are empty strings. It is
	tough to track down lists of valid strings for many of these languages. Contributions
	are welcome. These may also be changes to a filename or a list of filenames.
	See KeyCheck() for details.
	*/
	WXDLLIMPEXP_XSTC Keyword_Sets Keys_Set;
#endif

	/**
	\brief Ide related colors. These are only here for convienience and are not used internally.
	*/
	WXDLLIMPEXP_XSTC wxString brkptcol, actbrkptcol, errorcol;

private:
	/**
	\brief Mrace match for the brace highlighting function.

	Matches braces for these symbols {} () [] <>
	*/
	void Bmatch(XSTC_EVENT& event);

	/**
	\brief Used for toggling bookmarks and folding.
	*/
	void MarginClicked(XSTC_EVENT& event);

	/**
	\brief Returns the appropriate lexer based on the lexer string name.

	If no matches are made it returns LEX_NULL. The lexer name is form the #def's
	in wxScintilla.h
	*/
	int AutoEXT(wxString filename);//returns the lexer number

#ifndef XSTC_NO_CONFIG
	/**
	\brief Returns true if the extention was in the config database and sets lexer accordingly.
	*/
	bool IsConfEXT(wxString extension, int &lexer);//was the extention loaded from conf?

	/**
	\brief Sets the properties in the config database for the given extention
	*/
	bool SetProperties(wxString extension);
#endif //XSTC_NO_CONFIG

	/**
	\brief Used to toggle/goto numbered bookmarks

	marker is the numbered bookmark to modify (0-9)
	move, if true the marker will be moved to the current cursor
		position. Otherwise the cursor will be moved to the bookmark.
	newline is the line to move the bookmark to. If it is <0 then the
		bookmark will be deleted as long as move is true.
	If the bookmark number is out of range nothing will happen.
	Also the bookmark will be deleted if move is true and we are on the
	same line as the marker.
	*/
	void MarkerToggle(int marker, bool move, int newline = -1);

	/**
	\brief Handles all of the Key events for XSTC, like toggling numbered bookmarks
	*/
	void KeyParse(wxKeyEvent& event);

	/**********************************************************************************************************************
	***********************************************Variables here***********************************************************
	***********************************************************************************************************************/


	/**
	\brief These functions are used in margin ordering. Usually default order is all that works well.
	*/
	//These store the order mapping for the margins.
	int linemgn, symmgn, fldmgn;

	/**
	\brief Used with the margin ordering to hold the correct margin widths for each margin.
	*/
	int linew, symw, fldw;

#ifndef XSTC_NO_CONFIG
	/**
	\brief call SetColorDbase(), this stores the value passed. Needed only for color-name lookup in string conversions.
	*/
	wxColourDatabase* XSTCcolorDbase;

	/**
	\brief Internal XSTC config location for the config database, defaults to "/"
	*/
	wxString confroot;
#endif //XSTC_NO_CONFIG

	/**
	\brief Stores the numbered bookmark handles so that when the user changes a position only one marker will exsist.
	*/
	int BmarkHandles[10];

	/**
	\brief Stores the names of config based themes from "XSTC/COLOR/MISIC/CSTYLES"

	The list is coma delimited. The use of this setting is not nessecary but is recomended
	when creating config based themes.
	*/
	wxString CStyles;

	/**
	\brief A simple way to check C/C++ extentions in AutoEXT() and SetLexerX().
	*/
	wxString c_ext_array[8];

	/**
	\brief These variables use StyleSetSpec() compliant strings. They are the heart of the theme system.
	*/
	wxString XS_comment, //stream comments
			 XS_comment2, //block comments
			 XS_comment3, //documentation comments
			 XS_s_string, //singe quote string ''
			 XS_d_string, //double quote string ""
			 XS_number,
			 XS_char,
			 XS_default, //anything that uses a default style
			 XS_key1,
			 XS_key2,
			 XS_key3,
			 XS_key4,
			 XS_preproc, //preprocessor
			 XS_symbol, //syntax characters, braces, operators
			 XS_tag, //script tags like in html/xml
			 XS_user,
			 XS_misic, //generic fill in variable
			 XS_lang,
			 XS_global,
			 XS_bad, //illegal, error
			 XS_instruction, //whatever
			 XS_dtype; //datatype

	/**
	\brief Fold margin foreground color
	*/
	wxString foldfg;

	/**
	\brief Fold margin background color
	*/
	wxString foldbg;
#ifndef XSTC_NO_CONFIG
	/**
	\brief The config database. XSTC will not write to it. Remember to set up a color database if you need to use color-names.
	*/
	wxConfigBase* colorconf;
#endif //XSTC_NO_CONFIG
};

#endif // XSTC_H

