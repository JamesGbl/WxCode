/*
xstc.h

this file is liceced under the wxWindows licence
just a quick reminder of what that means:

This software is released under the GNU GPL licence
with a few exceptins applied, check the wxWindows licence
to see what those are

visit: http://opensource.org/ to see both
the GNU GPL and wxWindows licences.

this software has absolutely no warranty, express or implied

just so you know, i don't care if you change the code
don't email me if you did someting to it.

no need to mark changes, you obviously may want to change the color
settings, it would only be a real pain if you had to mark them.

if you edit a function to change its behavior, it would be courtious
to others to let them know that the file is not an official release,
but you don't have to do that either.

XSTC was developed by Nuklear Zelph
copyright 2006
*/

#ifndef XSTC_H
#define XSTC_H

#ifdef WXMAKINGDLL
    #define WXDLLIMPEXP_XSTC WXEXPORT
#else
    #define WXDLLIMPEXP_XSTC
#endif

#include <wx/fileconf.h>
#include <wx/msw/regconf.h>
#include <wx/version.h>
#include <wx/ffile.h>
#include <wx/string.h>
#include <wx/arrstr.h>
#include <wx/filedlg.h>
#include <wx/utils.h>
#include <wx/config.h>

/*
XCLASS commandline macro defines whether wxSTC or wxScintilla is used

XSTC_CLASS this macro replaces the class thus: wxStyledTextCtrl

XSTC_EVENT this macro replaces the event type, thus wxStyledTextEvent

XSTC_EVENT_DEF() this macro concatonates the correct event definition type for connect thus:wxEVT_STC_

XSTC_DEF() this macro abstracts the difference between most of both libraries #defines
           that means that you get the right prefix when you use the macro. thus wxSTC
           and you must give the rest of the define. it is still sugested to use this
           with wxScintilla only definitions because of future possibilities. still put that code in
           a preproccessor conditional though.

XSTC_NO_KEYS removes initkeys, purgekeys Keyword_Sets Keys_Set and all keyword 
             setup in setlexerx, this is a command line macro, if you want to 
             do all the work yourself or just not support keywords. and it may
             speed up compilation a little.
             
XSTC_USELVL for functionality added after wx2.6.x (inno opal alphaFunctions and spice)
            it is internally defined like most of these macros
*/

#if XCLASS == 0
 #define XSTC_CLASS wxStyledTextCtrl
 #include <wx/stc/stc.h>
 #define XSTC_EVENT wxStyledTextEvent
 #define XSTC_EVENT_DEF(Cx) wxEVT_STC_ ## Cx
 #define XSTC_DEF(Cy) wxSTC_ ## Cy
  #if wxMINOR_VERSION >6
   #define XSTC_USELVL
  #endif
#elif XCLASS == 1
 #define XSTC_CLASS wxScintilla
 #include <wx/wxscintilla.h>
 #define XSTC_EVENT wxScintillaEvent
 #define XSTC_EVENT_DEF(Cx)  wxEVT_SCI_ ## Cx
 #define XSTC_DEF(Cy) wxSCI_ ## Cy
 #define XSTC_USELVL//can't parse a string with preprocessor, if you use an old version of
                    //wxScintilla, comment this macro out.
#endif


//macros for a few functions that are different between versions/libraries
#if XCLASS == 0
 #ifdef XSTC_USELVL
  #define SetCaretBk SetCaretLineBackground
  #define GetCaretBk GetCaretLineBackground
  #define GetCaretALPHA GetCaretLineBackAlpha
  #define SetCaretALPHA SetCaretLineBackAlpha
 #else
  #define SetCaretBk SetCaretLineBack
  #define GetCaretBk GetCaretLineBack
 #endif
#elif XCLASS == 1
 #define SetCaretBk SetCaretLineBackground
 #define GetCaretBk GetCaretLineBackground
 #ifdef XSTC_USELVL
  #define GetCaretALPHA GetCaretLineBackgroundAlpha
  #define SetCaretALPHA SetCaretLineBackgroundAlpha
 #endif
