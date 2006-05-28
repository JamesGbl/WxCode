 /*
Class: wxFSpinCtrl
File: wxfspinctrl.cpp
Author Víctor Alejandro Gil Sepúlveda
From: Spain
Finishing Date: 14-2-02
Definition: See Docs
*/




#include "fdragspinctrl.h"
#include <stdio.h>

IMPLEMENT_DYNAMIC_CLASS(wxFDSCEvent, wxEvent)
DEFINE_EVENT_TYPE(wxEVT_FDSC_TEXT_CHANGED)
DEFINE_EVENT_TYPE(wxEVT_FDSC_ERROR)

//Utils for compatibility
wxString withcomma2(wxString& s){
         if(s.Find('.')!=-1){
              s.Replace(_T("."),_T(","));
         }
         else{
              s.Replace(_T(","),_T("."));
         }                    
         return s;
         
}

inline bool  is_letter(char c)
{
    return ((c<91 && c>64)||(c<123 && c>96));
}

//Special SetValue
inline void SSetValue(wxTextCtrl* t1,wxTextCtrl* t2,wxString& s){
    cout<<"SpecialSetValue "<<s<<endl;   
	t1->SetValue(s);
	t2->SetValue(s);
}


wxFDragSpinCtrl::wxFDragSpinCtrl(wxWindow *parent,wxWindowID id ,
               const wxPoint& pos ,const wxSize& sz ,long style,
               const double initial ,double min, double max, int precision ,
               int alignment,
               const wxString& unit, 
               wxSciUnitParser* scup, 
               const wxString& name)
                :wxPanel(parent,id,pos,sz, wxBORDER_NONE,name)
               {
                    wxString ini;
                    wxSize size;
                       
                    shift = false;    
                    pushed = false;   
		            isenabled= true;
                    

                                  
                    parser = scup;
                   
                    
                    // Create new unique ids 
                    tc_id = wxNewId();
                    wxRegisterId(tc_id);
		    
		            tc2_id = wxNewId();
                    wxRegisterId(tc2_id);
                    
                    id_drag =  wxNewId();
                    wxRegisterId(id_drag);
                        
                    this->unit = unit; 
                    this->min = min;
                    this->max = max;
                    
                    size=sz;
                    if(sz.GetHeight()<0){
                        size.SetHeight(25);                 
                    }
                    if(sz.GetWidth()<0){
                       size.SetWidth(40);                  
                    }
                    
		   #if defined (__WXGTK__)
		                   
		   if(sz.GetHeight()<25){
		    	size=wxSize(sz.GetWidth(),25);
		     	this->SetSize(size);
           }
                    
		   #endif
                    
		    if(alignment ==2){
		            tc2 =  new wxRTextCtrl(this,tc2_id,_T(""),wxPoint(0,0),wxSize(size.GetWidth()-17,size.GetHeight()),wxTE_PROCESS_ENTER|wxTE_LEFT,
                    wxDefaultValidator,_T("TXTCTRL_LEFT"));
                    
                    tc =  new wxSTextCtrl(this,tc_id,_T(""),(wxRTextCtrl*)tc2,
		            wxPoint(0,0),wxSize(size.GetWidth()-17,size.GetHeight()),wxTE_PROCESS_ENTER|wxTE_RIGHT,
                    wxDefaultValidator,_T("TXTCTRL_RIGHT"));
            }
            else{
                 if(alignment == 1){
                     tc =  new wxTextCtrl(this,tc_id,_T(""),
		             wxPoint(0,0),wxSize(size.GetWidth()-17,size.GetHeight()),wxTE_PROCESS_ENTER|wxTE_RIGHT,
                     wxDefaultValidator,_T("TXTCTRL_RIGHT"));
                     
                     tc2_id = tc_id;
                     tc2=tc;
                 }
                 else{
                     tc =  new wxTextCtrl(this,tc_id,_T(""),
		             wxPoint(0,0),wxSize(size.GetWidth()-17,size.GetHeight()),wxTE_PROCESS_ENTER|wxTE_LEFT,
                     wxDefaultValidator,_T("TXTCTRL_LEFT"));
                     
                     tc2_id = tc_id;
                     tc2=tc;
                 }
            }
		    db = new wxDragButton( this,id_drag,wxPoint(size.GetWidth()-17,0),wxSize(17,size.GetHeight()),
		                 style,	_T("uno"));
                    
                    db->SetRange(min,max);
                    db->SetValue(initial);
                    db->SetPrecision(precision);
                    
                  
                    // Create sizers
                    all = new wxBoxSizer(wxHORIZONTAL); 
		            box = new wxBoxSizer(wxHORIZONTAL); 
                    box->Add(tc);
		            all->Add(box);
                    all->Add(db);
                    SetSizer(all);
                    
		    // It seems that there's a bug when using wxDRAG_HORIZONTAL flag in dragbutton
                    // this line fixes it
                    this->SetSize(size);
                    db->SetSize(wxSize(17,this->GetSize().GetHeight()));
                    
                    
		                                                     
                    
		    // Make connections
                   
                    this->Connect(tc2_id,wxEVT_COMMAND_TEXT_ENTER,
                    (wxObjectEventFunction) &wxFDragSpinCtrl::OnEnter);
                   
                    this->Connect(db->GetId(),wxEVT_DRAGBUTTON_DRAG ,
                    (wxObjectEventFunction) &wxFDragSpinCtrl::OnDrag );
                    this->Connect(db->GetId(),wxEVT_DRAGBUTTON_DOWN ,
                    (wxObjectEventFunction) &wxFDragSpinCtrl::OnDown );
                    this->Connect(db->GetId(),wxEVT_DRAGBUTTON_UP ,
                    (wxObjectEventFunction) &wxFDragSpinCtrl::OnUp );
                    this->Connect(db->GetId(),wxEVT_DRAGBUTTON_DCLICK ,
                    (wxObjectEventFunction) &wxFDragSpinCtrl::OnDClick ); 
                    
                    tc2->Connect(tc2_id,wxEVT_KEY_UP ,
                        (wxObjectEventFunction) &wxFDragSpinCtrl::OnKUp );
                    tc2->Connect(tc2_id,wxEVT_KEY_DOWN ,
                        (wxObjectEventFunction) &wxFDragSpinCtrl::OnKDown );
                    
                    SetValue(initial);

		    wxCommandEvent enter_event(wxEVT_COMMAND_TEXT_ENTER ,this->GetId());
		    wxPostEvent(this->GetParent(),enter_event);
                  
                  
                 
}
                
