/*
 *	davtool.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Thu Jul 15 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 *
 *	Licence: wxWidgets Licence
 */

// davtool.cpp
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <wx/mstream.h>

#if wxCHECK_VERSION(2, 5, 2)
#include <wx/xml/xml.h> //in 2.5.2 the XML/XRC stuff got rolled into core (from contrib)
#else
#include <wx/xrc/xml.h> 
#endif

#include <wx/curl/davtool.h>

//////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////

// davtool.cpp: implementation of the wxCurlDAVFs class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlDAVFs::wxCurlDAVFs()
: m_iContentLength(0)
{
}

wxCurlDAVFs::wxCurlDAVFs(const wxString& szHREF			,
						 const wxString& szStatus		,
						 const wxString& szCreationDate	,
						 const wxString& szLastModified	,
						 const long& iContentLength		,
						 const wxString& szContentType	)
: m_szHREF(szHREF), m_szStatus(szStatus),
m_iContentLength(iContentLength), m_szContentType(szContentType)
{
	m_dtCreationDate = wxCurlBase::GetDateFromString(szCreationDate);
	m_dtLastModified = wxCurlBase::GetDateFromString(szLastModified);
}

wxCurlDAVFs::~wxCurlDAVFs()
{
}

// davtool.cpp: implementation of the wxCurlDAVTool class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlDAVTool::wxCurlDAVTool(const wxString& szURL /*= wxEmptyString*/, const wxString& szUserName /*= wxEmptyString*/, const wxString& szPassword /*= wxEmptyString*/, wxEvtHandler* pEvtHandler /*= NULL*/, const bool& bSendUpdateEvents /*= false*/, const bool& bSendBeginEndEvents /*= false*/)
: wxCurlDAV(szURL, szUserName, szPassword, pEvtHandler, bSendUpdateEvents, bSendBeginEndEvents)
{
}

wxCurlDAVTool::~wxCurlDAVTool()
{
}

//////////////////////////////////////////////////////////////////////
// Member Data Access Methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Action Methods
//////////////////////////////////////////////////////////////////////

bool wxCurlDAVTool::GetDAVFs(wxArrayDAVFs& fs, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayString arrProps;

	arrProps.Add(_T("creationdate"));
	arrProps.Add(_T("getlastmodified"));
	arrProps.Add(_T("getcontentlength"));
	arrProps.Add(_T("getcontenttype"));

	if(Propfind(arrProps, szRemoteLoc))
	{
		// Construct Input Source...
		wxMemoryInputStream inStream(m_szResponseBody, m_szResponseBody.Len());

		if(inStream.IsOk())
		{
			// Construct XML Parser
			wxXmlDocument xmlPropfind(inStream);

			if(xmlPropfind.IsOk())
			{
				// Process XML!
				wxXmlNode* pNode = xmlPropfind.GetRoot();

				// Strip Past First "Multistatus" tag...
				while(pNode)
				{
					if(pNode->GetName().Find(wxT("multistatus")) != -1)
					{
						pNode = pNode->GetChildren();
						break;
					}
					else
						pNode = pNode->GetNext();
				}
				
				// Process "Response" tags...
				while(pNode)
				{
					wxCurlDAVFs fsItem;

					wxString szName = pNode->GetName();

					if(pNode->GetName().Find(wxT("response")) != -1)
					{
						if(ParseResponseXml(fsItem, pNode))
							fs.Add(fsItem);
					}

					pNode = pNode->GetNext();
				}

				return true;
			}
		}
	}

	return false;
}

bool wxCurlDAVTool::Exists(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return true;
	}

	return false;
}

bool wxCurlDAVTool::IsDirectory(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().IsDirectory();
	}

	return false;
}

bool wxCurlDAVTool::IsOk(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().IsOK();
	}

	return false;
}

wxDateTime wxCurlDAVTool::GetLastModified(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetLastModified();
	}

	return wxDateTime();
}

wxDateTime wxCurlDAVTool::GetCreationDate(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetCreationDate();
	}

	return wxDateTime();
}