#endif


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
      {//if a '.' is found in it, a filename is assumed

         wxString ada;

         wxString apdl;//preprocessor
         wxString apdl1;//commands
         wxString apdl2;//slashcommands
         wxString apdl3;//starcommands
         wxString apdl4;//arguments
         wxString apdl5;//functions

         wxString asm_0;//cpu instructions
         wxString asm_1;//fpu instructions
         wxString asm_2;//registers
         wxString asm_3;//directives
         wxString asm_4;//directive operands
         wxString asm_5;//extended instructions

         wxString asn1_0;//keywords
         wxString asn1_1;//attributes
         wxString asn1_2;//descriptors
         wxString asn1_3;//types

         wxString au3_0;
         wxString au3_1;
         wxString au3_2;
         wxString au3_3;
         wxString au3_4;
         wxString au3_5;
         wxString au3_6;
         wxString au3_7;

         wxString ave;
         wxString ave1;
         wxString ave2;
         wxString ave3;
         wxString ave4;
         wxString ave5;

         wxString baan;
         wxString baan1;

         wxString basic;//these vary on the language
         wxString basic1bl;//blitz
         wxString basic1po;//power
         wxString basic1pu;//pure
         wxString basic1fr;//free
         wxString basic2;
         wxString basic3;

         wxString bash;//keywords
         
         wxString batch;

         wxString bullant;//keywords

         wxString caml;//primary keywords
         wxString caml1;
         wxString caml2;

         wxString clw;//clarion keywords
         wxString clw1;//compiler directives
         wxString clw2;//built-in functions
         wxString clw3;//runtime expressions
         wxString clw4;//structures data types
         wxString clw5;//attributes
         wxString clw6;//standard equates
         wxString clw7;//reserved words labels
         wxString clw8;//reserved words procedure labels

         wxString conf;//directives
         wxString conf1;//parameters

         wxString cpp;//primary keywords
         wxString cpp1;//secondary keywords [stl]
         wxString cpp2;//documentation
         wxString cpp3;//global classes and typedefs

         wxString csound;//opcodes
         wxString csound1;//header statements
         wxString csound2;//user keywords

         wxString css;//css1 keywords
         wxString css1;//pseudo classes
         wxString css2;//css2 keywords
         
         wxString diff;

         wxString eiffel;//keywords
         
         wxString eiffelkw;

         wxString erlang;//keywords
         
         wxString errorlist;

         wxString escript;
         wxString escript1;
         wxString escript2;

         wxString flagship;//keywords
         wxString flagship1;//functions
         wxString flagship2;//user 1
         wxString flagship3;//user2

         wxString forth;
         wxString forth1;
         wxString forth2;
         wxString forth3;
         wxString forth4;
         wxString forth5;

         wxString fortran;
         wxString fortran1;
         wxString fortran2;
         
         wxString f77_0;
         wxString f77_1;
         wxString f77_2;

         wxString gui4cli;//globals
         wxString gui4cli1;//events
         wxString gui4cli2;//sttributes
         wxString gui4cli3;//contorl
         wxString gui4cli4;//commands
         
         wxString haskell;

         wxString html;//html elements attributes
         wxString html1;//javascript keywords
         wxString html2;//vbscript keywords
         wxString html3;//python keywords;
         wxString html4;//php keywords
         wxString html5;//sgml dtd keywords
         wxString html6;
         wxString html7;//phpscript

#ifdef XSTC_USELVL
         wxString inno;//sectoins
         wxString inno1;//keywords
         wxString inno2;//parameters
         wxString inno3;//preprocessor directives
         wxString inno4;//pascal keywords
         wxString inno5;//user defined keywords
#endif

         wxString js;//same as cpp lexer unless i can get html sublexer to work right.
         wxString js1;//i may not use that for primary javascript files anyway
         wxString js2;
         
         wxString java;//same as cpp lexer
         wxString java1;
         wxString java2;
         wxString java3;

         wxString kix;
         wxString kix1;
         wxString kix2;
         
         wxString latex;

         wxString lisp;//functions and secial operators
         wxString lisp1;//keywords

         wxString lout;
         wxString lout1;
         wxString lout2;

         wxString lua;//keywords
         wxString lua1;//basic functions
         wxString lua2;//string math functions
         wxString lua3;//coroutines, I/O system facilities
         wxString lua4;//user1
         wxString lua5;//user2
         wxString lua6;//user3
         wxString lua7;//user4

         wxString makefile;

         wxString matlab;//keywords

         wxString metapost;//metapost
         wxString metapost1;//metafun

         wxString mmixal;//operator codes
         wxString mmixal1;//special registers
         wxString mmixal2;//predefined symbols

         wxString mssql;//statements
         wxString mssql1;//data tables
         wxString mssql2;//system tables
         wxString mssql3;//global variables
         wxString mssql4;//functions
         wxString mssql5;//system stored procedures
         wxString mssql6;//operators

         wxString nncrontab;//section forth keywords
         wxString nncrontab1;//nncrontab keywords
         wxString nncrontab2;//modifiers

         wxString nsis;//functions
         wxString nsis1;//variables
         wxString nsis2;//lables
         wxString nsis3;//user defined
         
         wxString octave;

#ifdef XSTC_USELVL
         wxString opal;//keywords
         wxString opal1;//sorts
#endif

         wxString pas;//keywords
         wxString pas1;//classwords

         wxString perl;//keywords

         wxString pov;
         wxString pov1;
         wxString pov2;
         wxString pov3;
         wxString pov4;
         wxString pov5;
         wxString pov6;
         wxString pov7;
         
         wxString properties;

         wxString ps;//L1 operators
         wxString ps1;//L2 operators
         wxString ps2;//L3 operators
         wxString ps3;//RIP-specific operators
         wxString ps4;//user defined operators

         wxString python;//keywords
         wxString python1;//highlighted identifiers

         wxString rebol;
         wxString rebol1;
         wxString rebol2;

         wxString ruby;

         wxString scriptol;

         wxString smalltalk;//special selectors

         wxString specman;
         wxString specman1;
         wxString specman2;
         wxString specman3;

#ifdef XSTC_USELVL
         wxString spice;//command
         wxString spice1;//functions
         wxString spice2;//params
#endif

         wxString sql;
         wxString sql1;
         wxString sql2;
         wxString sql3;
         wxString sql4;
         wxString sql5;
         wxString sql6;
         wxString sql7;

         wxString tads3_0;
         wxString tads3_1;
         wxString tads3_2;
         wxString tads3_3;

#ifdef XSTC_USELVL
         wxString tcl;
         wxString tcl1;
         wxString tcl2;
         wxString tcl3;
         wxString tcl4;
         wxString tcl5;
         wxString tcl6;
         wxString tcl7;
         wxString tcl8;
#endif

         wxString tex;

         wxString vb;
         wxString vb1;
         wxString vb2;
         wxString vb3;
         
         wxString vbscript;
         wxString vbscript1;
         wxString vbscript2;
         wxString vbscript3;

         wxString verilog;
         wxString verilog1;
         wxString verilog2;
         wxString verilog3;

         wxString vhdl;//keywords
         wxString vhdl1;//operators
         wxString vhdl2;//attributes
         wxString vhdl3;//functions
         wxString vhdl4;//packages
         wxString vhdl5;//types
         wxString vhdl6;//user

         wxString yaml;
      };
