/*
xstc.cpp

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
//Jeff and Zenburn styles found on codinghorror
//is you're ide hot or not
//http://www.codinghorror.com/blog/archives/000682.html
//given the poor quality of the images i adapted both
//styles as well as i could.
#ifdef XSTC_H_FLAT
  #include "xstc.h"
#else
  #include <wx/xstc.h>
#endif

/*

8- final build option setup, cbp, bakefile whatever
*/



XSTC::XSTC(wxWindow *parent, wxWindowID id,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name):
XSTC_CLASS(parent, id, pos, size, style, name)
{//sets up default behavior, links events and calls default coloring
    WIN_ID = this->GetId();

    Connect(WIN_ID, XSTC_EVENT_DEF(MARGINCLICK),
                (wxObjectEventFunction)
                (wxEventFunction)
                (wxCommandEventFunction)&XSTC::MarginClicked);

    Connect(WIN_ID, XSTC_EVENT_DEF(UPDATEUI),
                (wxObjectEventFunction)
                (wxEventFunction)
                (wxCommandEventFunction)&XSTC::XSTC::Bmatch);
   
    ResetStyle();
    
    linemgn = 0;
    symmgn = 1;
    fldmgn = 2;
    linew = 30;
    symw = 15;
    fldw = 15;
    SetMargins(3,2);
    SetMarginWidth(linemgn,30);
    SetMarginWidth(symmgn,16);
    SetMarginSensitive(1,true);
    SetMarginMask(symmgn, ~XSTC_DEF(MASK_FOLDERS));
    SetMarginWidth(fldmgn,0);
    SetMarginSensitive(fldmgn,true);

    c_ext_array[0] = wxT("c");
    c_ext_array[1] = wxT("h");
    c_ext_array[2] = wxT("cpp");
    c_ext_array[3] = wxT("hpp");
    c_ext_array[4] = wxT("c++");
    c_ext_array[5] = wxT("h++");
    c_ext_array[6] = wxT("cxx");
    c_ext_array[7] = wxT("hxx");

    prop1[0] = wxT("");//empty the properties
    prop1[1] = wxT("");
    prop1[2] = wxT("");
    prop1[3] = wxT("");
    prop1[4] = wxT("");
    prop1[5] = wxT("");
    prop1[6] = wxT("");
    prop1[7] = wxT("");
    prop1[8] = wxT("");
    prop1[9] = wxT("");
    
    extset.Empty();

    cppcase = false;
    clwcase = false;
    useext  = false;
    usecolor  = false;
    trimspaces = false;
    spaceconv = 0;//no convertion
    spaces = 4;
#ifndef XSTC_NO_KEYS
    usekeys = false;
    InitKeys();
#endif
    colorstyle = wxT("null");
    XFilename = wxT("Untitled");
    SetMarginMask(fldmgn, XSTC_DEF(MASK_FOLDERS));
}

XSTC::~XSTC()
{
   wxDELETE(colorconf)
   wxDELETE(rcconf)
   wxDELETE(fcconf)

}

void XSTC::Bmatch(XSTC_EVENT& event)
{
 int p = GetCurrentPos();
 int c1 = GetCharAt(p);
 int c2;
 if(p > 1)
 {
    c2 = GetCharAt(p-1);
 }
 else
 {
    c2 = 0;
 }

 int sp,q;

 if (c2=='(' || c2==')' || c1=='(' || c1==')')
 {
    if(c2=='(' || c2==')')
    {
       sp = p-1;
    }
    else
    {
       sp = p;
    }
    q = BraceMatch(sp);
    if(q == XSTC_DEF(INVALID_POSITION))
    {
       BraceBadLight(sp);
    }
    else
    {
       BraceHighlight(sp, q);
    }
 }
 else
 {
    if (c2=='{' || c2=='}' || c1=='{' || c1=='}')
    {
       if(c2=='{' || c2=='}')
       {
          sp = p-1;
       }
       else
       {
          sp = p;
       }
       q = BraceMatch(sp);
       if(q == XSTC_DEF(INVALID_POSITION))
       {
          BraceBadLight(sp);
       }
       else
       {
          BraceHighlight(sp, q);
       }
    }
    else
    {
       if (c2=='[' || c2==']' || c1=='[' || c1==']')
       {
          if(c2=='[' || c2==']')
          {
             sp = p-1;
          }
          else
          {
             sp = p;
          }
          q = BraceMatch(sp);
          if(q == XSTC_DEF(INVALID_POSITION))
          {
             BraceBadLight(sp);
          }
          else
          {
             BraceHighlight(sp, q);
          }
       }
       else
       {
          if (c2=='<' || c2=='>' || c1=='<' || c1=='>')
          {
             if(c2=='<' || c2=='>')
             {
                sp = p-1;
             }
             else
             {
                sp = p;
             }
             q = BraceMatch(sp);
             if(q == XSTC_DEF(INVALID_POSITION))
             {
                BraceBadLight(sp);
             }
             else
             {
                BraceHighlight(sp, q);
             }
          }
          else
          {
             BraceBadLight(XSTC_DEF(INVALID_POSITION));
          }
       }
    }
 }
event.Skip();
}

void XSTC::MarginClicked(XSTC_EVENT& event)
{
    if (event.GetMargin() == symmgn)
    {
        int line = LineFromPosition(event.GetPosition());
        ToggleBookMark(line);
    }

    if (event.GetMargin() == fldmgn)
    {
        int line = LineFromPosition(event.GetPosition());

        if (GetFoldLevel(line) & XSTC_DEF(FOLDLEVELHEADERFLAG))
        {
            ToggleFold(line);
        }
    }
event.Skip();
}

void XSTC::ToggleBookMark(int line)
{
    if (MarkerGet(line) & (1 << 0))
    {
        MarkerDelete(line, 0);
    }

    else
    {
        MarkerAdd(line, 0);
    }
}

void XSTC::SetLexerX(int lexer, bool useprop, wxString properties[5])//useprop=true checks the properties arg else uses defaults
{//sets a lexer and any default properties unless properties are provided
//properties lexcolor

this->SetMarginWidth(fldmgn,fldw);
//turn on the margin so only those that
//don't support folding will need to be coded in
int x;
    switch(lexer)
    {
case XSTC_DEF(LEX_PYTHON):
     {
        this->LexPYTHON();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.python));
           this->SetKeyWords(1,KeyCheck(Keys_Set.python1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CPP):
     {
        
        if(prop1[0]==wxT("LOADJS"))
        {
           this->LexJS();
           prop1[0]=wxT("");
#ifndef XSTC_NO_KEYS
           if(usekeys)
              {
                 this->SetKeyWords(0,KeyCheck(Keys_Set.js));
                 this->SetKeyWords(1,KeyCheck(Keys_Set.js1));
                 this->SetKeyWords(2,KeyCheck(Keys_Set.js2));
              }
#endif
        }
        else
        if(prop1[0]==wxT("LOADJAVA"))
        {
           this->LexJAVA();
           prop1[0]=wxT("");
#ifndef XSTC_NO_KEYS
           if(usekeys)
              {
                 this->SetKeyWords(0,KeyCheck(Keys_Set.java));
                 this->SetKeyWords(1,KeyCheck(Keys_Set.java1));
                 this->SetKeyWords(2,KeyCheck(Keys_Set.java2));
                 this->SetKeyWords(4,KeyCheck(Keys_Set.java3));
              }
#endif
        }
        else
        {
           this->LexCPP();
           for(x=0;x<10;++x)
              if(prop1[x].Lower() == wxT("cppnocase_1"))
                 cppcase = false;
              else
              if(prop1[x].Lower() == wxT("cppnocase_0"))
                 cppcase = true;
           if(!cppcase)
              lexer = XSTC_DEF(LEX_CPPNOCASE);
#ifndef XSTC_NO_KEYS
           if(usekeys)
           {
              this->SetKeyWords(0,KeyCheck(Keys_Set.cpp));
              this->SetKeyWords(1,KeyCheck(Keys_Set.cpp1));
              this->SetKeyWords(2,KeyCheck(Keys_Set.cpp2));
              this->SetKeyWords(4,KeyCheck(Keys_Set.cpp3));
           }
#endif
        }
        break;
     }
case XSTC_DEF(LEX_HTML):
     {
        this->LexHTM();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.html));
           this->SetKeyWords(1,KeyCheck(Keys_Set.html1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.html2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.html3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.html4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.html5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.html6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.html7));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_XML):
     {
        this->LexXML();
        break;
     }
case XSTC_DEF(LEX_PERL):
     {
        this->LexPERL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.perl));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_SQL):
     {
        this->LexSQL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.sql));
           this->SetKeyWords(1,KeyCheck(Keys_Set.sql1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.sql2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.sql3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.sql4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.sql5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.sql6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.sql7));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_VB):
     {
        this->LexVB();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.vb));
           this->SetKeyWords(1,KeyCheck(Keys_Set.vb1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.vb2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.vb3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_PROPERTIES):
     {
        this->LexPROPERTIES();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.properties));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ERRORLIST):
     {
        this->LexERRORLIST();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.errorlist));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_MAKEFILE):
     {
        this->LexMAKEFILE();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.makefile));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_MATLAB):
     {
        this->LexMATLAB();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.matlab));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_BATCH):
     {
        this->LexBATCH();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.batch));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_XCODE):
     {
        this->LexXCODE();
        break;
     }
case XSTC_DEF(LEX_LATEX):
     {
        this->LexLATEX();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.latex));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_LUA):
     {
        this->LexLUA();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.lua));
           this->SetKeyWords(1,KeyCheck(Keys_Set.lua1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.lua2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.lua3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.lua4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.lua5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.lua6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.lua7));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_DIFF):
     {
        this->LexDIFF();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.diff));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CONF):
     {
        this->LexCONF();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.conf));
           this->SetKeyWords(1,KeyCheck(Keys_Set.conf1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_PASCAL):
     {
        this->LexPAS();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.pas));
           this->SetKeyWords(1,KeyCheck(Keys_Set.pas1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_AVE):
     {
        this->LexAVE();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.ave));
           this->SetKeyWords(1,KeyCheck(Keys_Set.ave1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.ave2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.ave3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.ave4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.ave5));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ADA):
     {
        this->LexADA();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.ada));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_LISP):
     {
        this->LexLISP();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.lisp));
           this->SetKeyWords(1,KeyCheck(Keys_Set.lisp1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_RUBY):
     {
        this->LexRUBY();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.ruby));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_EIFFEL):
     {
        this->LexEIFFEL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.eiffel));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_EIFFELKW):
     {
        this->LexEIFFELKW();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.eiffelkw));
        }
#endif
        break;
     }
#ifdef XSTC_USELVL
case XSTC_DEF(LEX_TCL):
     {
        this->LexTCL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.tcl));
           this->SetKeyWords(1,KeyCheck(Keys_Set.tcl1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.tcl2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.tcl3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.tcl4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.tcl5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.tcl6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.tcl7));
           this->SetKeyWords(8,KeyCheck(Keys_Set.tcl8));
        }
#endif
        break;
     }
