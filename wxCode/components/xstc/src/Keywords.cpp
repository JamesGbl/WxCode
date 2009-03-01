
/*
 * Keywords.h
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

#ifndef XSTC_NO_KEYS

#ifdef XSTC_H_FLAT
  #include "xstc.h"
#else
  #include <wx/XSTC/xstc.h>
#endif


void XSTC::PurgeKeys()
{
#ifndef XSTC_NO_ABAQUS
this->Keys_Set.abaqus = wxT("");
this->Keys_Set.abaqus1 = wxT("");
this->Keys_Set.abaqus2 = wxT("");
this->Keys_Set.abaqus3 = wxT("");
this->Keys_Set.abaqus4 = wxT("");
this->Keys_Set.abaqus5 = wxT("");
#endif //XSTC_NO_ABAQUS
#ifndef XSTC_NO_ADA
this->Keys_Set.ada = wxT("");
#endif //XSTC_NO_ADA
#ifndef XSTC_NO_APDL
this->Keys_Set.apdl = wxT("");
this->Keys_Set.apdl1 = wxT("");
this->Keys_Set.apdl2 = wxT("");
this->Keys_Set.apdl3 = wxT("");
this->Keys_Set.apdl4 = wxT("");
this->Keys_Set.apdl5 = wxT("");
#endif //XSTC_NO_APDL
#ifndef XSTC_NO_ASM
this->Keys_Set.asm_0 = wxT("");
this->Keys_Set.asm_1 = wxT("");
this->Keys_Set.asm_2 = wxT("");
this->Keys_Set.asm_3 = wxT("");
this->Keys_Set.asm_4 = wxT("");
this->Keys_Set.asm_5 = wxT("");
#endif //XSTC_NO_ASM
#ifndef XSTC_NO_ASN1
this->Keys_Set.asn1_0 = wxT("");
this->Keys_Set.asn1_1 = wxT("");
this->Keys_Set.asn1_2 = wxT("");
this->Keys_Set.asn1_3 = wxT("");
#endif //XSTC_NO_ASN1
#ifndef XSTC_NO_ASYMPTOTE
this->Keys_Set.asymptote = wxT("");
this->Keys_Set.asymptote1 = wxT("");
#endif //XSTC_NO_ASYMPTOTE
#ifndef XSTC_NO_AU3
this->Keys_Set.au3_0 = wxT("");
this->Keys_Set.au3_1 = wxT("");
this->Keys_Set.au3_2 = wxT("");
this->Keys_Set.au3_3 = wxT("");
this->Keys_Set.au3_4 = wxT("");
this->Keys_Set.au3_5 = wxT("");
this->Keys_Set.au3_6 = wxT("");
this->Keys_Set.au3_7 = wxT("");
#endif //XSTC_NO_AU3
#ifndef XSTC_NO_AVE
this->Keys_Set.ave = wxT("");
this->Keys_Set.ave1 = wxT("");
this->Keys_Set.ave2 = wxT("");
this->Keys_Set.ave3 = wxT("");
this->Keys_Set.ave4 = wxT("");
this->Keys_Set.ave5 = wxT("");
#endif //XSTC_NO_AVE
#ifndef XSTC_NO_BAAN
this->Keys_Set.baan = wxT("");
this->Keys_Set.baan1 = wxT("");
#endif //XSTC_NO_BAAN

#ifndef XSTC_NO_BLITZBASIC
#ifndef XSTC_NO_FREEBASIC
#ifndef XSTC_NO_POWERBASIC
#ifndef XSTC_NO_PUREBASIC
this->Keys_Set.basic = wxT("");
this->Keys_Set.basic2 = wxT("");
this->Keys_Set.basic3 = wxT("");
#endif //XSTC_NO_PUREBASIC
#endif //XSTC_NO_POWERBASIC
#endif //XSTC_NO_FREEBASIC
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_BLITZBASIC
this->Keys_Set.basic1bl = wxT("");
#endif //XSTC_NO_BLITZBASIC
#ifndef XSTC_NO_FREEBASIC
this->Keys_Set.basic1fr = wxT("");
#endif //XSTC_NO_FREEBASIC
#ifndef XSTC_NO_POWERBASIC
this->Keys_Set.basic1po = wxT("");
#endif //XSTC_NO_POWERBASIC
#ifndef XSTC_NO_PUREBASIC
this->Keys_Set.basic1pu = wxT("");
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
this->Keys_Set.bash = wxT("");
#endif //XSTC_NO_BASH
#ifndef XSTC_NO_BATCH
this->Keys_Set.batch = wxT("");
#endif //XSTC_NO_BATCH
#ifndef XSTC_NO_BULLANT
this->Keys_Set.bullant = wxT("");
#endif //XSTC_NO_BULLANT
#ifndef XSTC_NO_CAML
this->Keys_Set.caml = wxT("");
this->Keys_Set.caml1 = wxT("");
this->Keys_Set.caml2 = wxT("");
#endif //XSTC_NO_CAML
#ifndef XSTC_NO_CLW
this->Keys_Set.clw = wxT("");
this->Keys_Set.clw1 = wxT("");
this->Keys_Set.clw2 = wxT("");
this->Keys_Set.clw3 = wxT("");
this->Keys_Set.clw4 = wxT("");
this->Keys_Set.clw5 = wxT("");
this->Keys_Set.clw6 = wxT("");
this->Keys_Set.clw7 = wxT("");
this->Keys_Set.clw8 = wxT("");
#endif //XSTC_NO_CLW
#ifndef XSTC_NO_CMAKE
this->Keys_Set.cmake = wxT("");
this->Keys_Set.cmake1 = wxT("");
this->Keys_Set.cmake2 = wxT("");
#endif //XSTC_NO_CMAKE
#ifndef XSTC_NO_CONF
this->Keys_Set.conf = wxT("");
this->Keys_Set.conf1 = wxT("");
#endif //XSTC_NO_CONF
#ifndef XSTC_NO_CPP
this->Keys_Set.cpp = wxT("");
this->Keys_Set.cpp1 = wxT("");
this->Keys_Set.cpp2 = wxT("");
this->Keys_Set.cpp3 = wxT("");
#endif //XSTC_NO_CPP
#ifndef XSTC_NO_CSOUND
this->Keys_Set.csound = wxT("");
this->Keys_Set.csound1 = wxT("");
this->Keys_Set.csound2 = wxT("");
#endif //XSTC_NO_CSOUND
#ifndef XSTC_NO_CSS
this->Keys_Set.css = wxT("");
this->Keys_Set.css1 = wxT("");
this->Keys_Set.css2 = wxT("");
#endif //XSTC_NO_CSS
#ifndef XSTC_NO_D
this->Keys_Set.d = wxT("");
this->Keys_Set.d1 = wxT("");
this->Keys_Set.d2 = wxT("");
this->Keys_Set.d3 = wxT("");
#endif //XSTC_NO_D
#ifndef XSTC_NO_EIFFEL
this->Keys_Set.eiffel = wxT("");
#endif //XSTC_NO_EIFFEL
#ifndef XSTC_NO_EIFFELKW
this->Keys_Set.eiffelkw = wxT("");
#endif //XSTC_NO_EIFFELKW
#ifndef XSTC_NO_ERLANG
this->Keys_Set.erlang = wxT("");
#endif //XSTC_NO_ERLANG
#ifndef XSTC_NO_ESCRIPT
this->Keys_Set.escript = wxT("");
this->Keys_Set.escript1 = wxT("");
this->Keys_Set.escript2 = wxT("");
#endif //XSTC_NO_ESCRIPT
#ifndef XSTC_NO_F77
this->Keys_Set.f77_0 = wxT("");
this->Keys_Set.f77_1 = wxT("");
this->Keys_Set.f77_2 = wxT("");
#endif //XSTC_NO_F77
#ifndef XSTC_NO_FLAGSHIP
this->Keys_Set.flagship = wxT("");
this->Keys_Set.flagship1 = wxT("");
this->Keys_Set.flagship2 = wxT("");
this->Keys_Set.flagship3 = wxT("");
#endif //XSTC_NO_FLAGSHIP
#ifndef XSTC_NO_FORTH
this->Keys_Set.forth = wxT("");
this->Keys_Set.forth1 = wxT("");
this->Keys_Set.forth2 = wxT("");
this->Keys_Set.forth3 = wxT("");
this->Keys_Set.forth4 = wxT("");
this->Keys_Set.forth5 = wxT("");
#endif //XSTC_NO_FORTH
#ifndef XSTC_NO_FORTRAN
this->Keys_Set.fortran = wxT("");
this->Keys_Set.fortran1 = wxT("");
this->Keys_Set.fortran2 = wxT("");
#endif //XSTC_NO_FORTRAN
#ifndef XSTC_NO_GAP
this->Keys_Set.gap = wxT("");
this->Keys_Set.gap1 = wxT("");
this->Keys_Set.gap2 = wxT("");
this->Keys_Set.gap3 = wxT("");
#endif //XSTC_NO_GAP
#ifndef XSTC_NO_GUI4CLI
this->Keys_Set.gui4cli = wxT("");
this->Keys_Set.gui4cli1 = wxT("");
this->Keys_Set.gui4cli2 = wxT("");
this->Keys_Set.gui4cli3 = wxT("");
this->Keys_Set.gui4cli4 = wxT("");
#endif //XSTC_NO_GUI4CLI
#ifndef XSTC_NO_HASKELL
this->Keys_Set.haskell = wxT("");
#endif //XSTC_NO_HASKELL
#ifndef XSTC_NO_HTM
this->Keys_Set.html = wxT("");
this->Keys_Set.html1 = wxT("");
this->Keys_Set.html2 = wxT("");
this->Keys_Set.html3 = wxT("");
this->Keys_Set.html4 = wxT("");
this->Keys_Set.html5 = wxT("");
this->Keys_Set.html6 = wxT("");
this->Keys_Set.html7 = wxT("");
#endif //XSTC_NO_HTM
#ifndef XSTC_NO_INNOSETUP
this->Keys_Set.inno = wxT("");
this->Keys_Set.inno1 = wxT("");
this->Keys_Set.inno2 = wxT("");
this->Keys_Set.inno3 = wxT("");
this->Keys_Set.inno4 = wxT("");
this->Keys_Set.inno5 = wxT("");
#endif //XSTC_NO_INNOSETUP
#ifndef XSTC_NO_JS
this->Keys_Set.js = wxT("");
this->Keys_Set.js1 = wxT("");
this->Keys_Set.js2 = wxT("");
#endif //XSTC_NO_JS
#ifndef XSTC_NO_JAVA
this->Keys_Set.java = wxT("");
this->Keys_Set.java1 = wxT("");
this->Keys_Set.java2 = wxT("");
this->Keys_Set.java3 = wxT("");
#endif //XSTC_NO_JAVA
#ifndef XSTC_NO_KIX
this->Keys_Set.kix = wxT("");
this->Keys_Set.kix1 = wxT("");
this->Keys_Set.kix2 = wxT("");
#endif //XSTC_NO_KIX
#ifndef XSTC_NO_LISP
this->Keys_Set.lisp = wxT("");
this->Keys_Set.lisp1 = wxT("");
#endif //XSTC_NO_LISP
#ifndef XSTC_NO_LOUT
this->Keys_Set.lout = wxT("");
this->Keys_Set.lout1 = wxT("");
this->Keys_Set.lout2 = wxT("");
#endif //XSTC_NO_LOUT
#ifndef XSTC_NO_LUA
this->Keys_Set.lua = wxT("");
this->Keys_Set.lua1 = wxT("");
this->Keys_Set.lua2 = wxT("");
this->Keys_Set.lua3 = wxT("");
this->Keys_Set.lua4 = wxT("");
this->Keys_Set.lua5 = wxT("");
this->Keys_Set.lua6 = wxT("");
this->Keys_Set.lua7 = wxT("");
#endif //XSTC_NO_LUA
#ifndef XSTC_NO_MAGIK
this->Keys_Set.magik = wxT("");
this->Keys_Set.magik1 = wxT("");
this->Keys_Set.magik2 = wxT("");
this->Keys_Set.magik3 = wxT("");
this->Keys_Set.magik4 = wxT("");
#endif //XSTC_NO_MAGIK
#ifndef XSTC_NO_MATLAB
this->Keys_Set.matlab = wxT("");
#endif //XSTC_NO_MATLAB
#ifndef XSTC_NO_METAPOST
this->Keys_Set.metapost = wxT("");
this->Keys_Set.metapost1 = wxT("");
#endif //XSTC_NO_METAPOST
#ifndef XSTC_NO_MMIXAL
this->Keys_Set.mmixal = wxT("");
this->Keys_Set.mmixal1 = wxT("");
this->Keys_Set.mmixal2 = wxT("");
#endif //XSTC_NO_MMIXAL
#ifndef XSTC_NO_NNCRONTAB
this->Keys_Set.nncrontab = wxT("");
this->Keys_Set.nncrontab1 = wxT("");
this->Keys_Set.nncrontab2 = wxT("");
#endif //XSTC_NO_NNCRONTAB
#ifndef XSTC_NO_NSIS
this->Keys_Set.nsis = wxT("");
this->Keys_Set.nsis1 = wxT("");
this->Keys_Set.nsis2 = wxT("");
this->Keys_Set.nsis3 = wxT("");
#endif //XSTC_NO_NSIS
#ifndef XSTC_NO_OCTAVE
this->Keys_Set.octave = wxT("");
#endif //XSTC_NO_OCTAVE
#ifndef XSTC_NO_OPAL
this->Keys_Set.opal = wxT("");
this->Keys_Set.opal1 = wxT("");
#endif //XSTC_NO_OPAL
#ifndef XSTC_NO_PASCAL
this->Keys_Set.pas = wxT("");
this->Keys_Set.pas1 = wxT("");
#endif //XSTC_NO_PASCAL
#ifndef XSTC_NO_PERL
this->Keys_Set.perl = wxT("");
#endif //XSTC_NO_PERL
#ifndef XSTC_NO_PLM
this->Keys_Set.plm = wxT("");
#endif //XSTC_NO_PLM
#ifndef XSTC_NO_POV
this->Keys_Set.pov = wxT("");
this->Keys_Set.pov1 = wxT("");
this->Keys_Set.pov2 = wxT("");
this->Keys_Set.pov3 = wxT("");
this->Keys_Set.pov4 = wxT("");
this->Keys_Set.pov5 = wxT("");
this->Keys_Set.pov6 = wxT("");
this->Keys_Set.pov7 = wxT("");
#endif //XSTC_NO_POV
#ifndef XSTC_NO_POWERSHELL
this->Keys_Set.powershell = wxT("");
this->Keys_Set.powershell1 = wxT("");
this->Keys_Set.powershell2 = wxT("");
#endif //XSTC_NO_POWERSHELL
#ifndef XSTC_NO_PROGRESS
this->Keys_Set.progress = wxT("");
this->Keys_Set.progress1 = wxT("");
this->Keys_Set.progress2 = wxT("");
#endif //XSTC_NO_PROGRESS
#ifndef XSTC_NO_PS
this->Keys_Set.ps = wxT("");
this->Keys_Set.ps1 = wxT("");
this->Keys_Set.ps2 = wxT("");
this->Keys_Set.ps3 = wxT("");
this->Keys_Set.ps4 = wxT("");
#endif //XSTC_NO_PS
#ifndef XSTC_NO_PYTHON
this->Keys_Set.python = wxT("");
this->Keys_Set.python1 = wxT("");
#endif //XSTC_NO_PYTHON
#ifndef XSTC_NO_R
this->Keys_Set.r = wxT("");
this->Keys_Set.r1 = wxT("");
this->Keys_Set.r2 = wxT("");
#endif //XSTC_NO_R
#ifndef XSTC_NO_REBOL
this->Keys_Set.rebol = wxT("");
this->Keys_Set.rebol1 = wxT("");
this->Keys_Set.rebol2 = wxT("");
#endif //XSTC_NO_REBOL
#ifndef XSTC_NO_RUBY
this->Keys_Set.ruby = wxT("");
#endif //XSTC_NO_RUBY
#ifndef XSTC_NO_SCRIPTOL
this->Keys_Set.scriptol = wxT("");
#endif //XSTC_NO_SCRIPTOL
#ifndef XSTC_NO_SMALLTALK
this->Keys_Set.smalltalk = wxT("");
#endif //XSTC_NO_SMALLTALK
#ifndef XSTC_NO_SPECMAN
this->Keys_Set.specman = wxT("");
this->Keys_Set.specman1 = wxT("");
this->Keys_Set.specman2 = wxT("");
this->Keys_Set.specman3 = wxT("");
#endif //XSTC_NO_SPECMAN
#ifndef XSTC_NO_SPICE
this->Keys_Set.spice = wxT("");
this->Keys_Set.spice1 = wxT("");
this->Keys_Set.spice2 = wxT("");
#endif //XSTC_NO_SPICE
#ifndef XSTC_NO_MSSQL
this->Keys_Set.mssql = wxT("");
this->Keys_Set.mssql1 = wxT("");
this->Keys_Set.mssql2 = wxT("");
this->Keys_Set.mssql3 = wxT("");
this->Keys_Set.mssql4 = wxT("");
this->Keys_Set.mssql5 = wxT("");
this->Keys_Set.mssql6 = wxT("");
#endif //XSTC_NO_MSSQL
#ifndef XSTC_NO_MYSQL
this->Keys_Set.mysql = wxT("");
this->Keys_Set.mysql1 = wxT("");
this->Keys_Set.mysql2 = wxT("");
this->Keys_Set.mysql3 = wxT("");
this->Keys_Set.mysql4 = wxT("");
this->Keys_Set.mysql5 = wxT("");
this->Keys_Set.mysql6 = wxT("");
this->Keys_Set.mysql7 = wxT("");
this->Keys_Set.mysql8 = wxT("");
#endif //XSTC_NO_MYSQL
#ifndef XSTC_NO_SQL
this->Keys_Set.sql = wxT("");
this->Keys_Set.sql1 = wxT("");
this->Keys_Set.sql2 = wxT("");
this->Keys_Set.sql3 = wxT("");
this->Keys_Set.sql4 = wxT("");
this->Keys_Set.sql5 = wxT("");
this->Keys_Set.sql6 = wxT("");
this->Keys_Set.sql7 = wxT("");
#endif //XSTC_NO_SQL
#ifndef XSTC_NO_TADS3
this->Keys_Set.tads3_0 = wxT("");
this->Keys_Set.tads3_1 = wxT("");
this->Keys_Set.tads3_2 = wxT("");
this->Keys_Set.tads3_3 = wxT("");
#endif //XSTC_NO_TADS3
#ifndef XSTC_NO_TCL
this->Keys_Set.tcl = wxT("");
this->Keys_Set.tcl1 = wxT("");
this->Keys_Set.tcl2 = wxT("");
this->Keys_Set.tcl3 = wxT("");
this->Keys_Set.tcl4 = wxT("");
this->Keys_Set.tcl5 = wxT("");
this->Keys_Set.tcl6 = wxT("");
this->Keys_Set.tcl7 = wxT("");
this->Keys_Set.tcl8 = wxT("");
#endif //XSTC_NO_TCL
#ifndef XSTC_NO_TEX
this->Keys_Set.tex = wxT("");
#endif //XSTC_NO_TEX
#ifndef XSTC_NO_VB
this->Keys_Set.vb = wxT("");
this->Keys_Set.vb1 = wxT("");
this->Keys_Set.vb2 = wxT("");
this->Keys_Set.vb3 = wxT("");
#endif //XSTC_NO_VB
#ifndef XSTC_NO_VBSCRIPT
this->Keys_Set.vbscript = wxT("");
this->Keys_Set.vbscript1 = wxT("");
this->Keys_Set.vbscript2 = wxT("");
this->Keys_Set.vbscript3 = wxT("");
#endif //XSTC_NO_VBSCRIPT
#ifndef XSTC_NO_VERILOG
this->Keys_Set.verilog = wxT("");
this->Keys_Set.verilog1 = wxT("");
this->Keys_Set.verilog2 = wxT("");
this->Keys_Set.verilog3 = wxT("");
#endif //XSTC_NO_VERILOG
#ifndef XSTC_NO_VHDL
this->Keys_Set.vhdl = wxT("");
this->Keys_Set.vhdl1 = wxT("");
this->Keys_Set.vhdl2 = wxT("");
this->Keys_Set.vhdl3 = wxT("");
this->Keys_Set.vhdl4 = wxT("");
this->Keys_Set.vhdl5 = wxT("");
this->Keys_Set.vhdl6 = wxT("");
#endif //XSTC_NO_VHDL
#ifndef XSTC_NO_YAML
this->Keys_Set.yaml = wxT("");
#endif //XSTC_NO_YAML
}

void XSTC::InitKeys()
{
#ifndef XSTC_NO_KEYS_DEFINED
#ifndef XSTC_NO_ABAQUS
this->Keys_Set.abaqus =  wxT("");

this->Keys_Set.abaqus1 =  wxT("");

this->Keys_Set.abaqus2 =  wxT("");

this->Keys_Set.abaqus3 =  wxT("");

this->Keys_Set.abaqus4 =  wxT("");

this->Keys_Set.abaqus5 =  wxT("");
#endif //XSTC_NO_ABAQUS

#ifndef XSTC_NO_ADA
this->Keys_Set.ada =  wxT("abort abstract accept access aliased all array at begin body "
                      "case constant declare delay delta digits do else elsif end entry exception exit for "
                      "function generic goto if in is limited loop new null of others out package pragma "
                      "private procedure protected raise range record renames requeue return reverse "
                      "select separate subtype tagged task terminate then type until use when while with "
                      "abs and mod not or rem xor ");
#endif //XSTC_NO_ADA

#ifndef XSTC_NO_APDL
this->Keys_Set.apdl =  wxT("");

this->Keys_Set.apdl1 =  wxT("");

this->Keys_Set.apdl2 =  wxT("");

this->Keys_Set.apdl3 =  wxT("");

this->Keys_Set.apdl4 =  wxT("");

this->Keys_Set.apdl5 =  wxT("");
#endif //XSTC_NO_APDL

#ifndef XSTC_NO_ASM
this->Keys_Set.asm_0 =  wxT("aaa aad aam aas adc add and call cbw "
                        "clc cld cli cmc cmp cmps cmpsb cmpsw cwd daa das dec div esc hlt "
                        "idiv imul in inc int into iret ja jae jb jbe jc jcxz je jg jge jl "
                        "jle jmp jna jnae jnb jnbe jnc jne jng jnge jnl jnle jno jnp jns "
                        "jnz jo jp jpe jpo js jz lahf lds lea les lods lodsb lodsw loop "
                        "loope loopew loopne loopnew loopnz loopnzw loopw loopz loopzw "
                        "mov movs movsb movsw mul neg nop not or out pop popf push pushf "
                        "rcl rcr ret retf retn rol ror sahf sal sar sbb scas scasb scasw "
                        "shl shr stc std sti stos stosb stosw sub test wait xchg xlat "
                        "xlatb xor bound enter ins insb insw leave outs outsb outsw popa "
                        "pusha pushw arpl lar lsl sgdt sidt sldt smsw str verr verw clts "
                        "lgdt lidt lldt lmsw ltr bsf bsr bt btc btr bts cdq cmpsd cwde insd "
                        "iretd iretdf  iretf jecxz lfs lgs lodsd loopd  looped  loopned "
                        "loopnzd  loopzd  lss movsd movsx movzx outsd popad popfd pushad pushd "
                        "pushfd scasd seta setae setb setbe setc sete setg setge setl setle "
                        "setna setnae setnb setnbe setnc setne setng setnge setnl setnle "
                        "setno setnp setns setnz seto setp setpe setpo sets setz shld shrd stosd "
                        "bswap cmpxchg invd  invlpg  wbinvd  xadd "
                        "lock rep repe repne repnz repz cflush cpuid emms femms "
                        "cmovo cmovno cmovb cmovc cmovnae cmovae cmovnb cmovnc "
                        "cmove cmovz cmovne cmovnz cmovbe cmovna cmova cmovnbe "
                        "cmovs cmovns cmovp cmovpe cmovnp cmovpo cmovl cmovnge "
                        "cmovge cmovnl cmovle cmovng cmovg cmovnle "
                        "cmpxchg486 cmpxchg8b loadall loadall286 ibts icebp int1 int3 int01 "
                        "int03 iretw popaw popfw pushaw pushfw rdmsr rdpmc rdshr rdtsc "
                        "rsdc rsldt rsm rsts salc smi smint smintold svdc svldt svts "
                        "syscall sysenter sysexit sysret ud0 ud1 ud2 umov xbts wrmsr wrshr ");

this->Keys_Set.asm_1 =  wxT("f2xm1 fabs fadd faddp fbld fbstp fchs fclex fcom fcomp fcompp fdecstp "
                        "fdisi fdiv fdivp fdivr fdivrp feni ffree fiadd ficom ficomp fidiv "
                        "fidivr fild fimul fincstp finit fist fistp fisub fisubr fld fld1 "
                        "fldcw fldenv fldenvw fldl2e fldl2t fldlg2 fldln2 fldpi fldz fmul "
                        "fmulp fnclex fndisi fneni fninit fnop fnsave fnsavew fnstcw fnstenv "
                        "fnstenvw fnstsw fpatan fprem fptan frndint frstor frstorw fsave"
                        "fsavew fscale fsqrt fst fstcw fstenv fstenvw fstp fstsw fsub fsubp "
                        "fsubr fsubrp ftst fwait fxam fxch fxtract fyl2x fyl2xp1 "
                        "fsetpm fcos fldenvd fnsaved fnstenvd fprem1 frstord fsaved fsin fsincos "
                        "fstenvd fucom fucomp fucompp fcomi fcomip ffreep "
                        "fcmovb fcmove fcmovbe fcmovu fcmovnb fcmovne fcmovnbe fcmovnu ");

this->Keys_Set.asm_2 =  wxT("ah al ax bh bl bp bx ch cl cr0 cr2 cr3 cr4 cs "
                        "cx dh di dl dr0 dr1 dr2 dr3 dr6 dr7 ds dx eax ebp ebx ecx edi edx "
	                     "es esi esp fs gs si sp ss st tr3 tr4 tr5 tr6 tr7 "
	                     "st0 st1 st2 st3 st4 st5 st6 st7 mm0 mm1 mm2 mm3 mm4 mm5 mm6 mm7 "
	                     "xmm0 xmm1 xmm2 xmm3 xmm4 xmm5 xmm6 xmm7 ");

this->Keys_Set.asm_3 =  wxT("186 286 286c 286p 287 386 386c 386p 387 486 486p "
	                     "8086 8087 alpha break code const continue cref data data? "
	                     "dosseg else elseif endif endw err err1 err2 errb "
	                     "errdef errdif errdifi erre erridn erridni errnb errndef "
	                     "errnz exit fardata fardata? if lall lfcond list listall "
	                     "listif listmacro listmacroall  model no87 nocref nolist "
	                     "nolistif nolistmacro radix repeat sall seq sfcond stack "
	                     "startup tfcond type until untilcxz while xall xcref "
	                     "xlist alias align assume catstr comm comment db dd df dosseg dq "
	                     "dt dup dw echo else elseif elseif1 elseif2 elseifb elseifdef elseifdif "
	                     "elseifdifi elseife elseifidn elseifidni elseifnb elseifndef end "
	                     "endif endm endp ends eq equ even exitm extern externdef extrn for "
	                     "forc ge goto group gt high highword if if1 if2 ifb ifdef ifdif "
	                     "ifdifi ife  ifidn ifidni ifnb ifndef include includelib instr invoke "
	                     "irp irpc label le length lengthof local low lowword lroffset "
	                     "lt macro mask mod msfloat name ne offset opattr option org %out "
	                     "page popcontext proc proto ptr public purge pushcontext record "
	                     "repeat rept seg segment short size sizeof sizestr struc struct "
	                     "substr subtitle subttl textequ this title type typedef union while width "
                        "db dw dd dq dt resb resw resd resq rest incbin equ times "
	                     "%define %idefine %xdefine %xidefine %undef %assign %iassign "
	                     "%strlen %substr %macro %imacro %endmacro %rotate nolist "
	                     "%if %elif %else %endif %ifdef %ifndef %elifdef %elifndef "
	                     "%ifmacro %ifnmacro %elifmacro %elifnmacro %ifctk %ifnctk %elifctk %elifnctk "
	                     "%ifidn %ifnidn %elifidn %elifnidn %ifidni %ifnidni %elifidni %elifnidni "
	                     "%ifid %ifnid %elifid %elifnid %ifstr %ifnstr %elifstr %elifnstr "
	                     "%ifnum %ifnnum %elifnum %elifnnum %error %rep %endrep %exitrep "
	                     "%include %push %pop %repl struct endstruc istruc at iend align alignb "
	                     "%arg %stacksize %local %line "
	                     "bits use16 use32 section absolute extern global common cpu org "
	                     "section group import export ");

this->Keys_Set.asm_4 =  wxT("$ ? @b @f addr basic byte c carry? dword "
                        "far far16 fortran fword near near16 overflow? parity? pascal qword "
                        "real4  real8 real10 sbyte sdword sign? stdcall sword syscall tbyte "
                        "vararg word zero? flat near32 far32 "
                        "abs all assumes at casemap common compact "
                        "cpu dotname emulator epilogue error export expr16 expr32 farstack flat "
                        "forceframe huge language large listing ljmp loadds m510 medium memory "
                        "nearstack nodotname noemulator nokeyword noljmp nom510 none nonunique "
                        "nooldmacros nooldstructs noreadonly noscoped nosignextend nothing "
                        "notpublic oldmacros oldstructs os_dos para private prologue radix "
                        "readonly req scoped setif2 smallstack tiny use16 use32 uses "
                        "a16 a32 o16 o32 byte word dword nosplit $ $$ seq wrt "
                        "flat large small text data bss near far "
                        "%0 %1 %2 %3 %4 %5 %6 %7 %8 %9 ");

this->Keys_Set.asm_5 =  wxT("addpd addps addsd addss andpd andps andnpd andnps "
	                     "cmpeqpd cmpltpd cmplepd cmpunordpd cmpnepd cmpnltpd cmpnlepd cmpordpd "
	                     "cmpeqps cmpltps cmpleps cmpunordps cmpneps cmpnltps cmpnleps cmpordps "
	                     "cmpeqsd cmpltsd cmplesd cmpunordsd cmpnesd cmpnltsd cmpnlesd cmpordsd "
	                     "cmpeqss cmpltss cmpless cmpunordss cmpness cmpnltss cmpnless cmpordss "
	                     "comisd comiss cvtdq2pd cvtdq2ps cvtpd2dq cvtpd2pi cvtpd2ps "
	                     "cvtpi2pd cvtpi2ps cvtps2dq cvtps2pd cvtps2pi cvtss2sd cvtss2si "
	                     "cvtsd2si cvtsd2ss cvtsi2sd cvtsi2ss "
	                     "cvttpd2dq cvttpd2pi cvttps2dq cvttps2pi cvttsd2si cvttss2si "
	                     "divpd divps divsd divss fxrstor fxsave ldmxscr lfence mfence "
	                     "maskmovdqu maskmovdq maxpd maxps paxsd maxss minpd minps minsd minss "
	                     "movapd movaps movdq2q movdqa movdqu movhlps movhpd movhps movd movq "
	                     "movlhps movlpd movlps movmskpd movmskps movntdq movnti movntpd movntps "
	                     "movntq movq2dq movsd movss movupd movups mulpd mulps mulsd mulss "
	                     "orpd orps packssdw packsswb packuswb paddb paddsb paddw paddsw "
	                     "paddd paddsiw paddq paddusb paddusw pand pandn pause paveb pavgb pavgw "
	                     "pavgusb pdistib pextrw pcmpeqb pcmpeqw pcmpeqd pcmpgtb pcmpgtw pcmpgtd "
	                     "pf2id pf2iw pfacc pfadd pfcmpeq pfcmpge pfcmpgt pfmax pfmin pfmul "
	                     "pmachriw pmaddwd pmagw pmaxsw pmaxub pminsw pminub pmovmskb "
	                     "pmulhrwc pmulhriw pmulhrwa pmulhuw pmulhw pmullw pmuludq "
	                     "pmvzb pmvnzb pmvlzb pmvgezb pfnacc pfpnacc por prefetch prefetchw "
	                     "prefetchnta prefetcht0 prefetcht1 prefetcht2 pfrcp pfrcpit1 pfrcpit2 "
	                     "pfrsqit1 pfrsqrt pfsub pfsubr pi2fd pf2iw pinsrw psadbw pshufd "
	                     "pshufhw pshuflw pshufw psllw pslld psllq pslldq psraw psrad "
	                     "psrlw psrld psrlq psrldq psubb psubw psubd psubq psubsb psubsw "
	                     "psubusb psubusw psubsiw pswapd punpckhbw punpckhwd punpckhdq punpckhqdq "
	                     "punpcklbw punpcklwd punpckldq punpcklqdq pxor rcpps rcpss "
	                     "rsqrtps rsqrtss sfence shufpd shufps sqrtpd sqrtps sqrtsd sqrtss "
	                     "stmxcsr subpd subps subsd subss ucomisd ucomiss "
	                     "unpckhpd unpckhps unpcklpd unpcklps xorpd xorps ");
#endif //XSTC_NO_ASM

#ifndef XSTC_NO_ASN1
this->Keys_Set.asn1_0 =  wxT("ACCESS AGENT AUGMENTS BEGIN BITS "
                         "CAPABILITIES CHOICE COMPLIANCE CONTACT CONVENTION "
                         "DEFINITIONS DEFVAL DESCRIPTION DISPLAY "
                         "END ENTERPRISE EXPORTS FALSE FROM GROUP GROUPS HINT "
                         "IDENTITY IMPLIED IMPORTS INCLUDES INDEX INFO "
                         "LAST MANDATORY MAX MIN MODULE "
                         "NOTATION NOTIFICATION NULL "
                         "OBJECTS OBJECT-TYPE OF ORGANIZATION PRODUCT "
                         "RELEASE REFERENCE REQUIRES REVISION "
                         "SEQUENCE SIZE STATUS SUPPORTS SYNTAX UPDATED "
                         "VALUE VARIABLES VARIATION WRITE ");

this->Keys_Set.asn1_1 =  wxT("accessible create current deprecated for "
                         "mandatory not notify not-accessible "
                         "obsolete only optional read read-only "
                         "read-write write ");

this->Keys_Set.asn1_2 =  wxT("ABSENT ANY APPLICATION BIT BOOLEAN BY "
                         "COMPONENT COMPONENTS DEFAULT DEFINED "
                         "ENUMERATED EXPLICIT EXTERNAL IMPLICIT "
                         "INIFINITY MAX MIN MINUS OPTIONAL "
                         "PRESENT PRIVATE REAL SET TAGS TRUE ");

this->Keys_Set.asn1_3 =  wxT("Counter Counter32 Counter64 DisplayString "
                         "Gauge Gauge32 IDENTIFIER INTEGER Integer32 "
                         "IpAddress NetworkAddress NsapAddress "
                         "OBJECT OCTET Opaque PhysAddress STRING "
                         "TimeTicks UInteger32 UNITS Unsigned32 ");
#endif //XSTC_NO_ASN1

#ifndef XSTC_NO_ASYMPTOTE
this->Keys_Set.asymptote =  wxT("");

this->Keys_Set.asymptote1 =  wxT("");
#endif //XSTC_NO_ASYMPTOTE

#ifndef XSTC_NO_AU3
this->Keys_Set.au3_0 =  wxT("");

this->Keys_Set.au3_1 =  wxT("and byref case continueloop dim do else elseif endfunc endif endselect exit exitloop for func "
                        "global if local next not or return select step then to until wend while exit ");

this->Keys_Set.au3_2 =  wxT("and byref case continueloop dim do else elseif endfunc endif endselect exit exitloop for func "
                        "global if local next not or return select step then to until wend while exit ");

this->Keys_Set.au3_3 =  wxT("abs acos adlibdisable adlibenable asc asin atan autoitsetoption autoitwingettitle autoitwinsettitle "
                        "bitand bitnot bitor bitshift bitxor blockinput break call cdtray chr clipget clipput controlclick "
                        "controlcommand controldisable controlenable controlfocus controlgetfocus controlgetpos controlgettext "
                        "controlhide controlmove controlsend controlsettext controlshow cos dec dircopy dircreate dirmove "
                        "dirremove drivegetdrive drivegetfilesystem drivegetlabel drivegetserial drivegettype drivesetlabel "
                        "drivespacefree drivespacetotal drivestatus envget envset envupdate eval exp filechangedir fileclose "
                        "filecopy filecreateshortcut filedelete fileexists filefindfirstfile filefindnextfile filegetattrib "
                        "filegetlongname filegetshortname filegetsize filegettime filegetversion fileinstall filemove "
                        "fileopen fileopendialog fileread filereadline filerecycle filerecycleempty filesavedialog fileselectfolder "
                        "filesetattrib filesettime filewrite filewriteline guicreate guicreateex guidefaultfont guidelete "
                        "guigetcontrolstate guihide guimsg guiread guirecvmsg guisendmsg guisetcontrol guisetcontroldata "
                        "guisetcontrolex guisetcontrolfont guisetcontrolnotify guisetcoord guisetcursor guishow guiwaitclose "
                        "guiwrite hex hotkeyset inidelete iniread iniwrite inputbox int isadmin isarray isdeclared isfloat "
                        "isint isnumber isstring log memgetstats mod mouseclick mouseclickdrag mousedown mousegetcursor "
                        "mousegetpos mousemove mouseup mousewheel msgbox number pixelchecksum pixelgetcolor pixelsearch "
                        "processclose processexists processsetpriority processwait processwaitclose progressoff progresson "
                        "progressset random regdelete regenumkey regenumval regread regwrite round run runasset runwait "
                        "send seterror shutdown sin sleep soundplay soundsetwavevolume splashimageon splashoff splashtexton "
                        "sqrt statusbargettext string stringaddcr stringformat stringinstr stringisalnum stringisalpha "
                        "stringisascii stringisdigit stringisfloat stringisint stringislower stringisspace stringisupper "
                        "stringisxdigit stringleft stringlen stringlower stringmid stringreplace stringright stringsplit "
                        "stringstripcr stringstripws stringtrimleft stringtrimright stringupper tan timerstart timerstop "
                        "tooltip traytip ubound urldownloadtofile winactivate winactive winclose winexists wingetcaretpos "
                        "wingetclasslist wingetclientsize wingethandle wingetpos wingetstate wingettext wingettitle "
                        "winkill winmenuselectitem winminimizeall winminimizeallundo winmove winsetontop winsetstate "
                        "winsettitle winwait winwaitactive winwaitclose winwaitnotactive ");

this->Keys_Set.au3_4 =  wxT("@appdatacommondir @appdatadir @autoitversion @commonfilesdir @compiled @computername @comspec "
                        "@cr @crlf @desktopcommondir @desktopdir @desktopheight @desktopwidth @documentscommondir @error "
                        "@favoritescommondir @favoritesdir @homedrive @homepath @homeshare @hour @ipaddress1 @ipaddress2 "
                        "@ipaddress3 @ipaddress4 @lf @logondnsdomain @logondomain @logonserver @mday @min @mon @mydocumentsdir "
                        "@osbuild @oslang @osservicepack @ostype @osversion @programfilesdir @programscommondir @programsdir "
                        "@scriptdir @scriptfullpath @scriptname @sec @startmenucommondir @startmenudir @startupcommondir "
                        "@startupdir @sw_hide @sw_maximize @sw_minimize @sw_restore @sw_show @systemdir @tab @tempdir "
                        "@userprofiledir @username @wday @windowsdir @workingdir @yday @year ");

this->Keys_Set.au3_5 =  wxT("{!} {#} {^} {{} {}} {+} {alt} {altdown} {altup} {appskey} {asc} {backspace} {browser_back} "
                        "{browser_favorites} {browser_forward} {browser_home} {browser_refresh} {browser_search} {browser_stop} {bs} {capslock} {ctrlbreak} "
                        "{ctrldown} {ctrlup} {del} {delete} {down} {end} {enter} {esc} {escape} {f1} {f10} {f11} {f12} {f2} {f3} {f4} {f5} {f6} {f7} {f8} {f9} "
                        "{home} {ins} {insert} {lalt} {launch_app1} {launch_app2} {launch_mail} {launch_media} {lctrl} {left} {lshift} {lwin} "
                        "{lwindown} {media_next} {media_play_pause} {media_prev} {media_stop} {numlock} "
                        "{numpad0} {numpad1} {numpad2} {numpad3} {numpad4} {numpad5} {numpad6} {numpad7} {numpad8} {numpad9} "
                        "{numpadadd} {numpaddiv} {numpaddot} {numpadenter} {numpadmult} {numpadsub} {pause} {pgdn} {pgup} "
                        "{printscreen} {ralt} {rctrl} {right} {rshift} {rwin} {rwindown} {scrolllock} {shiftdown} {shiftup} {sleep} {space} {tab} {up} "
                        "{volume_down} {volume_mute} {volume_up} ");

this->Keys_Set.au3_6 =  wxT("#include #include-once ");

this->Keys_Set.au3_7 =  wxT("#region #endregion ");
#endif //XSTC_NO_AU3

#ifndef XSTC_NO_AVE
this->Keys_Set.ave =  wxT("nil true false else for if while then elseif end av self in exit ");

this->Keys_Set.ave1 =  wxT("nil true false else for if while then elseif end av self in exit ");

this->Keys_Set.ave2 =  wxT("nil true false else for if while then elseif end av self in exit ");

this->Keys_Set.ave3 =  wxT("a b c d e f g h i j k l m n o p q r s t u v w x y z "
                       "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z "
                       "0 1 2 3 4 5 6 7 8 9 _#");

this->Keys_Set.ave4 =  wxT("a b c d e f g h i j k l m n o p q r s t u v w x y z "
                       "A B C D E F G H I J K L M N O P Q R S T U V W X Y Z "
                       "0 1 2 3 4 5 6 7 8 9 _# ");

this->Keys_Set.ave5 =  wxT("");
#endif //XSTC_NO_AVE

#ifndef XSTC_NO_BAAN
this->Keys_Set.baan =  wxT("");

this->Keys_Set.baan1 =  wxT("");
#endif //XSTC_NO_BAAN

#ifndef XSTC_NO_BLITZBASIC
#ifndef XSTC_NO_FREEBASIC
#ifndef XSTC_NO_POWERBASIC
#ifndef XSTC_NO_PUREBASIC
this->Keys_Set.basic =  wxT("");

this->Keys_Set.basic2 =  wxT("");

this->Keys_Set.basic3 =  wxT("");
#endif //XSTC_NO_PUREBASIC
#endif //XSTC_NO_POWERBASIC
#endif //XSTC_NO_FREEBASIC
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_BLITZBASIC
this->Keys_Set.basic1bl =  wxT("abs accepttcpstream acos after and apptitle asc asin atan atan2 "
                           "automidhandle autosuspend availvidmem backbuffer banksize before bin calldll "
                           "case ceil changedir channelpan channelpitch channelplaying channelvolume chr "
                           "closedir closefile closemovie closetcpserver closetcpstream closeudpstream cls "
                           "clscolor color colorblue colorgreen colorred commandline const copybank copyfile "
                           "copyimage copypixel copypixelfast copyrect copystream cos countgfxdrivers "
                           "countgfxmodes counthostips createbank createdir createimage createnetplayer "
                           "createprocess createtcpserver createtimer createudpstream currentdate currentdir "
                           "currenttime data debuglog default delay delete deletedir deletefile "
                           "deletenetplayer desktopbuffer dim dottedip drawblock drawblockrect drawimage "
                           "drawimagerect drawmovie each else else if elseif end end function end if end "
                           "select end type endgraphics endif eof execfile exit exp false field filepos "
                           "filesize filetype first flip float floor flushjoy flushkeys flushmouse "
                           "fontheight fontname fontsize fontstyle fontwidth for forever freebank freefont "
                           "freeimage freesound freetimer frontbuffer function gammablue gammagreen gammared "
                           "getcolor getenv getkey getmouse gfxdrivername gfxmodedepth gfxmodeexists "
                           "gfxmodeformat gfxmodeheight gfxmodewidth global gosub goto grabimage graphics "
                           "graphicsbuffer graphicsdepth graphicsformat graphicsheight graphicswidth "
                           "handleimage hex hidepointer hostip hostnetgame if imagebuffer imageheight "
                           "imagerectcollide imagerectoverlap imagescollide imagesoverlap imagewidth "
                           "imagexhandle imageyhandle include input insert instr int joinnetgame joydown "
                           "joyhat joyhit joypitch joyroll joytype joyu joyudir joyv joyvdir joyx joyxdir "
                           "joyy joyyaw joyydir joyz joyzdir keydown keyhit keywait last left len line "
                           "loadanimimage loadbuffer loadfont loadimage loadsound local lockbuffer "
                           "lockedformat lockedpitch lockedpixels log log10 loopsound lower lset maskimage "
                           "mid midhandle millisecs mod morefiles mousedown mousehit mousex mousexspeed "
                           "mousey mouseyspeed mousez mousezspeed movemouse movieheight movieplaying "
                           "moviewidth netmsgdata netmsgfrom netmsgto netmsgtype netplayerlocal "
                           "netplayername new next nextfile not null openfile openmovie opentcpstream or "
                           "origin oval pausechannel pausetimer peekbyte peekfloat peekint peekshort pi "
                           "playcdtrack playmusic playsound plot pokebyte pokefloat pokeint pokeshort print "
                           "queryobject rand read readavail readbyte readbytes readdir readfile readfloat "
                           "readint readline readpixel readpixelfast readshort readstring rect rectsoverlap "
                           "recvnetmsg recvudpmsg repeat replace resettimer resizebank resizeimage restore "
                           "resumechannel resumetimer return right rnd rndseed rotateimage rset runtimeerror "
                           "sar savebuffer saveimage scaleimage scanline seedrnd seekfile select sendnetmsg "
                           "sendudpmsg setbuffer setenv setfont setgamma setgfxdriver sgn shl showpointer "
                           "shr sin soundpan soundpitch soundvolume sqr startnetgame step stop stopchannel "
                           "stopnetgame str string stringheight stringwidth systemproperty tan tcpstreamip "
                           "tcpstreamport tcptimeouts text tformfilter tformimage then tileblock tileimage "
                           "timerticks to totalvidmem trim true type udpmsgip udpmsgport udpstreamip "
                           "udpstreamport udptimeouts unlockbuffer until updategamma upper viewport vwait "
                           "waitkey waitmouse waittimer wend while write writebyte writebytes writefile "
                           "writefloat writeint writeline writepixel writepixelfast writeshort writestring xor ");
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_FREEBASIC
this->Keys_Set.basic1fr =  wxT("append as asc asin asm atan2 atn beep bin binary bit bitreset bitset bload "
                           "bsave byref byte byval call callocate case cbyte cdbl cdecl chain chdir chr "
                           "cint circle clear clng clngint close cls color command common cons const "
                           "continue cos cshort csign csng csrlin cubyte cuint culngint cunsg curdir "
                           "cushort custom cvd cvi cvl cvlongint cvs cvshort data date deallocate declare "
                           "defbyte defdbl defined defint deflng deflngint defshort defsng defstr defubyte "
                           "defuint defulngint defushort dim dir do double draw dylibload dylibsymbol else "
                           "elseif end enum environ environ$ eof eqv erase err error exec exepath exit exp "
                           "export extern field fix flip for fre freefile function get getjoystick getkey "
                           "getmouse gosub goto hex hibyte hiword if iif imagecreate imagedestroy imp "
                           "inkey inp input instr int integer is kill lbound lcase left len let lib line "
                           "lobyte loc local locate lock lof log long longint loop loword lset ltrim "
                           "mid mkd mkdir mki mkl mklongint mks mkshort mod multikey mutexcreate "
                           "mutexdestroy mutexlock mutexunlock name next not oct on once open option or out "
                           "output overload paint palette pascal pcopy peek peeki peeks pipe pmap point "
                           "pointer poke pokei pokes pos preserve preset print private procptr pset ptr "
                           "public put random randomize read reallocate redim rem reset restore resume "
                           "resume next return rgb rgba right rmdir rnd rset rtrim run sadd screen "
                           "screencopy screeninfo screenlock screenptr screenres screenset screensync "
                           "screenunlock seek statement seek function selectcase setdate setenviron "
                           "setmouse settime sgn shared shell shl short shr sin single sizeof sleep space "
                           "spc sqr static stdcall step stop str string string strptr sub swap system tab "
                           "tan then threadcreate threadwait time time timer to trans trim type ubound "
                           "ubyte ucase uinteger ulongint union unlock unsigned until ushort using va_arg "
                           "va_first va_next val val64 valint varptr view viewprint wait wend while width "
                           "window windowtitle with write xor zstring ");
#endif //XSTC_NO_FREEBASIC

#ifndef XSTC_NO_POWERBASIC
this->Keys_Set.basic1po =  wxT("");
#endif //XSTC_NO_POWERBASIC

#ifndef XSTC_NO_PUREBASIC
this->Keys_Set.basic1pu =  wxT("and break case continue data "
                           "datasection declare declarecdll declaredll default deftype dim else "
                           "elseif end enddatasection endenumeration endif endinterface endprocedure "
                           "endselect endstructure endstructureunion enumeration extends fakereturn "
                           "for foreach forever global gosub goto if includebinary includefile "
                           "includepath interface newlist next or procedure procedurecdll "
                           "proceduredll procedurereturn protected read repeat restore return select "
                           "shared static step structure structureunion to until wend while xincludefile ");
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
this->Keys_Set.bash =  wxT("");
#endif //XSTC_NO_BASH

#ifndef XSTC_NO_BATCH
this->Keys_Set.batch =  wxT("rem set if exist errorlevel for in do "
                        "break call chcp cd chdir choice cls country ctty date del erase dir echo "
                        "exit goto loadfix loadhigh mkdir md move path pause prompt rename ren "
                        "rmdir rd shift time type ver verify vol "
                        "com con lpt nul "
                        "color copy defined else not start ");
#endif //XSTC_NO_BATCH

#ifndef XSTC_NO_BULLANT
this->Keys_Set.bullant = wxT("abstract all ancestor and application "
                         "assert attributes author begin "
                         "callback class concrete config constants construct continue "
                         "depends description downcast driver "
                         "elif else ensures error exception exposure extension "
                         "false fatal final function generics glyph help hidden host "
                         "immutable in inherits is kernel label leave library locals "
                         "mutable none not null obsolete options or other "
                         "parameters peer private public raise reason restricted retry return "
                         "returns rollback route security self settings severity step "
                         "task test transaction true unknown varying warning when "
                         "method end if until while trap case debug for foreach lock "
                         "boolean character character$ date date$ datetime datetime$ "
                         "float hex$ identifier identifier$ integer interval interval$ "
                         "money money$ raw raw$ string tick tick$ time time$ version version$ ");
#endif //XSTC_NO_BULLANT

#ifndef XSTC_NO_CAML
this->Keys_Set.caml =  wxT("and as assert asr begin class constraint do done downto else end "
                       "exception external false for fun function functor if in include "
                       "inherit initializer land lazy let lor lsl lsr lxor match method "
                       "mod module mutable new object of open or private rec sig struct "
                       "then to true try type val virtual when while with");

this->Keys_Set.caml1 =  wxT("option Some None ignore ref lnot succ pred ");

this->Keys_Set.caml2 =  wxT("array bool char float int list string unit ");
#endif //XSTC_NO_CAML

#ifndef XSTC_NO_CLW
this->Keys_Set.clw =  wxT("");

this->Keys_Set.clw1 =  wxT("");

this->Keys_Set.clw2 =  wxT("");

this->Keys_Set.clw3 =  wxT("");

this->Keys_Set.clw4 =  wxT("");

this->Keys_Set.clw5 =  wxT("");

this->Keys_Set.clw6 =  wxT("");

this->Keys_Set.clw7 =  wxT("");

this->Keys_Set.clw8 =  wxT("");
#endif //XSTC_NO_CLW

#ifndef XSTC_NO_CMAKE
this->Keys_Set.cmake =  wxT("");

this->Keys_Set.cmake1 =  wxT("");

this->Keys_Set.cmake2 =  wxT("");
#endif //XSTC_NO_CMAKE

#ifndef XSTC_NO_CONF
this->Keys_Set.conf =  wxT("acceptmutex acceptpathinfo accessconfig accessfilename "
                       "action addalt addaltbyencoding addaltbytype addcharset "
                       "adddefaultcharset adddescription addencoding "
                       "addhandler addicon addiconbyencoding addiconbytype "
                       "addinputfilter addlanguage addmodule addmoduleinfo "
                       "addoutputfilter addoutputfilterbytype addtype agentlog "
                       "alias aliasmatch all allow allowconnect "
                       "allowencodedslashes allowoverride anonymous "
                       "anonymous_authoritative anonymous_logemail "
                       "anonymous_mustgiveemail anonymous_nouserid "
                       "anonymous_verifyemail assignuserid authauthoritative "
                       "authdbauthoritative authdbgroupfile "
                       "authdbmauthoritative authdbmgroupfile authdbmtype "
                       "authdbmuserfile authdbuserfile authdigestalgorithm "
                       "authdigestdomain authdigestfile authdigestgroupfile "
                       "authdigestnccheck authdigestnonceformat "
                       "authdigestnoncelifetime authdigestqop "
                       "authdigestshmemsize authgroupfile "
                       "authldapauthoritative authldapbinddn "
                       "authldapbindpassword authldapcharsetconfig "
                       "authldapcomparednonserver authldapdereferencealiases "
                       "authldapenabled authldapfrontpagehack "
                       "authldapgroupattribute authldapgroupattributeisdn "
                       "authldapremoteuserisdn authldapurl authname authtype "
                       "authuserfile bindaddress browsermatch "
                       "browsermatchnocase bs2000account bufferedlogs "
                       "cachedefaultexpire cachedirlength cachedirlevels "
                       "cachedisable cacheenable cacheexpirycheck cachefile "
                       "cacheforcecompletion cachegcclean cachegcdaily "
                       "cachegcinterval cachegcmemusage cachegcunused "
                       "cacheignorecachecontrol cacheignoreheaders "
                       "cacheignorenolastmod cachelastmodifiedfactor "
                       "cachemaxexpire cachemaxfilesize cacheminfilesize "
                       "cachenegotiateddocs cacheroot cachesize "
                       "cachetimemargin cgimapextension charsetdefault "
                       "charsetoptions charsetsourceenc checkspelling "
                       "childperuserid clearmodulelist contentdigest "
                       "cookiedomain cookieexpires cookielog cookiename "
                       "cookiestyle cookietracking coredumpdirectory customlog "
                       "dav davdepthinfinity davlockdb davmintimeout "
                       "defaulticon defaultlanguage defaulttype define "
                       "deflatebuffersize deflatecompressionlevel "
                       "deflatefilternote deflatememlevel deflatewindowsize "
                       "deny directory directoryindex directorymatch "
                       "directoryslash documentroot dumpioinput dumpiooutput "
                       "enableexceptionhook enablemmap enablesendfile "
                       "errordocument errorlog example expiresactive "
                       "expiresbytype expiresdefault extendedstatus "
                       "extfilterdefine extfilteroptions fancyindexing "
                       "fileetag files filesmatch forcelanguagepriority "
                       "forcetype forensiclog from group header headername "
                       "hostnamelookups identitycheck ifdefine ifmodule "
                       "imapbase imapdefault imapmenu include indexignore "
                       "indexoptions indexorderdefault isapiappendlogtoerrors "
                       "isapiappendlogtoquery isapicachefile isapifakeasync "
                       "isapilognotsupported isapireadaheadbuffer keepalive "
                       "keepalivetimeout languagepriority ldapcacheentries "
                       "ldapcachettl ldapconnectiontimeout ldapopcacheentries "
                       "ldapopcachettl ldapsharedcachefile ldapsharedcachesize "
                       "ldaptrustedca ldaptrustedcatype limit limitexcept "
                       "limitinternalrecursion limitrequestbody "
                       "limitrequestfields limitrequestfieldsize "
                       "limitrequestline limitxmlrequestbody listen "
                       "listenbacklog loadfile loadmodule location "
                       "locationmatch lockfile logformat loglevel maxclients "
                       "maxkeepaliverequests maxmemfree maxrequestsperchild "
                       "maxrequestsperthread maxspareservers maxsparethreads "
                       "maxthreads maxthreadsperchild mcachemaxobjectcount "
                       "mcachemaxobjectsize mcachemaxstreamingbuffer "
                       "mcacheminobjectsize mcacheremovalalgorithm mcachesize "
                       "metadir metafiles metasuffix mimemagicfile "
                       "minspareservers minsparethreads mmapfile "
                       "modmimeusepathinfo multiviewsmatch namevirtualhost "
                       "nocache noproxy numservers nwssltrustedcerts "
                       "nwsslupgradeable options order passenv pidfile port "
                       "protocolecho proxy proxybadheader proxyblock "
                       "proxydomain proxyerroroverride proxyiobuffersize "
                       "proxymatch proxymaxforwards proxypass proxypassreverse "
                       "proxypreservehost proxyreceivebuffersize proxyremote "
                       "proxyremotematch proxyrequests proxytimeout proxyvia "
                       "qsc readmename redirect redirectmatch "
                       "redirectpermanent redirecttemp refererignore "
                       "refererlog removecharset removeencoding removehandler "
                       "removeinputfilter removelanguage removeoutputfilter "
                       "removetype requestheader require resourceconfig "
                       "rewritebase rewritecond rewriteengine rewritelock "
                       "rewritelog rewriteloglevel rewritemap rewriteoptions "
                       "rewriterule rlimitcpu rlimitmem rlimitnproc satisfy "
                       "scoreboardfile script scriptalias scriptaliasmatch "
                       "scriptinterpretersource scriptlog scriptlogbuffer "
                       "scriptloglength scriptsock securelisten sendbuffersize "
                       "serveradmin serveralias serverlimit servername "
                       "serverpath serverroot serversignature servertokens "
                       "servertype setenv setenvif setenvifnocase sethandler "
                       "setinputfilter setoutputfilter singlelisten ssiendtag "
                       "ssierrormsg ssistarttag ssitimeformat ssiundefinedecho "
                       "sslcacertificatefile sslcacertificatepath "
                       "sslcarevocationfile sslcarevocationpath "
                       "sslcertificatechainfile sslcertificatefile "
                       "sslcertificatekeyfile sslciphersuite sslengine "
                       "sslmutex ssloptions sslpassphrasedialog sslprotocol "
                       "sslproxycacertificatefile sslproxycacertificatepath "
                       "sslproxycarevocationfile sslproxycarevocationpath "
                       "sslproxyciphersuite sslproxyengine "
                       "sslproxymachinecertificatefile "
                       "sslproxymachinecertificatepath sslproxyprotocol "
                       "sslproxyverify sslproxyverifydepth sslrandomseed "
                       "sslrequire sslrequiressl sslsessioncache "
                       "sslsessioncachetimeout sslusername sslverifyclient "
                       "sslverifydepth startservers startthreads "
                       "suexecusergroup threadlimit threadsperchild "
                       "threadstacksize timeout transferlog typesconfig "
                       "unsetenv usecanonicalname user userdir "
                       "virtualdocumentroot virtualdocumentrootip virtualhost "
                       "virtualscriptalias virtualscriptaliasip "
                       "win32disableacceptex xbithack ");

this->Keys_Set.conf1 =  wxT("on off standalone inetd "
                       "force-response-1 downgrade-1 nokeepalive "
                       "indexes includes followsymlinks none "
                       "x-compress x-gzip ");
#endif //XSTC_NO_CONF

#ifndef XSTC_NO_CPP
this->Keys_Set.cpp =  wxT("and and_eq asm auto bitand bitor bool break case catch "
                      "char class compl const const_cast continue default "
                      "delete do double dynamic_cast else enum explicit export "
                      "extern false float for friend goto if inline int long "
                      "mutable namespace new not not_eq operator or or_eq "
                      "private protected public register reinterpret_cast return "
                      "short signed sizeof static static_cast struct switch "
                      "template this throw true try typedef typeid typename "
                      "union unsigned using virtual void volatile wchar_t while "
                      "xor xor_eq ");

this->Keys_Set.cpp1 = wxT("bitset deque exception bad_exception filebuf fstream ifstream "
                      "ofstream wfilebuf wfstream wifstream wofstream fpos ios_base "
                      "streamoff streamsize iostream istream wiostream wistream "
                      "back_insert_iterator istream_iterator istreambuf_iterator "
                      "iterator iterator_traits ostream_iterator ostreambuf_iterator "
                      "reverse_iterator numeric_limits list codecvt collate collate_byname "
                      "ctype locale messages numpunct map multimap allocator auto_ptr "
                      "bad_alloc ostream wostream priority_queue queue multiset set "
                      "istringstream ostringstream stringbuf stringstream wistringstream "
                      "wostringstream wstringbuf wstringstream stack streambuf wstreambuf "
                      "char_traits string wstring istrstream ostrstream strstream strstreambuf "
                      "type_info pair gslice gslice_array indirect_array mask_array valarray vector ");

this->Keys_Set.cpp2 = wxT("a addindex addtogroup anchor arg attention author b brief bug c class "
                      "code date def defgroup deprecated dontinclude detailed e em endcode endhtmlonly "
                      "endif endlatexonly endlink endverbatim enum example exception f$ f[ f] "
                      "file fn hideinitializer htmlinclude htmlonly if image include ingroup "
                      "internal invariant interface latexonly li line link mainpage name "
                      "namespace nosubgrouping note overload p page par param post pre ref "
                      "relates remarks return retval sa section see showinitializer since skip "
                      "skipline struct subsection test throw todo typedef union until "
                      "var verbatim verbinclude version warning weakgroup $ @ \\ & < > # { } ");

this->Keys_Set.cpp3 = wxT("");
#endif //XSTC_NO_CPP

#ifndef XSTC_NO_CSOUND
this->Keys_Set.csound = wxT("");

this->Keys_Set.csound1 = wxT("sr kr ar ksmps nchnls ");

this->Keys_Set.csound2 = wxT("a i db in or zr Add Dec Div Inc Mul Sub abs and cos dam dec div exp "
                         "fin fof fog inh ino inq ins int inx inz lfo log mac mod mul not out "
                         "pan pow rms rnd shl sin sqr sub sum tab tan tb0 tb1 tb2 tb3 tb4 tb5 "
                         "tb6 tb7 tb8 tb9 urd vco xin xor zar zaw zir ziw zkr zkw adsr babo "
                         "buzz cent clip comb cosh diff divz fini fink fmb3 fof2 fold fout "
                         "frac ftsr gain goto in32 inch init line maca moog mute nrpn outc "
                         "outh outo outq outs outx outz peak port pset pvoc rand seed sinh "
                         "sqrt stix tabw tanh tb10 tb11 tb12 tb13 tb14 tb15 tone vadd vco2 "
                         "vdiv vexp vibr vmap vmul vpow wrap xout xyin zacl zarg zawm ziwm "
                         "zkcl zkwm FLbox FLjoy FLrun adsyn ampdb atone birnd bqrez butbp "
                         "butbr buthp butlp clear ctrl7 dbamp dconv delay dumpk endin endop "
                         "event expon fouti foutk ftgen ftlen gauss gbuzz grain guiro igoto "
                         "ihold instr integ kgoto limit linen log10 loopg loopl lpf18 madsr "
                         "max_k metro noise nsamp oscil out32 outch outic outkc outq1 outq2 "
                         "outq3 outq4 outs1 outs2 pareq pitch pluck portk print pvadd randh "
                         "randi rbjeq readk reson rezzy rnd31 scans scanu sense space tab_i "
                         "table tbvcf tempo timek times tival tonek tonex vaddv vbap4 vbap8 "
                         "vbapz vcomb vcopy vdecr vdivv veloc vexpv vibes vincr vmult voice "
                         "vport vpowv vpvoc vsubv vwrap wgbow xadsr zamod zkmod FLhide FLkeyb "
                         "FLknob FLpack FLshow FLtabs FLtext active adsynt alpass areson "
                         "atonek atonex bamboo bbcutm bbcuts biquad cabasa cauchy cggoto "
                         "cigoto ckgoto clfilt cngoto convle cosinv cpsoct cpspch cpstun "
                         "cpuprc cross2 crunch ctrl14 ctrl21 delay1 delayk delayr delayw "
                         "deltap denorm diskin dumpk2 dumpk3 dumpk4 envlpx expseg filesr "
                         "fiopen fmbell follow foscil foutir ftlen2 ftload ftmorf ftsave "
                         "grain2 grain3 harmon hrtfer initc7 interp jitter linenr lineto "
                         "linseg locsig loopge loople lorenz loscil lowres lpread lpslot "
                         "mandel mandol mclock mdelay midic7 midiin midion mirror moscil "
                         "mpulse mrtmsg mxadsr nlfilt noteon notnum ntrpol octave octcps "
                         "octpch opcode oscbnk oscil1 oscil3 oscili osciln oscils oscilx "
                         "outiat outipb outipc outkat outkpb outkpc pchoct phasor planet "
                         "poscil printk prints pvread pvsftr pvsftw random readk2 readk3 "
                         "readk4 reinit resonk resonr resonx resony resonz reverb rigoto "
                         "s16b14 s32b14 sekere sfload sfplay shaker sininv spat3d spdist "
                         "spsend strset table3 tablei tablew tabw_i taninv tigoto timout "
                         "turnon upsamp vbap16 vcella vco2ft vdel_k vdelay vlimit vmultv "
                         "randh vrandi wgclar xscans xscanu FLcolor FLcount FLgroup FLlabel "
                         "FLpanel FLvalue aftouch ampdbfs ampmidi aresonk balance bexprnd "
                         "biquada changed clockon cps2pch cpsmidi cpstmid cpstuni cpsxpch "
                         "dbfsamp dcblock deltap3 deltapi deltapn deltapx dispfft display "
                         "envlpxr exprand expsega expsegr filelen filter2 flanger fmmetal "
                         "fmrhode fmvoice follow2 foscili fprints ftchnls ftloadk ftlptim "
                         "ftsavek gogobel granule hilbert initc14 initc21 invalue jitter2 "
                         "jspline linrand linsegr locsend logbtwo loopseg loscil3 lowresx "
                         "lphasor lposcil lpreson lpshold marimba massign midic14 midic21 "
                         "midichn midion2 midiout moogvcf noteoff nreverb nstrnum octmidi "
                         "oscil1i outic14 outipat outkc14 outkpat pcauchy pchbend pchmidi "
                         "phaser1 phaser2 pinkish poisson polyaft poscil3 printk2 printks "
                         "product pvcross pvsanal pvsinfo pvsynth randomh randomi release "
                         "repluck reverb2 rspline rtclock seqtime sfilist sfinstr sfplay3 "
                         "sfplaym sfplist slider8 sndwarp soundin spat3di spat3dt specsum "
                         "streson tableiw tablekt tableng tablera tablewa taninv2 tempest "
                         "tlineto transeg trigger trigseq trirand turnoff unirand valpass "
                         "vco2ift vdelay3 vdelayk vdelayx vexpseg vibrato vlinseg vlowres "
                         "vmirror waveset weibull wgbrass wgflute wgpluck wguide1 wguide2 "
                         "xtratim zakinit FLbutton FLcolor2 FLprintk FLroller FLscroll "
                         "FLsetBox FLsetVal FLslider FLupdate betarand butterbp butterbr "
                         "userrnd deltapxw distort1 downsamp duserrnd filepeak fmpercfl "
                         "fmwurlie fprintks hsboscil lowpass2 lpfreson lpinterp lposcil3 "
                         "maxalloc midictrl multitap nestedap octmidib oscilikt outvalue "
                         "pchmidib powoftwo prealloc pvinterp pvsadsyn pvscross pvsfread "
                         "pvsmaska rireturn samphold schedule semitone sensekey setksmps "
                         "sfinstr3 sfinstrm sfplay3m sfpreset slider16 slider32 slider64 "
                         "slider8f soundout specaddm specdiff specdisp specfilt spechist "
                         "specptrk specscal spectrum sprintks subinstr svfilter tablegpw "
                         "tableikt tablemix tableseg tablewkt tablexkt tb0_init tb1_init "
                         "tb2_init tb3_init tb4_init tb5_init tb6_init tb7_init tb8_init "
                         "tb9_init tempoval vco2init vdelayxq vdelayxs vdelayxw vecdelay "
                         "wgpluck2 wterrain xscanmap zfilter2 FLbutBank FLgetsnap FLpackEnd "
                         "FLprintk2 FLsetFont FLsetSize FLsetText FLsetsnap FLslidBnk "
                         "FLtabsEnd dripwater eventname ktableseg noteondur osciliktp "
                         "oscilikts pgmassign phasorbnk pitchamdf pvbufread readclock "
                         "sandpaper scantable schedwhen sfinstr3m sfpassign slider16f "
                         "slider32f slider64f sndwarpst soundoutc soundouts tablecopy "
                         "tableigpw tableimix tablexseg tb10_init tb11_init tb12_init "
                         "tb13_init tb14_init tb15_init timeinstk timeinsts vbap4move "
                         "vbap8move vbapzmove vdelayxwq vdelayxws xscansmap FLgroupEnd "
                         "FLloadsnap FLpack_end FLpanelEnd FLsavesnap FLsetAlign FLsetColor "
                         "FLsetVal_i FLtabs_end filenchnls noteondur2 scanhammer schedkwhen "
                         "tableicopy tambourine vbap16move vbaplsinit wgbowedbar FLgroup_end "
                         "FLpanel_end FLscrollEnd FLsetColor2 mididefault midinoteoff "
                         "sleighbells FLscroll_end subinstrinit FLsetPosition FLsetTextSize "
                         "FLsetTextType midinoteoncps midinoteonkey midinoteonoct "
                         "midinoteonpch midipitchbend schedwhenname FLsetTextColor "
                         "schedkwhenname midicontrolchange midiprogramchan "
                         "midipolyaftertouch midichannelaftertouch ");
#endif //XSTC_NO_CSOUND

#ifndef XSTC_NO_CSS
this->Keys_Set.css =  wxT("border-top-color border-right-color border-bottom-color border-left-color border-color "
                      "border-top-style border-right-style border-bottom-style border-left-style border-style "
                      "top right bottom left position z-index direction unicode-bidi "
                      "min-width max-width min-height max-height overflow clip visibility content quotes "
                      "counter-reset counter-increment marker-offset "
                      "size marks page-break-before page-break-after page-break-inside page orphans widows "
                      "font-stretch font-size-adjust unicode-range units-per-em src "
                      "panose-1 stemv stemh slope cap-height x-height ascent descent widths bbox definition-src "
                      "baseline centerline mathline topline text-shadow "
                      "caption-side table-layout border-collapse border-spacing empty-cells speak-header "
                      "cursor outline outline-width outline-style outline-color "
                      "volume speak pause-before pause-after pause cue-before cue-after cue "
                      "play-during azimuth elevation speech-rate voice-family pitch pitch-range stress richness "
                      "speak-punctuation speak-numeral ");

this->Keys_Set.css1 =  wxT("first-letter first-line link active visited "
                       "first-child focus hover lang before after left right first ");

this->Keys_Set.css2 =  wxT("border-top-color border-right-color border-bottom-color border-left-color border-color "
                       "border-top-style border-right-style border-bottom-style border-left-style border-style "
                       "top right bottom left position z-index direction unicode-bidi "
                       "min-width max-width min-height max-height overflow clip visibility content quotes "
                       "counter-reset counter-increment marker-offset "
                       "size marks page-break-before page-break-after page-break-inside page orphans widows "
                       "font-stretch font-size-adjust unicode-range units-per-em src "
                       "panose-1 stemv stemh slope cap-height x-height ascent descent widths bbox definition-src "
                       "baseline centerline mathline topline text-shadow "
                       "caption-side table-layout border-collapse border-spacing empty-cells speak-header "
                       "cursor outline outline-width outline-style outline-color "
                       "volume speak pause-before pause-after pause cue-before cue-after cue "
                       "play-during azimuth elevation speech-rate voice-family pitch pitch-range stress richness "
                       "speak-punctuation speak-numeral ");
#endif //XSTC_NO_CSS

#ifndef XSTC_NO_D
this->Keys_Set.d = wxT("");

this->Keys_Set.d1 = wxT("");

this->Keys_Set.d2 = wxT("");

this->Keys_Set.d3 = wxT("");
#endif //XSTC_NO_D

#ifndef XSTC_NO_EIFFEL
this->Keys_Set.eiffel = wxT("alias all and any as bit boolean "
                        "check class character clone create creation current "
                        "debug deferred div do double "
                        "else elseif end ensure equal expanded export external "
                        "false feature forget from frozen general "
                        "if implies indexing infix inherit inspect integer invariant is "
                        "language like local loop mod name nochange none not "
                        "obsolete old once or platform pointer prefix precursor "
                        "real redefine rename require rescue result retry "
                        "select separate string strip then true undefine unique until "
                        "variant void when xor ");
#endif //XSTC_NO_EIFFEL

#ifndef XSTC_NO_EIFFELKW
this->Keys_Set.eiffelkw = wxT("alias all and any as bit boolean "
                          "check class character clone create creation current "
                          "debug deferred div do double "
                          "else elseif end ensure equal expanded export external "
                          "false feature forget from frozen general "
                          "if implies indexing infix inherit inspect integer invariant is "
                          "language like local loop mod name nochange none not "
                          "obsolete old once or platform pointer prefix precursor "
                          "real redefine rename require rescue result retry "
                          "select separate string strip then true undefine unique until "
                          "variant void when xor ");
#endif //XSTC_NO_EIFFELKW

#ifndef XSTC_NO_ERLANG
this->Keys_Set.erlang = wxT("after begin case catch cond end fun if let of query receive when "
                        "define record export import include include_lib ifdef ifndef else endif undef "
                        "apply attribute call do in letrec module primop try ");
#endif //XSTC_NO_ERLANG

#ifndef XSTC_NO_ESCRIPT
this->Keys_Set.escript = wxT("basic basicio boats cfgfile file http npc os uo util accessible addmenuitem appendconfigfileelem "
                         "applyconstraint applydamage applyrawdamage assignrecttoweatherregion append baseskilltorawskill boatfromitem "
                         "broadcast ban cdbl cint cstr checklineofsight checklosat checkskill consumemana consumereagents "
                         "consumesubstance createaccount createitematlocation createiteminbackpack createitemininventory "
                         "createitemincontainer createmenu createmultiatlocation createnpcfromtemplate "
                         "createrootiteminstoragearea createstoragearea clear_script_profile_counters close damage destroyitem destroymulti "
                         "destroyrootiteminstoragearea detach disableevents disconnectclient distance disable enableevents "
                         "enumerateitemsincontainer enumerateonlinecharacters equipfromtemplate "
                         "equipitem eraseglobalproperty eraseobjproperty enable enabled erase events_waiting exists findconfigelem "
                         "findobjtypeincontainer findrootiteminstoragearea findstoragearea fclose find fopen fread fseek ftell"
                         "fwrite gamestat getamount getcommandhelp getconfigint getconfigintkeys getconfigmaxintkey getconfigreal "
                         "getconfigstring getconfigstringkeys getconfigstringarray getelemproperty getequipmentbylayer getglobalproperty "
                         "getharvestdifficulty getmapinfo getmenuobjtypes getobjproperty getobjtype getobjtypebyname getproperty "
                         "getrawskill getregionstring getskill getspelldifficulty getstandingheight getworldheight grantprivilege "
                         "harvestresource healdamage hex islegalmove insert keys listequippeditems listghostsnearlocation listhostiles "
                         "listitemsatlocation listitemsnearlocation listitemsnearlocationoftype listmobilesinlineofsight "
                         "listmobilesnearlocation listmobilesnearlocationex listobjectsinbox loadtusscpfile left "
                         "len log_profile lower makeboundingbox move moveboat moveboatrelative movecharactertolocation moveitemtocontainer "
                         "moveitemtolocation move_offline_mobiles openpaperdoll open pack performaction playlightningbolteffect "
                         "playmovingeffect playmovingeffectxyz playobjectcenteredeffect playsoundeffect playsoundeffectprivate "
                         "playstationaryeffect printtextabove printtextaboveprivate packages polcore position print queryparam "
                         "randomdiceroll randomint rawskilltobaseskill readconfigfile readgameclock releaseitem registerforspeechevents "
                         "registeritemwithboat requestinput reserveitem restartscript resurrect revokeprivilege runawayfrom "
                         "runawayfromlocation runtoward runtowardlocation reverse run_script_to_completion saveworldstate selectmenuitem2 "
                         "self sendbuywindow senddialoggump sendevent sendopenspecialcontainer sendpacket sendsellwindow sendskillwindow "
                         "sendstringastipwindow sendsysmessage sendtextentrygump setanchor setglobalproperty setname setobjproperty "
                         "setopponent setproperty setrawskill setregionlightlevel setregionweatherlevel setscriptcontroller setwarmode "
                         "shutdown speakpowerwords splitwords startspelleffect subtractamount systemfindboatbyserial "
                         "systemfindobjectbyserial say set_critical set_debug set_priority set_priority_divide set_script_option "
                         "setcmdlevel setdex setint setlightlevel setmaster setname setpassword setstr shrink size sleep sleepms sort "
                         "spendgold squelch start_script syslog system_rpm target targetcoordinates targetmultiplacement turnawayfrom "
                         "turnawayfromlocation turnboat turntoward turntowardlocation toggle unloadconfigfile unpack unban unload_scripts "
                         "upper walkawayfrom walkawayfromlocation walktoward walktowardlocation wander writehtml writehtmlraw wait_for_event "
                         "movechar_forcelocation moveitem_forcelocation moveitem_normal scriptopt_debug scriptopt_no_interrupt "
                         "scriptopt_no_runaway te_cancel_disable te_cancel_enable te_style_disable te_style_normal te_style_numerical "
                         "tgtopt_check_los tgtopt_harmful tgtopt_helpful tgtopt_neutral tgtopt_nocheck_los setprop getprop ");

this->Keys_Set.escript1 = wxT("array const dictionary global local var and default in next not or "
                          "return to include use enum ");

this->Keys_Set.escript2 = wxT("while for endfor function program endprogram endfunction foreach "
                          "case else elseif if endcase endenum endforeach endif endwhile ");
#endif //XSTC_NO_ESCRIPT

#ifndef XSTC_NO_F77
this->Keys_Set.f77_0 = wxT("");

this->Keys_Set.f77_1 = wxT("");

this->Keys_Set.f77_2 = wxT("");
#endif //XSTC_NO_F77

#ifndef XSTC_NO_FLAGSHIP
this->Keys_Set.flagship = wxT("? @ accept access all alternate announce ansi any append as assign autolock average "
                          "begin bell bitmap blank box call cancel case century charset checkbox clear close cls color combobox commit "
                          "confirm console constant continue copy count create cursor "
                          "date dbread dbwrite decimals declare default delete deleted delimiters device dir "
                          "directory display do draw edit else elseif eject end endcase enddo endif endtext epoch erase error escape eval eventmask "
                          "exact exclusive extended external extra field file filter find fixed font for form format from "
                          "get gets global global_extern go goto gotop guialign guicolor guicursor guitransl html htmltext "
                          "if image index input intensity join key keyboard keytransl label lines list listbox local locate "
                          "margin memory memvar menu message method multibyte multilocks next nfs nfslock nfs_force note on openerror order outmode "
                          "pack parameters path pixel pop printer private prompt public push pushbutton quit "
                          "radiobutton radiogroup read recall refresh reindex relation release rename replace report request restore "
                          "richtext rowadapt rowalign run "
                          "save say scoreboard scrcompress screen seek select sequence set setenhanced setstandard setunselected "
                          "skip softseek sort source static store struct structure sum "
                          "tag tbrowse text to total type typeahead unique unlock update use wait while with wrap xml zap zerobyteo ");

this->Keys_Set.flagship1 = wxT("_displarr _displarrerr _displarrstd _displobj _displobjerr _displobjstd "
                           "aadd abs achoice aclone acopy adel adir aelemtype aeval afields afill ains alert alias alltrim altd ansi2oem "
                           "appiomode appmdimode appobject array asc ascan asize asort at atail atanychar autoxlock "
                           "between bin2i bin2l bin2w binand binlshift binor binrshift binxor bof break browse "
                           "cdow chr chr2screen cmonth col col2pixel color2rgb colorselect colvisible consoleopen consolesize crc32 ctod curdir "
                           "date datevalid day dbappend dbclearfilter dbclearindex dbclearrelation dbcloseall dbclosearea dbcommit dbcommitall "
                           "dbcreate dbcreateindex dbdelete dbedit dbeval dbf dbfilter dbfinfo dbflock dbfused dbgetlocate dbgobottom dbgoto "
                           "dbgotop dbobject dbrecall dbreindex dbrelation dbrlock dbrlocklist dbrselect dbrunlock dbseek dbselectarea "
                           "dbsetdriver dbsetfilter dbsetindex dbsetlocate dbsetorder dbsetrelation dbskip dbstruct dbunlock dbunlockall "
                           "dbusearea default deleted descend devout devoutpict devpos directory diskspace dispbegin dispbox dispcount "
                           "dispend dispout doserror doserror2str dow drawline dtoc dtos empty eof errorblock errorlevel eval execname execpidnum exp "
                           "fattrib fclose fcount fcreate ferase ferror ferror2str fieldblock fielddeci fieldget fieldgetarr fieldlen fieldname "
                           "fieldpos fieldput fieldputarr fieldtype fieldwblock file findexefile fklabel fkmax flagship_dir flock flockf fopen "
                           "found fread freadstdin freadstr freadtxt frename fs_set fseek fwrite "
                           "getactive getalign getapplykey getdosetkey getenv getenvarr getfunction getpostvalid getprevalid getreader guidrawline "
                           "i2bin iif indexcheck indexcount indexdbf indexext indexkey indexnames indexord infobox inkey inkey2read inkey2str inkeytrap "
                           "instdchar instdstring int int2num isalpha isbegseq iscolor isdbexcl isdbflock isdbmultip isdbmultiple isdbmultipleopen "
                           "isdbrlock isdigit isfunction isguimode islower isobjclass isobjequiv isobjproperty isprinter isupper "
                           "l2bin lastkey lastrec left len listbox lock log lower ltrim lupdate "
                           "macroeval macrosubst max max_col max_row maxcol maxrow mcol mdblck mdiclose mdiopen mdiselect memocode memodecode "
                           "memoedit memoencode memoline memoread memory memotran memowrit memvarblock mhide min minmax mlcount mlctopos mleftdown "
                           "mlpos mod month mpostolc mpresent mreststate mrightdown mrow msavestate msetcursor msetpos mshow mstate "
                           "neterr netname nextkey num2hex num2int hardcr header hex2num qout qout2 qqout qqout2 "
                           "objclone oem2ansi onkey ordbagext ordbagname ordcond ordcondset ordcreate orddescend orddestroy ordfor ordisinique "
                           "ordkey ordkeyadd ordkeycount ordkeydel ordkeygoto ordkeyno ordkeyval ordlistadd ordlistclear ordlistrebui ordname "
                           "ordnumber ordscope ordsetfocu ordsetrelat ordskipunique os outerr outstd "
                           "padc padl padr param parameters pcalls pcol pcount pixel2col pixel2row printstatus procfile procline procname procstack proper prow "
                           "rat rddlist rddname rddsetdefault readexit readinsert readkey readkill readmodal readsave readupdated readvar reccount recno recsize "
                           "replicate restscreen right rlock rlockverify round row row2pixel rowadapt rowvisible rtrim "
                           "savescreen scrdos2unix screen2chr scroll scrunix2dos seconds secondscpu select serial set setansi setblink setcancel setcol2get "
                           "setcolor setcolorba setcursor setevent setguicursor setkey setmode setpos setprc setvarempty sleep sleepms soundex space "
                           "sqrt statbarmsg statusmessage stod str strlen strlen2col strlen2pix strlen2space strpeek strpoke strtran strzero stuff substr "
                           "tbcolumnnew tbmouse tbrowsearr tbrowsedb tbrowsenew tempfilename time tone transform trim truepath type "
                           "updated upper used usersactive usersdbf usersmax val valtype version year "
                           "webdate weberrorhandler webgetenvir webgetformdata webhtmlbegin webhtmlend weblogerr webmaildomain weboutdata websendmail word ");

this->Keys_Set.flagship2 = wxT("function procedure return exit ");

this->Keys_Set.flagship3 = wxT("class instance export hidden protect prototype ");
#endif //XSTC_NO_FLAGSHIP

#ifndef XSTC_NO_FORTH
this->Keys_Set.forth = wxT("AGAIN BEGIN CASE DO ELSE ENDCASE ENDOF IF LOOP OF REPEAT THEN UNTIL  WHILE "
                       "[IF] [ELSE] [THEN] ?DO ");

this->Keys_Set.forth1 = wxT("DUP DROP ROT SWAP OVER @ ! 2@ 2! 2DUP 2DROP 2SWAP 2OVER NIP R@ >R R> 2R@ 2>R 2R> "
                        "0= 0< SP@ SP! W@ W! C@ C! < > = <> 0<> "
                        "SPACE SPACES KEY? KEY THROW CATCH ABORT */ 2* /MOD CELL+ CELLS CHAR+ "
                        "CHARS MOVE ERASE DABS TITLE HEX DECIMAL HOLD <# # #S #> SIGN "
                        "D  U DUMP (\") >NUMBER \' IMMEDIATE EXIT RECURSE UNLOOP LEAVE HERE ALLOT ,"
                        "C, W, COMPILE, BRANCH, RET, LIT, DLIT, ?BRANCH, \", >MARK >RESOLVE1 <MARK >RESOLVE "
                        "ALIGN ALIGNED USER-ALLOT USER-HERE HEADER DOES> SMUDGE HIDE :NONAME LAST-WORD "
                        "?ERROR ERROR2 FIND1 SFIND SET-CURRENT GET-CURRENT DEFINITIONS GET-ORDER FORTH "
                        "ONLY SET-ORDER ALSO PREVIOUS VOC-NAME ORDER LATEST LITERAL 2LITERAL SLITERAL "
                        "CLITERAL ?LITERAL1 ?SLITERAL1 HEX-LITERAL HEX-SLITERAL ?LITERAL2 ?SLITERAL2 SOURCE "
                        "EndOfChunk CharAddr PeekChar IsDelimiter GetChar OnDelimiter SkipDelimiters OnNotDelimiter "
                        "SkipWord SkipUpTo ParseWord NextWord PARSE SKIP CONSOLE-HANDLES REFILL DEPTH ?STACK "
                        "?COMP WORD INTERPRET BYE QUIT MAIN1 EVALUATE INCLUDE-FILE INCLUDED >BODY +WORD "
                        "WORDLIST CLASS! CLASS@ PAR! PAR@ ID ?IMMEDIATE ?VOC IMMEDIATE VOC WordByAddrWl "
                        "WordByAddr NLIST WORDS SAVE OPTIONS /notransl ANSI>OEM ACCEPT EMIT CR TYPE EKEY? "
                        "EKEY EKEY>CHAR EXTERNTASK ERASE-IMPORTS ModuleName ModuleDirName ENVIRONMENT? "
                        "DROP-EXC-HANDLER SET-EXC-HANDLER HALT ERR CLOSE-FILE CREATE-FILE CREATE-FILE-SHARED "
                        "OPEN-FILE-SHARED DELETE-FILE FILE-POSITION FILE-SIZE OPEN-FILE READ-FILE REPOSITION-FILE "
                        "DOS-LINES UNIX-LINES READ-LINE WRITE-FILE RESIZE-FILE WRITE-LINE ALLOCATE FREE RESIZE "
                        "START SUSPEND RESUME STOP PAUSE MIN MAX TRUE FALSE ASCIIZ> "
                        "R/O W/O ;CLASS ENDWITH OR AND /STRING SEARCH COMPARE EXPORT ;MODULE SPACE ");

