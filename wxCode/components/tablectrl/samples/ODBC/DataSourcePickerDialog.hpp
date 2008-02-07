////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        DataSourcePickerDialog.hpp                                    //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef DATASOURCEPICKERDIALOG_HPP
#define DATASOURCEPICKERDIALOG_HPP



#include <wx/dialog.h>

#include <wx/ODBC/ODBCTable.h>



class  DataSourcePickerDialog : public  wxDialog
{
   private   :
      typedef wxDialog     super;
  
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
//    wxDbConnectInf &     connect;
      
   private   :
      wxODBCEnv &          env;
      
      wxSizer *            sizer;
      wxSizer *            input;
      wxSizer *            button;
      
      wxListBox *          sources;
      wxTextCtrl *         userid;
      wxTextCtrl *         passwd;
      wxTextCtrl *         directory;
      
   private   :
      void                 OnOK              ( wxCommandEvent & );
      
   private   :
      void                 LoadDataSources   ();
      
   public    :
      DataSourcePickerDialog  ( wxWindow *, wxODBCEnv & );
      
      const wxString       GetDSN            () const;
      const wxString       GetUserId         () const;
      const wxString       GetPasswd         () const;
      const wxString       GetDirectory      () const;
};



#endif