#endif
case XSTC_DEF(LEX_NNCRONTAB):
     {
        this->LexNNCRONTAB();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.nncrontab));
           this->SetKeyWords(1,KeyCheck(Keys_Set.nncrontab1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.nncrontab2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_BULLANT):
     {
        this->LexBULLANT();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.bullant));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_VBSCRIPT):
     {
        this->LexVBSCRIPT();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.vb));
           this->SetKeyWords(1,KeyCheck(Keys_Set.vb1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.vb2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.vb3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_BAAN):
     {
        this->LexBAAN();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.baan));
           this->SetKeyWords(1,KeyCheck(Keys_Set.baan1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_SCRIPTOL):
     {
        this->LexSCRIPTOL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.scriptol));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ASM):
     {
        this->LexASM();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.asm_0));
           this->SetKeyWords(1,KeyCheck(Keys_Set.asm_1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.asm_2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.asm_3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.asm_4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.asm_5));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CPPNOCASE):
     {
        this->LexCPP();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.cpp));
           this->SetKeyWords(1,KeyCheck(Keys_Set.cpp1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.cpp2));
           this->SetKeyWords(4,KeyCheck(Keys_Set.cpp3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_FORTRAN):
     {
        this->LexFORTRAN();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.fortran));
           this->SetKeyWords(1,KeyCheck(Keys_Set.fortran1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.fortran2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_F77):
     {
        this->LexF77();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.f77_0));
           this->SetKeyWords(1,KeyCheck(Keys_Set.f77_1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.f77_2));
        }
        break;
     }
case XSTC_DEF(LEX_CSS):
     {
        this->LexCSS();
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.css));
           this->SetKeyWords(1,KeyCheck(Keys_Set.css1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.css2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_POV):
     {
        this->LexPOV();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.pov));
           this->SetKeyWords(1,KeyCheck(Keys_Set.pov1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.pov2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.pov3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.pov4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.pov5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.pov6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.pov7));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_LOUT):
     {
        this->LexLOUT();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.lout));
           this->SetKeyWords(1,KeyCheck(Keys_Set.lout1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.lout2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ESCRIPT):
     {
        this->LexESCRIPT();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.escript));
           this->SetKeyWords(1,KeyCheck(Keys_Set.escript1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.escript2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_PS):
     {
        this->LexPS();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.ps));
           this->SetKeyWords(1,KeyCheck(Keys_Set.ps1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.ps2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.ps3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.ps4));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_NSIS):
     {
        this->LexNSIS();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.nsis));
           this->SetKeyWords(1,KeyCheck(Keys_Set.nsis1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.nsis2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.nsis3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_MMIXAL):
     {
        this->LexMMIXAL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.mmixal));
           this->SetKeyWords(1,KeyCheck(Keys_Set.mmixal1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.mmixal2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CLW):
     {
        this->LexCLW();
        for(x=0;x<10;++x)
              if(prop1[x].Lower() == wxT("clwnocase_1"))
                 clwcase = false;
              else
              if(prop1[x].Lower() == wxT("clwnocase_0"))
                 clwcase = true;
        if(!clwcase)
           lexer = XSTC_DEF(LEX_CLWNOCASE);
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.clw));
           this->SetKeyWords(1,KeyCheck(Keys_Set.clw1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.clw2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.clw3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.clw4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.clw5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.clw6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.clw7));
           this->SetKeyWords(8,KeyCheck(Keys_Set.clw8));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_YAML):
     {
        this->LexYAML();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.yaml));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_TEX):
     {
        this->LexTEX();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.tex));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_METAPOST):
     {
        this->LexMETAPOST();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.metapost));
           this->SetKeyWords(1,KeyCheck(Keys_Set.metapost1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_POWERBASIC):
     {
        this->LexPOBASIC();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.basic));
           this->SetKeyWords(1,KeyCheck(Keys_Set.basic1po));
           this->SetKeyWords(2,KeyCheck(Keys_Set.basic2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.basic3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_FORTH):
     {
        this->LexFORTH();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.forth));
           this->SetKeyWords(1,KeyCheck(Keys_Set.forth1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.forth2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.forth3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.forth4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.forth5));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ERLANG):
     {
        this->LexERLANG();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.erlang));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_OCTAVE):
     {
        this->LexOCTAVE();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.octave));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_MSSQL):
     {
        this->LexMSSQL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
            this->SetKeyWords(0,KeyCheck(Keys_Set.mssql));
            this->SetKeyWords(1,KeyCheck(Keys_Set.mssql1));
            this->SetKeyWords(2,KeyCheck(Keys_Set.mssql2));
            this->SetKeyWords(3,KeyCheck(Keys_Set.mssql3));
            this->SetKeyWords(4,KeyCheck(Keys_Set.mssql4));
            this->SetKeyWords(5,KeyCheck(Keys_Set.mssql5));
            this->SetKeyWords(6,KeyCheck(Keys_Set.mssql6));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_VERILOG):
     {
        this->LexVERILOG();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.verilog));
           this->SetKeyWords(1,KeyCheck(Keys_Set.verilog1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.verilog2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.verilog3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_KIX):
     {
        this->LexKIX();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.kix));
           this->SetKeyWords(1,KeyCheck(Keys_Set.kix1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.kix2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_GUI4CLI):
     {
        this->LexGUI4CLI();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.gui4cli));
           this->SetKeyWords(1,KeyCheck(Keys_Set.gui4cli1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.gui4cli2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.gui4cli3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.gui4cli4));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_SPECMAN):
     {
        this->LexSPECMAN();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.specman));
           this->SetKeyWords(1,KeyCheck(Keys_Set.specman1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.specman2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.specman3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_AU3):
     {
        this->LexAU3();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.au3_0));
           this->SetKeyWords(1,KeyCheck(Keys_Set.au3_1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.au3_2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.au3_3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.au3_4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.au3_5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.au3_6));
           this->SetKeyWords(7,KeyCheck(Keys_Set.au3_7));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_APDL):
     {
        this->LexAPDL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.apdl));
           this->SetKeyWords(1,KeyCheck(Keys_Set.apdl1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.apdl2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.apdl3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.apdl4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.apdl5));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_BASH):
     {
        this->LexBASH();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.bash));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_ASN1):
     {
        this->LexASN1();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.asn1_0));
           this->SetKeyWords(1,KeyCheck(Keys_Set.asn1_1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.asn1_2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.asn1_3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_VHDL):
     {
        this->LexVHDL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.vhdl));
           this->SetKeyWords(1,KeyCheck(Keys_Set.vhdl1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.vhdl2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.vhdl3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.vhdl4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.vhdl5));
           this->SetKeyWords(6,KeyCheck(Keys_Set.vhdl6));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CAML):
     {
        this->LexCAML();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.caml));
           this->SetKeyWords(1,KeyCheck(Keys_Set.caml1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.caml2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_BLITZBASIC):
     {
        this->LexBLBASIC();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.basic));
           this->SetKeyWords(1,KeyCheck(Keys_Set.basic1bl));
           this->SetKeyWords(2,KeyCheck(Keys_Set.basic2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.basic3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_PUREBASIC):
     {
        this->LexPUBASIC();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.basic));
           this->SetKeyWords(1,KeyCheck(Keys_Set.basic1pu));
           this->SetKeyWords(2,KeyCheck(Keys_Set.basic2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.basic3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_HASKELL):
     {
        this->LexHASKELL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.haskell));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_TADS3):
     {
        this->LexTADS3();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.tads3_0));
           this->SetKeyWords(1,KeyCheck(Keys_Set.tads3_1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.tads3_2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.tads3_3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_REBOL):
     {
        this->LexREBOL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.rebol));
           this->SetKeyWords(1,KeyCheck(Keys_Set.rebol));
           this->SetKeyWords(2,KeyCheck(Keys_Set.rebol));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_SMALLTALK):
     {
        this->LexSMALLTALK();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.smalltalk));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_FLAGSHIP):
     {
        this->LexFLAGSHIP();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.flagship));
           this->SetKeyWords(1,KeyCheck(Keys_Set.flagship1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.flagship2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.flagship3));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_CSOUND):
     {
        this->LexCSOUND();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.csound));
           this->SetKeyWords(1,KeyCheck(Keys_Set.csound1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.csound2));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_FREEBASIC):
     {
        this->LexFRBASIC();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.basic));
           this->SetKeyWords(1,KeyCheck(Keys_Set.basic1fr));
           this->SetKeyWords(2,KeyCheck(Keys_Set.basic2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.basic3));
        }
#endif
        break;
     }
#ifdef XSTC_USELVL
case XSTC_DEF(LEX_INNOSETUP):
     {
        this->LexINNO();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.inno));
           this->SetKeyWords(1,KeyCheck(Keys_Set.inno1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.inno2));
           this->SetKeyWords(3,KeyCheck(Keys_Set.inno3));
           this->SetKeyWords(4,KeyCheck(Keys_Set.inno4));
           this->SetKeyWords(5,KeyCheck(Keys_Set.inno5));
        }
#endif
        break;

     }
case XSTC_DEF(LEX_OPAL):
     {
        this->LexOPAL();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.opal));
           this->SetKeyWords(1,KeyCheck(Keys_Set.opal1));
        }
#endif
        break;
     }
case XSTC_DEF(LEX_SPICE):
     {
        this->LexSPICE();
#ifndef XSTC_NO_KEYS
        if(usekeys)
        {
           this->SetKeyWords(0,KeyCheck(Keys_Set.spice));
           this->SetKeyWords(1,KeyCheck(Keys_Set.spice1));
           this->SetKeyWords(2,KeyCheck(Keys_Set.spice2));
        }
#endif
        break;
     }
