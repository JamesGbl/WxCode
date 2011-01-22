/****************************************************************************

 Project     :
 Author      :
 Description :

 VERSION INFORMATION:
 File    : $Source$
 Version : $Revision$
 Date    : $Date$
 Author  : $Author$
 Licence : wxWidgets licence

 History:
 $Log: wxsmtp.cpp,v $
 Revision 1.5  2004/06/29 11:06:20  tavasti
 - Added OnResponse handler also for initial state (sometimes OnConnect
   arrives after first data)
 - Minor changes in indentation & comments

 Revision 1.4  2003/11/21 12:36:46  tavasti
 - Makefilet -Wall optioilla
 - Korjattu 'j‰rkev‰t' varoitukset pois (J‰‰nyt muutama joita ei saa
   kohtuudella poistettua)

 Revision 1.3  2003/11/13 17:12:15  tavasti
 - Muutettu tiedostojen nimet wx-alkuisiksi

 Revision 1.2  2003/11/07 09:17:40  tavasti
 - K‰‰ntyv‰ versio, fileheaderit lis‰tty.


****************************************************************************/

//static char cvs_id[] = "$Header: /v/CVS/olive/notifier/wxSMTP/src/wxsmtp.cpp,v 1.3 2004/09/09 20:38:51 paul Exp $";

/*
 * Purpose: private wxWindows mail transport implementation
 * Author:  Frank Buﬂ
 * Created: 2002
 */

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

#include <wx/sckstrm.h>

#include "wx/smtp/wxsmtp.h"

/*!
 * The default timeout used for communication with SMTP server, in seconds
 */
#define DEFAULT_SMTP_TIMEOUT 30

/*!
 * All SMTP states instances
 */
const wxSMTP::ConnectState      wxSMTP::g_connectState;
const wxSMTP::HeloState         wxSMTP::g_heloState;
const wxSMTP::SendMailFromState wxSMTP::g_sendMailFromState;
const wxSMTP::RcptListState     wxSMTP::g_rcptListState;
const wxSMTP::BeginDataState    wxSMTP::g_beginDataState;
const wxSMTP::DataState         wxSMTP::g_dataState;
const wxSMTP::QuitState         wxSMTP::g_quitState;
const wxSMTP::ClosedState       wxSMTP::g_closedState;

/*! Other static variables instanciation */
const unsigned long wxSMTP::timeout = DEFAULT_SMTP_TIMEOUT;

/**
 * Default listener, if the user doesn't specify one. Does nothing.
 */
wxSMTP::Listener wxSMTP::g_nullListener;

wxSMTP::wxSMTP(const wxString& host,
               unsigned short port,
               Listener* listener)
       :shall_trigger_disconnection_callback(0)
{
   /* Configure host */
   SetHost(host, port);

   /* Initialise state */
   ChangeState(g_closedState);

   /* Configure listener */
   if (listener)
   {
      m_pListener = listener;
   }
   else
   {
      m_pListener = &g_nullListener;
   }
}

wxSMTP::~wxSMTP()
{
   /* Nothing to do... */
}

void wxSMTP::SendMessage(const wxEmailMessage& message, bool shall_start_sending_process)
{
   /* Append message to list */
   messages_to_send.push_back(wxSmtpEmailMessage(message));

   /* Restart sending mails, if necessary */
   if (shall_start_sending_process)
   {
      RestartSendingProcess();
   }
}

void wxSMTP::RestartSendingProcess()
{
   ((SMTPState&)GetCurrentState()).onStartSendingMails(*this);
}

void wxSMTP::TestConnection()
{
   /* Just start sending mails. If no mail available, it will stop with proper status */
   RestartSendingProcess();
}

bool wxSMTP::IsRunning() const
{
   /* Check that we are not in closed state */
   return &GetCurrentState() != &g_closedState;
}

void wxSMTP::FlushAllMessages()
{
   ((SMTPState&)GetCurrentState()).onFlushMessages(*this);
}

unsigned long wxSMTP::GetNbRetryMessages() const
{
   unsigned long result = 0;

   std::list<RetryInfo_t>::const_iterator it;
   for (it = retry_messages_list.begin(); it != retry_messages_list.end(); it++)
   {
      if (!it->is_reconnection)
      {
         result++;
      }
   }
   return result;
}
