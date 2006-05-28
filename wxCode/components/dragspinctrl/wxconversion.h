
/*
Class: wxConversion
File: wxconversion.h
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/


#ifndef WXCONVERSION_H
#define WXCONVERSION_H

#include <iostream>
#include <wx/string.h>

using namespace std;

enum convOperation
{
     cOpAdd='+',
     cOpMult='*',
     cOpInvalid='#'
};


class wxConversion
{
      public:
                            wxConversion    (void);
                            wxConversion    (const wxConversion&);
                            wxConversion    (wxString Unit,convOperation Op, double Factor);
                            ~wxConversion   (void);
            convOperation   GetOp           (void) const;
            double          GetFactor       (void) const;
            wxString        GetUnit         (void) const; 
            
            void            SetOp           (convOperation c);
            void            SetFactor       (double f);
            void            SetUnit         (wxString u);
            
            void            operator=       (const wxConversion& conv);
            bool            operator!=      (const wxConversion& conv);
            bool            operator==      (const wxConversion& conv);
            
            friend          istream& operator>> (istream &is,  wxConversion& conv);
            friend          ostream& operator<< (ostream &os, wxConversion& conv);
      
      private:
              wxString       unit;
              convOperation  op;
              double         factor;

};



#endif 
