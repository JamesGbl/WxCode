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
 $Log: wxcmdprot.cpp,v $
 Revision 1.7  2004/06/29 11:06:20  tavasti
 - Added OnResponse handler also for initial state (sometimes OnConnect
   arrives after first data)
 - Minor changes in indentation & comments

 Revision 1.6  2004/05/19 04:06:26  tavasti
 Fixes based on comments from Edwards John-BLUW23 <jedwards@motorola.com>
 - Removed -m486 flags from makefile
 - Added filetypes wav & mp3
 - Removed default arguments from wxmime.cpp (only in .h)
 - Commented out iostream.h includes

 Revision 1.5  2003/11/21 12:36:46  tavasti
 - Makefilet -Wall optioilla
 - Korjattu 'j‰rkev‰t' varoitukset pois (J‰‰nyt muutama joita ei saa
   kohtuudella poistettua)

 Revision 1.4  2003/11/14 15:43:09  tavasti
 Sending email with alternatives works

 Revision 1.3  2003/11/13 17:12:15  tavasti
 - Muutettu tiedostojen nimet wx-alkuisiksi

 Revision 1.2  2003/11/07 09:17:40  tavasti
 - K‰‰ntyv‰ versio, fileheaderit lis‰tty.


****************************************************************************/

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
#include <wx/sckstrm.h>

#endif

#include "wx/wxcmdprot.h"

static long SOCKET_ID = wxNewId();

//
// Socket events could be delivered after the socket is closed. Since
// we would like to reuse the socket, we have to ensure events for a
// previous connection are not interpreted as applying to the current
// connection.
//
// Fortunately, we are able to set a void* serial number ("ClientData")
// for the socket which will be copied into each event, allowing us to
// distinguish old events from current events.
//
// We increment the serial number (m_serial) every time we want to start
// ignoring old events. Events arriving with a non-current serial number
// are discarded.
//
class wxCmdlineProtocolSocketEventHandler : public wxEvtHandler
{
   public:
      wxCmdlineProtocolSocketEventHandler(wxCmdlineProtocol& callback)
                        :m_callback(callback),
                         m_serial(1)
      {}

      void OnSocketEvent(wxSocketEvent& event)
      {
         if (event.GetClientData() == (void *)m_serial)
         {
            m_callback.OnSocketEvent(event);
         }
      }

      void incserial()
      {
         ++m_serial;
      }

      int getserial()
      {
         return m_serial;
      }

   private:
      wxCmdlineProtocol &m_callback;
      int         m_serial;

   DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(wxCmdlineProtocolSocketEventHandler, wxEvtHandler)
   EVT_SOCKET(SOCKET_ID, wxCmdlineProtocolSocketEventHandler::OnSocketEvent)
END_EVENT_TABLE()


wxCmdlineProtocol::wxCmdlineProtocol()
                  :m_host(""),
                   m_service(""),
                   m_port(0),
                   m_user(""),
                   m_password(""),
                   m_inputLine("")
{
   m_pCmdlineProtocolSocketEventHandler = new wxCmdlineProtocolSocketEventHandler(*this);
}

wxCmdlineProtocol::~wxCmdlineProtocol()
{
   delete m_pCmdlineProtocolSocketEventHandler;
}

bool wxCmdlineProtocol::Close()
{
   m_pCmdlineProtocolSocketEventHandler->incserial();
   return wxSocketClient::Close();
}

void wxCmdlineProtocol::Connect()
{
   if (IsConnected())
   {
      Close();
   }

   //
   // I'm paranoid that we might not have called Close() in
   // some uncommon scenario where, for example, the server
   // goes away suddenly, so I'm not confident incserial()
   // got called between retries. It's OK to call it here
   // again.
   //
   m_pCmdlineProtocolSocketEventHandler->incserial();
   SetClientData((void *)m_pCmdlineProtocolSocketEventHandler->getserial());

   SetTimeout(60);
   SetEventHandler(*m_pCmdlineProtocolSocketEventHandler, SOCKET_ID);
   SetNotify(wxSOCKET_CONNECTION_FLAG |
             wxSOCKET_INPUT_FLAG |
             wxSOCKET_LOST_FLAG);
   Notify(TRUE);

   // connect
   wxIPV4address addr;
   addr.Hostname(m_host);
   if (m_port)
   {
      addr.Service(m_port);
   }
   else
   {
      addr.Service(m_service);
   }

   //TODO : it would be nice to don't perform a blocking call so that, if the server does
   // not respond, the GUI is not blocked. But this is not enough to call Connect(addr, false) as,
   // in this case, if the server takes time to respond, the mail is not sent at all...
   if (!wxSocketClient::Connect(addr, true))
   {
      //
      // generate a socket lost event
      //
      wxSocketEvent ev(SOCKET_ID);
      ev.m_event = wxSOCKET_LOST;
      ev.m_clientData = (void *)m_pCmdlineProtocolSocketEventHandler->getserial();
      ev.SetEventObject(this);
      m_pCmdlineProtocolSocketEventHandler->AddPendingEvent(ev);
   }
}

void wxCmdlineProtocol::OnInput(wxSocketEvent& WXUNUSED(event))
{
   // TODO: implementing response timeout somewhere

   // get data
   const int bufsize = 256;
   char buf[bufsize];
   Read(buf, bufsize);
   m_inputLine += wxString(buf, LastCount());

   // search for a newline
   while (true)
   {
      size_t pos = 0;
      while (pos < m_inputLine.Length() - 1)
      {
         if (m_inputLine[pos] == 13)
         {
            if (m_inputLine[pos + 1] == 10)
            {
               // line found, evaluate
               EvaluateLine(m_inputLine.Mid(0, pos));

               // adjust buffer
               m_inputLine = m_inputLine.Mid(pos + 2);
               return;
            }
            break;
         }
         pos++;
      }
   }
}

void wxCmdlineProtocol::OnSocketEvent(wxSocketEvent& event)
{
   wxString s = _("OnSocketEvent: ");

   switch(event.GetSocketEvent())
   {
      case wxSOCKET_INPUT:
         OnInput(event);
         break;
      case wxSOCKET_LOST:
         OnConnectionLost(event);
         break;
      case wxSOCKET_CONNECTION:
         OnConnect(event);
         break;
      default:
         break;
   }
}

void wxCmdlineProtocol::Write(const wxString& msg)
{
   wxSocketClient::Write(msg.GetData(), msg.Length());
}
