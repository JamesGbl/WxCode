
/*
 * lex.cpp
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

#ifdef XSTC_H_FLAT
  #include "xstc.h"
#else
  #include <wx/xstc/xstc.h>
#endif

#ifndef XSTC_NO_ABAQUS
void XSTC::LexABAQUS()
{
   this->StyleSetSpec(XSTC_DEF(ABAQUS_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMENTBLOCK), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_PROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMAND), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_SLASHCOMMAND), XS_misic);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_STARCOMMAND), XS_misic);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_ARGUMENT), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(ABAQUS_FUNCTION), XS_instruction);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMENTBLOCK),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_WORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/PROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_PROCESSOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_COMMAND),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/SLASHCOMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_SLASHCOMMAND),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/STARCOMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_STARCOMMAND),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/ARGUMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_ARGUMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ABAQUS/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ABAQUS_FUNCTION),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ABAQUS

#ifndef XSTC_NO_ADA
void XSTC::LexADA()
{
   SetStyleBits(5);
   this->StyleSetSpec(XSTC_DEF(ADA_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(ADA_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(ADA_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(ADA_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(ADA_DELIMITER), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(ADA_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(ADA_CHARACTEREOL), XS_char);
   this->StyleSetSpec(XSTC_DEF(ADA_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(ADA_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(ADA_LABEL), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(ADA_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(ADA_ILLEGAL), XS_bad);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_DEFAULT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_WORD),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_IDENTIFIER),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_NUMBER),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/GLOBALCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_DELIMITER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_CHARACTER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/CHARACTEREOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_CHARACTEREOL),           colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_STRING),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_STRINGEOL),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_LABEL),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ADA/ILLEGAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ADA_ILLEGAL),                colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ADA

#ifndef XSTC_NO_APDL
void XSTC::LexAPDL()
{
    this->StyleSetSpec(XSTC_DEF(APDL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(APDL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(APDL_COMMENTBLOCK), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(APDL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(APDL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(APDL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(APDL_PROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(APDL_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(APDL_SLASHCOMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(APDL_STARCOMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(APDL_ARGUMENT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(APDL_FUNCTION), XS_lang);
	this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/PROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_PROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/SLASHCOMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_SLASHCOMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/STARCOMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_STARCOMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/ARGUMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_ARGUMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/APDL/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(APDL_FUNCTION),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_APDL

#ifndef XSTC_NO_ASM
void XSTC::LexASM()
{
    this->StyleSetSpec(XSTC_DEF(ASM_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ASM_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ASM_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(ASM_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ASM_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ASM_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(ASM_CPUINSTRUCTION), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ASM_MATHINSTRUCTION), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ASM_REGISTER), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASM_DIRECTIVE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASM_DIRECTIVEOPERAND), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASM_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ASM_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(ASM_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ASM_EXTINSTRUCTION), XS_instruction);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/CPUINSTRUCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_CPUINSTRUCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/MATHINSTRUCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_MATHINSTRUCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/REGISTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_REGISTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_DIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/DIRECTIVEOPERAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_DIRECTIVEOPERAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASM/EXTINSTRUCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASM_EXTINSTRUCTION),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ASM

#ifndef XSTC_NO_ASN1
void XSTC::LexASN1()
{
    this->StyleSetSpec(XSTC_DEF(ASN1_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ASN1_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ASN1_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(ASN1_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ASN1_OID), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ASN1_SCALAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(ASN1_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(ASN1_ATTRIBUTE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASN1_DESCRIPTOR), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASN1_TYPE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ASN1_OPERATOR), XS_symbol);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/OID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_OID),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/SCALAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_SCALAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/DESCRIPTOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_DESCRIPTOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/TYPE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_TYPE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASN1/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASN1_OPERATOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ASN1

#ifndef XSTC_NO_ASYMPTOTE
void XSTC::LexASYMPTOTE()
{
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENTLINE), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_STRINGEOL), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENTLINEDOC), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_WORD2), XS_key2);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENTLINE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_WORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_STRINGEOL),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_COMMENTLINEDOC),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/ASYMPTOTE/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ASYMPTOTE_WORD2),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ASYMPTOTE

#ifndef XSTC_NO_AU3
void XSTC::LexAU3()
{
	this->StyleSetSpec(XSTC_DEF(AU3_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(AU3_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(AU3_COMMENTBLOCK), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(AU3_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(AU3_FUNCTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(AU3_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(AU3_MACRO), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(AU3_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(AU3_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(AU3_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(AU3_SENT), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(AU3_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(AU3_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(AU3_EXPAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(AU3_COMOBJ), XS_misic);
#ifdef USELVL
	this->StyleSetSpec(XSTC_DEF(AU3_UDF), XS_misic);
#endif
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_FUNCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/MACRO"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_MACRO),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/SENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_SENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/EXPAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_EXPAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/COMOBJ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_COMOBJ),            colorval);

#ifdef XSTC_USELVL
    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AU3/UDF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AU3_UDF),            colorval);
#endif
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_AU3

#ifndef XSTC_NO_AVE
void XSTC::LexAVE()
{
	this->StyleSetSpec(XSTC_DEF(AVE_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(AVE_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(AVE_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(AVE_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(AVE_ENUM), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(AVE_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(AVE_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(AVE_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD1), XS_key2);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD2), XS_key3);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD3), XS_key4);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD4), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD5), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(AVE_WORD6), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/ENUM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_ENUM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD5),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/AVE/WORD6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(AVE_WORD6),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_AVE

#ifndef XSTC_NO_BAAN
void XSTC::LexBAAN()
{
	this->StyleSetSpec(XSTC_DEF(BAAN_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(BAAN_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(BAAN_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(BAAN_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(BAAN_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(BAAN_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(BAAN_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(BAAN_OPERATOR ), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(BAAN_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(BAAN_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(BAAN_WORD2), XS_key2);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAAN/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAAN_WORD2),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_BAAN

#ifndef XSTC_NO_BLITZBASIC
void XSTC::LexBLBASIC()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_BLITZBASIC

#ifndef XSTC_NO_FREEBASIC
void XSTC::LexFRBASIC()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_FREEBASIC

#ifndef XSTC_NO_POWERBASIC
void XSTC::LexPOBASIC()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_POWERBASIC

#ifndef XSTC_NO_PUREBASIC
void XSTC::LexPUBASIC()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PUREBASIC

#ifndef XSTC_NO_BASH
void XSTC::LexBASH()
{
	this->StyleSetSpec(XSTC_DEF(SH_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(SH_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(SH_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SH_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(SH_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(SH_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SH_CHARACTER ), XS_char);
	this->StyleSetSpec(XSTC_DEF(SH_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SH_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(SH_SCALAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(SH_PARAM ), XS_lang);
	this->StyleSetSpec(XSTC_DEF(SH_BACKTICKS), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SH_HERE_DELIM), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SH_HERE_Q), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/SCALAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_SCALAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/PARAM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_PARAM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/BACKTICKS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_BACKTICKS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/HERE_DELIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_HERE_DELIM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SH/HERE_Q"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SH_HERE_Q),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_BASH

#ifndef XSTC_NO_BATCH
void XSTC::LexBATCH()
{
	this->StyleSetSpec(XSTC_DEF(BAT_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(BAT_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(BAT_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(BAT_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(BAT_HIDE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(BAT_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(BAT_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(BAT_OPERATOR), XS_symbol);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/HIDE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_HIDE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/BAT/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(BAT_OPERATOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_BATCH

#ifndef XSTC_NO_BULLANT
/* has no properties to set up
void XSTC::LexBULLANT()
{
}
*/
#endif //XSTC_NO_BULLANT

#ifndef XSTC_NO_CAML
void XSTC::LexCAML()
{
	this->StyleSetSpec(XSTC_DEF(CAML_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(CAML_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(CAML_TAGNAME), XS_tag);
	this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(CAML_LINENUM), XS_number);
	this->StyleSetSpec(XSTC_DEF(CAML_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(CAML_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(CAML_CHAR), XS_char);
	this->StyleSetSpec(XSTC_DEF(CAML_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(CAML_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CAML_COMMENT1), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(CAML_COMMENT2), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(CAML_COMMENT3), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());SetStyleBits(5);

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/TAGNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_TAGNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/LINENUM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_LINENUM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/CHAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_CHAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/COMMENT1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/COMMENT2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_COMMENT2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CAML/COMMENT3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CAML_COMMENT3),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CAML

#ifndef XSTC_NO_CLW
void XSTC::LexCLW()
{
	this->StyleSetSpec(XSTC_DEF(CLW_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(CLW_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CLW_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CLW_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(CLW_USER_IDENTIFIER), XS_user);
	this->StyleSetSpec(XSTC_DEF(CLW_INTEGER_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CLW_REAL_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CLW_PICTURE_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(CLW_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(CLW_COMPILER_DIRECTIVE), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(CLW_RUNTIME_EXPRESSIONS), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(CLW_BUILTIN_PROCEDURES_FUNCTION), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(CLW_STRUCTURE_DATA_TYPE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CLW_ATTRIBUTE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CLW_STANDARD_EQUATE), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(CLW_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(CLW_DEPRECATED), XS_lang);
    this->SetStyleBits(GetStyleBitsNeeded());SetStyleBits(5);

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/USER_IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_USER_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/INTEGER_CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_INTEGER_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/REAL_CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_REAL_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/PICTURE_STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_PICTURE_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/COMPILER_DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_COMPILER_DIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/RUNTIME_EXPRESSIONS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/BUILTIN_PROCEDURES_FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_BUILTIN_PROCEDURES_FUNCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/STRUCTURE_DATA_TYPE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_STRUCTURE_DATA_TYPE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/STANDARD_EQUATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_STANDARD_EQUATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CLW/DEPRECATED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CLW_DEPRECATED),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CLW

#ifndef XSTC_NO_CMAKE
void XSTC::LexCMAKE()
{
   this->StyleSetSpec(XSTC_DEF(CMAKE_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(CMAKE_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGDQ), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGLQ), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGRQ), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(CMAKE_COMMANDS), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(CMAKE_PARAMETERS), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(CMAKE_VARIABLE), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(CMAKE_USERDEFINED), XS_default);
   this->StyleSetSpec(XSTC_DEF(CMAKE_WHILEDEF), XS_misic);
   this->StyleSetSpec(XSTC_DEF(CMAKE_FOREACHDEF), XS_misic);
   this->StyleSetSpec(XSTC_DEF(CMAKE_IFDEFINEDEF), XS_misic);
   this->StyleSetSpec(XSTC_DEF(CMAKE_MACRODEF), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGVAR), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(CMAKE_NUMBER), XS_number);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/STRINGDQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGDQ),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/STRINGLQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGLQ),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/STRINGRQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGRQ),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/COMMANDS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_COMMANDS),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/PARAMETERS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_PARAMETERS),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_VARIABLE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/USERDEFINED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_USERDEFINED),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/WHILEDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_WHILEDEF),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/FOREACHDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_FOREACHDEF),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/IFDEFINEDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_IFDEFINEDEF),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MCMAKE/MACRODEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_MACRODEF),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/STRINGVAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_STRINGVAR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/CMAKE/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CMAKE_NUMBER),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CMAKE