this->Keys_Set.forth2 = wxT("VARIABLE CREATE : VALUE CONSTANT VM: M: var dvar chars OBJ "
                        "CONSTR: DESTR: CLASS: OBJECT: POINTER "
                        "USER USER-CREATE USER-VALUE VECT "
                        "WNDPROC: VOCABULARY -- TASK: CEZ: MODULE: ");

this->Keys_Set.forth3 = wxT("CHAR [CHAR] POSTPONE WITH [\'] TO [COMPILE] CHAR ASCII \' ");

this->Keys_Set.forth4 = wxT("REQUIRE WINAPI: ");

this->Keys_Set.forth5 = wxT("S\" ABORT\" Z\" \" \" C\" ");
#endif //XSTC_NO_FORTH

#ifndef XSTC_NO_FORTRAN
this->Keys_Set.fortran = wxT("access action advance allocatable allocate "
                         "apostrophe assign assignment associate asynchronous backspace "
                         "bind blank blockdata call case character class close common "
                         "complex contains continue cycle data deallocate decimal delim "
                         "default dimension direct do dowhile double doubleprecision else "
                         "elseif elsewhere encoding end endassociate endblockdata enddo "
                         "endfile endforall endfunction endif endinterface endmodule endprogram "
                         "endselect endsubroutine endtype endwhere entry eor equivalence "
                         "err errmsg exist exit external file flush fmt forall form format "
                         "formatted function go goto id if implicit in include inout "
                         "integer inquire intent interface intrinsic iomsg iolength "
                         "iostat kind len logical module name named namelist nextrec nml "
                         "none nullify number only open opened operator optional out pad "
                         "parameter pass pause pending pointer pos position precision "
                         "print private program protected public quote read readwrite "
                         "real rec recl recursive result return rewind save select "
                         "selectcase selecttype sequential sign size stat status stop stream "
                         "subroutine target then to type unformatted unit use value "
                         "volatile wait where while write ");

