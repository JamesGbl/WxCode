/*
Class: wxConversion
File: wxconversion.cpp
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/


#include "wxconversion.h"

/////
/// Comp utils
wxString withcomma3(wxString s){
         if(s.Find('.')!=-1){
              s.Replace(_T("."),_T(","));
         }
         else{
              s.Replace(_T(","),_T("."));
         }                    
         return s;
         
}
////

wxConversion::wxConversion(void)
{
}
wxConversion::wxConversion(const wxConversion& c){
*this = c;                                  
}
wxConversion::wxConversion(wxString Unit,convOperation Op, double Factor)
{
unit = Unit;
op = Op;
factor = Factor;
}
                  
wxConversion::~wxConversion(){}

convOperation wxConversion::GetOp(void) const
{
return op;              
}

double wxConversion::GetFactor(void) const
{
return factor;              
}

wxString wxConversion::GetUnit(void) const
{
return unit;              
}

void wxConversion::SetOp(convOperation c){ op = c;}
void wxConversion::SetFactor(double f){ factor = f;}
void wxConversion::SetUnit(wxString u){unit = u; }

void wxConversion::operator= (const wxConversion& conv)
{
unit = conv.unit;
op = conv.op;
factor = conv.factor;
}

bool wxConversion::operator!= (const wxConversion& conv)
{
return (!((*this) == conv) );         
}


bool wxConversion::operator== (const wxConversion& conv)
{
return ( unit .IsSameAs( conv.unit) && 
       op ==  conv.op && 
       factor == conv.factor);         
}

//TODO: read spaces before operate (try to get end)
istream& operator>> (istream &is,  wxConversion& conv)
 {
  
   wxString s;
   unsigned char ax[256];
   double f;
   unsigned int i;
   char c;
   
   is.get(c);
   while((c==' ' || c=='\t' || c=='\n') && !is.eof())
   {
     is.get(c);
   }
   
   
   is.unget();
   i=0;
   is.get(c);
   while(c!=' ' && c!='\t' && c!='\n' && !is.eof())
   {
        ax[i]= c; 
        i++;
        is.get(c);                     
   }
   ax[i]='\0';            
   s.Append(wxString::Format((wxChar*)"%s",ax));
   
  
   
   if(s.GetChar(0)=='*')
   {
      conv.SetOp(cOpMult);
   }
   else
   {
       if(s.GetChar(0)=='+')
       {
          conv.SetOp(cOpAdd);
       }
       else
       {
         conv.SetOp(cOpInvalid); 
         if(s.Cmp(_T(""))!=0){
             cout<<"Error: Conversion format not suported for "<<s<<endl;
             cout<<"\t - No legal operator"<<endl;
         }
         return(is); 
       }
   }
  
         
  for(i=1;s.GetChar(i)<58 && i<s.Len();++i){
	 
	
	#if defined( __WXMSW__)
	if(s.GetChar(i)==',')
                  (s.GetWritableChar(i))='.';
	#elif defined (__WXGTK__)
	if(s.GetChar(i)=='.')
                  (s.GetWritableChar(i))=',';
	#else
		#error
	#endif 
                 
  }
  
  if(i==s.Len())
  {
    conv.SetOp(cOpInvalid);  
    cout<<"Error: Conversion format not suported for "<<s<<endl;
    cout<<"\t - No unit found"<<endl;
    return(is); 
    }
  else{
    if( !((s.Mid(1,i-1)).ToDouble(&f))){
        if(!(withcomma3(s).Mid(1,i-1).ToDouble(&f))){      
            cout<<"Error: Conversion format not suported for "<<s<<endl;
            cout<<"\t - Unable to convert factor "<<endl;
            return(is); 
        }
    }
    else{
         conv.SetFactor (f);
         }
         
    }
       
    conv.SetUnit(s.Mid(i));
        
   
   
   return(is); 

 }

 
 ostream& operator<< (ostream &os, wxConversion& conv)
 {
   os<<(char)conv.GetOp()<<conv.GetFactor()<<conv.GetUnit();
   return (os);
 }            
