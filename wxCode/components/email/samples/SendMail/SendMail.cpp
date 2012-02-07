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

#include "wx/smtp/wxsmtp.h"

class App : public wxApp
{
   public:
      App();
      ~App();

      void MailSent();

   private:

      bool OnInit();

      wxSMTP*             smtp;

      wxBusyInfo* wait_msg;

      static wxString RequestUser(const wxString& message, bool is_multiline);

      static bool AskUser(const wxString& message);

   DECLARE_NO_COPY_CLASS(App)
};

IMPLEMENT_APP(App)

class MySMTPListener : public wxSMTP::Listener
{
   public:
      MySMTPListener(App& app) : app(app) {}

   private:

      virtual void OnMessageStatus(const wxEmailMessage::MessageId&           WXUNUSED(message_id),
                                   SendingStatus_t                            status,
                                   unsigned long                              WXUNUSED(nb_pending_messages),
                                   unsigned long                              WXUNUSED(nb_retry_messages),
                                   const std::list<wxEmailMessage::Address>&  rejected_addresses,
                                   const std::list<wxEmailMessage::Address>&  accepted_addresses,
                                   bool                                       WXUNUSED(can_retry),
                                   bool&                                      shall_retry,
                                   unsigned long&                             WXUNUSED(retry_delay_s),
                                   bool&                                      shall_stop)
      {
         /* Do not perform retry */
         shall_retry = false;
         shall_stop = false;

         /* Construct list of accepted addresses */
         wxString accepted_list;
         std::list<wxEmailMessage::Address>::const_iterator it = accepted_addresses.begin();
         for (;it != accepted_addresses.end(); it++)
         {
            if (!accepted_list.empty())
            {
               accepted_list += _T(", ");
            }
            accepted_list += it->GetAddress();
         }

         /* Construct list of rejected addresses */
         wxString rejected_list;
         it = rejected_addresses.begin();
         for (;it != rejected_addresses.end(); it++)
         {
            if (!rejected_list.empty())
            {
               rejected_list += _T(", ");
            }
            rejected_list += it->GetAddress();
         }

         wxMessageDialog dlg(NULL,
                             wxString(_T("The message has been sent to recipients addresses (")) <<  accepted_list
                                      << _T(").\n\nThe following addresses have been rejected : (") <<
                                      rejected_list << _T(").\n\n The sending process finished with status ") <<
                             (status==SendingSucceeded?_T("'SendingSucceeded'"):
                                status==SendingTimeout?_T("'SendingTimeout'"):
                                  status==SendingDisconected?_T("'SendingDisconected'"):
                                    status==SendingMessageRejected?_T("'SendingMessageRejected'"):
                                      status==SendingNoValidRecipient?_T("'SendingNoValidRecipient'"):
                                        status==SendingRetry?_T("'SendingRetry'"):
                                          status==SendingError?_T("'SendingError'"):_T("'Unknown'")),
                             _T("Message Notification Status"),
                             wxOK);

         dlg.ShowModal();
      }

      void OnDisconnect(DisconnectionStatus_t status,
                        unsigned long         WXUNUSED(nb_pending_messages),
                        unsigned long         WXUNUSED(nb_retry_messages),
                        bool&                 shall_retry,
                        unsigned long&        WXUNUSED(retry_delay_s))
      {
         /* Display dialog warning user that SMTP client is disconnected */
         wxMessageDialog dlg(NULL,
                             wxString(_T("The SMTP client is now disconnected from server with status ")) <<
                             (status==StatusOK?_T("'StatusOK'"):
                                status==StatusTimeout?_T("'StatusTimeout'"):
                                  status==StatusDisconnect?_T("'StatusDisconnect'"):
                                    status==StatusRetry?_T("'StatusRetry'"):
                                      status==StatusError?_T("StatusError"):
                                        status==StatusUserAbort?_T("StatusUserAbort"):_T("'Unknown'")),
                             _T("Server Disconnection Notification Status"),
                             wxOK);
         dlg.ShowModal();

         /* notify app that mail is sent */
         app.MailSent();

         /* Request no restart */
         shall_retry = false;
      }


