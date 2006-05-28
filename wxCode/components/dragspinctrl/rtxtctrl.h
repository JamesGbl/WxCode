#ifndef _ejjejejeeje_
#define _ejjejejeeje_
#include "wx/wx.h"
#include <stdio.h>

class wxRTextCtrl:  public  wxTextCtrl
{
		    
private:
       wxTextCtrl* textright;
       wxSizer* mysizer;
public:
	
	wxRTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value , 
					const wxPoint& pos , 
					const wxSize& size , long style, 
					const wxValidator& validator, const wxString& name)
					:wxTextCtrl(parent,id,value,pos,size,style|wxTE_LEFT,validator,name)
	{
		textright = NULL;
		this->Connect(id,wxEVT_SET_FOCUS ,
			(wxObjectEventFunction) &wxRTextCtrl::OnGetFocus );
		this->Connect(id,wxEVT_KILL_FOCUS ,
			(wxObjectEventFunction) &wxRTextCtrl::OnLooseFocus );
	
	}
	
	~wxRTextCtrl(){}
	
	void Hide(wxTextCtrl* t){
	textright = t;
	Show(false);          
	}
	
	void DarSizer(wxSizer* s){
		mysizer=s;
	}
	
	void OnGetFocus(wxFocusEvent& e){
		//SetSelection(0,0);
		SetSelection(-1,-1);
		cout<<"***"<<this->GetParent()->GetName()<<endl;
		this->GetParent()->SetFocus();
	}
	
	void OnLooseFocus(wxFocusEvent& e){
	
		if(textright!=NULL && mysizer!=NULL){
			wxSizerItem * i;
			i = mysizer->GetItem(this,true);
			if(i!=NULL){
		    	i->SetWindow(textright);
		     	mysizer->Layout();
		       	Show(false);
		        textright->Show(true);
          }
		}  
		else{
		     //	cout<<textright<<mysizer<<endl;
		     // ERROR then
		} 
	}

};







#endif