#endif
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
    XSTC(wxWindow *parent, wxWindowID id=wxID_ANY,
         const wxPoint& pos = wxDefaultPosition,
         const wxSize& size = wxDefaultSize,
         long style = 0,
         const wxString& name = "XSTC");

    /**
    \brief in case you didn't notice it's a destructor

    \detailed probably will always remain empty
    */
    virtual ~XSTC();

    /**
    \brief load file

    \detailed loads a file through scintilla then sets up the lexer
              unless setlexer is false, then the current lexer will
              color the document and the fold margin will stay as
              it was toggled already
    */
    bool LoadFileX(wxString filename, bool setlexer);

    /**
    \brief save file

    \detailed if trimming space, space to tab or any other functionality
              was aded to xstc and set to on for saving, then it will be
              called in this function. also if a file doesn't exist it will
              bring up a save as dialog anyway
    */
    bool SaveFileX(const wxString filename, bool saveas);

    /**
    \brief clears the editor

    \detailed clears the editor sets lexer to NULL and Filename to 'Untitled'
    */
    void CloseFile();

    /**
    \brief sets all keywords to ""
    */
    void PurgeKeys();
    
    /**
    \brief simply it resets ALL of the keys_set strings to default
    */
    void InitKeys();

    /**
    \brief toggles a bookmark

    \detailed toggles a bookamrk at the specified line, internaly this is the
              line where the margin was clicked
    */
    void ToggleBookMark(int line);

    /**
    \brief sets margin number for each margin type

    \detailed sets the margins and sets the withs to those set with SetMgnWidth
    */
    void MapMargins(int line, int symbol, int fold);

    /**
    \brief sets widths of the mapped margins

    \detailed sets the margin widths and stores them so that
              when mapping margins the same settings will be used
    */
    void SetMgnsWidth(int line, int symbol, int fold);

    /**
    \brief load keywords from a file

    \detailed loads a newline delimited keyword set from file and returns it
              as space delimited or a space delimited should just pass as is.
              use: KeyCheck() instead if you might support keywords too.
    */
    wxString LoadKeyWords(wxString filename);

    /**
    \brief sets the visual mode to white on black

    \detailed sets up scintilla for white over black, all lexers adjust
              accordingly if this is used to set up the general colors
    */
    void DarkStyle();

    /**
    \brief attempts to emulate visual studio coloring

    \detailed i don't use VS too often and i copied these settings
              with some creativity for readability from Dev-C++
    */
    void VisualStudioStyle();

    /**
    \brief attempts to emulate classic colors (whatever that means)

    \detailed dunno i copied these settings with some
              creativity for readability from Dev-C++
    */
    void ClassicStyle();

    /**
    \brief attempts to emulate the old borland TUI ide

    \detailed yes i have a copy of Turbo C++ and in fact that is what
              i learned c++ on, if i can figure out how to set up the
              default colors again i definatly can get this one right.
    */
    void BorlandStyle();
	 
	   /**
    \brief a style i found on the internet

    \detailed i found it on codinghorror in is you're ide hot or not as i recall
    */
    void JeffStyle();
	 
	   /**
    \brief a style i found on the internet

    \detailed i found it on codinghorror in is you're ide hot or not as i recall
    */
    void ZenburnStyle();

    /**
    \brief turns off enviornment colorstyles

    \detailed it changes the colorstyle string to "NULL" and changes
              the margin colors back to button face (or whatever) all
              other settings will keep, so you could call DarkStyle()
              then this and the lexers will ignore the black bg
    */
    void ResetStyle();
    
    /**
    \brief sets config based colors for colorstyle functions

    \detailed called by all colorstyle functions since it does the same
              thing for each
    */
    void StyleConf();
    
    /**
    \brief sets config based colors for foldstyle functions

    \detailed called by all foldstyle functions since it does the same
              thing for each
    */
    void FoldConf();

    /**
    \brief sets a lexer and proretties

    \detailed internaly this function uses prop1 as the properties settings
              that should have been set in AutoEXT() for manual loading
              useprop should be true and a properties array given
              alternatly you can use the extention subsytem to deal with the
              properties for you
    */
    void SetLexerX(int lexer, bool useprop, wxString properties[5]);

    /**
    \brief gets property and value from a string

    \detailed parses a property_value string. the '_' underscore is a
              delimiter between them. property is a lexer property
              string value is the setting to use with that property
    */
    PropSTR GetPropStr(wxString property);

    /**
    \brief checks keyname for either being a filename or keyword set

    \detailed checks keyname for a '.' dot (if it is a filename) and either
              loads the file as the keyword set or returns its given value
              which sould be a set of keywords
    */
    wxString KeyCheck(wxString keyname);

    /**
    \brief ada lexcolor setup
    */
