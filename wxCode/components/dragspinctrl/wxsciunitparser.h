/*
Class: wxSciUnitParser
File: wxsciunitparser.h
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/

#ifndef WXSCIUNITPARSER_H
#define WXSCIUNITPARSER_H


#include <wx/string.h>
#include <wx/list.h>
#include "wxconversion.h"
#include "wxoperandbinarytree.h"
#include <cmath>
#include <fstream>


#define wxSUP_DEFAULT 2
#define wxSUP_CIRCUMFLEX_POW 1 // Finally unused
#define wxSUP_USE_COMMA 2      // Finally unused too (as parameter)
#define wxSUP_USE_RICH_OUTPUT 4



#define wxSUP_NO_ERROR 0
#define wxSUP_COMMA_ERROR 1
#define wxSUP_EXTRACTION_ERROR 2
#define wxSUP_EXPONENT_ERROR 3
#define wxSUP_UNDEFINED_ERROR 4
#define wxSUP_OP_ERROR 5
#define wxSUP_OP_FORMAT_ERROR 6
#define wxSUP_BRACKET_FORMAT_ERROR 7
#define wxSUP_CONVERSION_ERROR 8



WX_DECLARE_LIST(wxConversion, wxConversionArray);
WX_DECLARE_LIST(wxConversionArray, wxConversionArrayList);
WX_DECLARE_LIST(wxString,wxStringArray);
WX_DECLARE_LIST(wxOBTNode*, wxStatesStack);

 
class wxSciUnitParser
{
	public:
                        wxSciUnitParser     (void);
		                wxSciUnitParser     (char op);
                        ~wxSciUnitParser    (void);
                     
               void     InsertConversion    (wxConversion& unit,wxConversion& equiv);
               void     SetProperties       (char prop);
               bool     IsPropEnabled       (char prop)const;
               int      Parse               (wxString astp, wxString& sp , wxString unit) ;


               bool       ReadConversionsFromFile (const char* path);
               void       InsertExpansion         (wxConversion& unit, const wxString* expset,int numberofchanges, double factor, double inifactor);
               
               
               
    private:
            wxConversionArrayList conversions; 
            wxStringArray units;      
            char options;
            
            enum state{
                waiting_bracket=0,
                waiting_operation,
                waiting_operand
              
            };
                       
             
            int          _Parse                  (wxString astp, wxString& sp,wxString unit);
            bool         number_extractor        (wxString s,number& n);
            bool         solve_pow               (wxString x, double y, wxString& res);
            bool         shrink_exponents        (wxString& x, unsigned int pos);
            bool         Add                     (wxOBTNode* n,wxString& unit);
            bool         Mul                     (wxOBTNode* n,wxString& unit);
            bool         Div                     (wxOBTNode* n,wxString& unit);
            bool         Pow                     (wxOBTNode* n,wxString& unit);
            bool         Pow2                    (wxOBTNode* n,wxString& unit);
            void         simplify_mul            (wxString& x);
            void         simplify_div            (wxString& x,wxString& y);
            inline bool  is_number               (char c);
            inline bool  is_letter               (char c);
            inline bool  is_op                   (char c); 
            //Debug 
            //void         printSS                 (wxStatesStack* s);   
            void         delete_point_zeros      (wxString& s); 
            public: void         print_conversions       ();
            private:
            bool                ComplexSearchConversions(wxString unit1,wxString unit2,float* val,wxStringArray *unitl);
            bool                find_conversion         (wxString x, wxString y,wxConversion& c1,wxConversion& c2);
            wxConversionArray*  genconversions          (wxString unit,wxStringArray* unit_list);
            void                perm                    (wxStringArray::Node* n,wxString newunit,float factor,wxStringArray* sa);
            void                Extensions              (wxString unit,wxString s);   
            bool                SearchConversion        (wxString unit1,wxString unit2,float* val,wxStringArray *unitl); 
     
            
};          

#endif 
