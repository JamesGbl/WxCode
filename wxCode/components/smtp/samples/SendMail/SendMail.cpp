/****************************************************************************

 Project     : wxSMTP
 Author      : Brice André
 Description : Sample illustrating the useage of wxSMTP library

 VERSION INFORMATION:
 File    : $Source$
 Version : $Revision$
 Date    : $Date$
 Author  : $Author$
 Licence : wxWidgets licence

 History:
 $Log: wxcmdprot.cpp,v $

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

#include "wx/wxsmtp.h"
#include "wx/wxemail.h"

class App : public wxApp
{
   public:
      App();
      ~App();

      void MailSent();

   private:

      bool OnInit();

      wxSMTP*         smtp;
      wxEmailMessage* msg;

      wxBusyInfo* wait_msg;

      static wxString RequestUser(const wxString& message, bool is_multiline);

   DECLARE_NO_COPY_CLASS(App)
};

IMPLEMENT_APP(App)

class MySMTPListener : public wxSMTPListener
{
   public:
      MySMTPListener(App& app) : app(app) {}

   private:
      void OnRecipientStatus(const wxString& address, wxEmailMessage::disposition u)
      {
         wxMessageDialog dlg(NULL,
                             wxString("The message has been sent to recipient address ") << address << " with status " <<
                             (u==wxEmailMessage::dispFail?"'Failed'":
                                  u==wxEmailMessage::dispRetry?"'Retry'":
                                       u==wxEmailMessage::dispAccept?"'Accept'":
                                            u==wxEmailMessage::dispSucceed?"'Succeed'":"'Unknown'"),
                             "Message Notification Status",
                             wxOK);
         dlg.ShowModal();
      }

      void OnMessageStatus(wxEmailMessage::disposition u, bool& halt)
      {
         /* Do not stop ongoing process */
         halt = false;

         wxMessageDialog dlg(NULL,
                             wxString("The message has been sent with status ") <<
                             (u==wxEmailMessage::dispFail?"'Failed'":
                                  u==wxEmailMessage::dispRetry?"'Retry'":
                                       u==wxEmailMessage::dispAccept?"'Accept'":
                                            u==wxEmailMessage::dispSucceed?"'Succeed'":"'Unknown'"),
                             "Message Notification Status",
                             wxOK);
         dlg.ShowModal();

         /* notify app that mail is sent */
         app.MailSent();
      }

   App& app;
};

App::App()
{
   smtp = NULL;
   msg = NULL;
   wait_msg =  NULL;
}

App::~App()
{
   delete smtp;
   delete msg;

   /* If we reach this point, wait shall be destroyed, but in cas of... */
   delete wait_msg;
}

bool App::OnInit()
{
   /* Instanciate the message */
   msg = new wxEmailMessage(RequestUser("Please enter the subject of the mail :", false),
                            RequestUser("Please enter the content of the mail :", true),
                            RequestUser("Please enter the address of the sender of the mail :", false));

   /* Add the recipients of the mail */
   msg->AddTo(RequestUser("Please enter the address of the recipient of the mail :", false));

   /* Instanciate the smtp client */
   smtp = new wxSMTP(new MySMTPListener(*this));
   smtp->SetHost(RequestUser("Please enter the smtp server address (ex: relay.skynet.be) :", false));

   /* instanciate a busy info window that will be displayed until the message will be sent*/
   wait_msg = new wxBusyInfo("Please wait while your mail is being processed");

   /* Initiate the sending process */
   smtp->Send(msg);

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
                         "Send Mail",
                         "",
                         style);
   dlg.ShowModal();
   return dlg.GetValue();
}