#endif
default:
     {
        this->SetMarginWidth(fldmgn,0);
        lexer = XSTC_DEF(LEX_NULL);
        break;
     }
    }

    this->SetLexer(lexer);

    //for these if statements the properties are set property_setting
    //the underscore is used to seperate the property from the setting
    PropSTR theprop;
    if(!useprop)
    {
       if(!(prop1[0] == wxT("")))
       {
          theprop = GetPropStr(prop1[0]);
          this->SetProperty(theprop.L, theprop.R);
       }

       if(!(prop1[1] == wxT("")))
       {
          theprop = GetPropStr(prop1[1]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(prop1[2] == wxT("")))
       {
          theprop = GetPropStr(prop1[2]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(prop1[3] == wxT("")))
       {
          theprop = GetPropStr(prop1[3]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(prop1[4] == wxT("")))
       {
          theprop = GetPropStr(prop1[4]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(prop1[5] == wxT("")))
       {
          theprop = GetPropStr(prop1[5]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(prop1[6] == wxT("")))
       {
          theprop = GetPropStr(prop1[6]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(prop1[7] == wxT("")))
       {
          theprop = GetPropStr(prop1[7]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(prop1[8] == wxT("")))
       {
          theprop = GetPropStr(prop1[8]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(prop1[9] == wxT("")))
       {
          theprop = GetPropStr(prop1[9]);
          this->SetProperty(theprop.L, theprop.R);
       }
    }
    else
    {
       if(!(properties[0] == wxT("")))
       {
          theprop = GetPropStr(properties[0]);
          this->SetProperty(theprop.L, theprop.R);
       }

       if(!(properties[1] == wxT("")))
       {
          theprop = GetPropStr(properties[1]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(properties[2] == wxT("")))
       {
          theprop = GetPropStr(properties[2]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(properties[3] == wxT("")))
       {
          theprop = GetPropStr(properties[3]);
          this->SetProperty(theprop.L, theprop.R);
       }


       if(!(properties[4] == wxT("")))
       {
          theprop = GetPropStr(properties[4]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(properties[5] == wxT("")))
       {
          theprop = GetPropStr(properties[5]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(properties[6] == wxT("")))
       {
          theprop = GetPropStr(properties[6]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(properties[7] == wxT("")))
       {
          theprop = GetPropStr(properties[7]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(properties[8] == wxT("")))
       {
          theprop = GetPropStr(properties[8]);
          this->SetProperty(theprop.L, theprop.R);
       }
       
       if(!(properties[9] == wxT("")))
       {
          theprop = GetPropStr(properties[9]);
          this->SetProperty(theprop.L, theprop.R);
       }
    }
}

wxString XSTC::LoadKeyWords(wxString filename)
{
   wxString keywords =wxT("");
   wxFFile *keys = new wxFFile(filename ,wxT("r"));

   if(keys->IsOpened())
   {
       keys->ReadAll(&keywords,wxConvUTF8);
       keys->Close();
   }
   wxDELETE(keys)
   
   return keywords;
}

bool XSTC::LoadFileX(wxString filename, bool setlexer)
{//loads a file and sets the lexer for its file extention
   bool result = false;
   result = this->LoadFile(filename);
   if(setlexer)
   {
      SetLexerX(AutoEXT(filename), false, NULL);
   }
   XFilename = filename;
   return result;
}

bool XSTC::SaveFileX(const wxString filename, bool saveas)
{
   bool result = false;
   int useropt;
   if(trimspaces)
      this->TrimTrailing();
   if(spaceconv == 1)
      TabSpace(true, spaces);
   else 
   if(spaceconv == 2)
      TabSpace(false, spaces);
   
   wxFileDialog *fileobj = new wxFileDialog(this, wxT("Save File As"), wxT(""), wxT(""), wxT("*.*"), wxSAVE | wxCHANGE_DIR | wxOVERWRITE_PROMPT, wxDefaultPosition);
   if(saveas)
   {
      useropt = fileobj->ShowModal();
      if(useropt == wxID_CANCEL)
      {
         return result;
      }
      XFilename = fileobj->GetPath();
      if(!(XFilename.IsEmpty()))
      {
         result = this->SaveFile(XFilename);
      }
   }
   else
   {
      if(filename == wxT("Untitled"))
      {
         useropt = fileobj->ShowModal();
         if(useropt == wxID_CANCEL)
         {
            return result;
         }
         XFilename = fileobj->GetPath();
         if(!(XFilename.IsEmpty()))
         {
            result = this->SaveFile(XFilename);
         }
      }
      else
      {
         if(filename.IsEmpty())
         {
            useropt = fileobj->ShowModal();
            if(useropt == wxID_CANCEL)
            {
               return result;
            }
            XFilename = fileobj->GetPath();
            if(!(XFilename.IsEmpty()))
            {
               result = this->SaveFile(XFilename);
            }
         }
         else
         {
            result = this->SaveFile(filename);
            XFilename = filename;
            if(!result)
            {
               fileobj->SetMessage(wxT("File Unreachable, set new target!"));
               fileobj->SetFilename(filename);
               useropt = fileobj->ShowModal();
               if(useropt == wxID_CANCEL)
               {
                  return result;
               }
               XFilename = fileobj->GetPath();
               if(!(XFilename.IsEmpty()))
               {
                  result = this->SaveFile(XFilename);
               }
            }
         }
      }
   }
   wxDELETE(fileobj)
   
   return result;
}

void XSTC::CloseFile()
{
 this->SetMarginWidth(fldmgn,0);
 this->SetLexer(XSTC_DEF(LEX_NULL));
 this->ClearAll();
 XFilename = wxT("Untitled");
}

int XSTC::AutoEXT(wxString filename)
{
 filename.MakeLower();
 filename.Trim(true);
 filename.Trim(false);
 wxString place,filext =wxT("");
 wxArrayString hold;
 hold.Empty();
 int thelexer = XSTC_DEF(LEX_NULL);
 int x;
 prop1[0] = wxT("");
 prop1[1] = wxT("");
 prop1[2] = wxT("");
 prop1[3] = wxT("");
 prop1[4] = wxT("");
 prop1[5] = wxT("");
 prop1[6] = wxT("");
 prop1[7] = wxT("");
 prop1[8] = wxT("");
 prop1[9] = wxT("");

 if(filename.IsEmpty())
 {
    return thelexer;
 }

 if(filename.Contains(wxT(".")))
 {
       filext = filename.AfterLast('.');
 }
 else
 {
    return thelexer;
 }
 if(useext)
 {
    if(IsConfEXT(filext, hold))
    {
       prop1[0]=hold[1];
       prop1[1]=hold[2];
       prop1[2]=hold[3];
       prop1[3]=hold[4];
       prop1[4]=hold[5];
       prop1[5]=hold[6];
       prop1[6]=hold[7];
       prop1[7]=hold[8];
       prop1[8]=hold[9];
       prop1[9]=hold[10];
       long lexx;
       hold[0].ToLong(&lexx, 10);
       thelexer = lexx;
       return thelexer;
    }
 }

    for(x=0;x<=7;++x)
    {
       if(filext == c_ext_array[x])
       {
          if(filext == c_ext_array[x])
          {
             prop1[0]=wxT("fold_1");
             prop1[1]=wxT("styling.within.preprocessor_0");
             prop1[2]=wxT("fold.comment_1");
             prop1[3]=wxT("fold.preprocessor_1");
             prop1[4]=wxT("fold.compact_0");
             return XSTC_DEF(LEX_CPP);
          }
       }

    }

       if(filext == wxT("htm") || filext == wxT("html") || filext == wxT("xhtml") || filext == wxT("shtml")  || filext == wxT("docbook") || filext == wxT("php") || filext == wxT("php3") || filext == wxT("phtml") || filext == wxT("asp") || filext == wxT("jsp"))
       {
          prop1[0]=wxT("fold.html_1");
          prop1[1]=wxT("fold_1");
          prop1[2]=wxT("fold.compact_0");
          return XSTC_DEF(LEX_HTML);
       }
       else
       if(filext == wxT("js"))
       {
          prop1[0]=wxT("LOADJS");//javascript, XSTC internal use only
          prop1[1]=wxT("fold_1");
          prop1[2]=wxT("fold.compact_0");
          return XSTC_DEF(LEX_CPP);
       }
       else
       if(filext == wxT("java"))
       {
          prop1[0]=wxT("LOADJAVA");//javascript, XSTC internal use only
          prop1[1]=wxT("fold_1");
          prop1[2]=wxT("fold.compact_0");
          return XSTC_DEF(LEX_CPP);
       }
       else
       if(filext == wxT("powerbasic"))
       {
          return XSTC_DEF(LEX_POWERBASIC);
       }
       else
       if(filext == wxT("xml") || filext == wxT("xsl") || filext == wxT("xslt") || filext == wxT("svg") || filext == wxT("xul") || filext == wxT("xsd") || filext == wxT("dtd") || filext == wxT("xrc") || filext == wxT("axl"))
       {
          return XSTC_DEF(LEX_XML);
          prop1[0]=wxT("fold_1");
       }
       else
       if(filext == wxT("vhd") || filext == wxT("vhdl"))
       {
          return XSTC_DEF(LEX_VHDL);
       }
       else
       if(filext == wxT("v") || filext == wxT("vh"))
       {
          return XSTC_DEF(LEX_VERILOG);
       }
       else
       if(filext == wxT("vbs"))
       {
          return XSTC_DEF(LEX_VBSCRIPT);
       }
       else
       if(filext == wxT("vb") || filext == wxT("vbp") || filext == wxT("frm") || filext == wxT("cls") || filext == wxT("clt") || filext == wxT("pag") || filext == wxT("dsr") || filext == wxT("dsm") || filext == wxT("dob"))
       {
          return XSTC_DEF(LEX_VB);
          prop1[0]=wxT("fold_1");
       }
       else
       if(filext == wxT("adb") || filext == wxT("ads"))
       {
          return XSTC_DEF(LEX_ADA);
       }
       else
       if(filext == wxT("yml") || filext == wxT("yaml"))
       {
          return XSTC_DEF(LEX_YAML);
       }
       else
       if(filext == wxT("asm"))
       {
          return XSTC_DEF(LEX_ASM);
       }
       else
       if(filext == wxT("mib"))
       {
          return XSTC_DEF(LEX_ASN1);
       }
       else
       if(filext == wxT("au3"))
       {
          return XSTC_DEF(LEX_AU3);
       }
       else
       if(filext == wxT("ave"))
       {
          return XSTC_DEF(LEX_AVE);
       }
       else
       if(filext == wxT("bc") || filext == wxT("cln"))
       {
          return XSTC_DEF(LEX_BAAN);
       }
       else
       if(filext == wxT("tads3"))
       {
          return XSTC_DEF(LEX_TADS3);
       }
       else
       if(filext == wxT("bb"))
       {
          return XSTC_DEF(LEX_BLITZBASIC);
       }
       else
       if(filext == wxT("haskell"))
       {
          return XSTC_DEF(LEX_HASKELL);
       }
       else
       if(filext == wxT("ant"))
       {
          return XSTC_DEF(LEX_BULLANT);
       }
       else
       if(filext == wxT("ml") || filext == wxT("mli"))
       {
          return XSTC_DEF(LEX_CAML);
       }
       else
       if(filext == wxT("conf"))
       {
          return XSTC_DEF(LEX_CONF);
       }
       else
       if(filext == wxT("orc") || filext == wxT("sco") || filext == wxT("csd"))
       {
          return XSTC_DEF(LEX_CSOUND);
       }
       else
       if(filext == wxT("e"))
       {
          return XSTC_DEF(LEX_EIFFEL);
       }
       else
       if(filext == wxT("ek"))
       {
          return XSTC_DEF(LEX_EIFFELKW);
       }
       else
       if(filext == wxT("erl"))
       {
          return XSTC_DEF(LEX_ERLANG);
       }
       else
       if(filext == wxT("err"))
       {
          return XSTC_DEF(LEX_ERRORLIST);
       }
       else
       if(filext == wxT("src") || filext == wxT("em"))
       {
          return XSTC_DEF(LEX_ESCRIPT);
       }
       else
       if(filext == wxT("prg"))
       {
          return XSTC_DEF(LEX_FLAGSHIP);
       }
       else
       if(filext == wxT("forth"))
       {
          return XSTC_DEF(LEX_FORTH);
       }
       else
       if(filext == wxT("sh") || filext == wxT("bsh") || filext == wxT("configure"))
       {
          return XSTC_DEF(LEX_BASH);
       }
       else
       if(filext == wxT("f77"))
       {
          return XSTC_DEF(LEX_F77);
       }
       else
       if(filext == wxT("f") || filext == wxT("for") || filext == wxT("f90") || filext == wxT("f95") || filext == wxT("f2k"))
       {
          return XSTC_DEF(LEX_FORTRAN);
       }
       else
       if(filext == wxT("bas") || filext == wxT("bi"))
       {
          return XSTC_DEF(LEX_FREEBASIC);
       }
#ifdef XSTC_USELVL
       else
       if(filext == wxT("iss"))
       {
          return XSTC_DEF(LEX_INNOSETUP);
       }
#endif
       else
       if(filext == wxT("kix"))
       {
          return XSTC_DEF(LEX_KIX);
       }
       else
       if(filext == wxT("lsp") || filext == wxT("lisp"))
       {
          return XSTC_DEF(LEX_LISP);
       }
       else
       if(filext == wxT("lot"))
       {
          return XSTC_DEF(LEX_LOT);
       }
       else
       if(filext == wxT("lt"))
       {
          return XSTC_DEF(LEX_LOUT);
       }
       else
       if(filext == wxT("lua"))
       {
          return XSTC_DEF(LEX_LUA);
       }
       else
       if(filext == wxT("m"))
       {
          return XSTC_DEF(LEX_MATLAB);
       }
       else
       if(filext == wxT("octave"))
       {
          return XSTC_DEF(LEX_OCTAVE);
       }
       else
       if(filext == wxT("mp") || filext == wxT("mpx") || filext == wxT("mpy"))
       {
          return XSTC_DEF(LEX_METAPOST);
       }
       else
       if(filext == wxT("mms"))
       {
          return XSTC_DEF(LEX_MMIXAL);
       }
       else
       if(filext == wxT("tab") || filext == wxT("spf"))
       {
          return XSTC_DEF(LEX_NNCRONTAB);
       }
       else
       if(filext == wxT("nsi") || filext == wxT("nsh"))
       {
          return XSTC_DEF(LEX_NSIS);
       }
#ifdef XSTC_USELVL
       else
       if(filext == wxT("impl") || filext == wxT("sign"))
       {
          return XSTC_DEF(LEX_OPAL);
       }
#endif
       else
       if(filext == wxT("inf") || filext == wxT("aut") || filext == wxT("cnf") || filext == wxT("cfg") || filext == wxT("url") || filext == wxT("reg") || filext == wxT("ini") || filext == wxT("properties"))
       {
          return XSTC_DEF(LEX_PROPERTIES);
       }
       else
       if(filext == wxT("bat") || filext == wxT("cmd") || filext == wxT("nt"))
       {
          return XSTC_DEF(LEX_BATCH);
       }
       else
       if(filext == wxT("makefile") || filext == wxT("mak"))
       {
          return XSTC_DEF(LEX_MAKEFILE);
       }
       else
       if(filext == wxT("pas") || filext == wxT("pp") || filext == wxT("inc") || filext == wxT("dpk") || filext == wxT("dfm") || filext == wxT("dpr"))
       {
          return XSTC_DEF(LEX_PASCAL);
       }
       else
       if(filext == wxT("sh") || filext == wxT("bsh") || filext == wxT("configure"))
       {
          return XSTC_DEF(LEX_BASH);
       }
       else
       if(filext == wxT("pl") || filext == wxT("pm") || filext == wxT("cgi") || filext == wxT("pod"))
       {
          return XSTC_DEF(LEX_PERL);
       }
       else
       if(filext == wxT("pov") || filext == wxT("inc"))
       {
          return XSTC_DEF(LEX_POV);
       }
       else
       if(filext == wxT("ps"))
       {
          return XSTC_DEF(LEX_PS);
       }
       else
       if(filext == wxT("pb"))
       {
          return XSTC_DEF(LEX_PUREBASIC);
       }
       else
       if(filext == wxT("py") || filext == wxT("pyw"))
       {
          return XSTC_DEF(LEX_PYTHON);
       }
       else
       if(filext == wxT("r") || filext == wxT("reb"))
       {
          return XSTC_DEF(LEX_REBOL);
       }
       else
       if(filext == wxT("rb") || filext == wxT("rbw"))
       {
          return XSTC_DEF(LEX_RUBY);
       }
       else
       if(filext == wxT("sol"))
       {
          return XSTC_DEF(LEX_SCRIPTOL);
       }
       else
       if(filext == wxT("st"))
       {
          return XSTC_DEF(LEX_SMALLTALK);
       }
       else
       if(filext == wxT("e"))
       {
          return XSTC_DEF(LEX_SPECMAN);
       }
#ifdef XSTC_USELVL
       else
       if(filext == wxT("scp") || filext == wxT("out") || filext == wxT("spice"))
       {
          return XSTC_DEF(LEX_SPICE);
       }
#endif
       else
       if(filext == wxT("sql") || filext == wxT("sp") || filext == wxT("sf") || filext == wxT("spb") || filext == wxT("sps") || filext == wxT("body") || filext == wxT("spec"))
       {
          return XSTC_DEF(LEX_SQL);
       }
#ifdef XSTC_USELVL
       else
       if(filext == wxT("tcl") || filext == wxT("itcl"))
       {
          return XSTC_DEF(LEX_TCL);
       }
#endif
       else
       if(filext == wxT("tex") || filext == wxT("tuo") || filext == wxT("tui") || filext == wxT("idx") || filext == wxT("toc") || filext == wxT("aux") || filext == wxT("sty"))
       {
          return XSTC_DEF(LEX_TEX);
       }
       else
       if(filext == wxT("latex"))
       {
          return XSTC_DEF(LEX_LATEX);
       }
       else
       if(filext == wxT("css"))
       {
          return XSTC_DEF(LEX_CSS);
       }
       else
       if(filext == wxT("xcode"))
       {
          return XSTC_DEF(LEX_XCODE);
       }
       else
       if(filext == wxT("txt") || filext == wxT("nfo") || filext == wxT("diz") || filext == wxT("doc") || filext == wxT("rtf") || filext == wxT("lst") || filext == wxT("log") || filext == wxT("text"))
       {
          return thelexer;
       }
       else
       if(filext == wxT("pas"))
       {
          prop1[0]=wxT("fold.comment_1");
          prop1[1]=wxT("fold.preprocessor_1");
          prop1[2]=wxT("fold.compact_0");
          return XSTC_DEF(LEX_PASCAL);
       }
       else
       if(filext == wxT("diff") || filext == wxT("patch"))
       {
          return XSTC_DEF(LEX_DIFF);
       }
   return thelexer;
}

void XSTC::FoldConf()
{
   wxString colorval,temp;
   int r=0,g=0,b=0;
   long n;

   if(colorconf->Read(wxT("XSTColor/MISIC/M0FG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(0, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/M0BG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(0, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/M0STYLE"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->MarkerDefine(0, r);
      r=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDER_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDER), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDER_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDER), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEROPEN_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPEN), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEROPEN_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPEN), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEREND_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEREND), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEREND_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEREND), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERMIDTAIL_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERMIDTAIL_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEROPENMID_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPENMID), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDEROPENMID_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPENMID), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERSUB_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERSUB), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERSUB_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERSUB), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERTAIL_F"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERTAIL), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MARKNUM/FOLDERTAIL_B"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERTAIL), wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }
}

void XSTC::FoldBox()
{
   wxString fgc, bgc;
   wxColour fg;
   wxColour bg;

   if(colorstyle == wxT("dark"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#000000");
      fg = wxColour(0x00,0x00,0x00);
      bg = wxColour(0xFF,0xFF,0xFF);
   }
   if(colorstyle == wxT("borland"))
   {
      fgc = wxT("#FFFF00");
      bgc = wxT("#000080");
      fg = wxColour(0x00,0x00,0x80);
      bg = wxColour(0xFF,0xFF,0x00);
   }
   if(colorstyle == wxT("classic"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xE0,0xDF,0xE3);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("vistudio"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("jeff"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#808080");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("zenburn"))
   {
      fgc = wxT("#3F3F3F");
      bgc = wxT("#808080");
      fg = wxColour(0x3F,0x3F,0x3F);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("null"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }
   
#if XCLASS == 0 //wxStyledTextCtrl

   if(markoutline)
      this->MarkerDefine(0, markshape, bg, fg);
   else
      this->MarkerDefine(0, markshape, fg, bg);
   
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_BOXPLUS),           fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_BOXMINUS),          fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_BOXPLUSCONNECTED),  fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_TCORNER),           fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_BOXMINUSCONNECTED), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_VLINE),             fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_LCORNER),           fg, bg);

#elif XCLASS == 1 //wxScintilla

   this->MarkerDefine(0, markshape);
   if(markoutline)
   {
      this->MarkerSetForeground(0, bg);
      this->MarkerSetBackground(0, fg);
   }
   else
   {
      this->MarkerSetForeground(0, fg);
      this->MarkerSetBackground(0, bg);
   }
   
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_BOXPLUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_BOXMINUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_BOXPLUSCONNECTED));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_TCORNER));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_BOXMINUSCONNECTED));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_VLINE));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_LCORNER));

   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDER),        fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPEN),    fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEREND),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPENMID), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERSUB),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERTAIL),    fg);

   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDER),        bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPEN),    bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEREND),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPENMID), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERSUB),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERTAIL),    bg);