wxFDragSpinCtrl::~wxFDragSpinCtrl(void){
  
}


void wxFDragSpinCtrl::SetUnit (const wxString& u){
    
    unit = u;
}


void wxFDragSpinCtrl::SetLabel (const wxString& v){
    wxString sout;
    int error;
    
    if((error = parser->Parse(v,sout,unit))!=wxSUP_NO_ERROR){
         SendParseError(error);
    }
    else{
        if( value.Cmp(sout.c_str())!=0){
            SSetValue(tc,tc2,sout);
            value = sout;
            SendTextChangedEvent();
        }
    }
}

void wxFDragSpinCtrl::SetValue (double val){
     cout<<"SetValue "<<val<<endl;
     wxDragButtonEvent e2(wxEVT_DRAGBUTTON_DRAG,GetId());
     db->SetValue(val);
     OnDrag(e2); 
}

void  wxFDragSpinCtrl::SetSCUP(wxSciUnitParser* scup){
    parser = scup;
}

void wxFDragSpinCtrl::SetDCPrecision (int precision){
    db->SetPrecision(precision);
}

void wxFDragSpinCtrl::SetDCAcceleration (double a){
    db->SetAcceleration(a);
}

void wxFDragSpinCtrl::SetDCLogarithmic (bool b){
    db->SetLogarithmic(b);
}

void wxFDragSpinCtrl::SetDCRange (double a,double b){
    db->SetRange(a,b);
}


wxString wxFDragSpinCtrl::GetUnit (void) const {
 
    return unit;
}

float wxFDragSpinCtrl::GetIncrement (void) const {
    
    return -1;
}

wxString wxFDragSpinCtrl::GetLabel (void) const {
    
    return value;
}

double wxFDragSpinCtrl::GetValue (void)  {
   unsigned int i,error;
   double res = 0;
   wxString s, sout;
    
    
    if(parser!=NULL){
         s= tc->GetLineText(0);
        
        
        if((error = parser->Parse(s,sout,unit))!=wxSUP_NO_ERROR){
             SendParseError(error);
             return res;
        }
        else{ 
            
            
            for(i=0;((sout.GetChar(i)<58 && sout.GetChar(i)>47)||
            sout.GetChar(i)==44 ||sout.GetChar(i)==46 ||
            sout.GetChar(i)==45) && i<sout.Len();++i){}
            
            if(! (sout.Mid(0,i).ToDouble(&res))){
              withcomma2(sout).Mid(0,i).ToDouble(&res);
            }
    }
 }
    return  res;
}

wxSciUnitParser* wxFDragSpinCtrl::GetSCUP (void) const {
    return parser;   
}

int wxFDragSpinCtrl::GetDCPrecision (void) const {
    return db->GetPrecision();  
}
bool wxFDragSpinCtrl::GetDCLogarithmic (void) const {
    return db->IsLogarithmic();  
}
double wxFDragSpinCtrl::GetMax (void) const {
    return db->GetMax();  
}
double wxFDragSpinCtrl::GetMin (void) const {
    return db->GetMin();  
}