this->Keys_Set.fortran1 = wxT("abs achar acos acosd adjustl adjustr "
                          "aimag aimax0 aimin0 aint ajmax0 ajmin0 akmax0 akmin0 all allocated alog "
                          "alog10 amax0 amax1 amin0 amin1 amod anint any asin asind associated "
                          "atan atan2 atan2d atand bitest bitl bitlr bitrl bjtest bit_size bktest break "
                          "btest cabs ccos cdabs cdcos cdexp cdlog cdsin cdsqrt ceiling cexp char "
                          "clog cmplx conjg cos cosd cosh count cpu_time cshift csin csqrt dabs "
                          "dacos dacosd dasin dasind datan datan2 datan2d datand date "
                          "date_and_time dble dcmplx dconjg dcos dcosd dcosh dcotan ddim dexp "
                          "dfloat dflotk dfloti dflotj digits dim dimag dint dlog dlog10 dmax1 dmin1 "
                          "dmod dnint dot_product dprod dreal dsign dsin dsind dsinh dsqrt dtan dtand "
                          "dtanh eoshift epsilon errsns exp exponent float floati floatj floatk floor fraction "
                          "free huge iabs iachar iand ibclr ibits ibset ichar idate idim idint idnint ieor ifix "
                          "iiabs iiand iibclr iibits iibset iidim iidint iidnnt iieor iifix iint iior iiqint iiqnnt iishft "
                          "iishftc iisign ilen imax0 imax1 imin0 imin1 imod index inint inot int int1 int2 int4 "
                          "int8 iqint iqnint ior ishft ishftc isign isnan izext jiand jibclr jibits jibset jidim jidint "
                          "jidnnt jieor jifix jint jior jiqint jiqnnt jishft jishftc jisign jmax0 jmax1 jmin0 jmin1 "
                          "jmod jnint jnot jzext kiabs kiand kibclr kibits kibset kidim kidint kidnnt kieor kifix "
                          "kind kint kior kishft kishftc kisign kmax0 kmax1 kmin0 kmin1 kmod knint knot kzext "
                          "lbound leadz len len_trim lenlge lge lgt lle llt log log10 logical lshift malloc matmul "
                          "max max0 max1 maxexponent maxloc maxval merge min min0 min1 minexponent minloc "
                          "minval mod modulo mvbits nearest nint not nworkers number_of_processors pack popcnt "
                          "poppar precision present product radix random random_number random_seed range real "
                          "repeat reshape rrspacing rshift scale scan secnds selected_int_kind "
                          "selected_real_kind set_exponent shape sign sin sind sinh size sizeof sngl snglq spacing "
                          "spread sqrt sum system_clock tan tand tanh tiny transfer transpose trim ubound unpack verify ");