void LexADA();

    /**
    \brief apdl lexcolor setup
    */
void LexAPDL();

    /**
    \brief asn1 lexcolor setup
    */
void LexASN1();

    /**
    \brief assembly lexcolor setup
    */
void LexASM();

    /**
    \brief au3 lexcolor setup
    */
void LexAU3();

    /**
    \brief avenuelexcolor setup
    */
void LexAVE();

    /**
    \brief baan lexcolor setup
    */
void LexBAAN();

    /**
    \brief blitzbasic lexcolor setup
    */
void LexBLBASIC();//all four use the same color defs, but doing this makes it easier to customize the coloring

    /**
    \brief freebasic lexcolor setup
    */
void LexFRBASIC();

    /**
    \brief powerbasic lexcolor setup
    */
void LexPOBASIC();

    /**
    \brief purebasic lexcolor setup
    */
void LexPUBASIC();

    /**
    \brief bash lexcolor setup
    */
void LexBASH();

    /**
    \brief batch lexcolor setup
    */
void LexBATCH();

    /**
    \brief bullant lexcolor setup
    */
void LexBULLANT();

    /**
    \brief caml lexcolor setup
    */
void LexCAML();

    /**
    \brief clw lexcolor setup
    */
void LexCLW();

    /**
    \brief apache conf lexcolor setup
    */