#endif //wxscintilla

   this->SetFoldFlags(foldline);
   
   if(usecolor)
   {
      FoldConf();
   }
}

void XSTC::FoldCircle()
{
   wxString fgc, bgc;
   wxColour fg;
   wxColour bg;

   if(colorstyle == wxT("dark"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#000000");
      fg = wxColour(0x00,0x00,0x00);
      bg = wxColour(0xFF,0xFF,0xFF);
   }
   if(colorstyle == wxT("borland"))
   {
      fgc = wxT("#FFFF00");
      bgc = wxT("#000080");
      fg = wxColour(0x00,0x00,0x80);
      bg = wxColour(0xFF,0xFF,0x00);
   }
   if(colorstyle == wxT("classic"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xE0,0xDF,0xE3);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("vistudio"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("jeff"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#808080");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("zenburn"))
   {
      fgc = wxT("#3F3F3F");
      bgc = wxT("#808080");
      fg = wxColour(0x3F,0x3F,0x3F);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("null"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }

#if XCLASS == 0 //wxStyledTextCtrl

   if(markoutline)
      this->MarkerDefine(0, markshape, bg, fg);
   else
      this->MarkerDefine(0, markshape, fg, bg);

   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_CIRCLEPLUS),           fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_CIRCLEMINUS),          fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_CIRCLEPLUSCONNECTED),  fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_TCORNERCURVE),              fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_CIRCLEMINUSCONNECTED), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_VLINE),                fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_LCORNERCURVE),              fg, bg);

#elif XCLASS == 1 //wxScintilla

   this->MarkerDefine(0, markshape);
   if(markoutline)
   {
      this->MarkerSetForeground(0, bg);
      this->MarkerSetBackground(0, fg);
   }
   else
   {
      this->MarkerSetForeground(0, fg);
      this->MarkerSetBackground(0, bg);
   }

   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_CIRCLEPLUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_CIRCLEMINUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_CIRCLEPLUSCONNECTED));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_TCORNERCURVE));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_CIRCLEMINUSCONNECTED));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_VLINE));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_LCORNERCURVE));

   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDER),        fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPEN),    fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEREND),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPENMID), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERSUB),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERTAIL),    fg);

   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDER),        bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPEN),    bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEREND),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPENMID), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERSUB),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERTAIL),    bg);

#endif //wxscintilla

   this->SetFoldFlags(foldline);

   if(usecolor)
   {
      FoldConf();
   }
}

void XSTC::FoldArrow()
{
   wxString fgc, bgc;
   wxColour fg;
   wxColour bg;

   if(colorstyle == wxT("dark"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#000000");
      fg = wxColour(0x00,0x00,0x00);
      bg = wxColour(0xFF,0xFF,0xFF);
   }
   if(colorstyle == wxT("borland"))
   {
      fgc = wxT("#FFFF00");
      bgc = wxT("#000080");
      fg = wxColour(0x00,0x00,0x80);
      bg = wxColour(0xFF,0xFF,0x00);
   }
   if(colorstyle == wxT("classic"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xE0,0xDF,0xE3);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("vistudio"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("jeff"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#808080");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("zenburn"))
   {
      fgc = wxT("#3F3F3F");
      bgc = wxT("#808080");
      fg = wxColour(0x3F,0x3F,0x3F);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("null"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }

#if XCLASS == 0 //wxStyledTextCtrl

   if(markoutline)
      this->MarkerDefine(0, markshape, bg, fg);
   else
      this->MarkerDefine(0, markshape, fg, bg);

   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_ARROW),     fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_ARROWDOWN), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_ARROW),     fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_EMPTY),     fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_ARROWDOWN), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_EMPTY),     fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_EMPTY),     fg, bg);

#elif XCLASS == 1 //wxScintilla

   this->MarkerDefine(0, markshape);
   if(markoutline)
   {
      this->MarkerSetForeground(0, bg);
      this->MarkerSetBackground(0, fg);
   }
   else
   {
      this->MarkerSetForeground(0, fg);
      this->MarkerSetBackground(0, bg);
   }

   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_ARROW));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_ARROWDOWN));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_ARROW));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_EMPTY));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_ARROWDOWN));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_EMPTY));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_EMPTY));

   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDER),        fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPEN),    fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEREND),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPENMID), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERSUB),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERTAIL),    fg);

   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDER),        bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPEN),    bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEREND),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPENMID), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERSUB),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERTAIL),    bg);

