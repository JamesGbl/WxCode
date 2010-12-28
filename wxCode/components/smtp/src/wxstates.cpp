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
 $Log: wxstates.cpp,v $
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

//static char cvs_id[] = "$Header: /v/CVS/olive/notifier/wxSMTP/src/wxstates.cpp,v 1.3 2004/09/09 20:38:51 paul Exp $";

/*
 * Purpose: private wxWindows helper classes for SMTP
 * Author:  Frank Buﬂ
 * Created: 2002
 */

#include "wx/wxstates.h"

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

const WXDLLIMPEXP_DATA_SMTP(InitialState)       g_initialState;
const WXDLLIMPEXP_DATA_SMTP(ConnectedState)     g_connectedState;
const WXDLLIMPEXP_DATA_SMTP(HeloState)          g_heloState;
const WXDLLIMPEXP_DATA_SMTP(SendMailFromState)  g_sendMailFromState;
const WXDLLIMPEXP_DATA_SMTP(RcptListState)      g_rcptListState;
const WXDLLIMPEXP_DATA_SMTP(StartDataState)     g_startDataState;
const WXDLLIMPEXP_DATA_SMTP(DataState)          g_dataState;
const WXDLLIMPEXP_DATA_SMTP(QuitState)          g_quitState;
const WXDLLIMPEXP_DATA_SMTP(ClosedState)        g_closedState;
const WXDLLIMPEXP_DATA_SMTP(RetryState)         g_retryState;

void MailState::onTimeout(wxSMTP& context) const
{
   context.OnMessageStatus(wxEmailMessage::dispRetry);
   context.Final();
}

void InitialState::onConnect(wxSMTP& context, wxSocketEvent& event) const
{
   if (event.GetSocketEvent() == wxSOCKET_CONNECTION)
   {
      context.ChangeState(g_connectedState);
      context.TimerStart(wxSMTP_TIMEOUT_READY);
   }
   else
   {
      // error
      context.OnMessageStatus(wxEmailMessage::dispRetry);
      context.Final();
   }
}

void InitialState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   // Make sure this is same as ConnectedState::onResponse
   g_connectedState.onResponse(context, smtpCode, smtpRsp);
}

void ConnectedState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   // Make sure this is same as InitialState::onResponse
   if (smtpCode == 220)
   {
      context.ChangeState(g_heloState);
      // TODO: using some wxWindows function for getting the hostname
      context.Write("HELO localhost\x00d\x00a");
      context.TimerStart(wxSMTP_TIMEOUT_HELO);
      return;
   }
   
   switch (((const char*)smtpRsp.ToAscii())[0])
   {
      case '4':
         context.OnMessageStatus(wxEmailMessage::dispRetry);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;

      case '5':
      default:
         context.OnMessageStatus(wxEmailMessage::dispFail);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;
   }
}

void HeloState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   if (smtpCode == 250)
   {
      context.ChangeState(g_sendMailFromState);
      context.SendFrom();
      context.TimerStart(wxSMTP_TIMEOUT_MAIL);
      return;
   }

   switch (((const char*)smtpRsp.ToAscii())[0])
   {
      case '4':
         context.OnMessageStatus(wxEmailMessage::dispRetry);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;

      case '5':
      default:
         context.OnMessageStatus(wxEmailMessage::dispFail);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;
   }
}

void SendMailFromState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   if (smtpCode == 250)
   {
      context.ChangeState(g_rcptListState);
      context.SendNextRecipient();
      context.TimerStart(wxSMTP_TIMEOUT_RCPT);
      return;
   }

   switch (((const char*)smtpRsp.ToAscii())[0])
   {
      case '4':
         context.OnMessageStatus(wxEmailMessage::dispRetry);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;

      case '5':
      default:
         context.OnMessageStatus(wxEmailMessage::dispFail);
         context.ChangeState(g_quitState);
         context.SendQuit();
         context.TimerStart(wxSMTP_TIMEOUT_QUIT);
         break;
   }
}


void RcptListState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   if (smtpCode == 421)
   {
      // 421 has per-message scope, not per-recipient scope
      context.OnMessageStatus(wxEmailMessage::dispRetry);
      context.ChangeState(g_quitState);
      context.TimerStart(wxSMTP_TIMEOUT_QUIT);
      context.SendQuit();
   }

   switch (((const char*)smtpRsp.ToAscii())[0])
   {
      case '2':
         context.OnRecipientStatus(wxEmailMessage::dispAccept);
         break;

      case '4':
         context.OnRecipientStatus(wxEmailMessage::dispRetry);

         // trigger early retry
         context.SetFastRetryMode();

         //
         // don't attempt further addrs since we probably
         // hit limit for this message
         //
         //TODO : remove goto !!!
         goto startdata;

      case '5':
         if (smtpCode == 552)
         {
            // see RFC 2821 p.56 - treat as temporary
            context.OnRecipientStatus(wxEmailMessage::dispRetry);

            // trigger early retry
            context.SetFastRetryMode();

            //
            // don't attempt further addrs since we probably
            // hit limit for this message
            //
            //TODO : remove goto !!! and fall through
            goto startdata;
         }
         // fall through

      default:
         context.OnRecipientStatus(wxEmailMessage::dispFail);
         break;
   }

   if (context.SendNextRecipient())
   {
      context.TimerStart(wxSMTP_TIMEOUT_RCPT);
      return;
   }

   startdata:

   context.Write("DATA\x00d\x00a");
   context.ChangeState(g_startDataState);
   context.TimerStart(wxSMTP_TIMEOUT_DATA);
}

void StartDataState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   if (smtpCode == 354)
   {
      context.ChangeState(g_dataState);
      context.SendData();
      context.TimerStart(wxSMTP_TIMEOUT_DATAEND);
      return;
   }

   if (((const char*)smtpRsp.ToAscii())[0] == '4')
   {
      context.OnMessageStatus(wxEmailMessage::dispRetry);
   }
   else
   {
      context.OnMessageStatus(wxEmailMessage::dispFail);
   }

   context.ChangeState(g_quitState);
   context.SendQuit();
   context.TimerStart(wxSMTP_TIMEOUT_QUIT);
}


void DataState::onResponse(wxSMTP& context, int smtpCode, const wxString &smtpRsp) const
{
   context.TimerStop();

   if (smtpCode == 250)
   {
      context.OnMessageStatus(wxEmailMessage::dispSucceed);
   }
   else if (((const char*)smtpRsp.ToAscii())[0] == '4')
   {
      context.OnMessageStatus(wxEmailMessage::dispRetry);
   }
   else
   {
      context.OnMessageStatus(wxEmailMessage::dispFail);
   }

   context.ChangeState(g_quitState);
   context.SendQuit();
   context.TimerStart(wxSMTP_TIMEOUT_QUIT);
}

void QuitState::onResponse(wxSMTP& context, int WXUNUSED(smtpCode), const wxString &WXUNUSED(smtpRsp)) const
{
   context.TimerStop();

   context.Final();
}

void QuitState::onTimeout(wxSMTP& context) const
{
   // message has already been accepted for delivery, so no error here
   context.Final();
}

void RetryState::onTimeout(wxSMTP& context) const
{
   context.UpdateElapsed();
   context.SendInner();
}
