// Soli Deo Gloria!



#ifndef TABLEPICKERDIALOG_HPP
#define TABLEPICKERDIALOG_HPP



#include <wx/listctrl.h>
#include <wx/dialog.h>

#include "ODBCTable.h"



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