void LexCONF();

    /**
    \brief c/c++ lexcolor setup
    */
void LexCPP();

    /**
    \brief csound lexcolor setup
    */
void LexCSOUND();

    /**
    \brief css lexcolor setup
    */
void LexCSS();

    /**
    \brief diff lexcolor setup
    */
void LexDIFF();

    /**
    \brief eiffel lexcolor setup
    */
void LexEIFFEL();

    /**
    \brief eiffelkw lexcolor setup
    */
void LexEIFFELKW();

    /**
    \brief erlang lexcolor setup
    */
void LexERLANG();

    /**
    \brief errorlist lexcolor setup
    */
void LexERRORLIST();

    /**
    \brief escript lexcolor setup
    */
void LexESCRIPT();

    /**
    \brief f77 lexcolor setup
    */
void LexF77();

    /**
    \brief flagship (clipper) lexcolor setup
    */
void LexFLAGSHIP();

    /**
    \brief forth lexcolor setup
    */
void LexFORTH();

    /**
    \brief fortran lexcolor setup
    */
void LexFORTRAN();

    /**
    \brief agui4cli lexcolor setup
    */
void LexGUI4CLI();

    /**
    \brief haskel lexcolor setup
    */
void LexHASKELL();

    /**
    \brief html php asp embeded javascript lexcolor setup
    */
void LexHTM();

#ifdef XSTC_USELVL
    /**
    \brief inno installer lexcolor setup
    */
void LexINNO();
#endif

    /**
    \brief javascript file (cpp lexer) lexcolor setup
    */
void LexJS();

    /**
    \brief java file (cpp lexer) lexcolor setup
    */
void LexJAVA();

    /**
    \brief kix lexcolor setup
    */
void LexKIX();

    /**
    \brief latexlexcolor setup
    */
void LexLATEX();

    /**
    \brief lisp lexcolor setup
    */
void LexLISP();

    /**
    \brief lot lexcolor setup
    */
void LexLOT();

    /**
    \brief lout lexcolor setup
    */
void LexLOUT();

    /**
    \brief lua lexcolor setup
    */
void LexLUA();

    /**
    \brief makefile lexcolor setup
    */
void LexMAKEFILE();

    /**
    \brief matlab lexcolor setup
    */
void LexMATLAB();

    /**
    \brief metapost lexcolor setup
    */
void LexMETAPOST();

    /**
    \brief mmixal lexcolor setup
    */
void LexMMIXAL();

    /**
    \brief nncrontab lexcolor setup
    */
void LexNNCRONTAB();

    /**
    \brief nsis lexcolor setup
    */
void LexNSIS();

    /**
    \brief octave lexcolor setup
    */
void LexOCTAVE();

#ifdef XSTC_USELVL
    /**
    \brief opal lexcolor setup
    */
