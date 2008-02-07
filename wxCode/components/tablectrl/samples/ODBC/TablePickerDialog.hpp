////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        TablePickerDialog.hpp                                         //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef TABLEPICKERDIALOG_HPP
#define TABLEPICKERDIALOG_HPP



#include <wx/listctrl.h>
#include <wx/dialog.h>

#include "wx/ODBC/ODBCTable.h"



class  TablePickerDialog : public  wxDialog
{
   private   :
      typedef wxDialog     super;
  
   private   :
      DECLARE_EVENT_TABLE  ()
      
   private   :
      wxODBCDbc &          dbc;
      wxODBCTableArray &   ta;
      
      wxSizer *            sizer;
      wxSizer *            input;
      wxSizer *            button;
      
      wxListCtrl *         tables;
      
   private   :
      void                 OnOK              ( wxCommandEvent & );
      
   private   :
      void                 LoadTables        ();
      
   public    :
      TablePickerDialog  ( wxWindow *, wxODBCDbc &, wxODBCTableArray & );
      
      const wxString       GetTable          () const;
};



#endif
