////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        CmdLineParser.hpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef CMDLINEPARSER_HPP
#define CMDLINEPARSER_HPP



#include <wx/cmdline.h>

#include "CmdLineParser.h"



class  CmdLineParser : public  wxCmdLineParser
{
   private   :
      typedef wxCmdLineParser 
                        super;
                        
   public    :
      enum  Action
      {
         Action_RUN        = 0,
         Action_INSTALL    = 1,
         Action_UNINSTALL  = 2,
         Action_DEBUG      = 3,
      };
      
   private   :
      Action            action;
      
   public    : 
      CmdLineParser  ();
      
      void              Parse       ();
      
      const Action      GetAction   () const       { return ( action ); }
};



extern CmdLineParser    cmdlineparser;



#endif