void LexOPAL();
#endif

    /**
    \brief pascal/delphi lexcolor setup
    */
void LexPAS();


    /**
    \brief perl lexcolor setup
    */
void LexPERL();

    /**
    \brief pov lexcolor setup
    */
void LexPOV();

    /**
    \brief properties lexcolor setup
    */
void LexPROPERTIES();

    /**
    \brief ps lexcolor setup
    */
void LexPS();

    /**
    \brief python lexcolor setup
    */
void LexPYTHON();

    /**
    \brief rebol lexcolor setup
    */
void LexREBOL();

    /**
    \brief ruby lexcolor setup
    */
void LexRUBY();

    /**
    \brief scriptol lexcolor setup
    */
void LexSCRIPTOL();

    /**
    \brief smalltalk lexcolor setup
    */
void LexSMALLTALK();


    /**
    \brief specman lexcolor setup
    */
void LexSPECMAN();

#ifdef XSTC_USELVL
    /**
    \brief spice lexcolor setup
    */
void LexSPICE();
#endif

    /**
    \brief Microsoft sql lexcolor setup
    */
void LexMSSQL();


    /**
    \brief sql lexcolor setup
    */
void LexSQL();

    /**
    \brief tads3 lexcolor setup
    */
void LexTADS3();

#ifdef XSTC_USELVL
    /**
    \brief tcl lexcolor setup
    */
void LexTCL();
#endif

    /**
    \brief tex lexcolor setup
    */
void LexTEX();

    /**
    \brief verilong lexcolor setup
    */
void LexVERILOG();

    /**
    \brief vhdl lexcolor setup
    */
void LexVHDL();

    /**
    \brief Visual Basic lexcolor setup
    */
void LexVB();

    /**
    \brief vb script lexcolor setup
    */
void LexVBSCRIPT();

    /**
    \brief xcode lexcolor setup
    */
void LexXCODE();

    /**
    \brief xml lexcolor setup
    */
void LexXML();

    /**
    \brief yaml lexcolor setup
    */
void LexYAML();

    /**
    \brief sets up coloring storage and usecolor to true

    \detailed this sets up the coloring config for the application
              local use local file, if false it will use a global file under the
                 users profile directory
              the rest are for wxConfigBase constructor
              this is deleted by XSTC in the destructor
	      if you opt to use the local option it is set to reletive paths
              how does xstc look for each color entry?
              wxSTC_C_DEFAULT
              in each #define for the lexer colors there is a common
                 pattern for its structure, the second place is used as the
                 group and the third for the specific color setting example
                 on the shown define: group C contains color setting DEFAULT
                 each of these color groups are located under the root
                 XSTColor so in order to write the shown value use this string:
                 XSTColor\C\DEFAULT and set it to whatever you wish.
              the string value in the conf file will be used verbatim in StyleSetSpec
    */
    wxFileConfig* SetColorConf(bool localf, wxString app, wxString vendor, wxString local, wxString global);

    /**
    \brief overloaded version of above that supports wxInputStream rather than
           dealing with files, save must be called by you.

    */
    wxFileConfig* SetColorConf(wxInputStream& is);
	 
	     /**
    \brief overloaded version that supports registry rather than file

    */
	 wxRegConfig* SetColorConf(wxString appName, wxString vendorName, wxString Filename);

    /**
    \brief trims all trailing spaces and tabs in the document

    \detailed if trimspaces is set to true, then this will be called in savefileX.
    */
    void TrimTrailing();

    /**
    \brief converts tabs to spaces or vice verca

    \detailed tabs=true tabs to spaces else spaces to tabs
              int spaces, is the number of spaces to equal one tab
    */
    void TabSpace(bool tabs, int spaces);

    /**
    \brief sets up the folding matgin mask for square markers
    */
    void FoldBox();

    /**
    \brief sets up the folding margin mask for circle markers
    */
    void FoldCircle();

    /**
    \brief sets the folding margin mask for arrow markers
    */
    void FoldArrow();

    /**
    \brief sets the foldimg margin mask for +- markers
    */
    void FoldSimple();


    /**
    \brief sets up extention to lexer maps

    \detailed the argument can be the conf file 'preloaded' or it can
              take a filename to load the conf file. the difference is decided
              by the presence of a '.' dot, extset is loaded with
              all valid extention to lexer maps as well as the properties
              5 properties are supported. this is the convention:
              ext_lexer (either a number or keyword)|prop1|2|3|4|5|comments after this bar
              cpp_3|fold_1|fold.compact_0|fold.preproccessor_1|styling.within.preproccessor_1|fold.comment_1|turn it all on
              if a property is present it is a property string to define if it is on or not
              anything not present is ignored and might cause a problem later.
              also you donn't need all of the pars to be present and they can be void space too
              htm_16|||||||||||
              or
              php_null
              no bars at all

              these are the keywords for lexers:
              container null python cpp html xml perl sql vb properties errorlist
              makefile batch xcode latex lua diff conf pascal ave ada lisp ruby
              eiffel eiffelkw tcl nncrontab bullant vbscript asp php baan matlab
              scriptol asm fortran f77 css pov lout escript ps nsis
              mmixal clw lot yaml tex metapost powerbasic forth erlang
              octave mssql verilog kix gui4cli specman au3 apdl bash asn1 vhdl
              automatic
              
              and use these to toggle case sensitivity:
              cppnocase clwnocase
    */
    void SetConfEXT(wxString extconf);


    /**
    \brief converts a string representing a lexer to an int

    \detailed the string can represent a number or lexer keyword, numbers are converted
           and passed verbatim, the keyword is converted to its respective lexer
    */
    int GetEXTLexer(wxString extparam);


    /**
    \brief checks if the entry has any characters that are invalid for use in a filename

    \detailed doens't do any well formed checks, just looks for characters
    */
    bool FilevalidCheck(wxString entry);