this->Keys_Set.fortran2 = wxT("cdabs cdcos cdexp cdlog cdsin cdsqrt cotan cotand "
                          "dcmplx dconjg dcotan dcotand decode dimag dll_export dll_import doublecomplex dreal "
                          "dvchk encode find flen flush getarg getcharqq getcl getdat getenv gettim hfix ibchng "
                          "identifier imag int1 int2 int4 intc intrup invalop iostat_msg isha ishc ishl jfix "
                          "lacfar locking locnear map nargs nbreak ndperr ndpexc offset ovefl peekcharqq precfill "
                          "prompt qabs qacos qacosd qasin qasind qatan qatand qatan2 qcmplx qconjg qcos qcosd "
                          "qcosh qdim qexp qext qextd qfloat qimag qlog qlog10 qmax1 qmin1 qmod qreal qsign qsin "
                          "qsind qsinh qsqrt qtan qtand qtanh ran rand randu rewrite segment setdat settim system "
                          "timer undfl unlock union val virtual volatile zabs zcos zexp zlog zsin zsqrt ");
#endif //XSTC_NO_FORTRAN

#ifndef XSTC_NO_GAP
this->Keys_Set.gap = wxT("");

this->Keys_Set.gap = wxT("");

this->Keys_Set.gap = wxT("");

