////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        CmdLineParser.cpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "CmdLineParser.hpp"



CmdLineParser           cmdlineparser;



CmdLineParser :: CmdLineParser ()
   : wxCmdLineParser ()
{
   action   = Action_RUN;
   
   AddSwitch   ( "Install"    , wxEmptyString, "Install wxWidgets Service"             , wxCMD_LINE_PARAM_OPTIONAL );
   AddSwitch   ( "UnInstall"  , wxEmptyString, "UnInstall wxWidgets Service"           , wxCMD_LINE_PARAM_OPTIONAL );
   AddSwitch   ( "Debug"      , wxEmptyString, "Debug i.e. Run as Console Application" , wxCMD_LINE_PARAM_OPTIONAL );
}   



void  CmdLineParser :: Parse ()
{
   super :: Parse ();
   
   if      ( Found ( "Install"   ) )
      action   = Action_INSTALL;
   else if ( Found ( "UnInstall" ) )
      action   = Action_UNINSTALL;
   else if ( Found ( "Debug"     ) )
      action   = Action_DEBUG;
}
