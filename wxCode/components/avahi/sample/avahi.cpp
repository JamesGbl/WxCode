#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/hashmap.h>

#include "wx/avahi/client.h"
#include "wx/avahi/pool.h"

#include "wx/avahi/resolve.h"
#include "wx/avahi/browse.h"

class MyApp : public wxApp
{
private:
    wxAvahiThreadPool pool;
    wxAvahiClient client;    
public:
    virtual bool OnInit();
    wxAvahiClient* GetAvahiClient(){return &client;}
};

DECLARE_APP(MyApp)

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
protected:
    wxTreeCtrl *m_tree;
    wxTreeItemId m_rootItem;
    
    void onTreeItemSelected(wxTreeEvent& event);
    
    DECLARE_EVENT_TABLE()
};


class MyTreeItemData : public wxTreeItemData{
public:
    AvahiIfIndex m_interface;
    AvahiProtocol m_protocol;
    wxString m_type;
    wxString m_name;
    wxString m_domain;

    MyTreeItemData(AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain):
    wxTreeItemData(),
    m_interface(interface),
    m_protocol(protocol),
    m_type(type),
    m_name(name),
    m_domain(domain)
    {
    }
};


WX_DECLARE_STRING_HASH_MAP(wxTreeItemId, wxTreeItemIdMap);
class MyServiceBrowser : public wxAvahiServiceBrowser{
public:
    MyServiceBrowser(const wxString &type, wxTreeCtrl *tree, wxTreeItemId rootItem):
    wxAvahiServiceBrowser(wxGetApp().GetAvahiClient(), 
       AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, type,
       wxT(""), (AvahiLookupFlags) 0),
    m_tree(tree)
    {
        m_parentItem = tree->AppendItem(rootItem, type);
    }
private:
    wxTreeCtrl *m_tree;
    wxTreeItemId m_parentItem;
    wxTreeItemIdMap m_items;
protected:
    DECLARE_EVENT_TABLE()
    void onNewService(wxAvahiServiceBrowserEvent& event){
        wxTreeItemId item = m_tree->AppendItem(m_parentItem, event.GetName(), -1, -1, (wxTreeItemData*)new MyTreeItemData(event.GetInterface(), event.GetProtocol(), event.GetName(), event.GetType(), event.GetDomain()));
        m_items[event.GetName()] = item;
    }
    void onRemService(wxAvahiServiceBrowserEvent& event){
        wxTreeItemId item = m_items[event.GetName()];
        if(item){
            m_tree->Delete(item);
            m_items.erase(event.GetName());
        }
    }
};

class MyServiceResolver : public wxAvahiServiceResolver{
public:
    MyServiceResolver(wxAvahiClient* client, AvahiIfIndex interface, AvahiProtocol protocol, const wxString &name, const wxString &type, const wxString &domain):
    wxAvahiServiceResolver(client, interface, protocol, name, type, domain, AVAHI_PROTO_UNSPEC, (AvahiLookupFlags)0)
    {
    }
protected:
    DECLARE_EVENT_TABLE()
    void onResolve(wxAvahiServiceResolverEvent& event){
        wxLogStatus(wxT("%s:%d - %s"), event.GetHostName().GetData(), event.GetPort(), event.GetStringList().ListToString().GetData()); 
        delete this;
    }
    void onFailure(wxAvahiServiceResolverEvent& event){
        wxMessageBox(wxT("Failed !"));
    }
};

class MyServiceTypeBrowser : public wxAvahiServiceTypeBrowser{
public:
    wxTreeCtrl *m_tree;
    wxTreeItemId m_rootItem;
    MyServiceTypeBrowser(wxTreeCtrl *tree, wxTreeItemId rootItem):
    wxAvahiServiceTypeBrowser(wxGetApp().GetAvahiClient(),
        AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC,wxT(""), (AvahiLookupFlags) 0),
    m_tree(tree),
    m_rootItem(rootItem)
    {
    }
protected:
    DECLARE_EVENT_TABLE()
    void OnNewServiceType(wxAvahiServiceTypeBrowserEvent& event)
    {
        new MyServiceBrowser(event.GetType(), m_tree, m_rootItem);
    }    
};


BEGIN_EVENT_TABLE(MyServiceTypeBrowser, wxAvahiServiceTypeBrowser)
    EVT_AVAHI_SERVICE_TYPE_BROWSER_NEW(MyServiceTypeBrowser::OnNewServiceType)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyServiceBrowser, wxAvahiServiceBrowser)
    EVT_AVAHI_SERVICE_BROWSER_NEW(MyServiceBrowser::onNewService)
    EVT_AVAHI_SERVICE_BROWSER_REMOVE(MyServiceBrowser::onRemService)
END_EVENT_TABLE()

BEGIN_EVENT_TABLE(MyServiceResolver, wxAvahiServiceResolver)
    EVT_AVAHI_SERVICE_RESOLVER_FOUND(MyServiceResolver::onResolve)
    EVT_AVAHI_SERVICE_RESOLVER_FAILURE(MyServiceResolver::onFailure)
END_EVENT_TABLE()


enum
{
    Avahi_File_About = wxID_ABOUT,
    Avahi_File_Quit = wxID_EXIT,
};

IMPLEMENT_APP(MyApp)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Avahi_File_Quit,  MyFrame::OnQuit)
    EVT_MENU(Avahi_File_About, MyFrame::OnAbout)
    EVT_TREE_SEL_CHANGED(wxID_ANY, MyFrame::onTreeItemSelected)
END_EVENT_TABLE()


bool MyApp::OnInit(){
    pool.Start();
    client.Create(&pool, AVAHI_CLIENT_NO_FAIL, NULL);
    
    
    MyFrame *frame = new MyFrame(_T("wxAvahi sample"));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
       : wxFrame((wxFrame *)NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(Avahi_File_About, _T("&About...\tCtrl-A"), _T("Show about dialog"));
    menuFile->AppendSeparator();
    menuFile->Append(Avahi_File_Quit, _T("E&xit\tAlt-X"), _T("Quit this program"));
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, _T("&File"));    
    SetMenuBar(menuBar);

    m_tree = new wxTreeCtrl(this, wxID_ANY);
    m_rootItem = m_tree->AddRoot(wxT("Avahi discovery"));

    CreateStatusBar(1);
    SetStatusText(_T("Welcome to wxWidgets!"));

    new MyServiceTypeBrowser(m_tree, m_rootItem);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("wxAvahi sample\n(c) 2006-2007 Emilien KIA"),
                 _T("About wxAvahi"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::onTreeItemSelected(wxTreeEvent& event){
    wxTreeItemId item = event.GetItem();
    MyTreeItemData* data = (MyTreeItemData*) m_tree->GetItemData(item);
    if(data!=NULL){
        new MyServiceResolver(wxGetApp().GetAvahiClient(), data->m_interface, data->m_protocol, data->m_name, data->m_type, data->m_domain);
    }
}
