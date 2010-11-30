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
 $Log: wxstates.h,v $
 Revision 1.4  2004/06/29 11:06:20  tavasti
 - Added OnResponse handler also for initial state (sometimes OnConnect
   arrives after first data)
 - Minor changes in indentation & comments

 Revision 1.3  2003/11/13 17:12:15  tavasti
 - Muutettu tiedostojen nimet wx-alkuisiksi

 Revision 1.2  2003/11/07 09:17:40  tavasti
 - K‰‰ntyv‰ versio, fileheaderit lis‰tty.


****************************************************************************/

/*
 * Purpose: private wxWindows helper classes for SMTP
 * Author:  Frank Buﬂ
 * Created: 2002
 */

#ifndef _WX_STATES_H
#define _WX_STATES_H

#include <wx/wx.h>
#include <wx/socket.h>

#include "wxsmtpdef.h"

#include "wxsmtp.h"

/**
 * Base class for all mail states. Every mail state is only instantiated once
 * and has no attributes (flyweight pattern? I don't know the name). The only
 * reason for the state classes is not to have a big switch-case block.
 */
class WXDLLIMPEXP_SMTP MailState
{
   public:
      virtual void onConnect(wxSMTP& WXUNUSED(context), wxSocketEvent& WXUNUSED(event)) const {}

      virtual void onResponse(wxSMTP& WXUNUSED(context), int WXUNUSED(smtpCode), const wxString& WXUNUSED(line)) const {}

      virtual void onTimeout(wxSMTP& context) const;
};

/**
 * This state is the initial state, when no server connection is made.
 */
class WXDLLIMPEXP_SMTP InitialState : public MailState
{
   public:
      /**
       * Switches to ConnectedState, if successfully connected.
       * \sa ConnectedState.
       */
      void onConnect(wxSMTP& context, wxSocketEvent& event) const ;

      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active, if the server connection is established.
 */
class WXDLLIMPEXP_SMTP ConnectedState : public MailState
{
   public:
      /**
       * Switches to HeloState, if welcome message is received.
       * \sa HeloState.
       */

      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active, when the helo message was sent.
 */
class WXDLLIMPEXP_SMTP HeloState : public MailState
{
   public:
      /**
       * Switches to SendMailFromState, if the helo message was acknowledged.
       * \sa SendMailFromState.
       */
      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active, when the from message was sent.
 */
class WXDLLIMPEXP_SMTP SendMailFromState : public MailState
{
   public:
      /**
       * Switches to RcptListState, if the from message was acknowledged.
       * \sa RcptListState.
       */

      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active as long as there are additional recipients
 * and no error occured (TODO: error-handling not implemented).
 */
class WXDLLIMPEXP_SMTP RcptListState : public MailState
{
   public:
      /**
       * Switches to StartDataState, if there are no more recipients.
       * \sa StartDataState.
       */

      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active, when the DATA command was sent.
 */
class WXDLLIMPEXP_SMTP StartDataState : public MailState
{
   public:
      /**
       * Switches to DataState, if the DATA command was acknowledged.
       * \sa DataState.
       */
      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};


/**
 * This state is active, when the data was sent.
 */
class WXDLLIMPEXP_SMTP DataState : public MailState
{
   public:
      /**
       * Acknowledges by calling MailContext::OnDataSuccess,
       * if the data sent was acknowledged.
       * \sa SendmailImpl::OnDataSuccess().
       */

      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

};

class WXDLLIMPEXP_SMTP QuitState : public MailState
{
   public:
      void onResponse(
      wxSMTP& context,
      int smtpCode,
      const wxString &smtpRsp) const ;

      void onTimeout(wxSMTP& context) const ;
};

class WXDLLIMPEXP_SMTP ClosedState : public MailState
{
   public:
      // No escape from this state
};

class WXDLLIMPEXP_SMTP RetryState : public MailState
{
   public:
      void onTimeout(wxSMTP& context) const ;
};

extern const WXDLLIMPEXP_DATA_SMTP(InitialState)      g_initialState;
extern const WXDLLIMPEXP_DATA_SMTP(ConnectedState)    g_connectedState;
extern const WXDLLIMPEXP_DATA_SMTP(HeloState)         g_heloState;
extern const WXDLLIMPEXP_DATA_SMTP(SendMailFromState) g_sendMailFromState;
extern const WXDLLIMPEXP_DATA_SMTP(RcptListState)     g_rcptListState;
extern const WXDLLIMPEXP_DATA_SMTP(StartDataState)    g_startDataState;
extern const WXDLLIMPEXP_DATA_SMTP(DataState)         g_dataState;
extern const WXDLLIMPEXP_DATA_SMTP(QuitState)         g_quitState;
extern const WXDLLIMPEXP_DATA_SMTP(ClosedState)       g_closedState;
extern const WXDLLIMPEXP_DATA_SMTP(RetryState)        g_retryState;

#endif
