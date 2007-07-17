////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        WinServiceApp.cpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007/06/10                                                    //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#include <wx/wxprec.h>

#include "wx/WinServiceApp.hpp"
#include "wx/WinServiceEvent.hpp"



DEFINE_EVENT_TYPE ( wxEVT_WINSERVICE )



wxEvent *  wxWinServiceEvent :: Clone () const
{
   return ( new  wxWinServiceEvent ( *this ) );
}


#if 0
wxWinServiceEvent :: wxWinServiceEvent ()
   : wxEvent ( 0, wxEVT_WINDOWSSERVICE )
{
   control  = Control_STOP;
   type     = 0;
   data     = 0;
   
   error    = Error_NONE;
}   
#endif


wxWinServiceEvent :: wxWinServiceEvent ( const wxWinServiceEvent &  that )
   : wxEvent ( that )
{
   control  = that.control;
   type     = that.type;
   data     = that.data;
   
   error    = that.error;
}


   
wxWinServiceEvent :: wxWinServiceEvent ( const Control &  _control, wxUint32  _type, const void *  _data )
   : wxEvent ( 0, wxEVT_WINSERVICE )
{
   control  = _control;
   type     = _type;
   data     = _data;
   
   error    = Error_NONE;
}



wxWinServiceEvent :: wxWinServiceEvent ( wxWinServiceApp *  _object, const Control &  _control, wxUint32  _type, const void *  _data )
   : wxEvent ( 0, wxEVT_WINSERVICE )
{
   m_eventObject  = _object;
   
   control        = _control;
   type           = _type;
   data           = _data;
   
   error          = Error_NONE;
}



void  wxWinServiceEvent :: SetError ( const Error &  _error )
{
   error = _error;
}