void wxFDragSpinCtrl::OnEnter(wxCommandEvent& WXUNUSED(event)){
    wxString s, sout;
    int error;
    unsigned int i;
    wxString value,n_unit;
    //cout<<"Enter"<<endl;
    //printf("Enter\n");
    if(parser != NULL){   
        s= tc2->GetLineText(0);
             
        
        if((error = parser->Parse(s,sout,unit))!=wxSUP_NO_ERROR){
             SendParseError(error);
        }
        else{
             
            SSetValue(tc,tc2,sout);
            db->SetValue(GetValue());
            
            for(i=0;i<sout.Length() && !is_letter(sout.GetChar(i));i++);
            
            value=   db->IsLogarithmic()? db->ToStringE(db->GetValue(),db->GetPrecision())
                         :db->ToString(db->GetValue(),db->GetPrecision());
            
            n_unit = sout.Mid(i);
            
             
            (parser->IsPropEnabled(wxSUP_USE_RICH_OUTPUT))? value<<_T(" ")<<n_unit
                                                         :value<<n_unit;     

            SSetValue(tc,tc2,value);                                                                 
                         
            SendTextChangedEvent();
            
        }
    }
    
    
}


void wxFDragSpinCtrl::OnDrag(wxDragButtonEvent& e){
          
    wxString increment;
    double antvalue,value;
   
    antvalue = GetValue();
     
    if(GetDCLogarithmic()){
      
           increment = db->ToStringE(db->GetValue(),db->GetPrecision());
    }
    else{
           increment = db->ToString(db->GetValue(),db->GetPrecision());  
    }
    
    increment+=unit;
    
    SSetValue(tc,tc2, increment);
    value= GetValue();
    cout<<"SetValue en OnDrag "<<value<<endl;   
    db->SetValue(value);
        
    SendTextChangedEvent();
    
}


void wxFDragSpinCtrl::OnDown(wxDragButtonEvent& e){
	pushed = true;
	tc->SetFocus();
	e.Skip();
}
void wxFDragSpinCtrl::OnUp (wxDragButtonEvent& e){
	pushed = false;
	e.Skip();
}

void wxFDragSpinCtrl::OnPrecision (wxDragButtonEvent& e){
	//cout<<"Precision cambiada"<<endl;
	//printf("precisioncambiada\n");
	e.Skip();
}

 void  wxFDragSpinCtrl::OnKDown(wxKeyEvent& e){
      wxDragButtonEvent e2(wxEVT_DRAGBUTTON_DRAG,((wxFDragSpinCtrl*)this->GetParent())->GetId());
      wxString value;
      unsigned int cursor=0;
      long from , to;
      e.Skip();
      if(e.GetKeyCode()==WXK_SHIFT){
                   shift = true;
      }
      else{
		
		
        	if(((wxFDragSpinCtrl*)this->GetParent())->DBpushed()){ 
        		switch(e.GetKeyCode()){
        				case WXK_LEFT:case WXK_DOWN:
        					
        					((wxFDragSpinCtrl*)this->GetParent())->db->SetPrecision(((wxFDragSpinCtrl*)this->GetParent())->db->GetPrecision()-1);
        					((wxFDragSpinCtrl*)this->GetParent())->OnDrag(e2);
        				break;
        				case WXK_RIGHT:case WXK_UP:
        					
        					((wxFDragSpinCtrl*)this->GetParent())->db->SetPrecision(((wxFDragSpinCtrl*)this->GetParent())->db->GetPrecision()+1);
        					((wxFDragSpinCtrl*)this->GetParent())->OnDrag(e2);
        				break;
        	            
        		}
                }
	}
     
}


void   wxFDragSpinCtrl::OnKUp(wxKeyEvent& e){
        switch(e.GetKeyCode()){
               case WXK_SHIFT:
                    shift = false;
               break;
        }
}    


 void  wxFDragSpinCtrl::OnDClick (wxDragButtonEvent&){
    wxDragButtonEvent e2(wxEVT_DRAGBUTTON_DRAG,this->GetId());
   
       if(!db->IsLogarithmic()){
           db->SetLogarithmic();                    
       }
       else{
            db->SetLogarithmic(false);
       }
       OnDrag(e2);
}



bool  wxFDragSpinCtrl:: DBpushed(){
	return pushed;
} 


bool  wxFDragSpinCtrl::Enable(bool enable) {
        isenabled = enable;
        this->tc->Enable(enable);
	this->tc2->Enable(enable);
	this->db->Enable(enable);
	return true;
}

void  wxFDragSpinCtrl::Disable(){
	Enable(false);
}

wxSize  wxFDragSpinCtrl::DoGetBestSize() const{
	return    wxSize(GetSize().GetWidth() ,GetSize().GetHeight() );
}


void wxFDragSpinCtrl::SendTextChangedEvent(void){
 wxFDSCEvent text_event(wxEVT_FDSC_TEXT_CHANGED,this->GetId());
 text_event.SetDouble(this->GetValue());
 text_event.SetString(this->GetUnit());
 wxPostEvent(this->GetParent(),text_event);   
    
}

void wxFDragSpinCtrl::SendParseError (int i){
 wxFDSCEvent error_event(wxEVT_FDSC_ERROR,this->GetId());   
 error_event.SetInt(i);
 wxPostEvent(this->GetParent(),error_event);   
}



     