/**********************************************************************************************************************
***********************************************Variables here***********************************************************
***********************************************************************************************************************/

    /**
    \brief for external extention conf

    \detailed this when set to true tells the class to search an external
              configuration file for extentions and which lexer it should be assigned
              if no assignment is found for the current extention, the default is used
              or null_lexer when there is no assignment also you can use a strings
              rather than a file should that be nessicary
    */
    bool useext;

    /**
    \brief for external coloring conf

    \detailed this when set to true tells the class to use an external conf
              for colorization or a string
    */
    bool usecolor;
    
    /**
    \brief for fold margin functions

    \detailed set to true and the marker will be styled as outline, if you 
              use a config color setting to overwrite the colors, this may
              have no effect. (the foreground color makes the outline)
    */
    bool markoutline;
    
    /**
    \brief for fold margin functions

    \detailed sets the shape of the marker when using foldmargin functions
              use the same value as the #defines wxSTC_MARK_CIRCLE
              if using char, you must add the char value to the marker
              1000+(ascii)char value defaults to circle
    */
    int markshape;
    
    /**
    \brief sets the fold line after/before contracted/expanded
    
    \detailed defaults to line after contracted
    */
    int foldline;
 
    /**
    \brief trim trailing spaces/tabs on save

    \detailed when calling saveX and this is set to true, TrimTrailing() will
              called. for resources and effectiveness this seems to be the
              best approach to trim the end spaces.
    */
    bool trimspaces;
    
    /**
    \brief trim trailing spaces/tabs on save

    \detailed when calling saveX and this is set to true, TrimTrailing() will
              called. for resources and effectiveness this seems to be the
              best approach to trim the end spaces.
    */
    int spaceconv; //0 off, 1 space to tab, 2 tab to space, >2 off
    
    /**
    \brief this is the # of spaces in a tab for TabSpace()
    */
    int spaces;

#ifndef XSTC_NO_KEYS
    /**
    \brief for loading keywords

    \detailed if true keywords will be loaded, default is keys_set strings, but
              you can either pass your own keywords to keycheck() or send it a
              filename. alternatly you can use loadkeywords() specifically if you
              KNOW that a file will alwayse be used. internally keycheck calls
              loadkeywords if a '.' dot is present in the string
    */
    bool usekeys;