this->Keys_Set.gap = wxT("");
#endif //XSTC_NO_GAP

#ifndef XSTC_NO_GUI4CLI
this->Keys_Set.gui4cli = wxT("");

this->Keys_Set.gui4cli1 = wxT("");

this->Keys_Set.gui4cli2 = wxT("");

this->Keys_Set.gui4cli3 = wxT("");

this->Keys_Set.gui4cli4 = wxT("");
#endif //XSTC_NO_GUI4CLI

#ifndef XSTC_NO_HASKELL
this->Keys_Set.haskell = wxT("");
#endif //XSTC_NO_HASKELL

#ifndef XSTC_NO_HTM
this->Keys_Set.html = wxT("active command contenteditable ping "
                      "article aside calendar canvas card command commandset datagrid datatree "
                      "footer gauge header m menubar menulabel nav progress section switch tabbox "
                      "abbr accept-charset accept accesskey action align alink "
                      "alt archive axis background bgcolor border "
                      "cellpadding cellspacing char charoff charset checked cite "
                      "class classid clear codebase codetype color cols colspan "
                      "compact content coords "
                      "data datafld dataformatas datapagesize datasrc datetime "
                      "declare defer dir disabled enctype event "
                      "face for frame frameborder "
                      "headers height href hreflang hspace http-equiv "
                      "id ismap label lang language leftmargin link longdesc "
                      "marginwidth marginheight maxlength media method multiple "
                      "name nohref noresize noshade nowrap "
                      "object onblur onchange onclick ondblclick onfocus "
                      "onkeydown onkeypress onkeyup onload onmousedown "
                      "onmousemove onmouseover onmouseout onmouseup "
                      "onreset onselect onsubmit onunload "
                      "profile prompt readonly rel rev rows rowspan rules "
                      "scheme scope selected shape size span src standby start style "
                      "summary tabindex target text title topmargin type usemap "
                      "valign value valuetype version vlink vspace width "
                      "text password checkbox radio submit reset "
                      "file hidden image "
                      "a abbr acronym address applet area b base basefont "
                      "bdo big blockquote body br button caption center "
                      "cite code col colgroup dd del dfn dir div dl dt em "
                      "fieldset font form frame frameset h1 h2 h3 h4 h5 h6 "
                      "head hr html i iframe img input ins isindex kbd label "
                      "legend li link map menu meta noframes noscript "
                      "object ol optgroup option p param pre q s samp "
                      "script select small span strike strong style sub sup "
                      "table tbody td textarea tfoot th thead title tr tt u ul "
                      "var xml xmlns ");

this->Keys_Set.html1 = wxT("abbrev abstract accel ackno acronym action address affiliation alt anchor "
                       "answer appendix appendixinfo application area areaset areaspec arg article "
                       "articleinfo artpagenums attribution audiodata audioobject author authorblurb "
                       "authorgroup authorinitials beginpage bibliocoverage bibliodiv biblioentry "
                       "bibliography bibliographyinfo biblioid bibliomisc bibliomixed bibliomset "
                       "bibliorelation biblioset bibliosource blockinfo blockquote book bookinfo "
                       "bridgehead callout calloutlist caption caution chapter chapterinfo citation "
                       "citebiblioid citerefentry citetitle city classname classsynopsis classsynopsisinfo "
                       "cmdsynopsis co collab collabname colophon colspec command computeroutput "
                       "confdates confgroup confnum confsponsor conftitle constant constraint "
                       "constraintdef constructorsynopsis contractnum contractsponsor contrib "
                       "copyright coref corpauthor corpname country database date dedication "
                       "destructorsynopsis edition editor email emphasis entry entrytbl envar "
                       "epigraph equation errorcode errorname errortext errortype example "
                       "exceptionname fax fieldsynopsis figure filename firstname firstterm "
                       "footnote footnoteref foreignphrase formalpara funcdef funcparams "
                       "funcprototype funcsynopsis funcsynopsisinfo function glossary glossaryinfo "
                       "glossdef glossdiv glossentry glosslist glosssee glossseealso glossterm "
                       "graphic graphicco group guibutton guiicon guilabel guimenu guimenuitem "
                       "guisubmenu hardware highlights holder honorific htm imagedata imageobject "
                       "imageobjectco important index indexdiv indexentry indexinfo indexterm "
                       "informalequation informalexample informalfigure informaltable initializer "
                       "inlineequation inlinegraphic inlinemediaobject interface interfacename "
                       "invpartnumber isbn issn issuenum itemizedlist itermset jobtitle keycap "
                       "keycode keycombo keysym keyword keywordset label legalnotice lhs lineage "
                       "lineannotation link listitem iteral literallayout lot lotentry manvolnum "
                       "markup medialabel mediaobject mediaobjectco member menuchoice methodname "
                       "methodparam methodsynopsis mm modespec modifier ousebutton msg msgaud "
                       "msgentry msgexplan msginfo msglevel msgmain msgorig msgrel msgset msgsub "
                       "msgtext nonterminal note objectinfo olink ooclass ooexception oointerface "
                       "option optional orderedlist orgdiv orgname otheraddr othercredit othername "
                       "pagenums para paramdef parameter part partinfo partintro personblurb "
                       "personname phone phrase pob postcode preface prefaceinfo primary primaryie "
                       "printhistory procedure production productionrecap productionset productname "
                       "productnumber programlisting programlistingco prompt property pubdate publisher "
                       "publishername pubsnumber qandadiv qandaentry qandaset question quote refclass "
                       "refdescriptor refentry refentryinfo refentrytitle reference referenceinfo "
                       "refmeta refmiscinfo refname refnamediv refpurpose refsect1 refsect1info refsect2 "
                       "refsect2info refsect3 refsect3info refsection refsectioninfo refsynopsisdiv "
                       "refsynopsisdivinfo releaseinfo remark replaceable returnvalue revdescription "
                       "revhistory revision revnumber revremark rhs row sbr screen screenco screeninfo "
                       "screenshot secondary secondaryie sect1 sect1info sect2 sect2info sect3 sect3info "
                       "sect4 sect4info sect5 sect5info section sectioninfo see seealso seealsoie "
                       "seeie seg seglistitem segmentedlist segtitle seriesvolnums set setindex "
                       "setindexinfo setinfo sgmltag shortaffil shortcut sidebar sidebarinfo simpara "
                       "simplelist simplemsgentry simplesect spanspec state step street structfield "
                       "structname subject subjectset subjectterm subscript substeps subtitle "
                       "superscript surname sv symbol synopfragment synopfragmentref synopsis "
                       "systemitem table tbody term tertiary tertiaryie textdata textobject tfoot "
                       "tgroup thead tip title titleabbrev toc tocback tocchap tocentry tocfront "
                       "toclevel1 toclevel2 toclevel3 toclevel4 toclevel5 tocpart token trademark "
                       "type ulink userinput varargs variablelist varlistentry varname videodata "
                       "videoobject void volumenum warning wordasword xref year "
                       "arch condition conformance id lang os remap role revision revisionflag security "
                       "userlevel vendor xreflabel "
                       "status label endterm linkend space width ");

this->Keys_Set.html2 = wxT("");

this->Keys_Set.html3 = wxT("");

this->Keys_Set.html4 = wxT("");

this->Keys_Set.html5 = wxT("and array as bool boolean break case cfunction class const continue declare "
                       "default die directory do double echo else elseif empty enddeclare endfor "
                       "endforeach endif endswitch endwhile eval exit extends false float for "
                       "foreach function global if include include_once int integer isset list new "
                       "null object old_function or parent print real require require_once resource "
                       "return static stdclass string switch true unset use var while xor "
                       "abstract catch clone exception final implements interface php_user_filter "
                       "private protected public this throw try "
                       "__class__ __file__ __function__ __line__ __method__ __sleep __wakeup ");

this->Keys_Set.html6 = wxT("ELEMENT DOCTYPE ATTLIST ENTITY NOTATION");

this->Keys_Set.html7 = wxT("");
#endif //XSTC_NO_HTM

#ifndef XSTC_NO_INNOSETUP
this->Keys_Set.inno = wxT("code components custommessages dirs files icons ini installdelete langoptions "
                      "languages messages registry run setup types tasks uninstalldelete uninstallrun _istool");

this->Keys_Set.inno1 = wxT("allowcancelduringinstall allownoicons allowrootdirectory allowuncpath "
                        "alwaysrestart alwaysshowcomponentslist alwaysshowdironreadypage "
                        "alwaysshowgrouponreadypage alwaysusepersonalgroup appcomments appcontact "
                        "appcopyright appenddefaultdirname appenddefaultgroupname appid appmodifypath "
                        "appmutex appname apppublisher apppublisherurl appreadmefile appsupporturl "
                        "appupdatesurl appvername appversion architecturesallowed "
                        "architecturesinstallin64bitmode backcolor backcolor2 backcolordirection "
                        "backsolid changesassociations changesenvironment compression copyrightfontname "
                        "copyrightfontsize createappdir createuninstallregkey defaultdirname "
                        "defaultgroupname defaultuserinfoname defaultuserinfoorg defaultuserinfoserial "
                        "dialogfontname dialogfontsize direxistswarning disabledirpage "
                        "disablefinishedpage disableprogramgrouppage disablereadymemo disablereadypage "
                        "disablestartupprompt diskclustersize diskslicesize diskspanning "
                        "enablesdirdoesntexistwarning encryption extradiskspacerequired "
                        "flatcomponentslist infoafterfile infobeforefile internalcompresslevel "
                        "languagedetectionmethod languagecodepage languageid languagename licensefile "
                        "mergeduplicatefiles minversion onlybelowversion outputbasefilename outputdir "
                        "outputmanifestfile password privilegesrequired reservebytes "
                        "restartifneededbyrun setupiconfile showcomponentsizes showlanguagedialog "
                        "showtaskstreelines slicesperdisk solidcompression sourcedir timestamprounding "
                        "timestampsinutc titlefontname titlefontsize touchdate touchtime uninstallable "
                        "uninstalldisplayicon uninstalldisplayname uninstallfilesdir uninstalllogmode "
                        "uninstallrestartcomputer updateuninstalllogappname usepreviousappdir "
                        "usepreviousgroup useprevioussetuptype useprevioustasks useprevioususerinfo "
                        "userinfopage usesetupldr versioninfocompany versioninfocopyright "
                        "versioninfodescription versioninfotextversion versioninfoversion "
                        "welcomefontname welcomefontsize windowshowcaption windowstartmaximized "
                        "windowresizable windowvisible wizardimagebackcolor wizardimagefile "
                        "wizardimagestretch wizardsmallimagefile ");

this->Keys_Set.inno2 = wxT("afterinstall attribs beforeinstall check comment components copymode "
                       "description destdir destname excludes extradiskspacerequired filename flags "
                       "fontinstall groupdescription hotkey infoafterfile infobeforefile iconfilename "
                       "iconindex key languages licensefile messagesfile minversion name "
                       "onlybelowversion parameters permissions root runonceid section source "
                       "statusmsg string subkey tasks type types valuedata valuename valuetype "
                       "workingdir ");

this->Keys_Set.inno3 = wxT("append define dim else emit endif endsub error expr file for if ifdef ifexist "
                       "ifndef ifnexist include insert pragma sub undef ");

this->Keys_Set.inno4 = wxT("begin break case const continue do downto else end except finally for function "
                       "if of procedure repeat then to try until uses var while with ");

this->Keys_Set.inno5 = wxT("");
#endif //XSTC_NO_INNOSETUP

#ifndef XSTC_NO_JS
this->Keys_Set.js =   wxT("abstract array boolean break byte case catch call char class "
                      "const continue debugger default delete do double else enum export extends "
                      "final finally float for function goto if implements import infinity in instanceof "
                      "int interface long NaN native new package private protected public "
                      "return short string static super switch synchronized this throw throws "
                      "transient try typeof var void volatile while with ");

this->Keys_Set.js1 =  wxT("onAbort onBlur onChange onClick onDblClick onDragDrop onFocus "
                      "onKeyDown onKeyPress onKeyUp onload onMouseDown onMouseMove "
                      "onMouseOut onMouseOver onMouseUp onMove onReset onResize onSelect "
                      "onSubmit onUnload toSource toString toUpperCase toUTCString toGMTString "
                      "toLocaleString toLowerCase getDate getDay getFullYear getHours "
                      "getMilliseconds getMinutes getMonth getSeconds getSelection getTime "
                      "getTimezoneOffset getUTCDate getUTCDay getUTCFullYear getUTCHours "
                      "getUTCMilliseconds getUTCMinutes getUTCMonth getUTCSeconds "
                      "setDate setFullYear setHours setInterval setMilliseconds setMinutes "
                      "setMonth setSeconds setTime setTimeout setUTCDate setUTCFullYear setUTCHours "
                      "setUTCMilliseconds setUTCMinutes setUTCMonth setUTCSeconds isFinite isNaN "
                      "moveAbove moveBelow moveBy moveTo moveToAbsolute resizeBy resizeTo write "
                      "writeln valueOf unescape unshift untaint unwatch ");

this->Keys_Set.js2 = wxT("");
#endif //XSTC_NO_JS

#ifndef XSTC_NO_JAVA
this->Keys_Set.java = wxT("instanceof assert if else switch case default break goto return for while do "
                      "continue new throw throws try catch finally this super extends implements import "
                      "true false null ");

this->Keys_Set.java1 = wxT("package transient strictfp void char short int long double float const static "
                       "volatile byte boolean class interface native private protected public final "
                       "abstract synchronized enum ");

this->Keys_Set.java2 = wxT("");

this->Keys_Set.java3 = wxT("");
#endif //XSTC_NO_JAVA

#ifndef XSTC_NO_KIX
this->Keys_Set.kix = wxT("? and beep big break call cd cls color cookie1 copy "
	                 "debug del dim display do until exit flushkb for each next function endfunction "
	                 "get gets global go gosub goto if else endif md or password play quit "
	                 "rd redim return run select case endselect set setl setm settime "
	                 "shell sleep small use while loop ");

this->Keys_Set.kix1 = wxT("abs addkey addprinterconnection addprogramgroup "
	                  "addprogramitem asc ascan at backupeventlog box cdbl chr cint cleareventlog "
	                  "close comparefiletimes createobject cstr dectohex delkey delprinterconnection "
	                  "delprogramgroup delprogramitem deltree delvalue dir enumgroup enumipinfo enumkey "
	                  "enumlocalgroup enumvalue execute exist existkey expandenvironmentvars fix "
	                  "formatnumber freefilehandle getdiskspace getfileattr getfilesize getfiletime "
	                  "getfileversion getobject iif ingroup instr instrrev int isdeclared join "
	                  "kbhit keyexist lcase left len loadhive loadkey logevent logoff ltrim "
	                  "memorysize messagebox open readline readprofilestring readtype readvalue "
	                  "redirectoutput right rnd round rtrim savekey sendkeys sendmessage setascii "
	                  "setconsole setdefaultprinter setfileattr setfocus setoption setsystemstate "
	                  "settitle setwallpaper showprogramgroup shutdown sidtoname split srnd substr "
	                  "trim ubound ucase unloadhive val vartype vartypename writeline "
	                  "writeprofilestring writevalue ");

this->Keys_Set.kix2 = wxT("address build color comment cpu crlf csd curdir "
	                  "date day domain dos error fullname homedir homedrive homeshr hostname "
	                  "inwin ipaddress0 ipaddress1 ipaddress2 ipaddress3 kix lanroot ldomain "
	                  "ldrive lm logonmode longhomedir lserver maxpwage mdayno mhz monthno "
	                  "month msecs pid primarygroup priv productsuite producttype pwage ras "
	                  "result rserver scriptdir scriptexe scriptname serror sid site startdir "
	                  "syslang ticks time userid userlang wdayno wksta wuserid ydayno year ");
#endif //XSTC_NO_KIX

#ifndef XSTC_NO_LISP
this->Keys_Set.lisp = wxT("not defun + - * / = < > <= >= princ "
	                  "eval apply funcall quote identity function complement backquote lambda set setq setf "
	                  "defun defmacro gensym make symbol intern symbol name symbol value symbol plist get "
	                  "getf putprop remprop hash make array aref car cdr caar cadr cdar cddr caaar caadr cadar "
	                  "caddr cdaar cdadr cddar cdddr caaaar caaadr caadar caaddr cadaar cadadr caddar cadddr "
	                  "cdaaar cdaadr cdadar cdaddr cddaar cddadr cdddar cddddr cons list append reverse last nth "
	                  "nthcdr member assoc subst sublis nsubst  nsublis remove length list length "
	                  "mapc mapcar mapl maplist mapcan mapcon rplaca rplacd nconc delete atom symbolp numberp "
	                  "boundp null listp consp minusp zerop plusp evenp oddp eq eql equal cond case and or let l if prog "
	                  "prog1 prog2 progn go return do dolist dotimes catch throw error cerror break "
	                  "continue errset baktrace evalhook truncate float rem min max abs sin cos tan expt exp sqrt "
	                  "random logand logior logxor lognot bignums logeqv lognand lognor "
	                  "logorc2 logtest logbitp logcount integer length nil ");

this->Keys_Set.lisp1 = wxT("+ - * / = < > <= >= => "
                       "abs acos and angle append apply asin assoc assoc assq assv atan begin boolean? "
                       "caar cadr call-with-current-continuation call/cc "
                       "call-with-input-file call-with-output-file call-with-values car cdr "
                       "caar cadr cdar cddr "
                       "caaar caadr cadar caddr cdaar cdadr cddar cdddr "
                       "caaaar caaadr caadar caaddr cadaar cadadr caddar cadddr "
                       "cdaaar cdaadr cdadar cdaddr cddaar cddadr cdddar cddddr "
                       "case ceiling char->integer "
                       "char-alphabetic? char-ci<=? char-ci<? char-ci=? char-ci>=? char-ci>? "
                       "char-downcase char-lower-case? char-numeric? char-ready? "
                       "char-upcase char-upper-case? char-whitespace? "
                       "char<=? char<? char=? char>=? char>? char? "
                       "close-input-port close-output-port complex? cond cons cos "
                       "current-input-port current-output-port "
                       "define define-syntax delay denominator display do dynamic-wind "
                       "else eof-object? eq? equal? eqv? eval even? exact->inexact exact? "
                       "exp expt floor for-each force gcd "
                       "if imag-part inexact->exact inexact? input-port? integer->char integer? interaction-environment "
                       "lambda lcm length let let* let-syntax letrec letrec-syntax "
                       "list list->string list->vector list-ref list-tail list? load log "
                       "magnitude make-polar make-rectangular make-string make-vector "
                       "map max member memq memv min modulo "
                       "negative? newline not null-environment null? number->string number? numerator "
                       "odd? open-input-file open-output-file or output-port? "
                       "pair? peek-char input-port? output-port? positive? procedure? "
                       "quasiquote quote quotient "
                       "rational? rationalize read read-char real-part real? remainder reverse round "
                       "scheme-report-environment set! set-car! set-cdr! sin sqrt string "
                       "string->list string->number string->symbol string-append "
                       "string-ci<=? string-ci<? string-ci=? string-ci>=? string-ci>? "
                       "string-copy string-fill! string-length string-ref string-set! "
                       "string<=? string<? string=? string>=? string>? string? "
                       "substring symbol->string symbol? syntax-rules "
                       "transcript-off transcript-on truncate "
                       "unquote unquote-splicing "
                       "values vector vector->list vector-fill! vector-length vector-ref vector-set! vector? "
                       "with-input-from-file with-output-to-file write write-char "
                       "zero?");
#endif //XSTC_NO_LISP

#ifndef XSTC_NO_LOUT
this->Keys_Set.lout = wxT("@OptGall @Filter @FilterIn @FilterOut "
                      "@FilterErr @FontDef @Family @Face @Name @Metrics @ExtraMetrics "
                      "@Mapping @Recode @Common @Rump @Meld @Insert @OneOf @Next @Plus "
                      "@Minus @Wide @High @HShift @VShift @BeginHeaderComponent "
                      "@EndHeaderComponent @SetHeaderComponent @ClearHeaderComponent @OneCol "
                      "@OneRow @HScale @VScale @HCover @VCover @Scale @KernShrink @HContract "
                      "@VContract @HLimited @VLimited @HExpand @VExpand @StartHVSpan "
                      "@StartHSpan @StartVSpan @HSpan @VSpan @PAdjust @HAdjust @VAdjust "
                      "@Rotate @Background @IncludeGraphic @SysIncludeGraphic @Graphic "
                      "@LinkSource @LinkDest @URLLink @PlainGraphic @Verbatim @RawVerbatim "
                      "@Case @Yield @BackEnd @Char @Font @Space @YUnit @ZUnit @Break "
                      "@Underline @SetColour @SetColor @SetTexture @Outline @Language "
                      "@CurrLang @CurrFamily @CurrFace @CurrYUnit @CurrZUnit @LEnv @LClos "
                      "@LUse @LEO @Open @Use @NotRevealed @Tagged @Database @SysDatabase "
                      "@Include @SysInclude @IncludeGraphicRepeated @PrependGraphic "
                      "@SysIncludeGraphicRepeated @SysPrependGraphic @Target @Null "
                      "@PageLabel @Galley @ForceGalley @LInput @Split @Tag @Key @Optimize "
                      "@Merge @Enclose @Begin @End @Moment @Second @Minute @Hour @Day @Month "
                      "@Year @Century @WeekDay @YearDay @DaylightSaving @@A @@B @@C @@D @@E @@V ");

this->Keys_Set.lout1 = wxT("&&& && & ^// ^/ ^|| ^| ^& // / || | ");

this->Keys_Set.lout2 = wxT("def langdef force horizontally into "
                       "extend import export precedence associativity left right body macro "
                       "named compulsory following preceding foll_or_prec now ");
#endif //XSTC_NO_LOUT

#ifndef XSTC_NO_LUA
this->Keys_Set.lua = wxT("and break do else elseif end false for function if "
                     "in local nil not or repeat return then true until while ");

this->Keys_Set.lua1 = wxT("_VERSION assert collectgarbage dofile error gcinfo loadfile loadstring "
                      "print rawget rawset require tonumber tostring type unpack ");

this->Keys_Set.lua2 = wxT("_ALERT _ERRORMESSAGE _INPUT _PROMPT _OUTPUT "
                      "_STDERR _STDIN _STDOUT call dostring foreach foreachi getn globals newtype "
                      "sort tinsert tremove _G getfenv getmetatable ipairs loadlib next pairs pcall "
                      "rawequal setfenv setmetatable xpcall "
                      "string table math coroutine io os debug "
                      "load module select ");