#endif //wxscintilla

   this->SetFoldFlags(foldline);

   if(usecolor)
   {
      FoldConf();
   }
}

void XSTC::FoldSimple()
{
   wxString fgc, bgc;
   wxColour fg;
   wxColour bg;

   if(colorstyle == wxT("dark"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#000000");
      fg = wxColour(0x00,0x00,0x00);
      bg = wxColour(0xFF,0xFF,0xFF);
   }
   if(colorstyle == wxT("borland"))
   {
      fgc = wxT("#FFFF00");
      bgc = wxT("#000080");
      fg = wxColour(0x00,0x00,0x80);
      bg = wxColour(0xFF,0xFF,0x00);
   }
   if(colorstyle == wxT("classic"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xE0,0xDF,0xE3);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("vistudio"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }
   if(colorstyle == wxT("jeff"))
   {
      fgc = wxT("#FFFFFF");
      bgc = wxT("#808080");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("zenburn"))
   {
      fgc = wxT("#3F3F3F");
      bgc = wxT("#808080");
      fg = wxColour(0x3F,0x3F,0x3F);
      bg = wxColour(0x80,0x80,0x80);
   }
   if(colorstyle == wxT("null"))
   {
      fgc = wxT("#000000");
      bgc = wxT("#FFFFFF");
      fg = wxColour(0xFF,0xFF,0xFF);
      bg = wxColour(0x00,0x00,0x00);
   }

#if XCLASS == 0 //wxStyledTextCtrl

   if(markoutline)
      this->MarkerDefine(0, markshape, bg, fg);
   else
      this->MarkerDefine(0, markshape, fg, bg);

   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_PLUS),  fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_MINUS), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_PLUS),  fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_EMPTY), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_MINUS), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_EMPTY), fg, bg);
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_EMPTY), fg, bg);
                                                                             
#elif XCLASS == 1 //wxScintilla                                              
      
   this->MarkerDefine(0, markshape);
   if(markoutline)
   {
      this->MarkerSetForeground(0, bg);
      this->MarkerSetBackground(0, fg);
   }
   else
   {
      this->MarkerSetForeground(0, fg);
      this->MarkerSetBackground(0, bg);
   }
                                                                       
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDER),       XSTC_DEF(MARK_PLUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPEN),   XSTC_DEF(MARK_MINUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEREND),    XSTC_DEF(MARK_PLUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERMIDTAIL),XSTC_DEF(MARK_EMPTY));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDEROPENMID),XSTC_DEF(MARK_MINUS));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERSUB),    XSTC_DEF(MARK_EMPTY));
   this->MarkerDefine(XSTC_DEF(MARKNUM_FOLDERTAIL),   XSTC_DEF(MARK_EMPTY));

   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDER),        fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPEN),    fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEREND),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDEROPENMID), fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERSUB),     fg);
   this->MarkerSetForeground(XSTC_DEF(MARKNUM_FOLDERTAIL),    fg);

   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDER),        bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPEN),    bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEREND),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERMIDTAIL), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDEROPENMID), bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERSUB),     bg);
   this->MarkerSetBackground(XSTC_DEF(MARKNUM_FOLDERTAIL),    bg);

#endif //wxscintilla

   this->SetFoldFlags(foldline);

   if(usecolor)
   {
      FoldConf();
   }
}