#endif

    /**
    \brief sets the default coloring for c/c++

    \detailed holds a specific string if a colorstyle function was called
              dark classic visualstudio borland
              more may be added, i am thinking of a matrix theme too
              it defaults to 'NULL' that is basically just ignore the colors
              that are set, make defaults. or a property string that can be
              sent to GetPropString() can be used for the fg_bg colors. this
              gives a small degree of flexability to the lexcolor functions
              as that property is seen as the whitespace color and they will
              try to adjust the color so any direct conflicts are avoided.
              this won't work too well cus the program doesn't have eyes, but
              its workth a try.
    */
    wxString colorstyle;
    
    /**
    \brief color of the edge column

    \detailed used in the colorstyle functions
    */
    wxColour edgcolr;
    
    /**
    \brief column number for the edge column

    \detailed used in the colorstyle functions
    */
    int edgcolm;
    
    /**
    \brief mode of the edge column

    \detailed used in the colorstyle functions
    */
    int edgmod;
    
    /**
    \brief mode for text wrap

    \detailed used in the colorstyle functions
    */
    int wrapmod;
    
    /**
    \brief visual flags for text wrap

    \detailed used in the colorstyle functions
    */
    int wrapvf;
    
    /**
    \brief cache mode for the document

    \detailed used in the colorstyle functions
    */
    int cachemod;
    
    /**
    \brief visibility fot the cater background highlight

    \detailed used in the colorstyle functions
    */
    bool caretlv;
    
    /**
    \brief case sensitive cpp lexer

    \detailed used in extention to lexer setting, defaults true
    */
    bool cppcase;
    
    /**
    \brief case sensitive clw lexer

    \detailed used in extention to lexer setting, defaults true
    */
    bool clwcase;

#ifdef XSTC_USELVL
    /**
    \brief alpha blending level for bookmarks, caret backgrounda and text selection

    \detailed this value sets all three to the same alpha setting, use 256 to turn 
              blending off (full visible), the config settings set them individually
    */
    int alphalvl;
#endif

    /**
    \brief filename of the loaded file

    \detailed if no file is loaded or CloseFile() was called, this is 'Untitled'
              the file name of the loaded file is stored here if loadfileX is
              used, that will contain the full path as passed to it, otherwize you
              set it and it is whatever you make it out to be
    */
    wxString XFilename;

#ifndef XSTC_NO_KEYS
    /**
    \brief a set of strings that hold a default set of keywords

    \detailed this also supports overwriting the keywords using spaces as a delimiter
              or a filename, a '.' dot is used to determine if it is a filename
    */
    Keyword_Sets Keys_Set;
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
    wxString brkptcol, actbrkptcol, errorcol;
	
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

    /**
    \brief checks if entry (file extention) was loaded into extset (handled in conf file)

    \detailed returns true if it was and sets dest to the handled properties
              if it returns false dest is emptied
    */
    bool IsConfEXT(wxString entry, wxArrayString& dest);//was the extention loaded from conf?

/**********************************************************************************************************************
***********************************************Variables here***********************************************************
***********************************************************************************************************************/
	 
    /**
    \brief holds the valid entries from the extention conf file
    */
    wxArrayString extset;

    /**
    \brief properties for lexers

    \detailed holds the properties needed by a lexer for default behavior
              used in SetLexerX()
    */
    wxString prop1[10];
 
    /**
    \brief holds the width for the internally mapped margins
    */
    int linemgn, symmgn, fldmgn;//these store the number for the left margins
	 
    /**
    \brief margin width settings

    \detailed widths for settings margins
    */
    int linew, symw, fldw;


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
    \brief holds the id passed to the constructor

    \detailed The window id is needed to dynamically connect events to this
              instance of the editor. its here so you can use it if you need
    */
    wxWindowID WIN_ID;


    /**
    \brief pointer to the color config object

    \detailed XSTC does not write any values to this object, it only reads them
              define color structure!!
    */
    wxConfigBase* colorconf;
	 wxRegConfig* rcconf;
	 wxFileConfig* fcconf;


};

#endif // XSTC_H

