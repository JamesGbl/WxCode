#include <wx/wxprec.h>	
#ifndef WX_PRECOMP	
#include <wx/wx.h>	
#endif  
					
#include <wx/app.h> 
#include <wx/frame.h>   
#include <wx/window.h>  
  
#define STYLE wxCAPTION | wxCLOSE_BOX | wxSYSTEM_MENU | wxRESIZE_BORDER			
	
class MyApp : public wxApp						
{	
	public:  
		virtual bool OnInit(); 
		int OnExit(); 
}; 
                                                           
class MyFrame: public wxFrame            
{ 
	public: 
		MyFrame(wxWindow *parent, 
        wxWindowID id = 1,	
        const wxString& title = wxT("TextEditor"),	
        const wxPoint& pos =wxDefaultPosition,	
        const wxSize& size = wxDefaultSize,	
        long style = STYLE);	
		~MyFrame();	
    enum	
    {	
       STC_ID = 1000  
    };  
	private:    
		void OnQuit(wxCloseEvent& event);                    
		DECLARE_EVENT_TABLE()              
};              
																			