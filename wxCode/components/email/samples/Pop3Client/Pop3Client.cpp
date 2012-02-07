/****************************************************************************

 Project     : wxSMTP
 Author      : Brice André
 Description : Sample illustrating the usage of wxSMTP library

 VERSION INFORMATION:
 File    : $Source$
 Version : $Revision$
 Date    : $Date$
 Author  : $Author$
 Licence : wxWidgets licence

 History:

****************************************************************************/

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// includes
#ifndef WX_PRECOMP
   // here goes the #include <wx/abc.h> directives for those
   // files which are not included by wxprec.h

#endif

#include "wx/wx.h"
#include "wx/busyinfo.h"
#include "wx/progdlg.h"
#include "wx/listctrl.h"
#include "wx/html/htmlwin.h"
#include "wx/protocol/http.h"
#include "wx/mimetype.h"
#include "wx/imaglist.h"

#include "wx/pop3/wxpop3.h"

class App;

class DownloadHeadersListener : public wxPOP3::Listener
{
   public:

      DownloadHeadersListener(App& app) : app(app) {}

   private:

      void OnOperationFinished(Status_t status);

      void OnFoundMessage(const wxEmailMessage::MessageId& message_id,
                          unsigned long current,
                          unsigned long total,
                          ExtractionMode_t& extraction_mode,
                          bool& shall_delete,
                          bool& shall_stop);

      void OnMessageContent(wxEmailMessage* message,
                            bool& shall_stop);

      App& app;
};

class DownloadMessageListener : public wxPOP3::Listener
{
   public:
      DownloadMessageListener(App& app) : app(app) {}

      void SetMessageId(const wxEmailMessage::MessageId& message_id) {this->message_id = message_id;}

   private:

      void OnOperationFinished(Status_t status);

      void OnFoundMessage(const wxEmailMessage::MessageId& message_id,
                          unsigned long current,
                          unsigned long total,
                          ExtractionMode_t& extraction_mode,
                          bool& shall_delete,
                          bool& shall_stop);

      void OnMessageContent(wxEmailMessage* message,
                            bool& shall_stop);

      wxEmailMessage::MessageId message_id;
      App& app;
};

class MessagesList : public wxFrame
{
   public:

      MessagesList(App& app);

   private:
      App& app;

      wxListCtrl* messages_list;

      void OnItemSelected(wxListEvent& event);
      void OnItemRightClick(wxListEvent& event);

   DECLARE_EVENT_TABLE()
};

class App : public wxApp
{
   public:
      App();
      ~App();

      bool OnInit();

      static wxString RequestUser(const wxString& message, bool is_hidden);

      void DisplayMessages();
      void DownloadMessage(const wxEmailMessage::MessageId& message_id);

      wxPOP3*        pop3;
      DownloadHeadersListener download_headers_listener;
      DownloadMessageListener download_message_listener;

      typedef struct
      {
         wxEmailMessage::MessageId message_id;
         wxString                      from;
         wxString                      subject;
         wxDateTime                    date;
         unsigned long                 size;
      } MessageHeader_t;
      std::vector<MessageHeader_t> message_headers;

      wxProgressDialog* progress_dialog;
      wxBusyInfo*       busy_info;
      MessagesList*     message_list;

      bool AskUser(const wxString& message);

   DECLARE_NO_COPY_CLASS(App)
};

class AttachmentsList : public wxListCtrl
{
   public:
      AttachmentsList(wxWindow* parent, wxEmailMessage& message);
   private:
      wxEmailMessage& message;
      wxImageList image_list;

      void OnElementSelected(wxListEvent& event);

   DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(AttachmentsList, wxListCtrl)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, AttachmentsList::OnElementSelected)
END_EVENT_TABLE()

class MessageDisplay : public wxFrame
{
   public:
      MessageDisplay(App& app, wxEmailMessage* message);
      ~MessageDisplay() {delete message;}
   private:
      App& app;
      wxEmailMessage* message;
};

IMPLEMENT_APP(App)

App::App()
    :download_headers_listener(*this),
     download_message_listener(*this)
{
   pop3 = NULL;
   progress_dialog = NULL;
   busy_info = NULL;
   message_list = NULL;
}