   App& app;
};

App::App()
{
   smtp = NULL;
   wait_msg =  NULL;
}

App::~App()
{
   delete smtp;

   /* If we reach this point, wait shall be destroyed, but in cas of... */
   delete wait_msg;
}


#include "wx/utils/wxmd5.hpp"
bool App::OnInit()
{
   /* Instanciate the message */
   wxEmailMessage msg(RequestUser(_T("Please enter the address of the sender of the mail :"), false),
                      RequestUser(_T("Please enter the subject of the mail :"), false),
                      RequestUser(_T("Please enter the content of the mail :"), true));

   /* Add the recipients of the mail */
   while (AskUser(_T("Do you want to add a TO recipient to the mail ?")))
   {
      msg.AddTo(RequestUser(_T("Please enter the address of the recipient of the mail :"), false));
   }
   while (AskUser(_T("Do you want to add a CC recipient to the mail ?")))
   {
      msg.AddCc(RequestUser(_T("Please enter the address of the recipient of the mail :"), false));
   }
   while (AskUser(_T("Do you want to add a BCC recipient to the mail ?")))
   {
      msg.AddBcc(RequestUser(_T("Please enter the address of the recipient of the mail :"), false));
   }

   /* Add all attachments */
   while (AskUser(_T("Do you want to add an attachment to the mail ?")))
   {
      wxFileDialog dlg(NULL,
                       _T("Choose a file"),
                       _T(""),
                       _T(""),
                       _T("*.*"),
                       wxFD_OPEN|wxFD_FILE_MUST_EXIST);
      if (dlg.ShowModal() == wxID_OK)
      {
         msg.AddAttachment(wxFileName(dlg.GetPath()));
      }
   }

   /* Add alternate html format */
   if (AskUser(_T("Do you want to add an alternate html format ?")))
   {
      msg.AddAlternativeHtmlBody(RequestUser(_T("Please enter the alternate html content :"), true).c_str());
   }

   /* Instanciate the smtp client */
   long port = 25;
   RequestUser(_T("Please enter the port of the smtp server (default : 25)"), false).ToLong(&port);
   smtp = new wxSMTP(RequestUser(_T("Please enter the smtp server address (ex: relay.skynet.be) :"), false),
                     port,
                     false,
                     new MySMTPListener(*this));

   /* Ask user if server requests authentication */
   if (AskUser(_T("Does your server requests authentication ?")))
   {
      smtp->ConfigureAuthenticationScheme(wxSMTP::LoginAuthentication,
            RequestUser(_T("Please enter the user name used for SMTP server connection :"), false),
            RequestUser(_T("Please enter the password used for SMTP server connection :"), false),
            AskUser(_T("Does your server requests an SSL connection ?")));
   }

   /* instanciate a busy info window that will be displayed until the message will be sent*/
   wait_msg = new wxBusyInfo(_T("Please wait while your mail is being processed"));

   /* Initiate the sending process */
   smtp->SendMessage(msg);

   /* Mail will be sent during normal event processing loop */
   return true;
}

void App::MailSent()
{
   /* We simply destroy the busy info window, which will close main program */
   delete wait_msg;
   wait_msg = NULL;
}

wxString App::RequestUser(const wxString& message, bool is_multiline)
{
   long style = wxOK;
   if (is_multiline)
   {
      style |= wxTE_MULTILINE;
   }

   wxTextEntryDialog dlg(NULL,
                         message,
                         _T("Send Mail"),
                         _T(""),
                         style);
   dlg.ShowModal();
   return dlg.GetValue();
}

bool App::AskUser(const wxString& message)
{
   wxMessageDialog dlg(NULL, message, _T("Send Mail"), wxYES_NO);
   return dlg.ShowModal()==wxID_YES;
}