this->Keys_Set.lua3 = wxT("abs acos asin atan atan2 ceil cos deg exp "
                      "floor format frexp gsub ldexp log log10 max min mod rad random randomseed "
                      "sin sqrt strbyte strchar strfind strlen strlower strrep strsub strupper tan "
                      "string byte char dump find len "
                      "lower rep sub upper format gfind gsub "
                      "concat table foreach foreachi getn sort insert remove setn "
                      "math abs acos asin atan atan2 ceil cos deg exp "
                      "floor frexp ldexp log log10 max min mod "
                      "pi pow rad random randomseed sin sqrt tan "
                      "gmatch match sreverse maxn "
                      "cosh fmod modf sinh tanh huge ");

this->Keys_Set.lua4 = wxT("openfile closefile readfrom writeto appendto "
                      "remove rename flush seek tmpfile tmpname read write "
                      "clock date difftime execute exit getenv setlocale time "
                      "coroutine create resume status "
                      "wrap yield "
                      "io close flush input lines open output read tmpfile type write "
                      "stdin stdout stderr "
                      "os clock date difftime execute exit getenv remove rename "
                      "setlocale time tmpname "
                      "running package cpath loaded loadlib ppath "
                      "preload seeall popen ");

this->Keys_Set.lua5 = wxT("");

this->Keys_Set.lua6 = wxT("");

this->Keys_Set.lua7 = wxT("");
#endif //XSTC_NO_LUA

#ifndef XSTC_NO_MAGIK
this->Keys_Set.magik = wxT("");

this->Keys_Set.magik1 = wxT("");

this->Keys_Set.magik2 = wxT("");

this->Keys_Set.magik3 = wxT("");

this->Keys_Set.magik4 = wxT("");
#endif //XSTC_NO_MAGIK

#ifndef XSTC_NO_MATLAB
this->Keys_Set.matlab = wxT("break case catch continue do "
                        "else elseif end end_unwind_protect endfor "
                        "endif endswitch endwhile for function endfunction global if "
                        "otherwise persistent return switch try until "
                        "unwind_protect unwind_protect_cleanup while ");
#endif //XSTC_NO_MATLAB

#ifndef XSTC_NO_METAPOST
this->Keys_Set.metapost = wxT("ahangle ahlength bboxmargin defaultpen defaultscale "
                          "labeloffset background currentpen currentpicture cuttings "
                          "defaultfont extra_beginfig extra_endfig "
                          "beveled black blue bp butt cc cm dd ditto down epsilon "
                          "evenly fullcircle green halfcircle identity in infinity left "
                          "mitered mm origin pensquare pt quartercircle red right "
                          "rounded squared unitsquare up white withdots "
                          "abs bbox ceiling center cutafter cutbefore dir "
                          "directionpoint div dotprod intersectionpoint inverse mod lft "
                          "round rt unitvector whatever "
                          "cutdraw draw drawarrow drawdblarrow fill filldraw drawdot "
                          "loggingall pickup tracingall tracingnone undraw unfill "
                          "unfilldraw "
                          "buildcycle dashpattern decr dotlabel dotlabels drawoptions "
                          "incr label labels max min thelabel z "
                          "beginchar blacker capsule_end change_width "
                          "define_blacker_pixels define_corrected_pixels "
                          "define_good_x_pixels define_good_y_pixels "
                          "define_horizontal_corrected_pixels define_pixels "
                          "define_whole_blacker_pixels define_whole_pixels "
                          "define_whole_vertical_blacker_pixels "
                          "define_whole_vertical_pixels endchar extra_beginchar "
                          "extra_endchar extra_setup font_coding_scheme "
                          "font_extra_space charcode day linecap linejoin miterlimit month pausing "
                          "prologues showstopping time tracingcapsules tracingchoices "
                          "tracingcommands tracingequations tracinglostchars "
                          "tracingmacros tracingonline tracingoutput tracingrestores "
                          "tracingspecs tracingstats tracingtitles truecorners "
                          "warningcheck year "
                          "false nullpicture pencircle true "
                          "and angle arclength arctime ASCII bluepart boolean bot "
                          "char color cosd cycle decimal directiontime floor fontsize "
                          "greenpart hex infont intersectiontimes known length llcorner "
                          "lrcorner makepath makepen mexp mlog normaldeviate not "
                          "numeric oct odd or path pair pen penoffset picture point "
                          "postcontrol precontrol redpart reverse rotated scaled "
                          "shifted sind slanted sqrt str string subpath substring "
                          "transform transformed ulcorner uniformdeviate unknown "
                          "urcorner xpart xscaled xxpart xypart ypart yscaled yxpart "
                          "yypart zscaled "
                          "addto clip input interim let newinternal save setbounds "
                          "shipout show showdependencies showtoken showvariable "
                          "special "
                          "begingroup endgroup of curl tension and controls "
                          "reflectedabout rotatedaround interpath on off beginfig "
                          "endfig def vardef enddef epxr suffix text primary secondary "
                          "tertiary primarydef secondarydef tertiarydef top bottom "
                          "ulft urt llft lrt randomseed also contour doublepath "
                          "withcolor withpen dashed if else elseif fi for endfor forever exitif "
                          "forsuffixes downto upto step until "
                          "charlist extensible fontdimen headerbyte kern ligtable "
                          "boundarychar chardp charext charht charic charwd designsize "
                          "fontmaking charexists "
                          "cullit currenttransform gfcorners grayfont hround "
                          "imagerules lowres_fix nodisplays notransforms openit "
                          "displaying currentwindow screen_rows screen_cols "
                          "pixels_per_inch cull display openwindow numspecial "
                          "totalweight autorounding fillin proofing tracingpens "
                          "xoffset chardx granularity smoothing turningcheck yoffset "
                          "chardy hppp tracingedges vppp "
                          "extra_beginfig extra_endfig mpxbreak "
                          "end btex verbatimtex etex ");

this->Keys_Set.metapost1 = wxT("");
#endif //XSTC_NO_METAPOST

#ifndef XSTC_NO_MMIXAL
this->Keys_Set.mmixal = wxT("2ADDU 4ADDU 8ADDU 16ADDU ADD ADDU AND ANDNH ANDNL ANDNMH ANDNML "
	                     "BDIF BEV BN BNN BNP BNZ BOD BP BSPEC BYTE BZ CMP CMPU CSEV CSN CSNN CSNP CSNZ CSOD CSP CSWAP CSZ "
                      	"DIV DIVU ESPEC EXPR FADD FCMP FCMPE FDIV FEQL FEQLE FIX FIXU FLOT FLOTU FMUL FREM FSQRT FSUB FUN FUNE "
                      	"GET GETA GO GREG I_BIT INCH INCL INCMH INCML IS JMP LDA LDB LDBU LDHT LDO LDOU LDSF LDT LDTU LDUNC LDVTS LDW LDWU LOC LOCAL "
                      	"MOR MUL MULU MUX MXOR NAND NEG NEGU NNIX NOR NXOR O_BIT OCTA ODIF OR ORH ORL ORMH ORML ORN "
                      	"PBEV PBN PBNN PBNP PBNZ PBOD PBP PBZ POP PREFIX PREGO PRELD PREST PUSHGO PUSHJ PUT "
                      	"RESUME SAVE SET SETH SETL SETMH SETML SFLOT SFLOTU SL SLU SR SRU "
                      	"STB STBU STCO STHT STO STOU STSF STT STTU STUNC STW STWU SUB SUBU SWYM SYNC SYNCD TDIF TETRA TRAP TRIP UNSAVE "
                      	"WDIF WYDEXOR ZSEV ZSN ZSNN ZSNP ZSNZ ZSOD ZSP ZSZ ");

this->Keys_Set.mmixal1 = wxT("rA rB rC rD rE rF rG rH rI rJ rK rL rM rN rO rP rQ rR rS rT rU rV rW rX rY rZ rBB rTT rWW rXX rYY rZZ ");

this->Keys_Set.mmixal2 = wxT("@ Text_Segment Data_Segment Pool_Segment Stack_Segment "
	                      "StdErr StdIn StdOut "
	                      "Fopen Fclose Fread Fwrite Fgets Fputs Fgetws Fputws Ftell Fseek "
	                      "TextRead TextWrite BinaryRead BinaryWrite BinaryReadWrite ");
#endif //XSTC_NO_MMIXAL

#ifndef XSTC_NO_NNCRONTAB
this->Keys_Set.nncrontab = wxT("AGAIN ALLOT AND Action BEGIN CASE COMPARE CONSTANT CREATE DO Days ELSE "
                         "ENDCASE ENDOF EVAL-SUBST EVALUATE FALSE Hours I IF LEAVE LOOP Minutes "
                         "Months NOT OF OFF ON OR PAD REPEAT Rule SET THEN TRUE Time UNTIL VALUE "
                         "VARIABLE WHILE WeekDays Years ");

this->Keys_Set.nncrontab1 = wxT("</JScript> </SCRIPT> </VBScript> <JScript> <VBScript> <SCRIPT> ACCESS-DATE BEEP "
                          "CALL_DIAL CALL_HANGUP CHAR CLIPBOARD CONSOLE CREATION-DATE CUR-DATE DATE- "
                          "DATE-INTERVAL DELAY DIAL DIR-CREATE DIR-DELETE DIR-EMPTY DIR-SIZE Day@ "
                          "Domain ERR-MSG EXIST FILE-ACCESS-DATE FILE-APPEND FILE-COPY FILE-CREATE "
                          "FILE-CREATION-DATE FILE-CROP FILE-DATE FILE-DELETE FILE-EMPTY FILE-EXIST "
                          "FILE-MOVE FILE-RENAME FILE-SIZE FILE-WRITE FILE-WRITE-DATE FOR-CHILD-WINDOWS "
                          "FOR-FILES FOR-WINDOWS FREE-SPACE GET-CUR-TIME GET-REG GET-VER GetTickCount "
                          "HANGUP HIDE-ICON HINT HINT-OFF HINT-POS HINT-SIZE HINTW HOST-EXIST Hour@ "
                          "IDLE INTERVAL IS-ARCHIVE IS-DIR IS-HIDDEN IS-READONLY IS-SYSTEM KILL LAUNCH "
                          "LOG LOGGEDON LOGOFF LogonBatch LogonInteractive LogonNetwork MOUSE-LBCLK "
                          "MOUSE-LBDCLK MOUSE-LBDN MOUSE-LBUP MOUSE-MOVE MOUSE-MOVER MOUSE-MOVEW "
                          "MOUSE-RBCLK MOUSE-RBDCLK MOUSE-RBDN MOUSE-RBUP MSG Min@ Mon@ "
                          "MonitorResponseTime NHOST-EXIST No ONLINE PAUSE PLAY-SOUND PLAY-SOUNDW "
                          "POP3-CHECK POWEROFF PROC-EXIST PROC-TIME PURGE-OLD PURGE-OLDA PURGE-OLDW "
                          "Password QSTART-APP QSTART-APPW QUERY QUIT RASDomain RASError RASPassword "
                          "RASPhone RASSecPassword RASUser RE-ALL RE-MATCH REBOOT REG-DELETE-KEY "
                          "REG-DELETE-VALUE REG-DWORD REG-SZ REMINDER RUN SEND-KEYS SEND-KEYS-DELAY "
                          "SHOW-ICON SHUTDOWN START-APP START-APPW START-QUIT START-TIME Sec@ "
                          "SecPassword THINT THINTW TimeSec@ TMSG TQUERY User WDay@ WIN-ACTIVATE "
                          "WIN-ACTIVE WIN-CLICK WIN-CLOSE WIN-EXIST WIN-HIDE WIN-HWND WIN-MAXIMIZE "
                          "WIN-MINIMIZE WIN-MOVE WIN-MOVER WIN-RESTORE WIN-SEND-KEYS WIN-SHOW "
                          "WIN-TERMINATE WIN-TOPMOST WIN-VER WIN-WAIT WINAPI WRITE-DATE WatchClipboard "
                          "WatchConnect WatchDir WatchDisconnect WatchDriveInsert WatchDriveRemove "
                          "WatchFile WatchProc WatchProcStop WatchWinActivate WatchWinCreate "
                          "WatchWinDestroy WatchWindow WinNT Year@ Yes ");

this->Keys_Set.nncrontab2 = wxT("ALL AboveNormalPriority AsService BelowNormalPriority FILESONLY "
                          "HighPriority IdlePriority LoadProfile NoActive NoDel NoLog NoRunAs "
                          "NormalPriority OnceADay OnceAHour OnceAMonth OnceAWeek RECURSIVE "
                          "RealtimePriority RunOnce SWHide ShowMaximized ShowMinimized ShowNoActivate "
                          "ShowNormal StartIn StartPos StartSize TODEPTH WATCH-CHANGE-ATTRIBUTES "
                          "WATCH-CHANGE-DIR-NAME WATCH-CHANGE-FILE-NAME WATCH-CHANGE-LAST-WRITE "
                          "WATCH-CHANGE-SECURITY WATCH-CHANGE-SIZE WaitFor WatchSubtree WithoutProfile ");
#endif //XSTC_NO_NNCRONTAB

#ifndef XSTC_NO_NSIS
this->Keys_Set.nsis = wxT("!addincludedir !addplugindir MakeNSIS Portions "
                      "Contributors: Abort AddBrandingImage AddSize AutoCloseWindow BGFont "
                      "BGGradient BrandingText BringToFront Call CallInstDLL Caption ChangeUI "
                      "ClearErrors ComponentText GetDLLVersion GetDLLVersionLocal GetFileTime "
                      "GetFileTimeLocal CopyFiles CRCCheck CreateDirectory CreateFont CreateShortCut "
                      "SetDatablockOptimize DeleteINISec DeleteINIStr DeleteRegKey DeleteRegValue "
                      "Delete DetailPrint DirText DirShow DirVar DirVerify GetInstDirError AllowRootDirInstall "
                      "CheckBitmap EnableWindow EnumRegKey EnumRegValue Exch Exec ExecWait "
                      "ExecShell ExpandEnvStrings FindWindow FindClose FindFirst FindNext File FileBufSize "
                      "FlushINI ReserveFile FileClose FileErrorText FileOpen FileRead FileWrite FileReadByte "
                      "FileWriteByte FileSeek Function FunctionEnd GetDlgItem GetFullPathName "
                      "GetTempFileName HideWindow Icon IfAbort IfErrors IfFileExists IfRebootFlag IfSilent "
                      "InstallDirRegKey InstallColors InstallDir InstProgressFlags InstType IntOp IntCmp "
                      "IntCmpU IntFmt IsWindow Goto LangString LangStringUP LicenseData "
                      "LicenseForceSelection LicenseLangString LicenseText LicenseBkColor "
                      "LoadLanguageFile LogSet LogText MessageBox Nop Name OutFile Page "
                      "PageCallbacks PageEx PageExEnd Pop Push Quit ReadINIStr ReadRegDWORD "
                      "ReadRegStr ReadEnvStr Reboot RegDLL Rename Return RMDir Section SectionEnd "
                      "SectionIn SubSection SectionGroup SubSectionEnd SectionGroupEnd SearchPath "
                      "SectionSetFlags SectionGetFlags SectionSetInstTypes SectionGetInstTypes "
                      "SectionGetText SectionSetText SectionGetSize SectionSetSize GetCurInstType "
                      "SetCurInstType InstTypeSetText InstTypeGetText SendMessage SetAutoClose "
                      "SetCtlColors SetBrandingImage SetCompress SetCompressor SetCompressorDictSize "
                      "SetCompressionLevel SetDateSave SetDetailsView SetDetailsPrint SetErrors "
                      "SetErrorLevel GetErrorLevel SetFileAttributes SetFont SetOutPath SetOverwrite "
                      "SetPluginUnload SetRebootFlag SetShellVarContext SetSilent ShowInstDetails "
                      "ShowUninstDetails ShowWindow SilentInstall SilentUnInstall Sleep StrCmp StrCpy "
                      "StrLen SubCaption UninstallExeName UninstallCaption UninstallIcon UninstPage "
                      "UninstallText UninstallSubCaption UnRegDLL WindowIcon WriteINIStr WriteRegBin "
                      "WriteRegDWORD WriteRegStr WriteRegExpandStr WriteUninstaller XPStyle !packhdr "
                      "!system !execute !AddIncludeDir !include !cd !ifdef !ifndef !endif !define !undef !else !echo !warning "
                      "!error !verbose !macro !macroend !insertmacro !ifmacrodef !ifmacrondef MiscButtonText "
                      "DetailsButtonText UninstallButtonText InstallButtonText SpaceTexts "
                      "CompletedText GetFunctionAddress GetLabelAddress GetCurrentAddress "
                      "!AddPluginDir InitPluginsDir AllowSkipFiles Var VIAddVersionKey VIProductVersion LockWindow");

this->Keys_Set.nsis1 = wxT("$0 $1 $2 $3 $4 $5 $6 $7 $8 $9 "
                       "$R0 $R1 $R2 $R3 $R4 $R5 $R6 $R7 $R8 $R9 $\\t $\\\" $\\' $\\` "
                       "$VARNAME $0, $INSTDIR $OUTDIR $CMDLINE $LANGUAGE $PROGRAMFILES "
                       "$COMMONFILES $DESKTOP $EXEDIR ${NSISDIR} $WINDIR $SYSDIR $TEMP "
                       "$STARTMENU $SMPROGRAMS $SMSTARTUP $QUICKLAUNCH $DOCUMENTS "
                       "$SENDTO $RECENT $FAVORITES $MUSIC $PICTURES $VIDEOS $NETHOOD "
                       "$FONTS $TEMPLATES $APPDATA $PRINTHOOD $INTERNET_CACHE $COOKIES "
                       "$HISTORY $PROFILE $ADMINTOOLS $RESOURCES $RESOURCES_LOCALIZED "
                       "$CDBURN_AREA $HWNDPARENT $PLUGINSDIR $$ $\\r $\\n ");

this->Keys_Set.nsis2 = wxT("ARCHIVE FILE_ATTRIBUTE_ARCHIVE FILE_ATTRIBUTE_HIDDEN "
                       "FILE_ATTRIBUTE_NORMAL FILE_ATTRIBUTE_OFFLINE FILE_ATTRIBUTE_READONLY "
                       "FILE_ATTRIBUTE_SYSTEM FILE_ATTRIBUTE_TEMPORARY HIDDEN HKCC HKCR HKCU "
                       "HKDD HKEY_CLASSES_ROOT HKEY_CURRENT_CONFIG HKEY_CURRENT_USER HKEY_DYN_DATA "
                       "HKEY_LOCAL_MACHINE HKEY_PERFORMANCE_DATA HKEY_USERS HKLM HKPD HKU IDABORT "
                       "IDCANCEL IDIGNORE IDNO IDOK IDRETRY IDYES MB_ABORTRETRYIGNORE MB_DEFBUTTON1 "
                       "MB_DEFBUTTON2 MB_DEFBUTTON3 MB_DEFBUTTON4 MB_ICONEXCLAMATION "
                       "MB_ICONINFORMATION MB_ICONQUESTION MB_ICONSTOP MB_OK MB_OKCANCEL "
                       "MB_RETRYCANCEL MB_RIGHT MB_SETFOREGROUND MB_TOPMOST MB_YESNO "
                       "MB_YESNOCANCEL NORMAL OFFLINE READONLY SW_SHOWMAXIMIZED SW_SHOWMINIMIZED "
                       "SW_SHOWNORMAL SYSTEM TEMPORARY auto colored false force hide ifnewer nevershow "
                       "normal off on show silent silentlog smooth true try lzma zlib bzip2 none listonly textonly "
                       "both top left bottom right license components directory instfiles uninstConfirm custom "
                       "all leave current ifdiff lastused LEFT RIGHT CENTER dlg_id ALT CONTROL EXT SHIFT "
                       "open print manual alwaysoff");

this->Keys_Set.nsis3 = wxT("");
#endif //XSTC_NO_NSIS

#ifndef XSTC_NO_OCTAVE
this->Keys_Set.octave = wxT("");
#endif //XSTC_NO_OCTAVE

#ifndef XSTC_NO_OPAL
this->Keys_Set.opal = wxT("ALL AND ANDIF ANY AS ASSERT AXM COMPLETELY DATA DEF DERIVE "
                      "DFD DISCRIMINATORS ELSE EX EXTERNAL FI FIX FUN IF IMPLEMENTATION IMPLIES IMPORT "
                      "IN INHERIT INJECTIONS INTERFACE INTERNAL LAW LAZY LEFTASSOC LET MODULE NOT ONLY "
                      "OR ORIF OTHERWISE POST PRE PRED PRIORITY PROPERTIES REALIZES REQUIRE RIGHTASSOC "
                      "SELECTORS SIGNATURE SORT SPC SPEC SPECIFICATION STRUCTURE THE THEN THEORY THM "
                      "TYPE UNIQ WHERE ");

this->Keys_Set.opal1 = wxT("aEntry agent align anchor ans arg arg1 arg2 array "
                       "arrowWhere bag bitmap bool bstree byte callback canvasEditor capStyle channel "
                       "char childstat codom codomFrom codomTo color colorModel com composeOp config "
                       "configCom cursor dArray data data1 data11 data2 data21 data3 data31 data4 "
                       "data41 dataFrom dataTo defaultPrio denotation device dist distOut dom domFrom "
                       "domTo drawing dyn emitter env event eventInfo file filemode filestat filetype "
                       "first first1 first2 first3 fission fmt font from from1 from2 funct group groupid "
                       "heap iconfig image in inData index inode input int inter interdom interpreter "
                       "iseq items joinStyle justifyHow long manager managerRequest map mapEntry mark "
                       "mid modifier nat natMap OBJECT option orient out outData output packOp pair "
                       "parser permission point positionRequest process procstat quad range real "
                       "regulator rel relief res res1 res2 result role sap script scroller scrollView "
                       "scrollWindow searchOpt second seekMode selector semaphor seq seqEntry set "
                       "setEntry short sigaction sighandler sigmask signal size sizeRequest some "
                       "sreal state stateId stateRequest string subrel tag textEditor time to tree "
                       "triple union user userid version view void wconfig wconfigCom wday widget "
                       "window wrapStyle ");
#endif //XSTC_NO_OPAL

#ifndef XSTC_NO_PASCAL
this->Keys_Set.pas = wxT("and array asm begin case cdecl class const constructor default "
                     "destructor div do downto else end except exit exports external far file "
                     "finalization finally for function goto if implementation in index inherited "
                     "initialization inline interface label library message mod near nil not "
                     "object of on or out overload override packed pascal private procedure program "
                     "property protected public published raise read record register repeat resourcestring "
                     "safecall set shl shr stdcall stored string then threadvar to try type unit "
                     "until uses var virtual while with write xor");

this->Keys_Set.pas1 = wxT("write read default public protected private property published stored");
#endif //XSTC_NO_PASCAL