App::~App()
{
   delete pop3;
}

bool App::OnInit()
{
   /* Initialise image handler */
   wxInitAllImageHandlers();

   /* Instanciate the pop3 client */
   pop3 = new wxPOP3(RequestUser(_T("Please enter the user name of the pop3 account"), false),
                     RequestUser(_T("Please enter the password of the pop3 account"), true),
                     RequestUser(_T("Please enter the pop3 server address"), false),
                     atoi(RequestUser(_T("Please enter the server port number (default 110)"), false).c_str()),
                     AskUser("Do you want to use the APOP authentication method ?")?wxPOP3::APopAuthenticationMethod:wxPOP3::UserPassAuthenticationMethod,
                     AskUser("Does your server requests a SSL/TLS connection ?"));

   /* Start a download operation */
   bool result = pop3->DownloadMessages(&download_headers_listener);
   if (!result)
   {
      wxMessageDialog dlg(NULL,
                          _T("The download messages operation could not be initiated"),
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
   }

   /* No need to wait if operation could not be performed */
   return result;
}


void App::DisplayMessages()
{
   message_list = new MessagesList(*this);
}

void App::DownloadMessage(const wxEmailMessage::MessageId& message_id)
{
   if (pop3->OperationInProgress())
   {
      wxMessageDialog dlg(NULL,
                          _T("An operation is in progress. Please wait termination and restart new operation after."),
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
   }
   else
   {
      /* Set busy info */
      busy_info = new wxBusyInfo(_T("Pease wait while the content of the message is downloaded"), message_list);

      /* Initiate download */
      download_message_listener.SetMessageId(message_id);
      pop3->DownloadMessages(&download_message_listener);
   }
}

wxString App::RequestUser(const wxString& message, bool is_hidden)
{
   long style = wxOK;
   if (is_hidden)
   {
      style |= wxTE_PASSWORD;
   }

   wxTextEntryDialog dlg(NULL,
                         message,
                         _T("wxPOP3 Client"),
                         _T(""),
                         style);
   dlg.ShowModal();
   return dlg.GetValue();
}
void DownloadHeadersListener::OnOperationFinished(Status_t status)
{
   /* Suppress the progress dialog */
   if (app.progress_dialog != NULL)
   {
      app.progress_dialog->Destroy();
      app.progress_dialog = NULL;
   }

   /* Check if an error occurred */
   if (status != Succeed)
   {
      wxString message = _T("An error occurred during server dialog : ");
      if (status == Abort)
          message << _T("Abort");
      else if (status==Timeout)
         message << _T("Timeout");
      else if (status==Error)
         message << _T("Error");
      else if (status==InvalidUserPass)
         message << _T("InvalidUserPass");
      else
         message << _T("Unknown");

      wxMessageDialog dlg(NULL,
                          message,
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
      exit(0);
   }

   /* Check if there are mails on the server */
   if (app.message_headers.size() <= 0)
   {
      wxMessageDialog dlg(NULL,
                          _T("The server does not contain any e-mail"),
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
      exit(0);
   }

   /* Request main frame to display messages then... */
   app.DisplayMessages();
}

void DownloadHeadersListener::OnFoundMessage(const wxEmailMessage::MessageId& WXUNUSED(message_id),
                                    unsigned long current,
                                    unsigned long total,
                                    ExtractionMode_t& extraction_mode,
                                    bool& shall_delete,
                                    bool& shall_stop)
{
   /* Update flags */
   extraction_mode = ExtractHeader;
   shall_delete = false;
   shall_stop = false;

   /* Check if we shall create progress dialog */
   if (app.progress_dialog == NULL)
   {
      app.progress_dialog = new wxProgressDialog(_T("POP3 Client : retrieving messages list"),
                                                 _T("Please wait while the messages are retrieved from the server"),
                                                 total);
   }

   /* Update the progress dialog status */
   app.progress_dialog->Update(current-1);
}

void DownloadHeadersListener::OnMessageContent(wxEmailMessage* message,
                                      bool& shall_stop)
{
   /* Set flags */
   shall_stop = false;

   /* Prepare the new messages header entry */
   App::MessageHeader_t message_header;
   message_header.message_id = message->GetMessageId();
   message_header.from = message->GetFrom().GetName() + _T(" <") + message->GetFrom().GetAddress() + _T(">");
   message_header.subject = message->GetSubject();
   message_header.date = message->GetDateTime();
   message_header.size = message->GetSize();

   /* Insert it in list */
   app.message_headers.push_back(message_header);

   /* Delete message */
   delete message;
}

void DownloadMessageListener::OnOperationFinished(Status_t status)
{
   /* Suppress busy info */
   if (app.busy_info != NULL)
   {
      delete app.busy_info;
      app.busy_info = NULL;
   }

   /* Check if an error occurred */
   if ((status != Succeed) &&
       (status != Abort))
   {
      wxString message = _T("An error occurred during server dialog : ");
      if (status == Timeout)
         message << _T("Timeout");
      else if (status==Error)
         message << _T("Error");
      else if (status==InvalidUserPass)
         message << _T("InvalidUserPass");
      else
         message << _T("Unknown");

      wxMessageDialog dlg(NULL,
                          message,
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
   }
}

void DownloadMessageListener::OnFoundMessage(const wxEmailMessage::MessageId& message_id,
                                             unsigned long WXUNUSED(current),
                                             unsigned long WXUNUSED(total),
                                             ExtractionMode_t& extraction_mode,
                                             bool& shall_delete,
                                             bool& shall_stop)
{
   if (message_id.is_unique &&
       (message_id.id == this->message_id.id))
   {
      extraction_mode = ExtractFullMessage;
   }
   else
   {
      extraction_mode = NoExtraction;
   }

   shall_delete = false;
   shall_stop = false;
}

void DownloadMessageListener::OnMessageContent(wxEmailMessage* message,
                                               bool& shall_stop)
{
   /* Suppress busy info */
   if (app.busy_info != NULL)
   {
      delete app.busy_info;
      app.busy_info = NULL;
   }

   /* Display the message dialog */
   new MessageDisplay(app, message);

   /* Just to don't list complete messages headers... */
   shall_stop = true;
}

BEGIN_EVENT_TABLE(MessagesList, wxFrame)
      EVT_LIST_ITEM_ACTIVATED(wxID_ANY, MessagesList::OnItemSelected)
      EVT_LIST_ITEM_RIGHT_CLICK(wxID_ANY, MessagesList::OnItemRightClick)
END_EVENT_TABLE()

MessagesList::MessagesList(App& app)
             :wxFrame(NULL, wxID_ANY, _T("POP3 Client")),
              app(app)
{
   /* Format the frame */
   wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
   messages_list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
   sizer->Add(messages_list, 1, wxALL|wxEXPAND, 0);
   SetSizer(sizer);

   /* Format columns */
   messages_list->InsertColumn(0, _T("From"));
   messages_list->InsertColumn(1, _T("Subject"));
   messages_list->InsertColumn(2, _T("Date"));
   messages_list->InsertColumn(3, _T("Size"));

   /* Insert all messages found */
   std::vector<App::MessageHeader_t>::const_iterator it;
   long index = 0;
   for (it = app.message_headers.begin(); it != app.message_headers.end(); it++)
   {
      messages_list->InsertItem(index, it->from);
      messages_list->SetItem(index, 1, it->subject);
      messages_list->SetItem(index, 2, it->date.FormatDate() << _T(" ") << it->date.FormatTime());
      messages_list->SetItem(index, 3, wxString::Format(_T("%lu"), it->size));
      index++;
   }

   /* Show the frame */
   Show(true);
}

void MessagesList::OnItemSelected(wxListEvent& event)
{
   /* Get element id */
   unsigned long index = event.GetIndex();

   /* Download the message content */
   wxEmailMessage::MessageId& message_id = app.message_headers.at(index).message_id;
   if (message_id.is_unique)
   {
      app.DownloadMessage(message_id);
   }
   else
   {
      wxMessageDialog dlg(NULL,
                          wxString(_T("Your POP3 server does not support persistent Ids and this sample does not allow downloading messages with non-persistent ids.\n\n")) +
                          _T("Note that you can use this library to communicate with servers that do not support persistent IDs, but you have to handle your own mechanism to uniquely identify messages between POP3 sessions."),
                          _T("wxPOP3 Client"),
                          wxOK);
      dlg.ShowModal();
   }
}

void MessagesList::OnItemRightClick(wxListEvent& WXUNUSED(event))
{
   //TODO...
   //We can propose to suppress it...
}

class HtmlWindow : public wxHtmlWindow
{
   public:

      HtmlWindow(wxWindow* parent, const wxEmailMessage& message)
         :wxHtmlWindow(parent), message(message) {}

      wxHtmlOpeningStatus OnOpeningURL(wxHtmlURLType type,const wxString& url, wxString *redirect) const
      {
         /* Check that we are trying to open an image */
         if (type != wxHTML_URL_IMAGE)
         {
            /* In this case, launch default browser */
            wxLaunchDefaultBrowser(url);
            return wxHTML_BLOCK;
         }

         /* Check that we are performing a download from internet */
         if (url.StartsWith(_T("http")))
         {
            /* Download target to local file. Note that this will block GUI... */
            wxString server = url.AfterFirst('/').AfterFirst('/').BeforeFirst('/');
            wxString page = _T("/") + url.AfterFirst('/').AfterFirst('/').AfterFirst('/').BeforeFirst('?');
            wxString arguments = url.AfterFirst('/').AfterFirst('/').AfterFirst('/').AfterFirst('?');

            /* Initialise Http connection */
            wxHTTP http;
            wxIPV4address address;
            address.Hostname(server);
            //TODO : we do not support url of type http://server.com:xxxx/image.png
            address.Service(80);

            /* set arguments, if any */
            if (arguments.Length() > 0)
            {
               http.SetHeader(_T("Content-type"),_T("application/x-www-form-urlencoded"));
               http.SetPostBuffer(arguments);
            }

            /* Perform connection */
            http.Connect(address, true);

            /* Try reading the response of server */
            wxInputStream* in = http.GetInputStream(page);
            if (in)
            {
               /* Initialise temporary file */
               wxString temp_file_path = wxFileName::CreateTempFileName(url.AfterLast('/'));

               /* Flush result in temp file */
               wxFileOutputStream stream(temp_file_path);
               in->Read(stream);
               delete in;

               if (http.GetResponse() == 200)
               {
                  /* Redirect to temp file created */
                  *redirect = temp_file_path;
                  return wxHTML_REDIRECT;
               }
               else
               {
                  /* What can we do ? */
                  return wxHTML_BLOCK;
               }
            }
            else
            {
               /* What can we do ? */
               return wxHTML_BLOCK;
            }
         }
         else if (url.StartsWith(_T("cid:")))
         {
            /* Internal mail reference */
            unsigned long id = 0;
            std::vector<wxString>::const_iterator it;
            for (it = message.GetAttachmentsMimeIds().begin(); it != message.GetAttachmentsMimeIds().end(); it++)
            {
               if (*it == url.AfterFirst(':'))
               {
                  /* Extract to temp file */
                  wxFileName temp_file = wxFileName::CreateTempFileName(*it);
                  message.ExtractAttachment(temp_file, id);
                  *redirect = temp_file.GetFullName();
                  return wxHTML_REDIRECT;
               }
               id++;
            }

            /* If we reach this point, it means that we did not found entry */
            return wxHTML_BLOCK;
         }

         /* Default behaviour is open */
         return wxHTML_OPEN;
      }

   private:

      void OnLinkClicked(wxHtmlLinkEvent& e)
      {
         wxLaunchDefaultBrowser(e.GetLinkInfo().GetHref());
      }

      const wxEmailMessage& message;

      DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(HtmlWindow, wxHtmlWindow)
EVT_HTML_LINK_CLICKED(wxID_ANY, HtmlWindow::OnLinkClicked)
END_EVENT_TABLE()

MessageDisplay::MessageDisplay(App& app, wxEmailMessage* message)
               :wxFrame(NULL, wxID_ANY, wxString(_T("POP3 Client : ")) + message->GetSubject()),
                app(app),
                message(message)
{
   /* Format the frame */
   wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

   /* check if message has html content */
   if (message->HasHtmlBody())
   {
      HtmlWindow* window = new HtmlWindow(this, *message);
      sizer->Add(window, 1, wxALL|wxEXPAND, 0);

      window->SetPage(message->GetHtmlBody());
   }
   else
   {

      wxTextCtrl* ctrl = new wxTextCtrl(this,
                                        wxID_ANY,
                                        message->GetBody(),
                                        wxDefaultPosition,
                                        wxDefaultSize,
                                        wxTE_MULTILINE|wxTE_READONLY);
      sizer->Add(ctrl, 1, wxALL|wxEXPAND, 0);
   }

   /* Append attachment list */
   if (message->GetAttachmentsNames().size() > 0)
   {
      sizer->Add(new AttachmentsList(this, *message), 0, wxALL|wxEXPAND, 0);
   }

   SetSizer(sizer);

   Show(true);
}

#include "default_icon.xpm"

AttachmentsList::AttachmentsList(wxWindow* parent, wxEmailMessage& message)
                :wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_SMALL_ICON),
                 message(message),
                 image_list(16, 16, true)
{
   /* Insert all attachments of message */
   std::vector<wxString>::const_iterator it;
   unsigned long current_id = 0;
   for (it = message.GetAttachmentsNames().begin(); it != message.GetAttachmentsNames().end(); it++)
   {
      /* Try to get mime icon of element */
      bool found_icon = false;
      wxFileType* file_type = wxTheMimeTypesManager->GetFileTypeFromExtension(it->AfterLast('.'));
      if (file_type != NULL)
      {
         wxIconLocation icon_location;
         if (file_type->GetIcon(&icon_location))
         {
            image_list.Add(wxIcon(icon_location));
            found_icon = true;
         }
      }

      /* If we did not found icon, display default one */
      if (!found_icon)
      {
         image_list.Add(wxIcon(default_icon_xpm));
      }

      /* Increment index */
      current_id++;
   }

   /* Configure images list */
   SetImageList(&image_list, wxIMAGE_LIST_SMALL);

   /* OK, now insert elements */
   current_id = 0;
   for (it = message.GetAttachmentsNames().begin(); it != message.GetAttachmentsNames().end(); it++)
   {
      /* Insert element */
      InsertItem(current_id,
                 it->AfterLast('\\').AfterLast('/'),
                 current_id);

      /* Increment index */
      current_id++;
   }
}

void AttachmentsList::OnElementSelected(wxListEvent& event)
{
   /* Create a temp directory */
   wxString temp_dir = wxFileName::GetTempDir() + wxFileName::GetPathSeparator()
                              + message.GetMessageId().id;
   if (!wxFileName::DirExists(temp_dir))
   {
      wxFileName::Mkdir(temp_dir);
   }

   /* Copy to file dir */
   wxString file_name = temp_dir + wxFileName::GetPathSeparator()
                                + message.GetAttachmentsNames().at(event.GetIndex()).AfterLast('\\').AfterLast('/');
   message.ExtractAttachment(file_name, event.GetIndex());

   /* Open file */
   bool found_command = false;
   wxFileType* file_type = wxTheMimeTypesManager->GetFileTypeFromExtension(message.GetAttachmentsNames().at(event.GetIndex()).AfterLast('.'));
   if (file_type != NULL)
   {
      wxString open_command = file_type->GetOpenCommand(file_name);
      wxExecute(open_command);
      found_command = true;
   }

   /* check if we can open this kinf of file */
   if (!found_command)
   {
      wxMessageDialog dlg(this,
                          _T("No command found to open file type ") + message.GetAttachmentsNames().at(event.GetIndex()).AfterLast('.'),
                          _T("POP3 Client"),
                          wxOK);
      dlg.ShowModal();
   }
}

bool App::AskUser(const wxString& message)
{
   wxMessageDialog dlg(NULL, message, _T("Pop3 Client"), wxYES_NO);
   return dlg.ShowModal()==wxID_YES;
}