long wxCurlDAVTool::GetContentLength(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetContentLength();
	}

	return -1;
}

wxString wxCurlDAVTool::GetContentType(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayDAVFs arrFs;

	if(GetDAVFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetContentType();
	}

	return wxEmptyString;
}

//////////////////////////////////////////////////////////////////////
// Helper Methods
//////////////////////////////////////////////////////////////////////

bool wxCurlDAVTool::ParseResponseXml(wxCurlDAVFs& fsItem, wxXmlNode* pNode)
{
	if(pNode->GetName().Find(wxT("response")) != -1)
	{
		wxXmlNode* pChild = pNode->GetChildren();

		bool bFoundHref = false;
		bool bParsedPropstat = false;

		while(pChild)
		{
			if(pChild->GetName().Find(wxT("href")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						fsItem.m_szHREF = pText->GetContent();
						bFoundHref = true;
					}

					pText = pText->GetNext();
				}
			}

			if(!bParsedPropstat)
			{
				if(pChild->GetName().Find(wxT("propstat")) != -1)
				{
					bParsedPropstat = ParsePropstatXml(fsItem, pChild);
				}
			}

			pChild = pChild->GetNext();
		}

		return bFoundHref && bParsedPropstat;
	}

	return false;
}

bool wxCurlDAVTool::ParsePropstatXml(wxCurlDAVFs& fsItem, wxXmlNode* pNode)
{
	if(pNode->GetName().Find(wxT("propstat")) != -1)
	{
		wxXmlNode* pChild = pNode->GetChildren();

		bool bFoundStatus = false;
		bool bParsedProps = false;

		while(pChild)
		{
			if(pChild->GetName().Find(wxT("status")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						if(fsItem.m_szStatus.IsEmpty())
							fsItem.m_szStatus = pText->GetContent();

						bFoundStatus = true;
					}

					pText = pText->GetNext();
				}
			}

			if(pChild->GetName().Find(wxT("prop")) != -1)
			{
				bParsedProps = ParsePropsXml(fsItem, pChild);
			}

			pChild = pChild->GetNext();
		}

		return bFoundStatus && bParsedProps;
	}

	return false;
}

bool wxCurlDAVTool::ParsePropsXml(wxCurlDAVFs& fsItem, wxXmlNode* pNode)
{
	if(pNode->GetName().Find(wxT("prop")) != -1)
	{
		wxXmlNode* pChild = pNode->GetChildren();

		bool bFoundCreationDate = false;
		bool bFoundLastModified = false;
		bool bFoundGetContentLength = false;
		bool bFoundGetContentType = false;

		while(pChild)
		{
			if(pChild->GetName().Find(wxT("creationdate")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						wxString szVal = pText->GetContent();

						fsItem.m_dtCreationDate.ParseFormat(szVal, wxT("%Y-%m-%dT%H:%M:%S"));
						bFoundCreationDate = true;
					}

					pText = pText->GetNext();
				}
			}

			if(pChild->GetName().Find(wxT("getlastmodified")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						wxString szVal = pText->GetContent();

						fsItem.m_dtLastModified.ParseRfc822Date(szVal);
						bFoundLastModified = true;
					}

					pText = pText->GetNext();
				}
			}

			if(pChild->GetName().Find(wxT("getcontentlength")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						wxString szVal = pText->GetContent();

						if(szVal.ToLong(&fsItem.m_iContentLength))
							bFoundGetContentLength = true;
					}

					pText = pText->GetNext();
				}
			}

			if(pChild->GetName().Find(wxT("getcontenttype")) != -1)
			{
				wxXmlNode* pText = pChild->GetChildren();

				while(pText)
				{
					if(pText->GetType() == wxXML_TEXT_NODE)
					{
						fsItem.m_szContentType = pText->GetContent();
						bFoundGetContentType = true;
					}

					pText = pText->GetNext();
				}
			}

			pChild = pChild->GetNext();
		}

		return bFoundCreationDate && bFoundLastModified;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// wxArrayDAVFs Implementation
//////////////////////////////////////////////////////////////////////
#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxArrayDAVFs);