#ifndef XSTC_NO_PERL
this->Keys_Set.perl = wxT("NULL __FILE__ __LINE__ __PACKAGE__ __DATA__ __END__ AUTOLOAD "
                      "BEGIN CORE DESTROY END EQ GE GT INIT LE LT NE CHECK abs accept "
                      "alarm and atan2 bind binmode bless caller chdir chmod chomp chop "
                      "chown chr chroot close closedir cmp connect continue cos crypt "
                      "dbmclose dbmopen defined delete die do dump each else elsif endgrent "
                      "endhostent endnetent endprotoent endpwent endservent eof eq eval "
                      "exec exists exit exp fcntl fileno flock for foreach fork format "
                      "formline ge getc getgrent getgrgid getgrnam gethostbyaddr gethostbyname "
                      "gethostent getlogin getnetbyaddr getnetbyname getnetent getpeername "
                      "getpgrp getppid getpriority getprotobyname getprotobynumber getprotoent "
                      "getpwent getpwnam getpwuid getservbyname getservbyport getservent "
                      "getsockname getsockopt glob gmtime goto grep gt hex if index "
                      "int ioctl join keys kill last lc lcfirst le length link listen "
                      "local localtime lock log lstat lt map mkdir msgctl msgget msgrcv "
                      "msgsnd my ne next no not oct open opendir or ord our pack package "
                      "pipe pop pos print printf prototype push quotemeta qu "
                      "rand read readdir readline readlink readpipe recv redo "
                      "ref rename require reset return reverse rewinddir rindex rmdir "
                      "scalar seek seekdir select semctl semget semop send setgrent "
                      "sethostent setnetent setpgrp setpriority setprotoent setpwent "
                      "setservent setsockopt shift shmctl shmget shmread shmwrite shutdown "
                      "sin sleep socket socketpair sort splice split sprintf sqrt srand "
                      "stat study sub substr symlink syscall sysopen sysread sysseek "
                      "system syswrite tell telldir tie tied time times truncate "
                      "uc ucfirst umask undef unless unlink unpack unshift untie until "
                      "use utime values vec wait waitpid wantarray warn while write "
                      "xor");
#endif //XSTC_NO_PERL

#ifndef XSTC_NO_PLM
this->Keys_Set.plm = wxT("");
#endif //XSTC_NO_PLM

#ifndef XSTC_NO_POV
this->Keys_Set.pov = wxT("declare local include undef fopen fclose read write default version "
                     "case range break debug error warning "
                     "if ifdef ifndef switch while macro else end");

this->Keys_Set.pov1 = wxT("camera light_source light_group "
                      "object blob sphere cylinder box cone height_field julia_fractal "
                      "lathe prism sphere_sweep superellipsoid sor text torus "
                      "bicubic_patch disc mesh mesh2 polygon triangle smooth_triangle "
                      "plane poly cubic quartic quadric isosurface parametric "
                      "union intersection difference merge function array spline "
                      "vertex_vectors normal_vectors uv_vectors face_indices normal_indices uv_indices "
                      "texture texture_list interior_texture "
                      "texture_map material_map image_map color_map colour_map pigment_map "
                      "normal_map slope_map bump_map density_map "
                      "pigment normal material interior finish reflection irid slope pigment_pattern image_pattern "
                      "warp media scattering density background fog sky_sphere rainbow "
                      "global_settings radiosity photons pattern "
                      "transform looks_like projected_through contained_by clipped_by bounded_by");

this->Keys_Set.pov2 = wxT("linear_spline quadratic_spline cubic_spline natural_spline bezier_spline b_spline "
                      "read write append inverse open "
                      "perspective orthographic fisheye ultra_wide_angle omnimax panoramic "
                      "spherical spotlight jitter circular orient "
                      "media_attenuation media_interaction shadowless parallel "
                      "refraction collect pass_through global_lights hierarchy sturm smooth "
                      "gif tga iff pot png pgm ppm jpeg tiff sys ttf "
                      "quaternion hypercomplex linear_sweep conic_sweep "
                      "type all_intersections split_union cutaway_textures "
                      "no_shadow no_image no_reflection double_illuminate hollow "
                      "uv_mapping all use_index use_color use_colour no_bump_scale "
                      "conserve_energy fresnel "
                      "average agate boxed bozo bumps cells crackle cylindrical density_file dents "
                      "facets granite leopard marble onion planar quilted radial ripples spotted waves wood wrinkles "
                      "solid use_alpha interpolate magnet noise_generator toroidal "
                      "ramp_wave triangle_wave sine_wave scallop_wave cubic_wave poly_wave "
                      "once map_type method fog_type hf_gray_16 charset ascii utf8 "
                      "rotate scale translate matrix location right up direction sky "
                      "angle look_at aperture blur_samples focal_point confidence variance "
                      "radius falloff tightness point_at area_light adaptive fade_distance fade_power "
                      "threshold strength water_level tolerance max_iteration precision slice "
                      "u_steps v_steps flatness inside_vector "
                      "accuracy max_gradient evaluate max_trace precompute "
                      "target ior dispersion dispersion_samples caustics "
                      "color colour rgb rgbf rgbt rgbft red green blue filter transmit gray hf "
                      "fade_color fade_colour quick_color quick_colour "
                      "brick checker hexagon brick_size mortar bump_size "
                      "ambient diffuse brilliance crand phong phong_size metallic specular "
                      "roughness reflection_exponent exponent thickness "
                      "gradient spiral1 spiral2 agate_turb form metric "
                      "offset df3 coords size mandel exterior julia "
                      "control0 control1 altitude turbulence octaves omega lambda "
                      "repeat flip black-hole orientation dist_exp major_radius "
                      "frequency phase intervals samples ratio absorption emission aa_threshold aa_level "
                      "eccentricity extinction distance turb_depth fog_offset fog_alt width arc_angle falloff_angle "
                      "adc_bailout ambient_light assumed_gamma irid_wavelength number_of_waves "
                      "always_sample brigthness count error_bound gray_threshold load_file "
                      "low_error_factor max_sample minimum_reuse nearest_count "
                      "pretrace_end pretrace_start recursion_limit save_file "
                      "spacing gather max_trace_level autostop expand_thresholds");

this->Keys_Set.pov3 = wxT("x y z t u v "
                      "yes no true false on off "
                      "clock clock_delta clock_on final_clock final_frame frame_number "
                      "image_height image_width initial_clock initial_frame pi version");

this->Keys_Set.pov4 = wxT("abs acos acosh asc asin asinh atan atanh atan2 "
                      "ceil cos cosh defined degrees dimensions dimension_size div exp file_exists floor "
                      "inside int ln log max min mod pow prod radians rand seed select sin sinh sqrt strcmp "
                      "strlen sum tan tanh val vdot vlength min_extent max_extent trace vaxis_rotate "
                      "vcross vrotate vnormalize vturbulence chr concat str strlwr strupr substr vstr "
                      "sqr cube reciprocal pwr");

this->Keys_Set.pov5 = wxT("");

this->Keys_Set.pov6 = wxT("");

this->Keys_Set.pov7 = wxT("");
#endif //XSTC_NO_POV

#ifndef XSTC_NO_POWERSHELL
this->Keys_Set.powershell = wxT("");

this->Keys_Set.powershell1 = wxT("");

this->Keys_Set.powershell2 = wxT("");
#endif //XSTC_NO_POWERSHELL

#ifndef XSTC_NO_PROGRESS
this->Keys_Set.progress = wxT("");

this->Keys_Set.progress1 = wxT("");

this->Keys_Set.progress2 = wxT("");
#endif //XSTC_NO_PROGRESS

#ifndef XSTC_NO_PS
this->Keys_Set.ps = wxT("$error = == FontDirectory StandardEncoding UserObjects abs add aload "
                    "anchorsearch and arc arcn arcto array ashow astore atan awidthshow begin bind "
                    "bitshift bytesavailable cachestatus ceiling charpath clear cleardictstack "
                    "cleartomark clip clippath closefile closepath concat concatmatrix copy copypage "
                    "cos count countdictstack countexecstack counttomark currentcmykcolor "
                    "currentcolorspace currentdash currentdict currentfile currentflat currentfont "
                    "currentgray currenthsbcolor currentlinecap currentlinejoin currentlinewidth "
                    "currentmatrix currentmiterlimit currentpagedevice currentpoint currentrgbcolor "
                    "currentscreen currenttransfer cvi cvlit cvn cvr cvrs cvs cvx def defaultmatrix "
                    "definefont dict dictstack div dtransform dup echo end eoclip eofill eq "
                    "erasepage errordict exch exec execstack executeonly executive exit exp false "
                    "file fill findfont flattenpath floor flush flushfile for forall ge get "
                    "getinterval grestore grestoreall gsave gt idetmatrix idiv idtransform if ifelse "
                    "image imagemask index initclip initgraphics initmatrix inustroke invertmatrix "
                    "itransform known kshow le length lineto ln load log loop lt makefont mark "
                    "matrix maxlength mod moveto mul ne neg newpath noaccess nor not null nulldevice "
                    "or pathbbox pathforall pop print prompt pstack put putinterval quit rand rcheck "
                    "rcurveto read readhexstring readline readonly readstring rectstroke repeat "
                    "resetfile restore reversepath rlineto rmoveto roll rotate round rrand run save "
                    "scale scalefont search setblackgeneration setcachedevice setcachelimit "
                    "setcharwidth setcolorscreen setcolortransfer setdash setflat setfont setgray "
                    "sethsbcolor setlinecap setlinejoin setlinewidth setmatrix setmiterlimit "
                    "setpagedevice setrgbcolor setscreen settransfer setvmthreshold show showpage "
                    "sin sqrt srand stack start status statusdict stop stopped store string "
                    "stringwidth stroke strokepath sub systemdict token token transform translate "
                    "true truncate type ueofill undefineresource userdict usertime version vmstatus "
                    "wcheck where widthshow write writehexstring writestring xcheck xor");

this->Keys_Set.ps1 = wxT("GlobalFontDirectory ISOLatin1Encoding SharedFontDirectory UserObject arct "
                     "colorimage cshow currentblackgeneration currentcacheparams currentcmykcolor "
                     "currentcolor currentcolorrendering currentcolorscreen currentcolorspace "
                     "currentcolortransfer currentdevparams currentglobal currentgstate "
                     "currenthalftone currentobjectformat currentoverprint currentpacking "
                     "currentpagedevice currentshared currentstrokeadjust currentsystemparams "
                     "currentundercolorremoval currentuserparams defineresource defineuserobject "
                     "deletefile execform execuserobject filenameforall fileposition filter "
                     "findencoding findresource gcheck globaldict glyphshow gstate ineofill infill "
                     "instroke inueofill inufill inustroke languagelevel makepattern packedarray "
                     "printobject product realtime rectclip rectfill rectstroke renamefile "
                     "resourceforall resourcestatus revision rootfont scheck selectfont serialnumber "
                     "setbbox setblackgeneration setcachedevice2 setcacheparams setcmykcolor setcolor "
                     "setcolorrendering setcolorscreen setcolorspace setcolortranfer setdevparams "
                     "setfileposition setglobal setgstate sethalftone setobjectformat setoverprint "
                     "setpacking setpagedevice setpattern setshared setstrokeadjust setsystemparams "
                     "setucacheparams setundercolorremoval setuserparams setvmthreshold shareddict "
                     "startjob uappend ucache ucachestatus ueofill ufill undef undefinefont "
                     "undefineresource undefineuserobject upath ustroke ustrokepath vmreclaim "
                     "writeobject xshow xyshow yshow");

this->Keys_Set.ps2 = wxT("cliprestore clipsave composefont currentsmoothness findcolorrendering "
                     "setsmoothness shfill");

this->Keys_Set.ps3 = wxT("begintransparencygroup begintransparencymask bytestring charboxpath "
                     "currentaccuratecurves currentblendmode currentcurvejoin currentdashadapt "
                     "currentdotlength currentfilladjust2 currentlimitclamp currentopacityalpha "
                     "currentoverprintmode currentrasterop currentshapealpha "
                     "currentsourcetransparent currenttextknockout currenttexturetransparent "
                     "dashpath dicttomark discardtransparencygroup discardtransparencymask "
                     "endtransparencygroup endtransparencymask execn filename filename "
                     "fileposition forceput forceundef forgetsave getbitsrect getdevice "
                     "inittransparencymask knownget locksafe makeoperator namestring oserrno "
                     "oserrorstring peekstring rectappend runandhide setaccuratecurves "
                     "setblendmode setcurvejoin setdashadapt setdebug setdefaultmatrix "
                     "setdotlength setfilladjust2 setlimitclamp setmaxlength setopacityalpha "
                     "setoverprintmode setrasterop setsafe setshapealpha setsourcetransparent "
                     "settextknockout settexturetransparent stringbreak stringmatch tempfile "
                     "type1decrypt type1encrypt type1execchar unread arccos arcsin copydevice "
                     "copyscanlines currentdevice finddevice findlibfile findprotodevice flushpage "
                     "getdeviceprops getenv makeimagedevice makewordimagedevice max min "
                     "putdeviceprops setdevice");

this->Keys_Set.ps4 = wxT("");
#endif //XSTC_NO_PS

#ifndef XSTC_NO_PYTHON
this->Keys_Set.python = wxT("and assert break class continue def del elif "
                        "else except exec finally for from global if import in is lambda None "
                        "not or pass print raise return try while yield");

this->Keys_Set.python1 = wxT("and assert break class continue def del elif "
                         "else except exec finally for from global if import in is lambda None "
                         "not or pass print raise return try while yield");
#endif //XSTC_NO_PYTHON

#ifndef XSTC_NO_R
this->Keys_Set.r = wxT("");

this->Keys_Set.r1 = wxT("");

this->Keys_Set.r2 = wxT("");
#endif //XSTC_NO_R

#ifndef XSTC_NO_REBOL
this->Keys_Set.rebol = wxT("about abs absolute add "
                       "alert alias all alter and "
                       "any any-block? any-function? any-string? any-type? "
                       "any-word? append arccosine arcsine arctangent "
                       "array as-pair ask at attempt "
                       "back binary? bind bitset? block? "
                       "break brightness? browse build-tag caret-to-offset "
                       "catch center-face change change-dir char? "
                       "charset checksum choose clean-path clear "
                       "clear-fields close comment complement component? "
                       "compose compress confirm connected? construct "
                       "context copy cosine datatype? date? "
                       "debase decimal? decode-cgi decompress dehex "
                       "delete detab difference dir? dirize "
                       "disarm dispatch divide do do-events "
                       "does dump-face dump-obj echo either "
                       "else email? empty? enbase entab "
                       "equal? error? even? event? exclude "
                       "exists? exit exp extract fifth "
                       "file? find first flash focus "
                       "for forall foreach forever form "
                       "forskip found? fourth free func "
                       "function function? get get-modes get-word? "
                       "greater-or-equal? greater? halt has hash? "
                       "head head? help hide hide-popup "
                       "if image? import-email in in-window? "
                       "index? info? inform input input? "
                       "insert integer? intersect issue? join "
                       "last launch layout length? lesser-or-equal? "
                       "lesser? library? license link? list-dir "
                       "list? lit-path? lit-word? load load-image "
                       "log-10 log-2 log-e logic? loop "
                       "lowercase make make-dir make-face max "
                       "maximum maximum-of min minimum minimum-of "
                       "modified? mold money? multiply native? "
                       "negate negative? next none? not "
                       "not-equal? now number? object? odd? "
                       "offset-to-caret offset? op? open or "
                       "pair? paren? parse parse-xml path? "
                       "pick poke port? positive? power "
                       "prin print probe protect protect-system "
                       "query quit random read read-io "
                       "recycle reduce refinement? reform rejoin "
                       "remainder remold remove remove-each rename "
                       "repeat repend replace request request-color "
                       "request-date request-download request-file request-list request-pass "
                       "request-text resend return reverse routine? "
                       "same? save script? second secure "
                       "select send series? set set-modes "
                       "set-net set-path? set-word? show show-popup "
                       "sign? sine size-text size? skip "
                       "sort source span? split-path square-root "
                       "strict-equal? strict-not-equal? string? struct? stylize "
                       "subtract suffix? switch tag? tail "
                       "tail? tangent third throw time? "
                       "to to-binary to-bitset to-block to-char "
                       "to-date to-decimal to-email to-file to-get-word "
                       "to-hash to-hex to-idate to-image to-integer "
                       "to-issue to-list to-lit-path to-lit-word to-local-file "
                       "to-logic to-money to-pair to-paren to-path "
                       "to-rebol-file to-refinement to-set-path to-set-word to-string "
                       "to-tag to-time to-tuple to-url to-word "
                       "trace trim try tuple? type? "
                       "unfocus union unique unprotect unset "
                       "unset? until unview update upgrade "
                       "uppercase url? usage use value? "
                       "view viewed? wait what what-dir "
                       "while within? word? write write-io xor zero? ");

this->Keys_Set.rebol1 = wxT("action? any-block? any-function? any-string? "
                        "any-type? any-word? binary? bitset? block? char? component? connected? "
                        "datatype? date? decimal? dir? email? empty? equal? error? even? event? exists? "
                        "file? found? function? get-word? greater-or-equal greater? hash? head? image? "
                        "index? info? input? integer? issue? length? lesser-or-equal? lesser? library? "
                        "link-app? link? list? lit-path? lit-word? logic? modified? money? native? negative? "
                        "none? not-equal? number? object? odd? offset? op? pair? paren? path? port? "
                        "positive? rebol-command? rebol-encap? rebol-link? rebol-pro? rebol-view? "
                        "refinement? routine? same? script? series? set-path? set-word? sign? size? "
                        "strict-equal? strict-not-equal string? struct? suffix? tag? tail? time? tuple? type? "
                        "unset? url? value? view? word? zero?");

this->Keys_Set.rebol2 = wxT("action! any-block! any-function! any-string! any-type! "
                        "any-word! binary! bitset! block! char! datatype! date! decimal! email! error! "
                        "event! file! function! get-word! hash! image! integer! issue! library! list! lit-path! "
                        "lit-word! logic! money! native! none! number! object! op! pair! paren! path! "
                        "port! refinement! routine! series! set-path! set-word! string! struct! symbol! tag! "
                        "time! tuple! unset! url! word!");
#endif //XSTC_NO_REBOL

#ifndef XSTC_NO_RUBY
this->Keys_Set.ruby = wxT("__FILE__ and def end in or self unless __LINE__ begin "
                      "defined? ensure module redo super until BEGIN break do false next rescue "
                      "then when END case else for nil retry true while alias class elsif if "
                      "not return undef yield");
#endif //XSTC_NO_RUBY

#ifndef XSTC_NO_SCRIPTOL
this->Keys_Set.scriptol = wxT("act action alias always and array as "
                          "bool boolean break by byte "
                          "class case catch const constant continue "
                          "dyn def define dict do double "
                          "echo else elsif end enum error false file for float forever function "
                          "globak gtk "
                          "in if ifdef import include int integer java javax "
                          "let long match mod nil not natural null number "
                          "or print protected public real return redo "
                          "scan script scriptol sol short super static step until using "
                          "var text then this true try "
                          "void volatile while when "
                          "undef zero");
#endif //XSTC_NO_SCRIPTOL

#ifndef XSTC_NO_SMALLTALK
this->Keys_Set.smalltalk = wxT("ifTrue: ifFalse: whileTrue: whileFalse: ifNil: ifNotNil: whileTrue whileFals" "repeat isNil notNil");
#endif //XSTC_NO_SMALLTALK

#ifndef XSTC_NO_SPECMAN
this->Keys_Set.specman = wxT("struct unit "
                         "integer real bool int long uint nibble byte bits bytes bit time string "
                         "var instance event "
                         "verilog vhdl "
                         "on compute start expect check that routine "
                         "specman is also first only with like "
                         "list of all radix hex dec bin ignore illegal "
                         "traceable untraceable "
                         "cover using count_only trace_only at_least transition item ranges "
                         "cross text call task within "
                         "packing low high "
                         "locker address "
                         "body code vec chars "
                         "byte_array external_pointer "
                         "choose matches "
                         "if then else when try "
                         "case casex casez default "
                         "and or not xor "
                         "until repeat while for from to step each do break continue "
                         "before next always -kind network "
                         "index it me in new return result select "
                         "cycle sample events forever "
                         "wait  change  negedge rise fall delay sync sim true detach eventually emit "
                         "gen keep keeping soft before "
                         "define as computed type extend "
                         "variable global sys "
                         "import "
                         "untyped symtab ECHO DOECHO "
                         "initialize non_terminal testgroup delayed exit finish "
                         "out append print outf appendf "
                         "post_generate pre_generate setup_test finalize_test extract_test "
                         "init run copy as_a a set_config dut_error add clear lock quit "
                         "lock unlock release swap quit to_string value stop_run "
                         "crc_8 crc_32 crc_32_flip get_config add0 all_indices and_all "
                         "apply average count delete exists first_index get_indices "
                         "has insert is_a_permutation is_empty key key_exists key_index "
                         "last last_index max max_index max_value min min_index "
                         "min_value or_all pop pop0 push push0 product resize reverse "
                         "sort split sum top top0 unique clear is_all_iterations "
                         "get_enclosing_unit hdl_path exec deep_compare deep_compare_physical "
                         "pack unpack warning error fatal "
                         "size "
                         "files load module ntv source_ref script read write "
                         "initial idle others posedge clock cycles "
                         "statement action command member exp block num file");

this->Keys_Set.specman1 = wxT("TRUE FALSE MAX_INT MIN_INT NULL UNDEF ");

this->Keys_Set.specman2 = wxT("any_sequence_item sequence any_sequence_driver driver "
                          "created_driver  parent_sequence "
                          "bfm_interaction_mode PULL_MODE PUSH_MODE MAIN SIMPLE RANDOM "
                          "max_random_count max_random_depth num_of_last_items "
                          "NORMAL NONE FULL LOW HIGH MEDIUM logger message "
                          "get_tags show_units show_actions show_message ignore_tags "
                          "set_style set_screen set_file set_flush_frequency "
                          "set_format set_units set_actions at_message_verbosity "
                          "short_name short_name_path short_name_style "
                          "private protected package rerun any_env "
                          "unqualified_clk clk reset_start reset_end "
                          "message_logger verbosity tags to_file "
                          "body pre_body post_body get_next_item send_to_bfm "
                          "get_depth get_driver nice_string get_index grab "
                          "is_blocked is_relevant ungrab mid_do post_do post_trace "
                          "pre_do current_grabber get_current_item get_num_items_sent "
                          "get_sequence_trace_list get_trace_list is_grabbed "
                          "try_next_item check_is_relevant delay_clock "
                          "get_sub_drivers regenerate_data wait_for_sequences "
                          "stop");

this->Keys_Set.specman3 = wxT("");
#endif //XSTC_NO_SPECMAN

#ifndef XSTC_NO_SPICE
this->Keys_Set.spice = wxT("ac  alias  alter  alterparam append askvalues assertvalid autoscale "
                       "break compose copy copytodoc dc  delete  destroy  destroyvec diff  display "
                       "disto  dowhile  echo  else  end  errorstop  fftinit filter foreach  fourier "
                       "freqtotime function  functionundef goto  homecursors if isdisplayed label "
                       "let  linearize listing  load  loadaccumulator makelabel movelabel makesmithplot "
                       "movecursorleft movecursorright  msgbox nameplot  newplot nextparam noise "
                       "nopoints  op plot plotf plotref poly print  printcursors  printevent printname "
                       "printplot printstatus printtext printtol printunits printval printvector "
                       "pwl  pz  quit  removesmithplot rename repeat  resume  rotate  runs rusage "
                       "save sendplot sendscript sens  set  setcursor setdoc setlabel setlabeltype "
                       "setmargins setnthtrigger  setunits setvec setparam setplot  setquery "
                       "setscaletype settracecolor settracestyle setsource settrigger  setvec "
                       "setxlimits setylimits show  showmod  sort status  step  stop  switch "
                       "tf  timetofreq  timetowave tran  unalias  unlet  unset  unalterparam "
                       "update version view   wavefilter  wavetotime where  while  write");

