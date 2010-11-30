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
 $Log: wxemail.cpp,v $
 Revision 1.7  2004/05/19 04:06:26  tavasti
 Fixes based on comments from Edwards John-BLUW23 <jedwards@motorola.com>
 - Removed -m486 flags from makefile
 - Added filetypes wav & mp3
 - Removed default arguments from wxmime.cpp (only in .h)
 - Commented out iostream.h includes

 Revision 1.6  2003/11/21 12:36:46  tavasti
 - Makefilet -Wall optioilla
 - Korjattu 'j‰rkev‰t' varoitukset pois (J‰‰nyt muutama joita ei saa
   kohtuudella poistettua)

 Revision 1.5  2003/11/14 15:46:50  tavasti
 Commented out some debug printing

 Revision 1.4  2003/11/14 15:43:09  tavasti
 Sending email with alternatives works

 Revision 1.3  2003/11/13 17:12:15  tavasti
 - Muutettu tiedostojen nimet wx-alkuisiksi

 Revision 1.2  2003/11/07 09:17:40  tavasti
 - K‰‰ntyv‰ versio, fileheaderit lis‰tty.


****************************************************************************/

//static char cvs_id[] = "$Header: /v/CVS/olive/notifier/wxSMTP/src/wxemail.cpp,v 1.2 2004/09/07 09:02:35 paul Exp $";