#ifndef XSTC_NO_COBOL
void XSTC::LexCOBOL()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),          colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/COBOL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),         colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_COBOL

#ifndef XSTC_NO_CONF
void XSTC::LexCONF()
{
	this->StyleSetSpec(XSTC_DEF(CONF_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(CONF_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CONF_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(CONF_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(CONF_EXTENSION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CONF_PARAMETER), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CONF_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(CONF_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(CONF_IP), XS_misic);
	this->StyleSetSpec(XSTC_DEF(CONF_DIRECTIVE), XS_instruction);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/EXTENSION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_EXTENSION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/PARAMETER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_PARAMETER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/IP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_IP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CONF/DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CONF_DIRECTIVE),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CONF

#ifndef XSTC_NO_CPP
void XSTC::LexCPP()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_key3);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_VERBATIM), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), XS_bad);
   this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),           colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRINGEOL),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/VERBATIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_VERBATIM),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC),         colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD),      colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/GLOBALCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CPP

#ifndef XSTC_NO_CSOUND
void XSTC::LexCSOUND()
{
	this->StyleSetSpec(XSTC_DEF(CSOUND_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(CSOUND_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CSOUND_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(CSOUND_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(CSOUND_INSTR), XS_misic);
	this->StyleSetSpec(XSTC_DEF(CSOUND_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(CSOUND_OPCODE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(CSOUND_HEADERSTMT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSOUND_USERKEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(CSOUND_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CSOUND_PARAM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSOUND_ARATE_VAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CSOUND_KRATE_VAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CSOUND_IRATE_VAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(CSOUND_GLOBAL_VAR), XS_global);
	this->StyleSetSpec(XSTC_DEF(CSOUND_STRINGEOL), XS_d_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/INSTR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_INSTR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/OPCODE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_OPCODE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/HEADERSTMT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_HEADERSTMT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/USERKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_USERKEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/PARAM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_PARAM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/ARATE_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_ARATE_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/KRATE_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_KRATE_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/IRATE_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_IRATE_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/GLOBAL_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_GLOBAL_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSOUND/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSOUND_STRINGEOL),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CSOUND

#ifndef XSTC_NO_CSS
void XSTC::LexCSS()
{
	this->StyleSetSpec(XSTC_DEF(CSS_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(CSS_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(CSS_CLASS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSS_PSEUDOCLASS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSS_UNKNOWN_PSEUDOCLASS), XS_bad);
	this->StyleSetSpec(XSTC_DEF(CSS_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(CSS_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(CSS_UNKNOWN_IDENTIFIER), XS_bad);
	this->StyleSetSpec(XSTC_DEF(CSS_VALUE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSS_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(CSS_ID), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSS_IMPORTANT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(CSS_DIRECTIVE), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(CSS_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(CSS_SINGLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(CSS_IDENTIFIER2), XS_default);
	this->StyleSetSpec(XSTC_DEF(CSS_ATTRIBUTE), XS_lang);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/CLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_CLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/PSEUDOCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_PSEUDOCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/UNKNOWN_PSEUDOCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_UNKNOWN_PSEUDOCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/UNKNOWN_IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_UNKNOWN_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/VALUE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_VALUE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/ID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_ID),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/IMPORTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_IMPORTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/SINGLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_SINGLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_DIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/IDENTIFIER2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_IDENTIFIER2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/CSS/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(CSS_ATTRIBUTE),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_CSS

#ifndef XSTC_NO_D
void XSTC::LexD()
{
   this->StyleSetSpec(XSTC_DEF(D_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(D_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTLINE), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTDOC), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTNESTED), XS_comment);
   this->StyleSetSpec(XSTC_DEF(D_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(D_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(D_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(D_WORD3), XS_key3);
   this->StyleSetSpec(XSTC_DEF(D_TYPEDEF), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(D_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(D_STRINGEOL), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(D_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(D_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(D_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTLINEDOC), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(D_COMMENTDOCKEYWORDERROR), XS_comment3);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTLINE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTDOC),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTNESTED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTNESTED),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_WORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_WORD2),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_WORD3),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/TYPEDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_TYPEDEF),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_STRINGEOL),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTLINEDOC),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTDOCKEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/D/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(D_COMMENTDOCKEYWORDERROR),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_D

#ifndef XSTC_NO_DIFF
void XSTC::LexDIFF()
{
	this->StyleSetSpec(XSTC_DEF(DIFF_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(DIFF_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(DIFF_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(DIFF_HEADER), XS_key1);
	this->StyleSetSpec(XSTC_DEF(DIFF_POSITION), XS_key2);
	this->StyleSetSpec(XSTC_DEF(DIFF_DELETED), XS_key3);
	this->StyleSetSpec(XSTC_DEF(DIFF_ADDED), XS_key4);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/HEADER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_HEADER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/POSITION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_POSITION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/DELETED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_DELETED),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/DIFF/ADDED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(DIFF_ADDED),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_DIFF

#ifndef XSTC_NO_EIFFEL
void XSTC::LexEIFFEL()
{
	this->StyleSetSpec(XSTC_DEF(EIFFEL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(EIFFEL_STRINGEOL), XS_d_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/EIFFEL/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(EIFFEL_STRINGEOL),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_EIFFEL

#ifndef XSTC_NO_EIFFELKW
void XSTC::LexEIFFELKW()
{
   LexEIFFEL();
}
#endif //XSTC_NO_EIFFELKW

#ifndef XSTC_NO_ERLANG
void XSTC::LexERLANG()
{
	this->StyleSetSpec(XSTC_DEF(ERLANG_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ERLANG_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ERLANG_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(ERLANG_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(ERLANG_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(ERLANG_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ERLANG_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ERLANG_ATOM), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(ERLANG_FUNCTION_NAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ERLANG_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(ERLANG_MACRO), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(ERLANG_RECORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(ERLANG_SEPARATOR), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ERLANG_NODE_NAME), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ERLANG_UNKNOWN), XS_bad);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/ATOM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_ATOM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/FUNCTION_NAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_FUNCTION_NAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/MACRO"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_MACRO),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/RECORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_RECORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/SEPARATOR"), &colorval) && colorval != wxT(""))

          this->StyleSetSpec(XSTC_DEF(ERLANG_SEPARATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/NODE_NAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_NODE_NAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERLANG/UNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERLANG_UNKNOWN),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ERLANG

#ifndef XSTC_NO_ERRORLIST
void XSTC::LexERRORLIST()
{
	this->StyleSetSpec(XSTC_DEF(ERR_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ERR_PYTHON), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ERR_GCC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(ERR_MS), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(ERR_CMD), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(ERR_BORLAND), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ERR_PERL), XS_number);
	this->StyleSetSpec(XSTC_DEF(ERR_NET), XS_char);
	this->StyleSetSpec(XSTC_DEF(ERR_LUA), XS_key1);
	this->StyleSetSpec(XSTC_DEF(ERR_CTAG), XS_key2);
	this->StyleSetSpec(XSTC_DEF(ERR_DIFF_CHANGED), XS_key3);
	this->StyleSetSpec(XSTC_DEF(ERR_DIFF_ADDITION), XS_key4);
	this->StyleSetSpec(XSTC_DEF(ERR_DIFF_DELETION), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(ERR_DIFF_MESSAGE), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ERR_PHP), XS_tag);
	this->StyleSetSpec(XSTC_DEF(ERR_ELF), XS_user);
	this->StyleSetSpec(XSTC_DEF(ERR_IFC), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ERR_IFORT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(ERR_ABSF), XS_global);
	this->StyleSetSpec(XSTC_DEF(ERR_TIDY), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ERR_JAVA_STACK), XS_dtype);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/PYTHON"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_PYTHON),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/GCC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_GCC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/MS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_MS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/CMD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_CMD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/BORLAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_BORLAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/PERL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_PERL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/NET"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_NET),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/LUA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_LUA),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/CTAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_CTAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/DIFF_CHANGED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_DIFF_CHANGED),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/DIFF_ADDITION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_DIFF_ADDITION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/DIFF_DELETION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_DIFF_DELETION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/DIFF_MESSAGE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_DIFF_MESSAGE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/PHP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_PHP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/ELF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_ELF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/IFC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_IFC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/IFORT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_IFORT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/ABSF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_ABSF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/TIDY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_TIDY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ERR/JAVA_STACK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ERR_JAVA_STACK),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ERRORLIST

#ifndef XSTC_NO_ESCRIPT
void XSTC::LexESCRIPT()
{
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_BRACE), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD3), XS_key3);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/BRACE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_BRACE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ESCRIPT/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ESCRIPT_WORD3),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_ESCRIPT

#ifndef XSTC_NO_F77
void XSTC::LexF77()
{
	this->StyleSetSpec(XSTC_DEF(F_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(F_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(F_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(F_STRING1), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(F_STRING2), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(F_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(F_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(F_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(F_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(F_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(F_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(F_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(F_OPERATOR2), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(F_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(F_CONTINUATION), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/STRING2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_STRING2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/OPERATOR2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_OPERATOR2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/CONTINUATION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(F_CONTINUATION),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_F77

#ifndef XSTC_NO_FLAGSHIP
void XSTC::LexFLAGSHIP()
{
	this->StyleSetSpec(XSTC_DEF(FS_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENTLINEDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOCKEYWORD), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOCKEYWORDERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(FS_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(FS_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(FS_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(FS_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(FS_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(FS_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(FS_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(FS_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(FS_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(FS_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(FS_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(FS_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(FS_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(FS_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(FS_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(FS_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(FS_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENTLINEDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOCKEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_COMMENTDOCKEYWORDERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FS/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FS_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_FLAGSHIP

#ifndef XSTC_NO_FORTH
void XSTC::LexFORTH()
{
	this->StyleSetSpec(XSTC_DEF(FORTH_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(FORTH_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(FORTH_COMMENT_ML), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(FORTH_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(FORTH_CONTROL), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(FORTH_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(FORTH_DEFWORD), XS_key2);
	this->StyleSetSpec(XSTC_DEF(FORTH_PREWORD1), XS_key3);
	this->StyleSetSpec(XSTC_DEF(FORTH_PREWORD2), XS_key4);
	this->StyleSetSpec(XSTC_DEF(FORTH_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(FORTH_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(FORTH_LOCALE), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/COMMENT_ML"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_COMMENT_ML),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/CONTROL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_CONTROL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/DEFWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_DEFWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/PREWORD1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_PREWORD1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/PREWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_PREWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/FORTH/LOCALE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(FORTH_LOCALE),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_FORTH

#ifndef XSTC_NO_FORTRAN
void XSTC::LexFORTRAN()
{
	this->StyleSetSpec(XSTC_DEF(F_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(F_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(F_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(F_STRING1), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(F_STRING2), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(F_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(F_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(F_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(F_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(F_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(F_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(F_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(F_OPERATOR2), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(F_OPERATOR2), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(F_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(F_CONTINUATION), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/DEFAULT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/COMMENT"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/NUMBER"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/STRING1"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_STRING1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/STRING2"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_STRING2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/STRINGEOL"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/OPERATOR"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/IDENTIFIER"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD2"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/WORD3"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/PREPROCESSOR"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/OPERATOR2"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_OPERATOR2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/LABEL"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/F/CONTINUATION"), &colorval) && colorval != wxT(""))
       this->StyleSetSpec(XSTC_DEF(F_CONTINUATION),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_FORTRAN

#ifndef XSTC_NO_GAP
void XSTC::LexGAP()
{
   this->StyleSetSpec(XSTC_DEF(GAP_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(GAP_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD3), XS_key3);
   this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD4), XS_key4);
   this->StyleSetSpec(XSTC_DEF(GAP_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(GAP_CHAR), XS_char);
   this->StyleSetSpec(XSTC_DEF(GAP_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(GAP_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(GAP_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(GAP_STRINGEOL), XS_s_string);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD2),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD3),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/CHAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_CHAR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/GAP/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GAP_STRINGEOL),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_GAP

#ifndef XSTC_NO_GUI4CLI
void XSTC::LexGUI4CLI()
{
	this->StyleSetSpec(XSTC_DEF(GC_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(GC_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(GC_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(GC_GLOBAL), XS_global);
	this->StyleSetSpec(XSTC_DEF(GC_EVENT), XS_misic);
	this->StyleSetSpec(XSTC_DEF(GC_ATTRIBUTE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(GC_CONTROL), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(GC_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(GC_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(GC_OPERATOR), XS_symbol);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/GLOBAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_GLOBAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/EVENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_EVENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/CONTROL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_CONTROL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/GC/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(GC_OPERATOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_GUI4CLI

#ifndef XSTC_NO_HASKELL
void XSTC::LexHASKELL()
{
	this->StyleSetSpec(XSTC_DEF(HA_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HA_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(HA_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(HA_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HA_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HA_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(HA_CLASS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HA_MODULE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HA_CAPITAL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HA_DATA), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HA_IMPORT), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(HA_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(HA_INSTANCE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HA_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK2), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK3), XS_comment3);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/CLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_CLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/MODULE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_MODULE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/CAPITAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_CAPITAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/DATA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_DATA),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/IMPORT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_IMPORT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/INSTANCE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_INSTANCE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/COMMENTBLOCK2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HA/COMMENTBLOCK3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HA_COMMENTBLOCK3),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_HASKELL

#ifndef XSTC_NO_HTM
void XSTC::LexHTM()
{
	this->StyleSetSpec(XSTC_DEF(H_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(H_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_TAGUNKNOWN), XS_bad);
	this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTEUNKNOWN), XS_bad);
	this->StyleSetSpec(XSTC_DEF(H_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(H_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(H_SINGLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(H_OTHER), XS_default);
	this->StyleSetSpec(XSTC_DEF(H_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(H_ENTITY), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(H_TAGEND), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_XMLSTART), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_XMLEND), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_SCRIPT), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_ASP), XS_lang);
	this->StyleSetSpec(XSTC_DEF(H_ASPAT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(H_CDATA), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(H_QUESTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(H_SGML_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(H_SGML_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(H_SGML_1ST_PARAM), XS_misic);
	this->StyleSetSpec(XSTC_DEF(H_SGML_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(H_SGML_SIMPLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(H_SGML_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(H_SGML_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(H_SGML_ENTITY), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(H_SGML_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(H_SGML_1ST_PARAM_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(H_SGML_BLOCK_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HJ_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HJ_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HJ_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HJ_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HJ_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(HJ_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HJ_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HJ_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(HJ_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HJ_SINGLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(HJ_SYMBOLS), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(HJ_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HJ_REGEX), XS_char);
	this->StyleSetSpec(XSTC_DEF(HJA_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HJA_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HJA_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HJA_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HJA_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(HJA_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HJA_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HJA_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(HJA_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HJA_SINGLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(HJA_SYMBOLS), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(HJA_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HJA_REGEX), XS_char);
	this->StyleSetSpec(XSTC_DEF(HB_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HB_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HB_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HB_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HB_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HB_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HB_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(HB_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HBA_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HBA_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HBA_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HBA_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HBA_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HBA_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HBA_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(HBA_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HP_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HP_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HP_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HP_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HP_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HP_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(HP_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HP_TRIPLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(HP_TRIPLEDOUBLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(HP_CLASSNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HP_DEFNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HP_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(HP_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(HPA_START), XS_default);
	this->StyleSetSpec(XSTC_DEF(HPA_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HPA_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HPA_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HPA_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HPA_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(HPA_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HPA_TRIPLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(HPA_TRIPLEDOUBLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(HPA_CLASSNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HPA_DEFNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(HPA_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(HPA_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(HPHP_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(HPHP_HSTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(HPHP_SIMPLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(HPHP_WORD), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HPHP_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(HPHP_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HPHP_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HPHP_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(HPHP_HSTRING_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(HPHP_OPERATOR), XS_symbol);
	this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAGUNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAGUNKNOWN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ATTRIBUTEUNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTEUNKNOWN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SINGLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SINGLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/OTHER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_OTHER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ENTITY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ENTITY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAGEND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAGEND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/XMLSTART"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_XMLSTART),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/XMLEND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_XMLEND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SCRIPT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SCRIPT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ASP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ASP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ASPAT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ASPAT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/CDATA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_CDATA),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/QUESTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_QUESTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_1ST_PARAM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_1ST_PARAM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_SIMPLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_SIMPLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H_SGML_SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_ENTITY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_ENTITY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_1ST_PARAM_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_1ST_PARAM_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SGML_BLOCK_DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SGML_BLOCK_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/SINGLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_SINGLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/SYMBOLS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_SYMBOLS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJ/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJ_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/SINGLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_SINGLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/SYMBOLS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_SYMBOLS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HJA/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HJA_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HB/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HB_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HBA/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HBA_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/TRIPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_TRIPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/TRIPLEDOUBLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_TRIPLEDOUBLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/CLASSNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_CLASSNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/DEFNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_DEFNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HP/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HP_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/START"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_START),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/TRIPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_TRIPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/TRIPLEDOUBLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_TRIPLEDOUBLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/CLASSNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_CLASSNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/DEFNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_DEFNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPA/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPA_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/HSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_HSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/SIMPLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_SIMPLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/HSTRING_VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_HSTRING_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/HPHP/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(HPHP_OPERATOR),            colorval);

 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_HTM

#ifndef XSTC_NO_INNOSETUP
void XSTC::LexINNO()
{
	this->StyleSetSpec(XSTC_DEF(INNO_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(INNO_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(INNO_PARAMETER), XS_lang);
	this->StyleSetSpec(XSTC_DEF(INNO_SECTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(INNO_PREPROC), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(INNO_PREPROC_INLINE), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(INNO_COMMENT_PASCAL), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD_PASCAL), XS_key2);
	this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD_USER), XS_user);
	this->StyleSetSpec(XSTC_DEF(INNO_STRING_DOUBLE), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(INNO_STRING_SINGLE), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(INNO_IDENTIFIER), XS_default);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/PARAMETER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_PARAMETER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/SECTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_SECTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/PREPROC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_PREPROC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/PREPROC_INLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_PREPROC_INLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/COMMENT_PASCAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_COMMENT_PASCAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/KEYWORD_PASCAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD_PASCAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/KEYWORD_USER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_KEYWORD_USER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/STRING_DOUBLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_STRING_DOUBLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/STRING_SINGLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_STRING_SINGLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/INNO/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(INNO_IDENTIFIER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_INNOSETUP

#ifndef XSTC_NO_JS
void XSTC::LexJS()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_key3);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_VERBATIM), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), XS_bad);
   this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),           colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRINGEOL),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/VERBATIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_VERBATIM),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC),         colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD),      colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/GLOBALCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_JS

#ifndef XSTC_NO_JAVA
void XSTC::LexJAVA()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_key3);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_VERBATIM), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), XS_bad);
   this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),           colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRINGEOL),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/VERBATIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_VERBATIM),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC),         colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD),      colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/JS/GLOBALCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_JAVA

#ifndef XSTC_NO_KIX
void XSTC::LexKIX()
{
	this->StyleSetSpec(XSTC_DEF(KIX_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(KIX_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(KIX_STRING1), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(KIX_STRING2), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(KIX_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(KIX_VAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(KIX_MACRO), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(KIX_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(KIX_FUNCTIONS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(KIX_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(KIX_IDENTIFIER), XS_default);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/STRING1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_STRING1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/STRING2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_STRING2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/MACRO"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_MACRO),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/FUNCTIONS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_FUNCTIONS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIX/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/KIXIDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(KIX_IDENTIFIER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_KIX

#ifndef XSTC_NO_LATEX
void XSTC::LexLATEX()
{
	this->StyleSetSpec(XSTC_DEF(L_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(L_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(L_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(L_MATH), XS_misic);
	this->StyleSetSpec(XSTC_DEF(L_COMMENT), XS_comment);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/L/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(L_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/L/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(L_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/L/TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(L_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/L/MATH"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(L_MATH),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/L/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(L_COMMENT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_LATEX

#ifndef XSTC_NO_LISP
void XSTC::LexLISP()
{
	this->StyleSetSpec(XSTC_DEF(LISP_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(LISP_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(LISP_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(LISP_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(LISP_KEYWORD_KW), XS_key2);
	this->StyleSetSpec(XSTC_DEF(LISP_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(LISP_STRING), XS_default);
	this->StyleSetSpec(XSTC_DEF(LISP_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(LISP_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(LISP_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(LISP_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(LISP_MULTI_COMMENT), XS_comment2);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/KEYWORD_KW"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_KEYWORD_KW),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LISP/MULTI_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LISP_MULTI_COMMENT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_LISP

#ifndef XSTC_NO_LOT
void XSTC::LexLOT()
{
	this->StyleSetSpec(XSTC_DEF(LOT_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(LOT_HEADER), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(LOT_BREAK), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(LOT_SET), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(LOT_PASS), XS_number);
	this->StyleSetSpec(XSTC_DEF(LOT_FAIL), XS_bad);
	this->StyleSetSpec(XSTC_DEF(LOT_ABORT), XS_instruction);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/HEADER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_HEADER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/BREAK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_BREAK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/SET"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_SET),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/PASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_PASS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/FAIL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_FAIL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOT/ABORT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOT_ABORT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_LOT

#ifndef XSTC_NO_LOUT
void XSTC::LexLOUT()
{
	this->StyleSetSpec(XSTC_DEF(LOUT_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(LOUT_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(LOUT_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(LOUT_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(LOUT_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(LOUT_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(LOUT_WORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(LOUT_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(LOUT_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(LOUT_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(LOUT_STRINGEOL), XS_d_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LOUT/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LOUT_STRINGEOL),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_LOUT

#ifndef XSTC_NO_LUA
void XSTC::LexLUA()
{
    this->StyleSetSpec(XSTC_DEF(LUA_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(LUA_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(LUA_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(LUA_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(LUA_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(LUA_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(LUA_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(LUA_LITERALSTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(LUA_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(LUA_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(LUA_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(LUA_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD5), XS_tag);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD6), XS_misic);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD7), XS_lang);
	this->StyleSetSpec(XSTC_DEF(LUA_WORD8), XS_global);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/LITERALSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_LITERALSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD5),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD6),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD7"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD7),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/LUA/WORD8"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(LUA_WORD8),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_LUA

#ifndef XSTC_NO_MAKEFILE
void XSTC::LexMAKEFILE()
{
   this->StyleSetSpec(XSTC_DEF(MAKE_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(MAKE_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MAKE_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(MAKE_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(MAKE_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(MAKE_TARGET), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MAKE_IDEOL), XS_d_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/TARGET"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_TARGET),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAKE/IDEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAKE_IDEOL),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MAKEFILE

#ifndef XSTC_NO_MAGIK
void XSTC::LexMAGIK()
{
   this->StyleSetSpec(XSTC_DEF(MAGIK_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(MAGIK_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(MAGIK_HYPER_COMMENT), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(MAGIK_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(MAGIK_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(MAGIK_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(MAGIK_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(MAGIK_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(MAGIK_FLOW), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(MAGIK_CONTAINER), XS_default);
   this->StyleSetSpec(XSTC_DEF(MAGIK_BRACKET_BLOCK), XS_misic);
   this->StyleSetSpec(XSTC_DEF(MAGIK_BRACE_BLOCK), XS_misic);
   this->StyleSetSpec(XSTC_DEF(MAGIK_SQBRACKET_BLOCK), XS_misic);
   this->StyleSetSpec(XSTC_DEF(MAGIK_UNKNOWN_KEYWORD), XS_bad);
   this->StyleSetSpec(XSTC_DEF(MAGIK_KEYWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(MAGIK_PRAGMA), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(MAGIK_SYMBOL), XS_symbol);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/HYPER_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_HYPER_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/FLOW"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_FLOW),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/CONTAINER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_CONTAINER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/BRACKET_BLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_BRACKET_BLOCK),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/BRACE_BLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_BRACE_BLOCK),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/SQBRACKET_BLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_SQBRACKET_BLOCK),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/UNKNOWN_KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_UNKNOWN_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/PRAGMA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_PRAGMA),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MAGIK/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MAGIK_SYMBOL),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MAGIK

#ifndef XSTC_NO_MATLAB
void XSTC::LexMATLAB()
{
   this->StyleSetSpec(XSTC_DEF(MATLAB_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(MATLAB_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MATLAB_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MATLAB_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(MATLAB_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(MATLAB_STRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(MATLAB_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(MATLAB_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(MATLAB_DOUBLEQUOTESTRING), XS_d_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/DOUBLEQUOTESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_DOUBLEQUOTESTRING),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MATLAB

#ifndef XSTC_NO_METAPOST
void XSTC::LexMETAPOST()
{
	this->StyleSetSpec(XSTC_DEF(METAPOST_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(METAPOST_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(METAPOST_GROUP), XS_lang);
	this->StyleSetSpec(XSTC_DEF(METAPOST_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(METAPOST_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(METAPOST_TEXT), XS_global);
	this->StyleSetSpec(XSTC_DEF(METAPOST_EXTRA), XS_tag);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/GROUP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_GROUP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_TEXT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/METAPOST/EXTRA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(METAPOST_EXTRA),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_METAPOST

#ifndef XSTC_NO_MMIXAL
void XSTC::LexMMIXAL()
{
	this->StyleSetSpec(XSTC_DEF(MMIXAL_LEADWS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_PRE), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_VALID), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_UNKNOWN), XS_bad);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_POST), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPERANDS), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_REF), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_CHAR), XS_char);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_REGISTER), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_HEX), XS_number);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(MMIXAL_INCLUDE), XS_preproc);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/LEADWS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_LEADWS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPCODE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPCODE_PRE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_PRE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPCODE_VALID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_VALID),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPCODE_UNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_UNKNOWN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPCODE_POST"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPCODE_POST),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPERANDS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPERANDS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/REF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_REF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/CHAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_CHAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/REGISTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_REGISTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/HEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_HEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MMIXAL/INCLUDE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MMIXAL_INCLUDE),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MMIXAL

#ifndef XSTC_NO_NIMROD
void XSTC::LexNIMROD()
{
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(P_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(P_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(P_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(P_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(P_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(P_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(P_TRIPLE), XS_misic);
   this->StyleSetSpec(XSTC_DEF(P_TRIPLEDOUBLE), XS_misic);
   this->StyleSetSpec(XSTC_DEF(P_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(P_IDENTIFIER), XS_default);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/TRIPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_TRIPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/TRIPLEDOUBLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_TRIPLEDOUBLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NIMROD/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_IDENTIFIER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_NIMROD

#ifndef XSTC_NO_NNCRONTAB
void XSTC::LexNNCRONTAB()
{
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_TASK), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_SECTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_MODIFIER), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_ASTERISK), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_ENVIRONMENT), XS_global);
	this->StyleSetSpec(XSTC_DEF(NNCRONTAB_IDENTIFIER), XS_default);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/TASK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_TASK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/SECTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_SECTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/MODIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_MODIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/ASTERISK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_ASTERISK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/ENVIRONMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_ENVIRONMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NNCRONTAB/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NNCRONTAB_IDENTIFIER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_NNCRONTAB

#ifndef XSTC_NO_NSIS
void XSTC::LexNSIS()
{
	this->StyleSetSpec(XSTC_DEF(NSIS_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(NSIS_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(NSIS_STRINGDQ), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(NSIS_STRINGLQ), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(NSIS_STRINGRQ), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(NSIS_FUNCTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(NSIS_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_USERDEFINED), XS_default);
	this->StyleSetSpec(XSTC_DEF(NSIS_SECTIONDEF), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_SUBSECTIONDEF), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_IFDEFINEDEF), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(NSIS_MACRODEF), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(NSIS_STRINGVAR), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(NSIS_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(NSIS_SECTIONGROUP), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_PAGEEX), XS_misic);
	this->StyleSetSpec(XSTC_DEF(NSIS_FUNCTIONDEF), XS_lang);
	this->StyleSetSpec(XSTC_DEF(NSIS_COMMENTBOX), XS_comment);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/STRINGDQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_STRINGDQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/STRINGLQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_STRINGLQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/STRINGRQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_STRINGRQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_FUNCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/USERDEFINED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_USERDEFINED),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/SECTIONDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_SECTIONDEF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/SUBSECTIONDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_SUBSECTIONDEF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/IFDEFINEDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_IFDEFINEDEF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/MACRODEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_MACRODEF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/STRINGVAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_STRINGVAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/SECTIONGROUP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_SECTIONGROUP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/PAGEEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_PAGEEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/FUNCTIONDEF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_FUNCTIONDEF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/NSIS/COMMENTBOX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(NSIS_COMMENTBOX),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_NSIS

#ifndef XSTC_NO_OCTAVE
void XSTC::LexOCTAVE()
{
    this->StyleSetSpec(XSTC_DEF(MATLAB_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(MATLAB_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MATLAB_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(MATLAB_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(MATLAB_KEYWORD), XS_key1);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MATLAB/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MATLAB_KEYWORD),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_OCTAVE

#ifndef XSTC_NO_OPAL
void XSTC::LexOPAL()
{
	this->StyleSetSpec(XSTC_DEF(OPAL_SPACE), XS_default);
	this->StyleSetSpec(XSTC_DEF(OPAL_COMMENT_BLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(OPAL_COMMENT_LINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(OPAL_INTEGER), XS_number);
	this->StyleSetSpec(XSTC_DEF(OPAL_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(OPAL_SORT), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(OPAL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(OPAL_PAR), XS_misic);
	this->StyleSetSpec(XSTC_DEF(OPAL_BOOL_CONST), XS_number);
	this->StyleSetSpec(XSTC_DEF(OPAL_DEFAULT), XS_default);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/SPACE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_SPACE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/BOOL_CONST"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_BOOL_CONST),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/COMMENT_BLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_COMMENT_BLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/COMMENT_LINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_COMMENT_LINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/INTEGER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_INTEGER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/SORT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_SORT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/PAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_PAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/OPAL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(OPAL_DEFAULT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_OPAL

#ifndef XSTC_NO_PASCAL
void XSTC::LexPAS()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_key3);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_VERBATIM), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), XS_bad);
   this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),                colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),             colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),                 colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),                   colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),           colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRINGEOL),              colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/VERBATIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_VERBATIM),               colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTLINEDOC),         colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),                  colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD),      colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/C/GLOBALCLASS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_GLOBALCLASS),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PASCAL

#ifndef XSTC_NO_PERL
void XSTC::LexPERL()
{
    this->StyleSetSpec(XSTC_DEF(PL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(PL_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(PL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(PL_POD), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(PL_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(PL_STRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(PL_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(PL_PUNCTUATION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(PL_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(PL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(PL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(PL_SCALAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PL_ARRAY), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PL_HASH), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PL_SYMBOLTABLE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(PL_VARIABLE_INDEXER), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PL_REGEX), XS_char);
	this->StyleSetSpec(XSTC_DEF(PL_REGSUBST), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PL_LONGQUOTE), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_BACKTICKS), XS_char);
	this->StyleSetSpec(XSTC_DEF(PL_DATASECTION), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PL_HERE_DELIM), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(PL_HERE_Q), XS_key3);
	this->StyleSetSpec(XSTC_DEF(PL_HERE_QQ), XS_key3);
	this->StyleSetSpec(XSTC_DEF(PL_HERE_QX), XS_key3);
	this->StyleSetSpec(XSTC_DEF(PL_STRING_Q), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_STRING_QQ), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_STRING_QX), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_STRING_QR), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_STRING_QW), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PL_POD_VERB), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/POD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_POD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/PUNCTUATION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_PUNCTUATION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/SCALAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_SCALAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/ARRAY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_ARRAY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/HASH"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_HASH),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/SYMBOLTABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_SYMBOLTABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/VARIABLE_INDEXER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_VARIABLE_INDEXER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/REGSUBST"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_REGSUBST),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/LONGQUOTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_LONGQUOTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/BACKTICKS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_BACKTICKS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/DATASECTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_DATASECTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/HERE_DELIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_HERE_DELIM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/HERE_Q"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_HERE_Q),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/HERE_QQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_HERE_QQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/HERE_QX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_HERE_QX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING_Q"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING_Q),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING_QQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING_QQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING_QX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING_QX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING_QR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING_QR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/STRING_QW"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_STRING_QW),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PL/POD_VERB"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PL_POD_VERB),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PERL

#ifndef XSTC_NO_PLM
void XSTC::LexPLM()
{
   this->StyleSetSpec(XSTC_DEF(PLM_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(PLM_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(PLM_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PLM_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(PLM_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(PLM_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(PLM_CONTROL), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(PLM_KEYWORD), XS_key1);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/CONTROL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_CONTROL),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PLM/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PLM_KEYWORD),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PLM

#ifndef XSTC_NO_PO
void XSTC::LexPO()
{
   this->StyleSetSpec(XSTC_DEF(PO_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(PO_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(PO_MSGID), XS_key1);
   this->StyleSetSpec(XSTC_DEF(PO_MSGID_TEXT), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PO_MSGSTR), XS_key2);
   this->StyleSetSpec(XSTC_DEF(PO_MSGSTR_TEXT), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PO_MSGCTXT), XS_key3);
   this->StyleSetSpec(XSTC_DEF(PO_MSGCTXT_TEXT), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PO_FUZZY), XS_misic);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGID),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGID_TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGID_TEXT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGSTR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGSTR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGSTR_TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGSTR_TEXT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGCTXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGCTXT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/MSGCTXT_TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_MSGCTXT_TEXT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PO/FUZZY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PO_FUZZY),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PO

#ifndef XSTC_NO_POV
void XSTC::LexPOV()
{
	this->StyleSetSpec(XSTC_DEF(POV_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(POV_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(POV_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(POV_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(POV_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(POV_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(POV_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(POV_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(POV_DIRECTIVE), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(POV_BADDIRECTIVE), XS_bad);
	this->StyleSetSpec(XSTC_DEF(POV_WORD2), XS_key1);
	this->StyleSetSpec(XSTC_DEF(POV_WORD3), XS_key2);
	this->StyleSetSpec(XSTC_DEF(POV_WORD4), XS_key3);
	this->StyleSetSpec(XSTC_DEF(POV_WORD5), XS_key4);
	this->StyleSetSpec(XSTC_DEF(POV_WORD6), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(POV_WORD7), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(POV_WORD8), XS_tag);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_DIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/BADDIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_BADDIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD5),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD6),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD7"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD7),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POV/WORD8"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POV_WORD8),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_POV

#ifndef XSTC_NO_POWERPRO
void XSTC::LexPOWERPRO()
{
   this->StyleSetSpec(XSTC_DEF(POWERPRO_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_COMMENTBLOCK), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD3), XS_key3);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD4), XS_key4);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_DOUBLEQUOTEDSTRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_SINGLEQUOTEDSTRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_LINECONTINUE), XS_char);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_VERBATIM), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_ALTQUOTE), XS_misic);
   this->StyleSetSpec(XSTC_DEF(POWERPRO_FUNCTION), XS_global);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/DOUBLEQUOTEDSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_DOUBLEQUOTEDSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/SINGLEQUOTEDSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_SINGLEQUOTEDSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/LINECONTINUE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_LINECONTINUE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/VERBATIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_VERBATIM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/ALTQUOTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_ALTQUOTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERPRO/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERPRO_FUNCTION),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_POWERPRO

#ifndef XSTC_NO_POWERSHELL
void XSTC::LexPOWERSHELL()
{
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_VARIABLE), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_KEYWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_CMDLET), XS_misic);
   this->StyleSetSpec(XSTC_DEF(POWERSHELL_ALIAS), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELLSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_VARIABLE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/CMDLET"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_CMDLET),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/POWERSHELL/ALIAS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(POWERSHELL_ALIAS),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_POWERSHELL

#ifndef XSTC_NO_PROGRESS
void XSTC::LexPROGRESS()
{
   this->StyleSetSpec(XSTC_DEF(PROGRESS_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_PREPROCESSOR), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_BLOCK), XS_key2);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_END), XS_key3);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT1), XS_comment);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT2), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT3), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT4), XS_key4);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT5), XS_misic);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT6), XS_lang);
   //note the sets are duplicated with underscores
   this->StyleSetSpec(XSTC_DEF(PROGRESS_DEFAULT_), XS_default);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_NUMBER_), XS_number);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_WORD_), XS_key1);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_STRING_), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_CHARACTER_), XS_char);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_PREPROCESSOR_), XS_preproc);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_OPERATOR_), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_IDENTIFIER_), XS_default);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_BLOCK_), XS_key2);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_END_), XS_key3);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT1_), XS_comment);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT2_), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT3_), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT4_), XS_key4);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT5_), XS_misic);
   this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT6_), XS_lang);
   //i had to get creative to support so many comment lines
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_WORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_CHARACTER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_PREPROCESSOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/BLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_BLOCK),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/END"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_END),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT1),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT2),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT3),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT4),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT5),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT6),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/DEFAULT_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_DEFAULT_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/NUMBER_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_NUMBER_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/WORD_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_WORD_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/STRING_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_STRING_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/CHARACTER_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_CHARACTER_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/PREPROCESSOR_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_PREPROCESSOR_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/OPERATOR_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_OPERATOR_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/IDENTIFIER_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_IDENTIFIER_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/BLOCK_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_BLOCK_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/END_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_END_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT1_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT1_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT2_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT2_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT3_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT3_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT4_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT4_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT5_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT5_),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROGRESS/COMMENT6_"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROGRESS_COMMENT6_),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PROGRESS

#ifndef XSTC_NO_PROPERTIES
void XSTC::LexPROPERTIES()
{
	this->StyleSetSpec(XSTC_DEF(PROPS_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(PROPS_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(PROPS_SECTION), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PROPS_ASSIGNMENT), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(PROPS_DEFVAL), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/SECTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_SECTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/ASSIGNMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_ASSIGNMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/DEFVAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_DEFVAL),            colorval);

#ifdef XSTC_USELVL
    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PROPS/KEY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PROPS_KEY),            colorval);
#endif
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PROPERTIES

#ifndef XSTC_NO_PS
void XSTC::LexPS()
{
	this->StyleSetSpec(XSTC_DEF(PS_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(PS_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(PS_DSC_COMMENT), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(PS_DSC_VALUE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(PS_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(PS_NAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(PS_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(PS_LITERAL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(PS_IMMEVAL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(PS_PAREN_ARRAY), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PS_PAREN_DICT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PS_PAREN_PROC), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(PS_TEXT ), XS_misic);
	this->StyleSetSpec(XSTC_DEF(PS_HEXSTRING), XS_number);
	this->StyleSetSpec(XSTC_DEF(PS_BASE85STRING), XS_number);
	this->StyleSetSpec(XSTC_DEF(PS_BADSTRINGCHAR), XS_bad);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/DSC_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_DSC_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/DSC_VALUE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_DSC_VALUE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/NAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_NAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/LITERAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_LITERAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/IMMEVAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_IMMEVAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/PAREN_ARRAY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_PAREN_ARRAY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/PAREN_DICT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_PAREN_DICT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/PAREN_PROC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_PAREN_PROC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_TEXT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/HEXSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_HEXSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/BASE85STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_BASE85STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/PS/BADSTRINGCHAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(PS_BADSTRINGCHAR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PS

#ifndef XSTC_NO_PYTHON
void XSTC::LexPYTHON()
{
    this->StyleSetSpec(XSTC_DEF(P_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(P_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(P_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(P_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(P_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(P_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(P_TRIPLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(P_TRIPLEDOUBLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(P_CLASSNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(P_DEFNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(P_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(P_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(P_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(P_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(P_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(P_DECORATOR), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/TRIPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_TRIPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/TRIPLEDOUBLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_TRIPLEDOUBLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/CLASSNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_CLASSNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/DEFNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_DEFNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/P/DECORATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(P_DECORATOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_PYTHON

#ifndef XSTC_NO_R
void XSTC::LexR()
{
   this->StyleSetSpec(XSTC_DEF(R_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(R_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(R_KWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(R_BASEKWORD), XS_key2);
   this->StyleSetSpec(XSTC_DEF(R_OTHERKWORD), XS_key3);
   this->StyleSetSpec(XSTC_DEF(R_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(R_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(R_STRING2), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(R_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(R_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(R_INFIX), XS_misic);
   this->StyleSetSpec(XSTC_DEF(R_INFIXEOL), XS_misic);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/KWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_KWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/BASEKWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_BASEKWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/OTHERKWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_OTHERKWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/STRING2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_STRING2),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/INFIX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_INFIX),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/R/INFIXEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(R_INFIXEOL),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_R

#ifndef XSTC_NO_REBOL
void XSTC::LexREBOL()
{
	this->StyleSetSpec(XSTC_DEF(REBOL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(REBOL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(REBOL_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(REBOL_PREFACE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(REBOL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(REBOL_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(REBOL_QUOTEDSTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(REBOL_BRACEDSTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(REBOL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_PAIR ), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_TUPLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_BINARY), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_MONEY), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_ISSUE ), XS_misic);
	this->StyleSetSpec(XSTC_DEF(REBOL_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(REBOL_FILE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(REBOL_EMAIL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(REBOL_URL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(REBOL_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_TIME), XS_number);
	this->StyleSetSpec(XSTC_DEF(REBOL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD2), XS_key1);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD3), XS_key2);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD4), XS_key3);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD5), XS_key4);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD6), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD7), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(REBOL_WORD8), XS_global);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/COMMENTBLOCK"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_COMMENTBLOCK),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/PREFACE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_PREFACE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/QUOTEDSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_QUOTEDSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/BRACEDSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_BRACEDSTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/PAIR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_PAIR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/TUPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_TUPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/BINARY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_BINARY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/MONEY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_MONEY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/ISSUE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_ISSUE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/FILE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_FILE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/EMAIL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_EMAIL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/URL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_URL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/TIME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_TIME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD5),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD6),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD7"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD7),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/REBOL/WORD8"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(REBOL_WORD8),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_REBOL

#ifndef XSTC_NO_RUBY
void XSTC::LexRUBY()
{
	this->StyleSetSpec(XSTC_DEF(RB_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(RB_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(RB_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(RB_POD), XS_misic);
	this->StyleSetSpec(XSTC_DEF(RB_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(RB_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(RB_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(RB_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(RB_CLASSNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(RB_DEFNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(RB_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(RB_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(RB_REGEX), XS_char);
	this->StyleSetSpec(XSTC_DEF(RB_GLOBAL), XS_global);
	this->StyleSetSpec(XSTC_DEF(RB_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(RB_MODULE_NAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(RB_INSTANCE_VAR), XS_lang);
	this->StyleSetSpec(XSTC_DEF(RB_CLASS_VAR), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(RB_BACKTICKS), XS_default);
	this->StyleSetSpec(XSTC_DEF(RB_DATASECTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(RB_HERE_DELIM), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(RB_HERE_Q), XS_misic);
	this->StyleSetSpec(XSTC_DEF(RB_HERE_QQ), XS_misic);
	this->StyleSetSpec(XSTC_DEF(RB_HERE_QX), XS_misic);
	this->StyleSetSpec(XSTC_DEF(RB_STRING_Q), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(RB_STRING_QQ), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(RB_STRING_QX), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(RB_STRING_QR), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(RB_STRING_QW), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(RB_WORD_DEMOTED), XS_key2);
	this->StyleSetSpec(XSTC_DEF(RB_STDIN), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(RB_STDOUT), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(RB_STDERR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(RB_UPPER_BOUND), XS_dtype);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/POD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_POD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/CLASSNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_CLASSNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/DEFNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_DEFNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/GLOBAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_GLOBAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/MODULE_NAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_MODULE_NAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/INSTANCE_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_INSTANCE_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/CLASS_VAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_CLASS_VAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/BACKTICKS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_BACKTICKS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/DATASECTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_DATASECTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/HERE_DELIM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_HERE_DELIM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/HERE_Q"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_HERE_Q),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/HERE_QQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_HERE_QQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/HERE_QX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_HERE_QX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING_Q"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING_Q),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING_QQ"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING_QQ),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING_QX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING_QX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING_QR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING_QR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STRING/QW"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STRING_QW),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/WORD_DEMOTED"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_WORD_DEMOTED),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STDIN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STDIN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STDOUT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STDOUT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/STDERR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_STDERR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/RB/UPPER_BOUND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(RB_UPPER_BOUND),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_RUBY

#ifndef XSTC_NO_SCRIPTOL
void XSTC::LexSCRIPTOL()
{
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_WHITE), XS_default);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_PERSISTENT), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CSTYLE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_COMMENTBLOCK), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_TRIPLE), XS_number);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CLASSNAME), XS_lang);
	this->StyleSetSpec(XSTC_DEF(SCRIPTOL_PREPROCESSOR), XS_preproc);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/WHITE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_WHITE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/PERSISTENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_PERSISTENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/CSTYLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CSTYLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/TRIPLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_TRIPLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/CLASSNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_CLASSNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SCRIPTOL/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SCRIPTOL_PREPROCESSOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SCRIPTOL

#ifndef XSTC_NO_SMALLTALK
void XSTC::LexSMALLTALK()
{
	this->StyleSetSpec(XSTC_DEF(ST_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(ST_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(ST_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(ST_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(ST_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(ST_BINARY), XS_number);
	this->StyleSetSpec(XSTC_DEF(ST_BOOL), XS_number);
	this->StyleSetSpec(XSTC_DEF(ST_SELF), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ST_SUPER), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ST_NIL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ST_GLOBAL), XS_global);
	this->StyleSetSpec(XSTC_DEF(ST_RETURN), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ST_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(ST_KWSEND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ST_ASSIGN), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(ST_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(ST_SPEC_SEL), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/BINARY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_BINARY),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/BOOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_BOOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/SELF"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_SELF),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/SUPER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_SUPER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/NIL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_NIL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/GLOBAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_GLOBAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/RETURN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_RETURN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/KWSEND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_KWSEND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/ASSIGN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_ASSIGN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/ST/SPEC_SEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(ST_SPEC_SEL),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SMALLTALK

#ifndef XSTC_NO_SPECMAN
void XSTC::LexSPECMAN()
{
	this->StyleSetSpec(XSTC_DEF(SN_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(SN_CODE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SN_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SN_COMMENTLINEBANG), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(SN_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(SN_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(SN_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SN_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(SN_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(SN_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(SN_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SN_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(SN_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SN_REGEXTAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(SN_SIGNAL), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(SN_USER), XS_user);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/CODE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_CODE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/COMMENTLINEBANG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_COMMENTLINEBANG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/REGEXTAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_REGEXTAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/SIGNAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_SIGNAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SN/USER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SN_USER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SPECMAN

#ifndef XSTC_NO_SML
void XSTC::LexSML()
{
   this->StyleSetSpec(XSTC_DEF(SML_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(SML_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(SML_TAGNAME), XS_tag);
   this->StyleSetSpec(XSTC_DEF(SML_KEYWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(SML_KEYWORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(SML_KEYWORD3), XS_key3);
   this->StyleSetSpec(XSTC_DEF(SML_LINENUM), XS_default);
   this->StyleSetSpec(XSTC_DEF(SML_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(SML_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(SML_CHAR), XS_char);
   this->StyleSetSpec(XSTC_DEF(SML_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(SML_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(SML_COMMENT1), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(SML_COMMENT2), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(SML_COMMENT3), XS_key4);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/TAGNAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_TAGNAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/LINENUM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_LINENUM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/CHAR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_CHAR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/COMMENT1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_COMMENT1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/COMMENT2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_COMMENT2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SML/COMMENT3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SML_COMMENT3),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SML

#ifndef XSTC_NO_SORCUS
void XSTC::LexSORCUS()
{
   this->StyleSetSpec(XSTC_DEF(SORCUS_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(SORCUS_COMMAND), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(SORCUS_PARAMETER), XS_misic);
   this->StyleSetSpec(XSTC_DEF(SORCUS_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(SORCUS_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(SORCUS_STRINGEOL), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(SORCUS_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(SORCUS_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(SORCUS_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(SORCUS_CONSTANT), XS_global);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/PARAMETER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_PARAMETER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SORCUS/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SORCUS_CONSTANT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SORCUS

#ifndef XSTC_NO_SPICE
void XSTC::LexSPICE()
{
	this->StyleSetSpec(XSTC_DEF(SPICE_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(SPICE_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(SPICE_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(SPICE_DELIMITER), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SPICE_VALUE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SPICE_COMMENTLINE), XS_comment);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/DELIMITER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_DELIMITER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/VALUE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_VALUE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SPICE/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SPICE_COMMENTLINE),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SPICE

#ifndef XSTC_NO_MSSQL
void XSTC::LexMSSQL()
{
	this->StyleSetSpec(XSTC_DEF(MSSQL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(MSSQL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MSSQL_LINE_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(MSSQL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(MSSQL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(MSSQL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(MSSQL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(MSSQL_VARIABLE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(MSSQL_COLUMN_NAME), XS_misic);
	this->StyleSetSpec(XSTC_DEF(MSSQL_STATEMENT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MSSQL_DATATYPE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MSSQL_SYSTABLE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MSSQL_GLOBAL_VARIABLE), XS_global);
	this->StyleSetSpec(XSTC_DEF(MSSQL_FUNCTION), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MSSQL_STORED_PROCEDURE), XS_lang);
	this->StyleSetSpec(XSTC_DEF(MSSQL_DEFAULT_PREF_DATATYPE), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(MSSQL_COLUMN_NAME_2), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/LINE_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_LINE_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/COLUMN_NAME"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_COLUMN_NAME),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/STATEMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_STATEMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/DATATYPE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_DATATYPE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/SYSTABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_SYSTABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/GLOBAL_VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_GLOBAL_VARIABLE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_FUNCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/STORED_PROCEDURE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_STORED_PROCEDURE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/DEFAULT_PREF_DATATYPE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_DEFAULT_PREF_DATATYPE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/MSSQL/COLUMN_NAME_2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MSSQL_COLUMN_NAME_2),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MSSQL

#ifndef XSTC_NO_MYSQL
void XSTC::LexMYSQL()
{
   this->StyleSetSpec(XSTC_DEF(MYSQL_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(MYSQL_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(MYSQL_COMMENTLINE), XS_comment);
   this->StyleSetSpec(XSTC_DEF(MYSQL_VARIABLE), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(MYSQL_SYSTEMVARIABLE), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(MYSQL_KNOWNSYSTEMVARIABLE), XS_dtype);
   this->StyleSetSpec(XSTC_DEF(MYSQL_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(MYSQL_MAJORKEYWORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(MYSQL_KEYWORD), XS_key2);
   this->StyleSetSpec(XSTC_DEF(MYSQL_DATABASEOBJECT), XS_key3);
   this->StyleSetSpec(XSTC_DEF(MYSQL_PROCEDUREKEYWORD), XS_key4);
   this->StyleSetSpec(XSTC_DEF(MYSQL_STRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(MYSQL_SQSTRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(MYSQL_DQSTRING), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(MYSQL_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(MYSQL_FUNCTION), XS_instruction);
   this->StyleSetSpec(XSTC_DEF(MYSQL_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(MYSQL_QUOTEDIDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(MYSQL_USER1), XS_misic);
   this->StyleSetSpec(XSTC_DEF(MYSQL_USER2), XS_misic);
   this->StyleSetSpec(XSTC_DEF(MYSQL_USER3), XS_misic);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
   if(usecolor)
   {
      wxString colorval;

      if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_DEFAULT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_COMMENT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_COMMENTLINE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/VARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_VARIABLE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/SYSTEMVARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_SYSTEMVARIABLE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/KNOWNSYSTEMVARIABLE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_KNOWNSYSTEMVARIABLE),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_NUMBER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/MAJORKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_MAJORKEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_KEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/DATABASEOBJECT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_DATABASEOBJECT),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/PROCEDUREKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_PROCEDUREKEYWORD),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_STRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/SQSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_SQSTRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/DQSTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_DQSTRING),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_OPERATOR),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/FUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_FUNCTION),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_IDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/QUOTEDIDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_QUOTEDIDENTIFIER),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/USER1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_USER1),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/USER2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_USER2),            colorval);

	  if(colorconf->Read(confroot + wxT("XSTC/COLOR/MYSQL/USER3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(MYSQL_USER3),            colorval);
   }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_MYSQL

#ifndef XSTC_NO_SQL
void XSTC::LexSQL()
{
	this->StyleSetSpec(XSTC_DEF(SQL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(SQL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(SQL_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(SQL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(SQL_CHARACTER), XS_char);
	this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS_PROMPT), XS_misic);
	this->StyleSetSpec(XSTC_DEF(SQL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(SQL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS_COMMENT), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENTLINEDOC), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(SQL_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOCKEYWORD), XS_global);
	this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOCKEYWORDERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(SQL_USER1), XS_user);
	this->StyleSetSpec(XSTC_DEF(SQL_USER2), XS_user);
	this->StyleSetSpec(XSTC_DEF(SQL_USER3), XS_user);
	this->StyleSetSpec(XSTC_DEF(SQL_USER4), XS_user);
	this->StyleSetSpec(XSTC_DEF(SQL_QUOTEDIDENTIFIER), XS_s_string);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/SQLPLUS"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/SQLPLUS_PROMPT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS_PROMPT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/SQLPLUS_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_SQLPLUS_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENTLINEDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENTLINEDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOCKEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_COMMENTDOCKEYWORDERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/USER1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_USER1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/USER2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_USER2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/USER3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_USER3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/USER4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_USER4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/SQL/QUOTEDIDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(SQL_QUOTEDIDENTIFIER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_SQL

#ifndef XSTC_NO_TACL
void XSTC::LexTACL()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD), XS_comment3);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_global);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/COMMENTDOCKEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/COMMENTDOCKEYWORDERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOCKEYWORDERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TACL/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_TACL

#ifndef XSTC_NO_TADS3
void XSTC::LexTADS3()
{
	this->StyleSetSpec(XSTC_DEF(T3_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(T3_X_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(T3_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(T3_BLOCK_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(T3_LINE_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(T3_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(T3_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(T3_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(T3_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(T3_S_STRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(T3_D_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(T3_X_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(T3_LIB_DIRECTIVE), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(T3_MSG_PARAM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(T3_HTML_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(T3_HTML_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(T3_HTML_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(T3_USER1), XS_user);
	this->StyleSetSpec(XSTC_DEF(T3_USER2), XS_user);
	this->StyleSetSpec(XSTC_DEF(T3_USER3), XS_user);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/X_DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_X_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/BLOCK_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_BLOCK_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/LINE_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_LINE_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/S_STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_S_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/D_STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_D_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/X_STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_X_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/LIB_DIRECTIVE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_LIB_DIRECTIVE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/MSG_PARAM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_MSG_PARAM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/HTML_TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_HTML_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/HTML_DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_HTML_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/HTML_STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_HTML_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/USER1"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_USER1),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/USER2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_USER2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/T3/USER3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(T3_USER3),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_TADS3

#ifndef XSTC_NO_TAL
void XSTC::LexTAL()
{
   this->StyleSetSpec(XSTC_DEF(C_DEFAULT), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER), XS_default);
   this->StyleSetSpec(XSTC_DEF(C_COMMENT), XS_comment);
   this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC), XS_comment2);
   this->StyleSetSpec(XSTC_DEF(C_OPERATOR), XS_symbol);
   this->StyleSetSpec(XSTC_DEF(C_NUMBER), XS_number);
   this->StyleSetSpec(XSTC_DEF(C_CHARACTER), XS_char);
   this->StyleSetSpec(XSTC_DEF(C_STRING), XS_d_string);
   this->StyleSetSpec(XSTC_DEF(C_WORD), XS_key1);
   this->StyleSetSpec(XSTC_DEF(C_WORD2), XS_key2);
   this->StyleSetSpec(XSTC_DEF(C_REGEX), XS_s_string);
   this->StyleSetSpec(XSTC_DEF(C_UUID), XS_global);
   this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR), XS_preproc);
   this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/COMMENTDOC"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_COMMENTDOC),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/CHARACTER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_CHARACTER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/REGEX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_REGEX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/UUID"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_UUID),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TAL/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(C_PREPROCESSOR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_TAL

#ifndef XSTC_NO_TCL
void XSTC::LexTCL()
{
	this->StyleSetSpec(XSTC_DEF(TCL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(TCL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(TCL_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(TCL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD_IN_QUOTE), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(TCL_IN_QUOTE), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(TCL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(TCL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(TCL_SUBSTITUTION), XS_misic);
	this->StyleSetSpec(XSTC_DEF(TCL_SUB_BRACE), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(TCL_MODIFIER), XS_misic);
	this->StyleSetSpec(XSTC_DEF(TCL_EXPAND), XS_misic);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD5), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD6), XS_comment3);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD7), XS_tag);
	this->StyleSetSpec(XSTC_DEF(TCL_WORD8), XS_user);
	this->StyleSetSpec(XSTC_DEF(TCL_COMMENT_BOX), XS_comment);
	this->StyleSetSpec(XSTC_DEF(TCL_BLOCK_COMMENT), XS_comment);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD_IN_QUOTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD_IN_QUOTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/IN_QUOTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_IN_QUOTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/SUBSTITUTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_SUBSTITUTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/SUB_BRACE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_SUB_BRACE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/MODIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_MODIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/EXPAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_EXPAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD5"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD5),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD6"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD6),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD7"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD7),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/WORD8"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_WORD8),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/COMMENT_BOX"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_COMMENT_BOX),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TCL/BLOCK_COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TCL_BLOCK_COMMENT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_TCL

#ifndef XSTC_NO_TEX
void XSTC::LexTEX()
{
    this->StyleSetSpec(XSTC_DEF(TEX_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(TEX_SPECIAL), XS_misic);
	this->StyleSetSpec(XSTC_DEF(TEX_GROUP), XS_global);
	this->StyleSetSpec(XSTC_DEF(TEX_SYMBOL), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(TEX_COMMAND), XS_instruction);
	this->StyleSetSpec(XSTC_DEF(TEX_TEXT), XS_default);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/SPECIAL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_SPECIAL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/GROUP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_GROUP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/SYMBOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_SYMBOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/COMMAND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_COMMAND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/TEX/TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(TEX_TEXT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_TEX

#ifndef XSTC_NO_VB
void XSTC::LexVB()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_VB

#ifndef XSTC_NO_VBSCRIPT
void XSTC::LexVBSCRIPT()
{
	this->StyleSetSpec(XSTC_DEF(B_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(B_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(B_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(B_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(B_DATE), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(B_KEYWORD4), XS_key4);
	this->StyleSetSpec(XSTC_DEF(B_CONSTANT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(B_ASM), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_LABEL), XS_lang);
	this->StyleSetSpec(XSTC_DEF(B_ERROR), XS_bad);
	this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(B_BINNUMBER), XS_number);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/DATE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_DATE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/KEYWORD4"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_KEYWORD4),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/CONSTANT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_CONSTANT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ASM"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ASM),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/LABEL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_LABEL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_ERROR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/HEXNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_HEXNUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/B/BINNUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(B_BINNUMBER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_VBSCRIPT

#ifndef XSTC_NO_VERILOG
void XSTC::LexVERILOG()
{
	this->StyleSetSpec(XSTC_DEF(V_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(V_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(V_COMMENTLINE), XS_comment);
	this->StyleSetSpec(XSTC_DEF(V_COMMENTLINEBANG), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(V_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(V_WORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(V_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(V_WORD2), XS_key2);
	this->StyleSetSpec(XSTC_DEF(V_WORD3), XS_key3);
	this->StyleSetSpec(XSTC_DEF(V_PREPROCESSOR), XS_preproc);
	this->StyleSetSpec(XSTC_DEF(V_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(V_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(V_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(V_USER), XS_user);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/COMMENTLINE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_COMMENTLINE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/COMMENTLINEBANG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_COMMENTLINEBANG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/WORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_WORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/WORD2"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_WORD2),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/WORD3"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_WORD3),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/PREPROCESSOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_PREPROCESSOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/V/USER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(V_USER),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_VERILOG

#ifndef XSTC_NO_VHDL
void XSTC::LexVHDL()
{
	this->StyleSetSpec(XSTC_DEF(VHDL_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(VHDL_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(VHDL_COMMENTLINEBANG), XS_comment2);
	this->StyleSetSpec(XSTC_DEF(VHDL_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(VHDL_STRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(VHDL_OPERATOR), XS_symbol);
	this->StyleSetSpec(XSTC_DEF(VHDL_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(VHDL_STRINGEOL), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(VHDL_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(VHDL_STDOPERATOR), XS_misic);
	this->StyleSetSpec(XSTC_DEF(VHDL_ATTRIBUTE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(VHDL_STDFUNCTION), XS_misic);
	this->StyleSetSpec(XSTC_DEF(VHDL_STDPACKAGE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(VHDL_STDTYPE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(VHDL_USERWORD), XS_user);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/COMMENTLINEBANG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_COMMENTLINEBANG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/OPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_OPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STRINGEOL"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STRINGEOL),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STDOPERATOR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STDOPERATOR),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STDFUNCTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STDFUNCTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STDPACKAGE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STDPACKAGE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/STDTYPE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_STDTYPE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/VHDL/USERWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(VHDL_USERWORD),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_VHDL

#ifndef XSTC_NO_XCODE
void XSTC::LexXCODE()
{
	this->StyleSetSpec(XSTC_DEF(H_XCCOMMENT), XS_comment);
	this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/XCCOMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_XCCOMMENT),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_XCODE

#ifndef XSTC_NO_XML
void XSTC::LexXML()
{
	this->StyleSetSpec(XSTC_DEF(H_TAGEND), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_XMLSTART), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_XMLEND), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_SCRIPT), XS_lang);
	this->StyleSetSpec(XSTC_DEF(H_ASP), XS_lang);
	this->StyleSetSpec(XSTC_DEF(H_ASPAT), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(H_CDATA), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(H_QUESTION), XS_dtype);
	this->StyleSetSpec(XSTC_DEF(H_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(H_TAG), XS_tag);
	this->StyleSetSpec(XSTC_DEF(H_TAGUNKNOWN), XS_bad);
	this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTEUNKNOWN), XS_bad);
	this->StyleSetSpec(XSTC_DEF(H_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(H_DOUBLESTRING), XS_d_string);
	this->StyleSetSpec(XSTC_DEF(H_SINGLESTRING), XS_s_string);
	this->StyleSetSpec(XSTC_DEF(H_OTHER), XS_default);
	this->StyleSetSpec(XSTC_DEF(H_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(H_ENTITY), XS_misic);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAGEND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAGEND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/XMLSTART"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_XMLSTART),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/XMLEND"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_XMLEND),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SCRIPT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SCRIPT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ASP"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ASP),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ASPAT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ASPAT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/CDATA"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_CDATA),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/QUESTION"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_QUESTION),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAG"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAG),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/TAGUNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_TAGUNKNOWN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ATTRIBUTE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ATTRIBUTEUNKNOWN"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ATTRIBUTEUNKNOWN),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/DOUBLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_DOUBLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/SINGLESTRING"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_SINGLESTRING),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/OTHER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_OTHER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/H/ENTITY"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(H_ENTITY),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_XML

#ifndef XSTC_NO_YAML
void XSTC::LexYAML()
{
	this->StyleSetSpec(XSTC_DEF(YAML_DEFAULT), XS_default);
	this->StyleSetSpec(XSTC_DEF(YAML_COMMENT), XS_comment);
	this->StyleSetSpec(XSTC_DEF(YAML_IDENTIFIER), XS_default);
	this->StyleSetSpec(XSTC_DEF(YAML_KEYWORD), XS_key1);
	this->StyleSetSpec(XSTC_DEF(YAML_NUMBER), XS_number);
	this->StyleSetSpec(XSTC_DEF(YAML_REFERENCE), XS_misic);
	this->StyleSetSpec(XSTC_DEF(YAML_DOCUMENT), XS_key2);
	this->StyleSetSpec(XSTC_DEF(YAML_TEXT), XS_default);
	this->StyleSetSpec(XSTC_DEF(YAML_ERROR), XS_bad);
    this->SetStyleBits(GetStyleBitsNeeded());

#ifndef XSTC_NO_CONFIG
 if(usecolor)
 {
    wxString colorval;

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/DEFAULT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_DEFAULT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/COMMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_COMMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/IDENTIFIER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_IDENTIFIER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/KEYWORD"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_KEYWORD),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/NUMBER"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_NUMBER),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/REFERENCE"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_REFERENCE),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/DOCUMENT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_DOCUMENT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/TEXT"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_TEXT),            colorval);

    if(colorconf->Read(confroot + wxT("XSTC/COLOR/YAML/ERROR"), &colorval) && colorval != wxT(""))
          this->StyleSetSpec(XSTC_DEF(YAML_ERROR),            colorval);
 }
#endif //XSTC_NO_CONFIG
}
#endif //XSTC_NO_YAML
