
#ifndef _jejeja_
#define _jejeja_

#include "wx/wx.h"
#include "rtxtctrl.h"
#include <stdio.h>

class wxSTextCtrl:  public  wxTextCtrl
{

private:
       
       wxRTextCtrl* textleft;


public:
	wxSTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value , wxRTextCtrl* r,
					const wxPoint& pos , 
					const wxSize& size , long style, 
					const wxValidator& validator, const wxString& name)
					:wxTextCtrl(parent,id,value,pos,size,style|wxTE_RIGHT,validator,name)
	{
		
		textleft=r;
		r->Hide(this);
				
		this->Connect(id,wxEVT_SET_FOCUS ,
			(wxObjectEventFunction) &wxSTextCtrl::OnGetFocus );
		this->Connect(id,wxEVT_KILL_FOCUS ,
			(wxObjectEventFunction) &wxSTextCtrl::OnLooseFocus );
	
	}
	
	~wxSTextCtrl(){}
	
	void OnGetFocus(wxFocusEvent& e){
		if(textleft!=NULL){
			SetSelection(0,0); 
			wxSizer * s;
			s = GetContainingSizer();
			if(s!=NULL){
    			textleft->DarSizer(s);
    			wxSizerItem * i;
    			i = s->GetItem(this,true);
    			if(i!=NULL) 
                  i->SetWindow(textleft);
    			s->Layout();
    			Show(false);
    			textleft->Show();
    			textleft->SetFocus();
            }
		}
	}
	
	void OnLooseFocus(wxFocusEvent& e){
		SetSelection(0,0);           
	}
	
	
};

#endif