void XSTC::StyleConf()
{
   wxString colorval,temp;
   int r=0,g=0,b=0;
   long n;
   
   
   if(colorconf->Read(wxT("XSTColor/MISIC/EDGCOLR"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;
      
      this->SetEdgeColour(wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/EDGCOLM"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetEdgeColumn(r);
      r=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/EDGMOD"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetEdgeMode(r);
      r=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/WRAPMOD"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetWrapMode(r);
      r=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/WRAPVF"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetWrapVisualFlags(r);
      r=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/CACHEMOD"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetLayoutCache(r);
      r=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/CARETLV"), &colorval) && colorval != wxT(""))
   {
      colorval.MakeLower();
      bool caret = (colorval == wxT("true") || colorval == wxT("1"));

      this->SetCaretLineVisible(caret);
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/FMC"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetFoldMarginColour(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/FMHI"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetFoldMarginHiColour(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/WSFG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetWhitespaceForeground(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/WSBG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetWhitespaceBackground(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/CLBG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetCaretBk(wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/CFG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetCaretForeground(wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/SELFG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetSelForeground(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/SELBG"), &colorval) && colorval != wxT(""))
   {
      temp = colorval.Mid(colorval.First('(')+1,colorval.First(')'));
      colorval = temp;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      r=n;
      temp = colorval.BeforeFirst(',');
      colorval.Remove(0,colorval.First(',')+1);
      temp.ToLong(&n,10);
      g=n;
      temp = colorval.BeforeFirst(')');
      temp.ToLong(&n,10);
      b=n;

      this->SetSelBackground(true, wxColour(r,g,b));
      colorval = wxT("");
      r=0;
      g=0;
      b=0;
   }

   if(colorconf->Read(wxT("XSTColor/MISIC/BRKPTCOL"), &colorval) && colorval != wxT(""))
      brkptcol = colorval;

   if(colorconf->Read(wxT("XSTColor/MISIC/ACTBRKPTCOL"), &colorval) && colorval != wxT(""))
      actbrkptcol = colorval;

   if(colorconf->Read(wxT("XSTColor/MISIC/ERRORCOL"), &colorval) && colorval != wxT(""))
      errorcol = colorval;

   if(colorconf->Read(wxT("XSTColor/WS/INVISIBLE"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),                colorval);

   if(colorconf->Read(wxT("XSTColor/WS/VISIBLEALWAYS"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),                colorval);

   if(colorconf->Read(wxT("XSTColor/WS/VISIBLEAFTERINDENT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT),                colorval);

   if(colorconf->Read(wxT("XSTColor/EOL/CRLF"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(EOL_CRLF),                colorval);

   if(colorconf->Read(wxT("XSTColor/EOL/CR"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(EOL_CR),                colorval);

   if(colorconf->Read(wxT("XSTColor/EOL/LF"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(EOL_LF),                colorval);

   if(colorconf->Read(wxT("XSTColor/CP/DBCS"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(CP_DBCS),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARKER/MAX"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARKER_MAX),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/CIRCLE"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/ROUNDRECT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/ARROW"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_ARROW),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/SMALLRECT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/SHORTARROW"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/EMPTY"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/ARROWDOWN"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/MINUS"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_MINUS),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/PLUS"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_PLUS),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/BACKGROUND"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/DOTDOTDOT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/ARROWS"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/PIXMAP"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/FULLRECT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),                colorval);

   if(colorconf->Read(wxT("XSTColor/MARK/CHARACTER"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/DEFAULT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/LINENUMBER"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/BRACELIGHT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/BRACEBAD"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/CONTROLCHAR"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/INDENTGUIDE"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/LASTPREDEFINED"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED),                colorval);

   if(colorconf->Read(wxT("XSTColor/STYLE/MAX"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(STYLE_MAX),                colorval);
      
#ifdef XSTC_USELVL
   if(colorconf->Read(wxT("XSTColor/MISIC/MARKALPHA"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->MarkerSetAlpha(0, r);
      r=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/SELALPHA"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetSelAlpha(r);
      r=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/CARETALPHA"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      this->SetCaretALPHA(r);
      r=0;
   }
   
   if(colorconf->Read(wxT("XSTColor/MISIC/ALPHALVL"), &colorval) && colorval != wxT(""))
   {
      colorval.ToLong(&n,10);
      r=n;
      alphalvl = r;
      r=0;
   }
#endif

      if(colorconf->Read(wxT("XSTColor/XS/XS_COMMENT"), &colorval) && colorval != wxT(""))
         XS_comment = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_COMMENT2"), &colorval) && colorval != wxT(""))
         XS_comment2 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_COMMENT3"), &colorval) && colorval != wxT(""))
         XS_comment3 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_S_STRING"), &colorval) && colorval != wxT(""))
         XS_s_string = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_D_STRING"), &colorval) && colorval != wxT(""))
         XS_d_string = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_NUMBER"), &colorval) && colorval != wxT(""))
         XS_number = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_CHAR"), &colorval) && colorval != wxT(""))
         XS_char = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_DEFAULT"), &colorval) && colorval != wxT(""))
         XS_default = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_KEY1"), &colorval) && colorval != wxT(""))
         XS_key1 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_KEY2"), &colorval) && colorval != wxT(""))
         XS_key2 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_KEY3"), &colorval) && colorval != wxT(""))
         XS_key3 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_KEY4"), &colorval) && colorval != wxT(""))
         XS_key4 = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_PREPROC"), &colorval) && colorval != wxT(""))
         XS_preproc = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_SYMBOL"), &colorval) && colorval != wxT(""))
         XS_symbol = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_TAG"), &colorval) && colorval != wxT(""))
         XS_tag = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_USER"), &colorval) && colorval != wxT(""))
         XS_user = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_MISIC"), &colorval) && colorval != wxT(""))
         XS_misic = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_LANG"), &colorval) && colorval != wxT(""))
         XS_lang = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_GLOBAL"), &colorval) && colorval != wxT(""))
         XS_global = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_BAD"), &colorval) && colorval != wxT(""))
         XS_bad = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_INSTRUCTION"), &colorval) && colorval != wxT(""))
         XS_instruction = colorval;

      if(colorconf->Read(wxT("XSTColor/XS/XS_DTYPE"), &colorval) && colorval != wxT(""))
         XS_dtype = colorval;
}

void XSTC::DarkStyle()
{
   this->SetFoldMarginColour(true, wxColour(0x00,0x00,0x00));
   this->SetFoldMarginHiColour(true, wxColour(0x00,0x00,0x00));
   this->SetWhitespaceForeground(true, wxColour(0xFF,0xFF,0xFF));
   this->SetWhitespaceBackground(true, wxColour(0x00,0x00,0x00));
   this->SetCaretBk(wxColour(0x19,0x120,0x91));
   this->SetCaretForeground(wxColour(0xCC,0xCC,0xCC));
   this->SetSelBackground(true, wxColour(0xFF,0xFF,0xFF));
   this->SetSelForeground(true, wxColour(0x00,0x00,0x00));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#FFFFFF,back:#000000"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#FFFFFF,back:#000000"));

   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER), wxT("fore:#FFFFFF,back:#000000"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#FFFFFF,back:#000000"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#808080,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#484848,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#FFFFFF,back:#000000"));

   colorstyle   = wxT("dark");
   brkptcol     = wxT("#808080");
   actbrkptcol  = wxT("#484848");
   errorcol     = wxT("#CACACA");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
    XS_comment = wxT("fore:#464646,back:#000000");
	 XS_comment2 = wxT("fore:#FF4646,back:#000000");
	 XS_comment3 = wxT("fore:#029c9bc,back:#000000");
	 XS_s_string = wxT("fore:#E00000,back:#000000");
	 XS_d_string = wxT("fore:#FF0000,back:#000000");
	 XS_number = wxT("fore:#8c14b9,back:#000000");
	 XS_char = wxT("fore:#00D500,back:#000000");
	 XS_default = wxT("fore:#FFFFFF,back:#000000");
	 XS_key1 = wxT("fore:#99CCFF,back:#000000");
	 XS_key2 = wxT("fore:#0099FF,back:#000000");
	 XS_key3 = wxT("fore:#AFCCFF,back:#000000");
	 XS_key4 = wxT("fore:#B2C9E9,back:#000000");
	 XS_preproc = wxT("fore:#0595b9,back:#000000");
	 XS_symbol = wxT("fore:#00CCFF,back:#000000");
	 XS_tag = wxT("fore:#66CC99,back:#000000");
	 XS_user = wxT("fore:#CACC3C,back:#000000");
	 XS_misic = wxT("fore:#A969A5,back:#000000");
	 XS_lang = wxT("fore:#967CE1,back:#000000");
	 XS_global = wxT("fore:#9CCC9C,back:#000000");
	 XS_bad = wxT("fore:#FF0000,back:#800000"); //illegal, error
	 XS_instruction = wxT("fore:#800000,back:#EEEE00"); //whatever
	 XS_dtype = wxT("fore:#B8B9B9,back:#00CC00"); //datatype

   if(usecolor)
   {
      StyleConf();
   } 
}

void XSTC::VisualStudioStyle()
{
   this->SetCaretBk(wxColour(0x6F,0x8A,0xEA));
   this->SetFoldMarginColour(true, wxColour(0xFF,0xFF,0xFF));
   this->SetFoldMarginHiColour(true, wxColour(0xFF,0xFF,0xFF));
   this->SetWhitespaceForeground(true, wxColour(0x00,0x00,0x00));
   this->SetWhitespaceBackground(true, wxColour(0xFF,0xFF,0xFF));
   this->SetCaretForeground(wxColour(0x00,0x00,0x00));
   this->SetSelBackground(true, wxColour(0x00,0x00,0x80));
   this->SetSelForeground(true, wxColour(0xFF,0xFF,0xFF));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#000000,back:#FFFFFF"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#000000,back:#FFFFFF"));
   

   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER), wxT("fore:#FFFFFF,back:#000000"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#FFFFFF,back:#000000"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#EEEEEE,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#FF0000,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#000000,back:#FFFFFF"));

   colorstyle   = wxT("vistudio");
   brkptcol     = wxT("#FF0000");
   actbrkptcol  = wxT("#0000FF");
   errorcol     = wxT("#FF0000");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
	 XS_comment = wxT("fore:#008000,italic,back:#FFFFFF");
	 XS_comment2 = wxT("fore:#008000,italic,bold,back:#FFFFFF");
	 XS_comment3 = wxT("fore:#008000,italic,underline,back:#FFFFFF");
	 XS_s_string = wxT("fore:#800000,back:#FFFFFF");
	 XS_d_string = wxT("fore:#FF0000,back:#FFFFFF");
	 XS_number = wxT("fore:#000080,back:#FFFFFF");
	 XS_char = wxT("fore:#000000,italic,back:#FFFFFF");
	 XS_default = wxT("fore:#000000,back:#FFFFFF");
	 XS_key1 = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_key2 = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_key3 = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_key4 = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_preproc = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_symbol = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_tag = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_user = wxT("fore:#000000,back:#FFFFFF");
	 XS_misic = wxT("fore:#000000,back:#FFFFFF");
	 XS_lang = wxT("fore:#000000,back:#FFFFFF");
	 XS_global = wxT("fore:#000000,back:#FFFFFF");
	 XS_bad = wxT("fore:#FF0000,back:#800000"); //illegal, error
	 XS_instruction = wxT("fore:#000000,back:#FFFFFF"); //whatever
	 XS_dtype = wxT("fore:#000000,back:#FFFFFF"); //datatype

   if(usecolor)
   {
      StyleConf();
   }
}

void XSTC::ClassicStyle()
{
   this->SetCaretBk(wxColour(0xCC,0xFF,0xFF));
   this->SetFoldMarginColour(true, wxColour(0xE0,0xDF,0xE3));
   this->SetFoldMarginHiColour(true, wxColour(0xE0,0xDF,0xE3));
   this->SetWhitespaceForeground(true, wxColour(0x00,0x00,0x00));
   this->SetWhitespaceBackground(true, wxColour(0xFF,0xFF,0xFF));
   this->SetCaretForeground(wxColour(0x00,0x00,0x00));
   this->SetSelBackground(true, wxColour(0x00,0x00,0x80));
   this->SetSelForeground(true, wxColour(0xFF,0xFF,0xFF));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#000000,back:#FFFFFF"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#FF0000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#FF0000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#000000,back:#E0DFE3"));


   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER), wxT("fore:#FFFFFF,back:#000000"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#FFFFFF,back:#000000"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#000000,back:#E0DFE3"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#000080,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#CCCCCC,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#000000,back:#FFFFFF"));

   colorstyle   = wxT("classic");
   brkptcol     = wxT("#0000FF");
   actbrkptcol  = wxT("#0000FF");
   errorcol     = wxT("#080000");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
     XS_comment = wxT("fore:#C8C7CA,italic,back:#FFFFFF");
	 XS_comment2 = wxT("fore:#C8C7CA,bold,back:#FFFFFF");
	 XS_comment3 = wxT("fore:#ACABAD,italic,back:#FFFFFF");
	 XS_s_string = wxT("fore:#E00000,back:#FFFFFF");
	 XS_d_string = wxT("fore:#FF0000,back:#FFFFFF");
	 XS_number = wxT("fore:#800080,back:#FFFFFF");
	 XS_char = wxT("fore:#800080,back:#FFFFFF");
	 XS_default = wxT("fore:#000000,back:#FFFFFF");
	 XS_key1 = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_key2 = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_key3 = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_key4 = wxT("fore:#000000,bold,back:#FFFFFF");
	 XS_preproc = wxT("fore:#008000,back:#FFFFFF");
	 XS_symbol = wxT("fore:#808000,back:#FFFFFF");
	 XS_tag = wxT("fore:#000000.underline,back:#FFFFFF");
	 XS_user = wxT("fore:#0000FF,back:#FFFFFF");
	 XS_misic = wxT("fore:#008000,back:#FFFFFF");
	 XS_lang = wxT("fore:#008000,back:#FFFFFF");
	 XS_global = wxT("fore:#008000,back:#FFFFFF");
	 XS_bad = wxT("fore:#FF0000,back:#000000"); //illegal, error
	 XS_instruction = wxT("fore:#008000,back:#FFFFFF"); //whatever
	 XS_dtype = wxT("fore:#008000,back:#FFFFFF"); //datatype

   if(usecolor)
   {
      StyleConf();
   }
}

void XSTC::BorlandStyle()
{
   this->SetCaretBk(wxColour(0x00,0x00,0xA5));
   this->SetFoldMarginColour(true, wxColour(0x00,0x00,0x80));
   this->SetFoldMarginHiColour(true, wxColour(0x00,0x00,0x80));
   this->SetWhitespaceForeground(true, wxColour(0x00,0xFF,0x00));
   this->SetWhitespaceBackground(true, wxColour(0x00,0x00,0x80));
   this->SetCaretForeground(wxColour(0xFF,0xFF,0x00));
   this->SetSelBackground(true, wxColour(0xC0,0xC0,0xC0));
   this->SetSelForeground(true, wxColour(0x00,0x00,0x80));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#FFFF00,back:#000080"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#FFFF00,back:#000080"));


   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#FFFFFF,back:#000000"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER), wxT("fore:#FFFFFF,back:#000000"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#FFFFFF,back:#000000"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#00FF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#C0C0C0,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#FF0000,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#FFFF00,back:#000080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#FFFF00,back:#000080"));


   colorstyle   = wxT("borland");
   brkptcol     = wxT("#800000");
   actbrkptcol  = wxT("#00FFFF");
   errorcol     = wxT("#080000");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
    XS_comment = wxT("fore:#008080,back:#000080");
	 XS_comment2 = wxT("fore:#008080,back:#000080");
	 XS_comment3 = wxT("fore:#008080,back:#000080");
	 XS_s_string = wxT("fore:#FF0000,back:#000080");
	 XS_d_string = wxT("fore:#FF0000,back:#000080");
	 XS_number = wxT("fore:#FFFFFF,back:#000080");
	 XS_char = wxT("fore:#800080,back:#000080");
	 XS_default = wxT("fore:#00FF00,back:#000080");
	 XS_key1 = wxT("fore:#FFFFFF,back:#000080");
	 XS_key2 = wxT("fore:#FFFFFF,back:#000080");
	 XS_key3 = wxT("fore:#FFFFFF,back:#000080");
	 XS_key4 = wxT("fore:#FFFFFF,back:#000080");
	 XS_preproc = wxT("fore:#000080,back:#008080");
	 XS_symbol = wxT("fore:#FFFF00,back:#000080");
	 XS_tag = wxT("fore:#800080,back:#000080");
	 XS_user = wxT("fore:#008000,back:#000080");
	 XS_misic = wxT("fore:#008000,back:#000080");
	 XS_lang = wxT("fore:#808000,back:#000080");
	 XS_global = wxT("fore:#FFFF00,back:#000080");
	 XS_bad = wxT("fore:#000000,back:#FF00FF"); //illegal, error
	 XS_instruction = wxT("fore:#808080,back:#000080"); //whatever
	 XS_dtype = wxT("fore:#00FFFF,back:#000080"); //datatype

   if(usecolor)
   {
      StyleConf();
   }
}

void XSTC::JeffStyle()
{
   this->SetCaretBk(wxColour(0xCE,0xFF,0x84));
   this->SetFoldMarginColour(true, wxColour(0xFF,0xFF,0xFF));
   this->SetFoldMarginHiColour(true, wxColour(0xFF,0xFF,0xFF));
   this->SetWhitespaceForeground(true, wxColour(0x00,0x00,0x00));
   this->SetWhitespaceBackground(true, wxColour(0xFF,0xFF,0xFF));
   this->SetCaretForeground(wxColour(0x00,0x00,0x00));
   this->SetSelBackground(true, wxColour(0xCE,0xFF,0x84));
   this->SetSelForeground(true, wxColour(0x00,0x00,0x00));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#000000,back:#FFFFFF"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#000000,back:#FFFFFF"));


   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER),  wxT("fore:#000000,back:#FFFFFF"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#000000,back:#FFFFFF"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#FFFFFF,back:#808080"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#388CD5,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#F8B163,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#000000,back:#FFFFFF"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#000000,back:#FFFFFF"));


   colorstyle   = wxT("jeff");
   brkptcol     = wxT("#800000");
   actbrkptcol  = wxT("#0000FF");
   errorcol     = wxT("#080000");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
     XS_comment = wxT("fore:#008000,back:#FFFFFF");
	 XS_comment2 = wxT("fore:#00CC00,back:#FFFFFF");
	 XS_comment3 = wxT("fore:#808080,back:#FFFFFF");
	 XS_s_string = wxT("fore:#000000,back:#FFFFDC");
	 XS_d_string = wxT("fore:#000000,back:#FFFFDC");
	 XS_number = wxT("fore:#813A00,back:#E6FFFF");
	 XS_char = wxT("fore:#C2C5F8,back:#FFFFDC");
	 XS_default = wxT("fore:#800000,back:#FFFFFF");
	 XS_key1 = wxT("fore:#000080,back:#FFFFFF");
	 XS_key2 = wxT("fore:#A65300,back:#FFFFFF");
	 XS_key3 = wxT("fore:#A6718C,back:#FFFFFF");
	 XS_key4 = wxT("fore:#388CE5,back:#FFFFFF");
	 XS_preproc = wxT("fore:#008000,italic,back:#FFFFFF");
	 XS_symbol = wxT("fore:#000000,back:#FFFFFF");
	 XS_tag = wxT("fore:#808080,back:#FFFFFF");
	 XS_user = wxT("fore:#CCCCCC,back:#FFFFFF");
	 XS_misic = wxT("fore:#008000,bold,back:#FFFFFF");
	 XS_lang = wxT("fore:#AA0063,back:#FFFFFF");
	 XS_global = wxT("fore:#00FF00,back:#FFFFFF");
	 XS_bad = wxT("fore:#CEFF84,back:#800000"); //illegal, error
	 XS_instruction = wxT("fore:#FFFFDC,back:#FFFFFF"); //whatever
	 XS_dtype = wxT("fore:#E6FFFF,back:#FFFFFF"); //datatype

   if(usecolor)
   {
      StyleConf();
   }
}

void XSTC::ZenburnStyle()
{
   this->SetCaretBk(wxColour(0x2E,0x43,0x40));
   this->SetFoldMarginColour(true, wxColour(0x3F,0x3F,0x3F));
   this->SetFoldMarginHiColour(true, wxColour(0x3F,0x3F,0x3F));
   this->SetWhitespaceForeground(true, wxColour(0xDF,0xDF,0xBF));
   this->SetWhitespaceBackground(true, wxColour(0x3F,0x3F,0x3F));
   this->SetCaretForeground(wxColour(0x00,0x00,0x00));
   this->SetSelBackground(true, wxColour(0xDF,0xDF,0xBF));
   this->SetSelForeground(true, wxColour(0x3F,0x3F,0x3F));

   this->StyleSetSpec(XSTC_DEF(WS_INVISIBLE),          wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEALWAYS),      wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(WS_VISIBLEAFTERINDENT), wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(EOL_CRLF),              wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(EOL_CR),                wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(EOL_LF),                wxT("fore:#DFDFBF,back:#3F3F3F"));

   this->StyleSetSpec(XSTC_DEF(CP_DBCS),         wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARKER_MAX),      wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_CIRCLE),     wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROW),      wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_SMALLRECT),  wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_SHORTARROW), wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_EMPTY),      wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWDOWN),  wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_MINUS),      wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_PLUS),       wxT("fore:#DFDFBF,back:#3F3F3F"));


   this->StyleSetSpec(XSTC_DEF(MARK_BACKGROUND), wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_DOTDOTDOT),  wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_ARROWS),     wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_PIXMAP),     wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_ROUNDRECT),  wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(MARK_CHARACTER), wxT("fore:#DFDFBF,back:#3F3F3F"));
#ifdef XSTC_USELVL
   this->StyleSetSpec(XSTC_DEF(MARK_FULLRECT),   wxT("fore:#DFDFBF,back:#3F3F3F"));
#endif

   this->StyleSetSpec(XSTC_DEF(STYLE_DEFAULT),        wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LINENUMBER),     wxT("fore:#85AC8D,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACELIGHT),     wxT("fore:#3F7997,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_BRACEBAD),       wxT("fore:#793F3F,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_CONTROLCHAR),    wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_INDENTGUIDE),    wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_LASTPREDEFINED), wxT("fore:#DFDFBF,back:#3F3F3F"));
   this->StyleSetSpec(XSTC_DEF(STYLE_MAX),            wxT("fore:#DFDFBF,back:#3F3F3F"));


   colorstyle   = wxT("zenburn");
   brkptcol     = wxT("#800000");
   actbrkptcol  = wxT("#008080");
   errorcol     = wxT("#080000");

   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
     XS_comment = wxT("fore:#85AC8D,back:#3F3F3F");
	 XS_comment2 = wxT("fore:#466661,back:#3F3F3F");
	 XS_comment3 = wxT("fore:#808080,back:#3F3F3F");
	 XS_s_string = wxT("fore:#C89191,back:#3F3F3F");
	 XS_d_string = wxT("fore:#C89191,back:#3F3F3F");
	 XS_number = wxT("fore:#8ACCCF,back:#3F3F3F");
	 XS_char = wxT("fore:#C89191,back:#3F3F3F");
	 XS_default = wxT("fore:#DFDFBF,back:#3F3F3F");
	 XS_key1 = wxT("fore:#EAEAAC,back:#3F3F3F");
	 XS_key2 = wxT("fore:#F0C77A,back:#3F3F3F");
	 XS_key3 = wxT("fore:#9AD0AC,back:#3F3F3F");
	 XS_key4 = wxT("fore:#669AC4,back:#3F3F3F");
	 XS_preproc = wxT("fore:#85AC8D,back:#3F3F3F");
	 XS_symbol = wxT("fore:#EAEAAC,back:#3F3F3F");
	 XS_tag = wxT("fore:#808080,bold,back:#3F3F3F");
	 XS_user = wxT("fore:#CCCCCC,back:#3F3F3F");
	 XS_misic = wxT("fore:#9AD0AC,bold,back:#3F3F3F");
	 XS_lang = wxT("fore:#9AD0AC,italic,back:#3F3F3F");
	 XS_global = wxT("fore:#669AC4,bold,back:#3F3F3F");
	 XS_bad = wxT("fore:#793F3F,back:#3F3F3F"); //illegal, error
	 XS_instruction = wxT("fore:#FFFF00,back:#3F3F3F"); //whatever
	 XS_dtype = wxT("fore:#00FFFF,back:#3F3F3F"); //datatype

   if(usecolor)
   {
      StyleConf();
   }
}

void XSTC::ResetStyle()
{   
    brkptcol     = wxT("#FF0000");
    actbrkptcol  = wxT("#0000FF");
    errorcol     = wxT("#800000");
    markshape    = XSTC_DEF(MARK_CIRCLE);
    markoutline  = false;
    foldline     = XSTC_DEF(FOLDFLAG_LINEAFTER_CONTRACTED);
    edgcolr      = wxColour(0x00,0xFF,0x00);
    edgcolm      = 50;
    edgmod       = XSTC_DEF(EDGE_LINE);
    wrapmod      = XSTC_DEF(WRAP_WORD);
    wrapvf       = XSTC_DEF(WRAPVISUALFLAG_END);
    cachemod     = XSTC_DEF(CACHE_CARET);
    caretlv      = true;
#ifdef XSTC_USELVL
    alphalvl     = XSTC_DEF(ALPHA_NOALPHA);
#endif
	 XS_comment = wxT("fore:#000000,back:#FFFFFF");
	 XS_comment2 = wxT("fore:#000000,back:#FFFFFF");
	 XS_comment3 = wxT("fore:#000000,back:#FFFFFF");
	 XS_s_string = wxT("fore:#000000,back:#FFFFFF");
	 XS_d_string = wxT("fore:#000000,back:#FFFFFF");
	 XS_number = wxT("fore:#000000,back:#FFFFFF");
	 XS_char = wxT("fore:#000000,back:#FFFFFF");
	 XS_default = wxT("fore:#000000,back:#FFFFFF");
	 XS_key1 = wxT("fore:#000000,back:#FFFFFF");
	 XS_key2 = wxT("fore:#000000,back:#FFFFFF");
	 XS_key3 = wxT("fore:#000000,back:#FFFFFF");
	 XS_key4 = wxT("fore:#000000,back:#FFFFFF");
	 XS_preproc = wxT("fore:#000000,back:#FFFFFF");
	 XS_symbol = wxT("fore:#000000,back:#FFFFFF");
	 XS_tag = wxT("fore:#000000,back:#FFFFFF");
	 XS_user = wxT("fore:#000000,back:#FFFFFF");
	 XS_misic = wxT("fore:#000000,back:#FFFFFF");
	 XS_lang = wxT("fore:#000000,back:#FFFFFF");
	 XS_global = wxT("fore:#000000,back:#FFFFFF");
	 XS_bad = wxT("fore:#000000,back:#FFFFFF"); //illegal, error
	 XS_instruction = wxT("fore:#000000,back:#FFFFFF"); //whatever
	 XS_dtype = wxT("fore:#000000,back:#FFFFFF"); //datatype

   this->StyleClearAll();
   this->SetCaretBk(wxColour(*wxCYAN));
   this->SetStyleBits(5);
   this->SetFoldFlags(foldline);
   this->SetEdgeColour(edgcolr);
   this->SetEdgeColumn(edgcolm);
   this->SetEdgeMode(edgmod);
   this->SetWrapMode(wrapmod);
   this->SetWrapVisualFlags(wrapvf);
   this->SetViewWhiteSpace(XSTC_DEF(WS_VISIBLEALWAYS));
   this->SetLayoutCache(cachemod);
   this->SetCaretLineVisible(caretlv);
   this->MarkerDefine(0, markshape);
   this->MarkerSetForeground(0, wxColour(0x00, 0x00, 0x00));
   this->MarkerSetBackground(0, wxColour(*wxCYAN));
   this->SetSelBackground(true, wxColour(0x00,0x00,0x00));
   this->SetSelForeground(true, wxColour(0xFF,0xFF,0xFF));
#ifdef XSTC_USELVL
   this->MarkerSetAlpha(0, alphalvl);
   this->SetSelAlpha(alphalvl);
   this->SetCaretALPHA(alphalvl);
#endif
}

PropSTR XSTC::GetPropStr(wxString property)
{
   PropSTR result;
   result.L = wxT("");
   result.R = wxT("");
   if(!(property.IsEmpty()))
   {
      result.L = property.Mid(0,property.First('_'));
      result.R = property.Mid((property.First('_')+1),property.Len());
   }
 return result;
}

wxString XSTC::KeyCheck(wxString keyname)
{//check the contents of a Keyword_Sets object substring, is it a keywordset or filename
 //if a '.' {period} is in the string somewhere, it is assumed a filename.
   if(keyname.Contains(wxT(".")))
   {
      keyname = this->LoadKeyWords(keyname);
   }
 return keyname;
}

void XSTC::SetConfEXT(wxString extconf)
{
   
   wxString passbuff,chop;
   int point,itr;
   PropSTR undot;

   if(extconf.Contains(wxT(".")))
   {
      wxFFile *extfile = new wxFFile(extconf ,wxT("r"));
      extfile->ReadAll(&passbuff, wxConvLibc);
      wxDELETE(extfile);
   }
   else
      passbuff = extconf;
   
   point = passbuff.Freq('\n');
   extset.Empty();
   extset.Alloc(point);
   if(!passbuff.IsEmpty())
   while(passbuff.length() > 0)
   {
      point = (passbuff.First('\n')+1);
      if(point == 0)
         point = passbuff.length();
      chop = passbuff.Mid(0, point);
      passbuff.Remove(0, point);
      extset.Add(chop, 1);
   }
   else
     return;

   point = extset.GetCount();
   for(itr=0;itr<point;itr++)
   {
      extset[itr].Trim(true);
      extset[itr].Trim(false);
      if(extset[itr].Contains(wxT("_")))
      {
         undot = GetPropStr(extset[itr]);
         if(undot.L.Contains(wxT(".")))
         {
            undot.L = undot.L.Remove(undot.L.First('.'), 1);
         }
         if(FilevalidCheck(undot.L))
            extset[itr] = (undot.L.MakeLower() + wxT("_") + undot.R.MakeLower());
         else
            extset.RemoveAt(itr, 1);
      }
      else
         extset.RemoveAt(itr, 1);
   }
   extset.Shrink();
}

int XSTC::GetEXTLexer(wxString extparam)
{
   extparam.Trim(true);
   extparam.Trim(false);

   int result;
   if(extparam.IsNumber())
   {//if a number is passed, that is sent verbatim
      long res;
      if(extparam.ToLong(&res, 10))
      return result=res;
   }
   extparam.MakeLower();

   if(extparam==wxT("container"))
      return XSTC_DEF(LEX_CONTAINER);

   if(extparam==wxT("null"))
      return XSTC_DEF(LEX_NULL);

   if(extparam==wxT("python"))
      return XSTC_DEF(LEX_PYTHON);

   if(extparam==wxT("cpp"))
      return XSTC_DEF(LEX_CPP);

   if(extparam==wxT("html"))
      return XSTC_DEF(LEX_HTML);

   if(extparam==wxT("xml"))
      return XSTC_DEF(LEX_XML);

   if(extparam==wxT("perl"))
      return XSTC_DEF(LEX_PERL);

   if(extparam==wxT("sql"))
      return XSTC_DEF(LEX_SQL);

   if(extparam==wxT("vb"))
      return XSTC_DEF(LEX_VB);

   if(extparam==wxT("properties"))
      return XSTC_DEF(LEX_PROPERTIES);

   if(extparam==wxT("errorlist"))
      return XSTC_DEF(LEX_ERRORLIST);

   if(extparam==wxT("makefile"))
      return XSTC_DEF(LEX_MAKEFILE);

   if(extparam==wxT("batch"))
      return XSTC_DEF(LEX_BATCH);

   if(extparam==wxT("xcode"))
      return XSTC_DEF(LEX_XCODE);

   if(extparam==wxT("latex"))
      return XSTC_DEF(LEX_LATEX);

   if(extparam==wxT("lua"))
      return XSTC_DEF(LEX_LUA);

   if(extparam==wxT("diff"))
      return XSTC_DEF(LEX_DIFF);

   if(extparam==wxT("conf"))
      return XSTC_DEF(LEX_CONF);

   if(extparam==wxT("pascal"))
      return XSTC_DEF(LEX_PASCAL);

   if(extparam==wxT("ave"))
      return XSTC_DEF(LEX_AVE);

   if(extparam==wxT("ada"))
      return XSTC_DEF(LEX_ADA);

   if(extparam==wxT("lisp"))
      return XSTC_DEF(LEX_LISP);

   if(extparam==wxT("ruby"))
      return XSTC_DEF(LEX_RUBY);

   if(extparam==wxT("eiffel"))
      return XSTC_DEF(LEX_EIFFEL);

   if(extparam==wxT("eiffelkw"))
      return XSTC_DEF(LEX_EIFFELKW);

   if(extparam==wxT("tcl"))
      return XSTC_DEF(LEX_TCL);

   if(extparam==wxT("nncrontab"))
      return XSTC_DEF(LEX_NNCRONTAB);

   if(extparam==wxT("bullant"))
      return XSTC_DEF(LEX_BULLANT);

   if(extparam==wxT("vbscript"))
      return XSTC_DEF(LEX_VBSCRIPT);

   if(extparam==wxT("baan"))
      return XSTC_DEF(LEX_BAAN);

   if(extparam==wxT("wxT(matlab"))
      return XSTC_DEF(LEX_MATLAB);

   if(extparam==wxT("scriptol"))
      return XSTC_DEF(LEX_SCRIPTOL);

   if(extparam==wxT("asm"))
      return XSTC_DEF(LEX_ASM);

   if(extparam==wxT("cppnocase"))
      {
         cppcase = false;
         return XSTC_DEF(LEX_CPP);
      }

   if(extparam==wxT("fortran"))
      return XSTC_DEF(LEX_FORTRAN);

   if(extparam==wxT("f77"))
      return XSTC_DEF(LEX_F77);

   if(extparam==wxT("css"))
      return XSTC_DEF(LEX_CSS);

   if(extparam==wxT("pov"))
      return XSTC_DEF(LEX_POV);

   if(extparam==wxT("lout"))
      return XSTC_DEF(LEX_LOUT);

   if(extparam==wxT("escript"))
      return XSTC_DEF(LEX_ESCRIPT);

   if(extparam==wxT("ps"))
      return XSTC_DEF(LEX_PS);

   if(extparam==wxT("nsis"))
      return XSTC_DEF(LEX_NSIS);

   if(extparam==wxT("mmixal"))
      return XSTC_DEF(LEX_MMIXAL);

   if(extparam==wxT("clw"))
      return XSTC_DEF(LEX_CLW);

   if(extparam==wxT("clwnocase"))
      {
         clwcase = false;
         return XSTC_DEF(LEX_CLW);
      }

   if(extparam==wxT("lot"))
      return XSTC_DEF(LEX_LOT);

   if(extparam==wxT("yaml"))
      return XSTC_DEF(LEX_YAML);

   if(extparam==wxT("tex"))
      return XSTC_DEF(LEX_TEX);

   if(extparam==wxT("metapost"))
      return XSTC_DEF(LEX_METAPOST);

   if(extparam==wxT("powerbasic"))
      return XSTC_DEF(LEX_POWERBASIC);

   if(extparam==wxT("wxT(forth"))
      return XSTC_DEF(LEX_FORTH);

   if(extparam==wxT("erlang"))
      return XSTC_DEF(LEX_ERLANG);

   if(extparam==wxT("octave"))
      return XSTC_DEF(LEX_OCTAVE);

   if(extparam==wxT("mssql"))
      return XSTC_DEF(LEX_MSSQL);

   if(extparam==wxT("verilog"))
      return XSTC_DEF(LEX_VERILOG);

   if(extparam==wxT("kix"))
      return XSTC_DEF(LEX_KIX);

   if(extparam==wxT("gui4cli"))
      return XSTC_DEF(LEX_GUI4CLI);

   if(extparam==wxT("specman"))
      return XSTC_DEF(LEX_SPECMAN);

   if(extparam==wxT("au3"))
      return XSTC_DEF(LEX_AU3);

   if(extparam==wxT("apdl"))
      return XSTC_DEF(LEX_APDL);

   if(extparam==wxT("bash"))
      return XSTC_DEF(LEX_BASH);

   if(extparam==wxT("asn1"))
      return XSTC_DEF(LEX_ASN1);

   if(extparam==wxT("vhdl"))
      return XSTC_DEF(LEX_VHDL);

   if(extparam==wxT("automatic"))
      return XSTC_DEF(LEX_AUTOMATIC);


   return XSTC_DEF(LEX_NULL);
}

bool XSTC::FilevalidCheck(wxString entry)
{
   if(entry.Contains(wxT("/")))
      return false;
   if(entry.Contains(wxT("\\")))
      return false;
   if(entry.Contains(wxT("!")))
      return false;
   if(entry.Contains(wxT("<")))
      return false;
   if(entry.Contains(wxT(">")))
      return false;
   if(entry.Contains(wxT("|")))
      return false;
   if(entry.Contains(wxT("*")))
      return false;
   if(entry.Contains(wxT("\"")))
      return false;
   if(entry.Contains(wxT("?")))
      return false;
   if(entry.Contains(wxT(":")))
      return false;

   return true;
}

bool XSTC::IsConfEXT(wxString entry, wxArrayString& dest)
{
   entry.MakeLower();
   int num = extset.GetCount();
   int x;
   bool matched = false;
   wxString temp;
   PropSTR split;
   dest.Empty();
   dest.Alloc(11);
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   dest.Add(wxT(""));
   for(x=0;x<num;++x)
   {
      split = GetPropStr(extset[x]);
      if(split.R.Contains(wxT("|")))
      {
         temp = split.R.Mid(0, split.R.First('|'));
      }
      else
         temp = split.R.Trim();

      if(split.L == entry)
      {
         dest[0]<<GetEXTLexer(temp);
         num = split.R.Freq('|');
         if(num > 0)
         {
            split.R = split.R.Remove(0, split.R.First('|')+1);
            if(num > 5)
            {
               num = 5;
               for(x=1;x<=num;++x)
               {
                  dest[x] = split.R.Mid(0, split.R.First('|'));
                  if(x < num)
                     split.R = split.R.Remove(0, split.R.First('|')+1);
                  if(x == num)
                     dest[x] = split.R.Mid(0, split.R.First('|'));
               }
            }
            else
            {
               for(x=1;x<=num;++x)
               {
                  dest[x] = split.R.Mid(0, split.R.First('|'));
                  if(x < num)
                     split.R = split.R.Remove(0, split.R.First('|')+1);
                  if(x == num)
                     dest[x].Trim(true);
               }
            }
         }
         matched = true;
      }
   }
   return matched;
}

void XSTC::MapMargins(int line, int symbol, int fold)
{
   linemgn = line;
   symmgn = symbol;
   fldmgn = fold;
   this->SetMarginWidth(linemgn, linew);
   this->SetMarginWidth(symmgn, symw);
   this->SetMarginWidth(fldmgn, fldw);
   this->SetMarginType(linemgn, XSTC_DEF(MARGIN_NUMBER));
   this->SetMarginType(symmgn, XSTC_DEF(MARGIN_SYMBOL));
   this->SetMarginType(fldmgn, XSTC_DEF(MARGIN_SYMBOL));
   this->SetMarginSensitive(linemgn, false);
   this->SetMarginSensitive(symmgn, true);
   this->SetMarginSensitive(fldmgn, true);
   this->SetMarginMask(symmgn, ~XSTC_DEF(MASK_FOLDERS));
   this->SetMarginMask(fldmgn, XSTC_DEF(MASK_FOLDERS));
}

void XSTC::SetMgnsWidth(int line, int symbol, int fold)
{
   this->SetMarginWidth(linemgn, line);
   this->SetMarginWidth(symmgn, symbol);
   this->SetMarginWidth(fldmgn, fold);
   linew = line;
   symw = symbol;
   fldw = fold;
}

void XSTC::TrimTrailing()
{
   wxString editor,text;
   editor = wxT("");
   unsigned int x,len;
   len = this->GetLineCount();
   for(x=0;x<len;++x)
   {
      text = this->GetLine(x);
      if(text.First(wxT("\n")))
      {
    		text.Trim();
        	text += wxT("\n");
	  }
	  else
	        text.Trim();
      editor += text;
   }
   this->SetText(editor);
}

void XSTC::TabSpace(bool tabs, int spaces)
{
   wxString editor = this->GetText();
   wxString ed,ed2;
   int x,len;
   ed2 = wxT("");
   for(x=0;x<spaces;++x)
      ed2 = ed2 + wxT(" ");
   len = editor.length();
   
   if(tabs)
   {
      for(x=0;x<len;++x)
      {
         if(editor[x] == 0x9)
         {
            ed = editor.Mid(0,x);
            editor.Remove(0,x+1);
            ed = ed+ed2;
            editor = ed+editor;
            len = editor.length();
         }
      }
   }
   else
   {
      while(editor.Contains(ed2))
      {
         char p = 0x9;
         x = editor.First(ed2);
         ed = editor.Mid(0, x);
         editor.Remove(0, x+spaces);
         ed += p;
         editor = ed + editor;
      }
   }
   this->SetText(editor);
}


wxFileConfig* XSTC::SetColorConf(bool localf, wxString app, wxString vendor, wxString local, wxString global)
{
   if(localf)
      fcconf = new wxFileConfig(app, vendor, local, global, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_RELATIVE_PATH, wxConvUTF8);
   else
      fcconf = new wxFileConfig(app, vendor, local, global, wxCONFIG_USE_GLOBAL_FILE, wxConvUTF8);
   
   usecolor = true;
	colorconf = (wxConfigBase*)fcconf;
   return fcconf;
}

wxFileConfig* XSTC::SetColorConf(wxInputStream& is)
{
   fcconf = new wxFileConfig(is, wxConvUTF8);
	colorconf = (wxConfigBase*)fcconf;
   usecolor = true;
   return fcconf;
}

wxRegConfig* XSTC::SetColorConf(wxString appName, wxString vendorName, wxString Filename)
{
    rcconf = new wxRegConfig(appName,vendorName,Filename,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
	 colorconf = (wxConfigBase*)rcconf;
    return rcconf;
}