/*
 * Purpose: wxWindows email implementation
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

#include "wx/wxemail.h"
#include "wx/wxbase64.h"


wxEmailMessage::wxEmailMessage(const wxString& subject, const wxString& text, const wxString& from)
               :m_subject(subject),
                m_text(text),
                m_from(from)
{}

const wxString& wxEmailMessage::GetFrom()
{
   return m_from;
}

void wxEmailMessage::AddRecipient(const wxString& address)
{
   m_rcptArray.Add(address);
   m_rcptAttempt.Add(address);
}

void wxEmailMessage::AddTo(const wxString& address)
{
   m_toArray.Add(address);
   m_rcptArray.Add(address);
   m_rcptAttempt.Add(address);
}

void wxEmailMessage::AddCc(const wxString& address)
{
   m_ccArray.Add(address);
   m_rcptArray.Add(address);
   m_rcptAttempt.Add(address);
}

void wxEmailMessage::AddBcc(const wxString& address)
{
   m_bccArray.Add(address);
   m_rcptArray.Add(address);
   m_rcptAttempt.Add(address);
}

//
// this gets called with results of RCPT command, which are per-recipient
//
void wxEmailMessage::OnRecipientStatus(const wxString &recipient, disposition d)
{
   //
   // yank from attempt list, place in appropriate disposition list
   //
   int goner = m_rcptAttempt.Index(recipient);
   wxASSERT(goner != wxNOT_FOUND);
   m_rcptAttempt.RemoveAt(goner, 1);

   m_rcptDisp[d].Add(recipient);
}

//
//
// Here we get the final disposition of the message transaction with the
// mail server. This enables us to adjust the per-recipient status and
// prepare for the next retry (if any)
//
// Fail:
// m_rcptAttempt  -> Fail
// [Accept] -> Fail
// [Retry]     -> m_rcptAttempt
//
// Retry:
// m_rcptAttempt  -> m_rcptAttempt
// [Retry]     -> m_rcptAttempt
// [Accept] -> m_rcptAttempt
//
// Succeed:
// m_rcptAttempt  -> m_rcptAttempt  // (not attempted yet)
// [Accept] -> Succeed
// [Retry]     -> m_rcptAttempt
//
// Accept: Not valid
//
void wxEmailMessage::OnMessageStatus(disposition d)
{
   switch (d)
   {

      case dispFail:
      {
         for (unsigned int i = 0; i < m_rcptAttempt.GetCount(); ++i)
         {
            m_rcptDisp[dispFail].Add(m_rcptAttempt.Item(i));
         }
         m_rcptAttempt.Empty();

         for (unsigned int i = 0; i < m_rcptDisp[dispAccept].GetCount(); ++i)
         {
            m_rcptDisp[dispFail].Add(m_rcptDisp[dispAccept].Item(i));
         }
         m_rcptDisp[dispAccept].Empty();

         // m_rcptAttempt emptied above
         for (unsigned int i = 0; i < m_rcptDisp[dispRetry].GetCount(); ++i)
         {
            m_rcptAttempt.Add(m_rcptDisp[dispRetry].Item(i));
         }
         m_rcptDisp[dispRetry].Empty();

         break;
      }

      case dispRetry:
      {
         for (unsigned int i = 0; i < m_rcptDisp[dispRetry].GetCount(); ++i)
         {
            m_rcptAttempt.Add(m_rcptDisp[dispRetry].Item(i));
         }
         m_rcptDisp[dispRetry].Empty();

         for (unsigned int i = 0; i < m_rcptDisp[dispAccept].GetCount(); ++i)
         {
            m_rcptAttempt.Add(m_rcptDisp[dispAccept].Item(i));
         }
         m_rcptDisp[dispAccept].Empty();

         break;
      }

      case dispSucceed:
      {
         for (unsigned int i = 0; i < m_rcptDisp[dispAccept].GetCount(); ++i)
         {
            m_rcptDisp[dispSucceed].Add(m_rcptDisp[dispAccept].Item(i));
         }
         m_rcptDisp[dispAccept].Empty();

         for (unsigned int i = 0; i < m_rcptDisp[dispRetry].GetCount(); ++i)
         {
            m_rcptAttempt.Add(m_rcptDisp[dispRetry].Item(i));
         }
         m_rcptDisp[dispRetry].Empty();

         break;
      }

      default:
         wxASSERT(0);
         break;
   }
}

void wxEmailMessage::AddFile(const wxFileName& fileName,
                             const wxString WXUNUSED(mimeMainType),
                             const wxString WXUNUSED(mimeSubType))
{
   // add new entry
   m_mimeParts.Add(wxMimePart(fileName));
}

void wxEmailMessage::AddAlternative(const wxString data,
                                    const wxString mimeMainType,
                                    const wxString mimeSubType)
{
   // Add alternative (normally html, but might there be others?
   m_mimeAlternatives.Add(wxMimePart(data,TRUE,mimeMainType,mimeSubType));
}

void wxEmailMessage::Encode(wxOutputStream& out)
{
   // TODO: use only MIME if neccessary
   wxString header;
   wxString cr("\x00d\x00a");
   //wxString cr("/n");
   // TODO: calculate a more random boundary

   wxString timestr = wxDateTime::Now().Format("%H%m%M%d%S%w%W");
   wxString boundary = "---_" + timestr + "-boundary-" + timestr
                       + "_---";
   wxString boundary2 = "---_" + timestr + "-02-boundary-02-"
                        + timestr + "_---";
   wxString boundarySep = "--" + boundary + cr;
   wxString boundarySep2 = "--" + boundary2 + cr;

   header << "From: " << m_from << cr;
   if(m_toArray.GetCount() > 0)
   {
      header << "To: ";
      for(unsigned int i = 0; i < m_toArray.GetCount() ; i++)
      {
         if(i > 0)
         {
            header << "," << cr;
         }
         header << m_toArray[i];
      }
      header << cr;
   }

   if(m_ccArray.GetCount() > 0)
   {
      header << "Cc: ";
      for(unsigned int i = 0; i < m_ccArray.GetCount() ; i++)
      {
         if(i > 0)
         {
            header << "," << cr;
         }
         header << m_ccArray[i];
      }
      header << cr;
   }

   if(m_bccArray.GetCount() > 0)
   {
      header << "Cc: ";
      for(unsigned int i = 0; i < m_bccArray.GetCount() ; i++)
      {
         if(i > 0)
         {
            header << "," << cr;
         }
         header << m_bccArray[i];
      }
      header << cr;
   }

   header << "Subject: " << m_subject << cr  // TODO: add Date:
          << "MIME-Version: 1.0" << cr
          << "Content-Type: multipart/mixed; boundary=\"" << boundary << "\"" << cr
          << cr
          << cr
          << "This is a multi-part message in MIME format" << cr
          << cr
          << boundarySep;

   if(m_mimeAlternatives.GetCount() > 0)
   {
      header << "Content-Type: multipart/alternative; boundary=\""
             << boundary2 << "\"" << cr << cr << boundarySep2;
   }

   header << "Content-Type: text/plain; charset=iso-8859-1" << cr
          << "Content-Transfer-Encoding: 8bit" << cr
          << cr
          << m_text << cr;

   // TODO: is it possible in MIME message to have a single '.' on a line?
   //cout << ">>" << header << "<<" << endl;
   out.Write((const char*) header.GetData(), header.Length());

   for (size_t i = 0; i < m_mimeAlternatives.GetCount(); i++)
   {
      // cout << ">>" << boundarySep2 << "<<" << endl;
      out.Write((const char*) boundarySep2.GetData(), boundarySep2.Length());
      m_mimeAlternatives[i].Encode(out);
   }
   if(m_mimeAlternatives.GetCount() > 0)
   {
      header = "--" + boundary2 + "--" + cr;
      //cout << ">>" << header << "<<" << endl;
      out.Write((const char*) header.GetData(), header.Length());
   }

   for (size_t i = 0; i < m_mimeParts.GetCount(); i++)
   {
      //cout << ">>" << boundarySep << "<<" << endl;
      out.Write((const char*) boundarySep.GetData(), boundarySep.Length());
      m_mimeParts[i].Encode(out);
   }
   wxString footer = "--" + boundary + "--" + cr + "." + cr;  // TODO: perhaps moving the '.\r\n' sequence to another place
   //cout << ">>" << footer << "<<" << endl;
   out.Write((const char*) footer.GetData(), footer.Length());
}