this->Keys_Set.spice1 = wxT("abs askvalue atan average ceil cos db differentiate differentiatex "
                        "exp finalvalue floor getcursorx getcursory getcursory0 getcursory1 "
                        "getparam im ln initialvalue integrate integratex interpolate isdef "
                        "isdisplayed j log length mag, max maxscale mean meanpts min minscale "
                        "nextplot  nextvector  norm operatingpoint ph phase phaseextend pk_pk "
                        "pos pulse re rms rmspts rnd sameplot  sin sqrt stddev stddevpts tan "
                        "tfall tolerance trise unitvec vector");

this->Keys_Set.spice2 = wxT("param nodeset include options dcconv subckt ends model");
#endif //XSTC_NO_SPICE

#ifndef XSTC_NO_MSSQL
this->Keys_Set.mssql = wxT("");

this->Keys_Set.mssql1 = wxT("");

this->Keys_Set.mssql2 = wxT("");

this->Keys_Set.mssql3 = wxT("");

this->Keys_Set.mssql4 = wxT("");

this->Keys_Set.mssql5 = wxT("");

this->Keys_Set.mssql6 = wxT("");
#endif //XSTC_NO_MSSQL

#ifndef XSTC_NO_MYSQL
this->Keys_Set.mysql = wxT("");

this->Keys_Set.mysql1 = wxT("");

this->Keys_Set.mysql2 = wxT("");

this->Keys_Set.mysql3 = wxT("");

this->Keys_Set.mysql4 = wxT("");

this->Keys_Set.mysql5 = wxT("");

this->Keys_Set.mysql6 = wxT("");

this->Keys_Set.mysql7 = wxT("");

this->Keys_Set.mysql8 = wxT("");
#endif //XSTC_NO_MYSQL

#ifndef XSTC_NO_SQL
this->Keys_Set.sql = wxT("absolute action add admin after aggregate "
                     "alias all allocate alter and any are array as asc "
                     "assertion at authorization "
                     "before begin binary bit blob boolean both breadth by "
                     "call cascade cascaded case cast catalog char character "
                     "check class clob close collate collation column commit "
                     "completion connect connection constraint constraints "
                     "constructor continue corresponding create cross cube current "
                     "current_date current_path current_role current_time current_timestamp "
                     "current_user cursor cycle "
                     "data date day deallocate dec decimal declare default "
                     "deferrable deferred delete depth deref desc describe descriptor "
                     "destroy destructor deterministic dictionary diagnostics disconnect "
                     "distinct domain double drop dynamic "
                     "each else end end-exec equals escape every except "
                     "exception exec execute external "
                     "false fetch first float for foreign found from free full "
                     "function "
                     "general get global go goto grant group grouping "
                     "having host hour "
                     "identity if ignore immediate in indicator initialize initially "
                     "inner inout input insert int integer intersect interval "
                     "into is isolation iterate "
                     "join "
                     "key "
                     "language large last lateral leading left less level like "
                     "limit local localtime localtimestamp locator "
                     "map match minute modifies modify module month "
                     "names national natural nchar nclob new next no none "
                     "not null numeric "
                     "object of off old on only open operation option "
                     "or order ordinality out outer output "
                     "pad parameter parameters partial path postfix precision prefix "
                     "preorder prepare preserve primary "
                     "prior privileges procedure public "
                     "read reads real recursive ref references referencing relative "
                     "restrict result return returns revoke right "
                     "role rollback rollup routine row rows "
                     "savepoint schema scroll scope search second section select "
                     "sequence session session_user set sets size smallint some| space "
                     "specific specifictype sql sqlexception sqlstate sqlwarning start "
                     "state statement static structure system_user "
                     "table temporary terminate than then time timestamp "
                     "timezone_hour timezone_minute to trailing transaction translation "
                     "treat trigger true "
                     "under union unique unknown "
                     "unnest update usage user using "
                     "value values varchar variable varying view "
                     "when whenever where with without work write "
                     "year "
                     "zone");

this->Keys_Set.sql1 = wxT("all alter and any array as asc at authid avg begin between "
                      "binary_integer "
                      "body boolean bulk by char char_base check close cluster collect "
                      "comment commit compress connect constant create current currval "
                      "cursor date day declare decimal default delete desc distinct "
                      "do drop else elsif end exception exclusive execute exists exit "
                      "extends false fetch float for forall from function goto group "
                      "having heap hour if immediate in index indicator insert integer "
                      "interface intersect interval into is isolation java level like "
                      "limited lock long loop max min minus minute mlslabel mod mode "
                      "month natural naturaln new nextval nocopy not nowait null number "
                      "number_base ocirowid of on opaque open operator option or order "
                      "organization others out package partition pctfree pls_integer "
                      "positive positiven pragma prior private procedure public raise "
                      "range raw real record ref release return reverse rollback row "
                      "rowid rownum rowtype savepoint second select separate set share "
                      "smallint space sql sqlcode sqlerrm start stddev subtype successful "
                      "sum synonym sysdate table then time timestamp to trigger true "
                      "type uid union unique update use user validate values varchar "
                      "varchar2 variance view when whenever where while with work write "
                      "year zone");

this->Keys_Set.sql2 = wxT("param author since return see deprecated todo");

this->Keys_Set.sql3 = wxT("acc~ept a~ppend archive log attribute "
                      "bre~ak bti~tle "
                      "c~hange cl~ear col~umn comp~ute conn~ect copy "
                      "def~ine del desc~ribe disc~onnect "
                      "e~dit exec~ute exit "
                      "get "
                      "help ho~st "
                      "i~nput "
                      "l~ist "
                      "passw~ord pau~se pri~nt pro~mpt "
                      "quit "
                      "recover rem~ark repf~ooter reph~eader r~un "
                      "sav~e set sho~w shutdown spo~ol sta~rt startup store "
                      "timi~ng tti~tle "
                      "undef~ine "
                      "var~iable "
                      "whenever oserror whenever sqlerror ");

this->Keys_Set.sql4 = wxT("dbms_output disable enable get_line "
                      "get_lines new_line put "
                      "put_line");

this->Keys_Set.sql5 = wxT("");

this->Keys_Set.sql6 = wxT("");

this->Keys_Set.sql7 = wxT("");
#endif //XSTC_NO_SQL

#ifndef XSTC_NO_TADS3
this->Keys_Set.tads3_0 = wxT("");

this->Keys_Set.tads3_1 = wxT("");

this->Keys_Set.tads3_2 = wxT("");

this->Keys_Set.tads3_3 = wxT("");
#endif //XSTC_NO_TADS3

#ifndef XSTC_NO_TCL
this->Keys_Set.tcl = wxT("after append array auto_execok "
                     "auto_import auto_load auto_load_index auto_qualify "
                     "beep bgerror binary break case catch cd clock "
                     "close concat continue dde default echo else elseif "
                     "encoding eof error eval exec exit expr fblocked "
                     "fconfigure fcopy file fileevent flush for foreach format "
                     "gets glob global history http if incr info "
                     "interp join lappend lindex linsert list llength load "
                     "loadTk lrange lreplace lsearch lset lsort memory msgcat "
                     "namespace open package pid pkg::create pkg_mkIndex Platform-specific proc "
                     "puts pwd re_syntax read regexp registry regsub rename "
                     "resource return scan seek set socket source split "
                     "string subst switch tclLog tclMacPkgSearch tclPkgSetup tclPkgUnknown tell "
                     "time trace unknown unset update uplevel upvar variable "
                     "vwait while");

this->Keys_Set.tcl1 = wxT("bell bind bindtags bitmap button canvas checkbutton clipboard "
                      "colors console cursors destroy entry event focus font "
                      "frame grab grid image Inter-client keysyms label labelframe "
                      "listbox lower menu menubutton message option options pack "
                      "panedwindow photo place radiobutton raise scale scrollbar selection "
                      "send spinbox text tk tk_chooseColor tk_chooseDirectory tk_dialog tk_focusNext "
                      "tk_getOpenFile tk_messageBox tk_optionMenu tk_popup tk_setPalette tkerror tkvars tkwait "
                      "toplevel winfo wish wm");

this->Keys_Set.tcl2 = wxT("@scope body class code common component configbody "
                      "constructor define destructor hull import inherit itcl itk itk_component "
                      "itk_initialize itk_interior itk_option iwidgets keep method "
                      "private protected public");

this->Keys_Set.tcl3 = wxT("tk_bisque tk_chooseColor tk_dialog tk_focusFollowsMouse tk_focusNext "
                      "tk_focusPrev tk_getOpenFile tk_getSaveFile tk_messageBox tk_optionMenu "
                      "tk_popup tk_setPalette tk_textCopy tk_textCut tk_textPaste "
                      "tkButtonAutoInvoke tkButtonDown tkButtonEnter tkButtonInvoke tkButtonLeave "
                      "tkButtonUp tkCancelRepeat tkCheckRadioDown tkCheckRadioEnter tkCheckRadioInvoke "
                      "tkColorDialog tkColorDialog_BuildDialog tkColorDialog_CancelCmd tkColorDialog_Config tkColorDialog_CreateSelector "
                      "tkColorDialog_DrawColorScale tkColorDialog_EnterColorBar tkColorDialog_HandleRGBEntry tkColorDialog_HandleSelEntr" "tkColorDialog_InitValues "
                      "tkColorDialog_LeaveColorBar tkColorDialog_MoveSelector tkColorDialog_OkCmd tkColorDialog_RedrawColorBar" "tkColorDialog_RedrawFinalColor "
                      "tkColorDialog_ReleaseMouse tkColorDialog_ResizeColorBars tkColorDialog_RgbToX tkColorDialog_SetRGBValue tkColorDialog_StartMove "
                      "tkColorDialog_XToRgb tkConsoleAbout tkConsoleBind tkConsoleExit tkConsoleHistory "
                      "tkConsoleInit tkConsoleInsert tkConsoleInvoke tkConsoleOutput tkConsolePrompt "
                      "tkConsoleSource tkDarken tkEntryAutoScan tkEntryBackspace tkEntryButton1 "
                      "tkEntryClosestGap tkEntryGetSelection tkEntryInsert tkEntryKeySelect tkEntryMouseSelect "
                      "tkEntryNextWord tkEntryPaste tkEntryPreviousWord tkEntrySeeInsert tkEntrySetCursor "
                      "tkEntryTranspose tkEventMotifBindings tkFDGetFileTypes tkFirstMenu tkFocusGroup_BindIn "
                      "tkFocusGroup_BindOut tkFocusGroup_Create tkFocusGroup_Destroy tkFocusGroup_In tkFocusGroup_Out "
                      "tkFocusOK tkGenerateMenuSelect tkIconList tkIconList_Add tkIconList_Arrange "
                      "tkIconList_AutoScan tkIconList_Btn1 tkIconList_Config tkIconList_Create tkIconList_CtrlBtn1 "
                      "tkIconList_Curselection tkIconList_DeleteAll tkIconList_Double1 tkIconList_DrawSelection tkIconList_FocusIn "
                      "tkIconList_FocusOut tkIconList_Get tkIconList_Goto tkIconList_Index tkIconList_Invoke "
                      "tkIconList_KeyPress tkIconList_Leave1 tkIconList_LeftRight tkIconList_Motion1 tkIconList_Reset "
                      "tkIconList_ReturnKey tkIconList_See tkIconList_Select tkIconList_Selection tkIconList_ShiftBtn1 "
                      "tkIconList_UpDown tkListbox tkListboxAutoScan tkListboxBeginExtend tkListboxBeginSelect "
                      "tkListboxBeginToggle tkListboxCancel tkListboxDataExtend tkListboxExtendUpDown tkListboxKeyAccel_Goto "
                      "tkListboxKeyAccel_Key tkListboxKeyAccel_Reset tkListboxKeyAccel_Set tkListboxKeyAccel_Unset tkListboxMotion "
                      "tkListboxSelectAll tkListboxUpDown tkMbButtonUp tkMbEnter tkMbLeave "
                      "tkMbMotion tkMbPost tkMenuButtonDown tkMenuDownArrow tkMenuDup "
                      "tkMenuEscape tkMenuFind tkMenuFindName tkMenuFirstEntry tkMenuInvoke "
                      "tkMenuLeave tkMenuLeftArrow tkMenuMotion tkMenuNextEntry tkMenuNextMenu "
                      "tkMenuRightArrow tkMenuUnpost tkMenuUpArrow tkMessageBox tkMotifFDialog "
                      "tkMotifFDialog_ActivateDList tkMotifFDialog_ActivateFEnt tkMotifFDialog_ActivateFList tkMotifFDialog_ActivateSEn" "tkMotifFDialog_BrowseDList "
                      "tkMotifFDialog_BrowseFList tkMotifFDialog_BuildUI tkMotifFDialog_CancelCmd tkMotifFDialog_Config tkMotifFDialog_Create "
                      "tkMotifFDialog_FileTypes tkMotifFDialog_FilterCmd tkMotifFDialog_InterpFilter tkMotifFDialog_LoadFiles tkMotifFDialog_MakeSList "
                      "tkMotifFDialog_OkCmd tkMotifFDialog_SetFilter tkMotifFDialog_SetListMode tkMotifFDialog_Update tkPostOverPoint "
                      "tkRecolorTree tkRestoreOldGrab tkSaveGrabInfo tkScaleActivate tkScaleButton2Down "
                      "tkScaleButtonDown tkScaleControlPress tkScaleDrag tkScaleEndDrag tkScaleIncrement "
                      "tkScreenChanged tkScrollButton2Down tkScrollButtonDown tkScrollButtonDrag tkScrollButtonUp "
                      "tkScrollByPages tkScrollByUnits tkScrollDrag tkScrollEndDrag tkScrollSelect "
                      "tkScrollStartDrag tkScrollTopBottom tkScrollToPos tkTabToWindow tkTearOffMenu "
                      "tkTextAutoScan tkTextButton1 tkTextClosestGap tkTextInsert tkTextKeyExtend "
                      "tkTextKeySelect tkTextNextPara tkTextNextPos tkTextNextWord tkTextPaste "
                      "tkTextPrevPara tkTextPrevPos tkTextPrevWord tkTextResetAnchor tkTextScrollPages "
                      "tkTextSelectTo tkTextSetCursor tkTextTranspose tkTextUpDownLine tkTraverseToMenu "
                      "tkTraverseWithinMenu");

this->Keys_Set.tcl4 = wxT("expand");

this->Keys_Set.tcl5 = wxT("");

this->Keys_Set.tcl6 = wxT("");

this->Keys_Set.tcl7 = wxT("");

this->Keys_Set.tcl8 = wxT("");
#endif //XSTC_NO_TCL

#ifndef XSTC_NO_TEX
this->Keys_Set.tex = wxT("");
#endif //XSTC_NO_TEX

#ifndef XSTC_NO_VB
this->Keys_Set.vb = wxT("addressof alias and as attribute base begin binary boolean byref byte byval call case compare "
                    "const currency date decimal declare defbool defbyte defint deflng defcur defsng defdbl defdec "
                    "defdate defstr defobj defvar dim do double each else elseif empty end enum eqv erase error "
                    "event exit explicit false for friend function get gosub goto if imp implements in input integer "
                    "is len let lib like load lock long loop lset me mid midb mod new next not nothing null object "
                    "on option optional or paramarray preserve print private property public raiseevent randomize "
                    "redim rem resume return rset seek select set single static step stop string sub then time to "
                    "true type typeof unload until variant wend while with withevents xor addhandler andalso as assembly auto catch cbool cbyt" "cchar cdate cdbl cdec char cint "
                    "class clng cobj cshort csng cstr ctype default delegate finally gettype handles imports "
                    "inherits interface mustinherit mustoverride mybase myclass namespace notinheritable "
                    "notoverridable orelse overloads overridable overrides protected readonly removehandler "
                    "shadows shared short structure synclock throw try unicode when writeonly");

this->Keys_Set.vb1 = wxT("appactivate beep chdir chdrive close filecopy get input kill line unlock mkdir name "
                     "open print put reset rmdir savepicture savesetting seek sendkeys setattr width write");

this->Keys_Set.vb2 = wxT("");

this->Keys_Set.vb3 = wxT("");
#endif //XSTC_NO_VB

#ifndef XSTC_NO_VBSCRIPT
this->Keys_Set.vbscript = wxT("addressof alias and as attribute base begin binary boolean byref byte byval call case compare "
                          "const currency date decimal declare defbool defbyte defint deflng defcur defsng defdbl defdec "
                          "defdate defstr defobj defvar dim do double each else elseif empty end enum eqv erase error "
                          "event exit explicit false for friend function get gosub goto if imp implements in input integer "
                          "is len let lib like load lock long loop lset me mid midb mod new next not nothing null object "
                          "on option optional or paramarray preserve print private property public raiseevent randomize "
                          "redim rem resume return rset seek select set single static step stop string sub then time to "
                          "true type typeof unload until variant wend while with withevents xor addhandler andalso as assembly auto catch cbool cbyt" "cchar cdate cdbl cdec char cint "
                          "class clng cobj cshort csng cstr ctype default delegate finally gettype handles imports "
                          "inherits interface mustinherit mustoverride mybase myclass namespace notinheritable "
                          "notoverridable orelse overloads overridable overrides protected readonly removehandler "
                          "shadows shared short structure synclock throw try unicode when writeonly");

this->Keys_Set.vbscript1 = wxT("appactivate beep chdir chdrive close filecopy get input kill line unlock mkdir name "
                           "open print put reset rmdir savepicture savesetting seek sendkeys setattr width write");

this->Keys_Set.vbscript2 = wxT("");

this->Keys_Set.vbscript3 = wxT("");
#endif //XSTC_NO_VBSCRIPT

#ifndef XSTC_NO_VERILOG
this->Keys_Set.verilog = wxT("always and assign begin "
                        "xbuf buf bufif0 bufif1 case casex casez cmos "
                        "default defparam else end endcase "
                        "endfunction endmodule endprimitive endspecify "
                        "endtable endtask event for force forever "
                        "fork function if initial inout input "
                        "integer join macromodule makefile module "
                        "nand negedge nmos nor not notif0 notif1 "
                        "or output parameter pmos posedge primitive "
                        "pulldown pullup rcmos real realtime reg "
                        "repeat rnmos rpmos rtran rtranif0 rtranif1 "
                        "signed specify specparam supply supply0 supply1 table "
                        "task time tran tranif0 tranif1 tri tri0 "
                        "tri1 triand trior trireg vectored wait "
                        "wand while wire wor xnor xor");

this->Keys_Set.verilog1 = wxT("always and assign begin "
                        "xbuf buf bufif0 bufif1 case casex casez cmos "
                        "default defparam else end endcase "
                        "endfunction endmodule endprimitive endspecify "
                        "endtable endtask event for force forever "
                        "fork function if initial inout input "
                        "integer join macromodule makefile module "
                        "nand negedge nmos nor not notif0 notif1 "
                        "or output parameter pmos posedge primitive "
                        "pulldown pullup rcmos real realtime reg "
                        "repeat rnmos rpmos rtran rtranif0 rtranif1 "
                        "signed specify specparam supply supply0 supply1 table "
                        "task time tran tranif0 tranif1 tri tri0 "
                        "tri1 triand trior trireg vectored wait "
                        "wand while wire wor xnor xor");

this->Keys_Set.verilog2 = wxT("$readmemb $readmemh $sreadmemb $sreadmemh $display $write $strobe $monitor $fdisplay $fwrite $fstrobe "
                        "$fmonitor $fopen $fclose $time $stime $realtime $scale $printtimescale $timeformat $stop $finish $save "
                        "$incsave $restart $input $log $nolog $key $nokey $scope $showscopes $showscopes $showvars $showvars "
                        "$countdrivers $list $monitoron $monitoroff $dumpon $dumpoff $dumpfile $dumplimit $dumpflush $dumpvars "
                        "$dumpall $reset $reset $reset $reset $reset $random $getpattern $rtoi $itor $realtobits $bitstoreal "
                        "$setup $hold $setuphold $period $width $skew $recovery");

this->Keys_Set.verilog3 = wxT("");
#endif //XSTC_NO_VERILOG

#ifndef XSTC_NO_VHDL
this->Keys_Set.vhdl = wxT("access after alias all architecture array assert attribute begin block "
                       "body buffer bus case component configuration constant disconnect downto else elsif end entity exit file "
                       "for function generate generic group guarded if impure in inertial inout is label library linkage literal "
                       "loop map new next null of on open others out package port postponed procedure process pure range record "
                       "register reject report return select severity shared signal subtype then to transport type unaffected "
                       "units until use variable wait when while with ");

this->Keys_Set.vhdl1 = wxT("abs and mod nand nor not or rem rol ror sla sll sra srl xnor xor ");

this->Keys_Set.vhdl2 = wxT("left right low high ascending image value pos val succ pred leftof rightof base range reverse_range "
                       "length delayed stable quiet transaction event active last_event last_active last_value driving "
                       "driving_value simple_name path_name instance_name ");

this->Keys_Set.vhdl3 = wxT("now readline read writeline write endfile resolved to_bit to_bitvector to_stdulogic to_stdlogicvector "
                       "to_stdulogicvector to_x01 to_x01z to_UX01 rising_edge falling_edge is_x shift_left shift_right rotate_left "
                       "rotate_right resize to_integer to_unsigned to_signed std_match to_01");

this->Keys_Set.vhdl4 = wxT("std ieee work standard textio std_logic_1164 std_logic_arith std_logic_misc std_logic_signed "
                       "std_logic_textio std_logic_unsigned numeric_bit numeric_std math_complex math_real vital_primitives "
                       "vital_timing");

this->Keys_Set.vhdl5 = wxT("boolean bit character severity_level integer real time delay_length natural positive string bit_vector "
                       "file_open_kind file_open_status line text side width std_ulogic std_ulogic_vector std_logic "
                       "std_logic_vector X01 X01Z UX01 UX01Z unsigned signed ");

this->Keys_Set.vhdl6 = wxT("");
#endif //XSTC_NO_VHDL

#ifndef XSTC_NO_YAML
this->Keys_Set.yaml = wxT("true false yes no");
#endif //XSTC_NO_YAML
#endif //XSTC_NO_KEYS_DEFINED
}

wxString XSTC::KeyCheck(wxString keyname)
{//check the contents of a Keyword_Sets object substring, is it a keywordset or filename
 //if "^ISFILE^" is at the beggining, it is assumed to be a file or set of coma delimited file names.
   wxString temp;
   int x,y;

   if(keyname.Contains(wxT("^ISFILE^")))
   {
      temp = keyname;
      keyname = wxT("");
      temp.Remove(0, temp.Find("^ISFILE^") + 8);
      temp.Trim(false);
      if(temp[0] == ',')
      {
         temp.Remove(0, 1);
      }
      y = temp.Freq(',');
      for(x=0;x<=y;x++)
      {
         keyname = keyname + this->LoadKeyWords(temp.Mid(0, temp.Find(",")));
         temp.Remove(0, temp.Find(",")+1);
         temp.Trim(false);
      }
   }
 return keyname;
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

#endif //XSTC_NO_KEYS
