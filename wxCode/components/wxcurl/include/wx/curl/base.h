/*
*	base.h
*	wxCURL
*
*	Created by Casey O'Donnell on Tue Jun 29 2004.
*	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
*
*  Contributions and Assistance:
*  Ryan Wilcox - Verbose Options
*
*	Licence: wxWidgets Licence
*/

#ifndef _WXCURLBASE_H__INCLUDED_
#define _WXCURLBASE_H__INCLUDED_

#ifdef WXMAKINGDLL_WXCURL
    #define WXDLLIMPEXP_CURL WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_CURL WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_CURL

    // if we do not define this symbol, cURL header will assume
    // a DLL build is being done and will export symbols:
    #ifndef CURL_STATICLIB
        #define CURL_STATICLIB
    #endif
#endif

#include <wx/event.h>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/stream.h>
#include <wx/mstream.h>
#include <wx/intl.h>        // for _()

// The cURL library header:
#include <curl/curl.h>



// base.h: wxWidgets Declarations for Event Types.
//
//////////////////////////////////////////////////////////////////////

BEGIN_DECLARE_EVENT_TYPES()
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CURL, wxCURL_DOWNLOAD_EVENT, 6578)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CURL, wxCURL_UPLOAD_EVENT, 6579)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CURL, wxCURL_BEGIN_PERFORM_EVENT, 6580)
    DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CURL, wxCURL_END_PERFORM_EVENT, 6581)
END_DECLARE_EVENT_TYPES()


class WXDLLIMPEXP_CURL wxCurlBase;


//! Private internal class used as base class for wxCurlDownloadEvent and wxCurlUploadEvent.
class WXDLLIMPEXP_CURL wxCurlProgressEvent : public wxEvent
{
public:
    wxCurlProgressEvent(int id, wxEventType type,
                        wxCurlBase *p = NULL, const wxString &url = wxEmptyString)
        : wxEvent(id, type) { m_pCURL = p; m_szURL = url; }

    //! Returns the URL you are transfering from.
    wxString GetURL() const { return m_szURL; }

    //! Returns the time elapsed since the beginning of the download up
    //! to the time this function is called.
    virtual wxTimeSpan GetElapsedTime() const;

    //! Returns the estimated time for the total download since it started.
    virtual wxTimeSpan GetEstimatedTime() const;

    //! Returns the estimated remaining time to the completion of the download.
    virtual wxTimeSpan GetEstimatedRemainingTime() const;

    //! Returns the current download/upload speed in a human readable format.
    wxString GetHumanReadableSpeed(const wxString &invalid = _("Not available")) const;

    virtual double GetSpeed() const = 0;
    virtual double GetTotalBytes() const = 0;

protected:
    wxCurlBase *m_pCURL;
    wxString m_szURL;
};


// base.h: interface for the wxCurlDownloadEvent class.
//
//////////////////////////////////////////////////////////////////////

//! This event gets posted by wxCURL with a frequent interval during operation
//! (roughly once per second) no matter if data is being transfered or not.
//! Unknown/unused argument values passed to the callback will be set to zero 
//! (like if you only download data, the upload size will remain 0).
//! Use the EVT_CURL_PROGRESS(id, function) macro to intercept this event.
class WXDLLIMPEXP_CURL wxCurlDownloadEvent : public wxCurlProgressEvent
{
public:
    wxCurlDownloadEvent();
    wxCurlDownloadEvent(int id, wxCurlBase *originator,
                        const double& rDownloadTotal, const double& rDownloadNow, 
                        const wxString& szURL = wxEmptyString);
    wxCurlDownloadEvent(const wxCurlDownloadEvent& event);

    virtual wxEvent* Clone() const { return new wxCurlDownloadEvent(*this); }


    //! Returns the number of bytes downloaded so far.
    double GetDownloadedBytes() const { return m_rDownloadNow; }

    //! Returns the total number of bytes to download.
    double GetTotalBytes() const { return m_rDownloadTotal; }

    //! Returns a number in [0;100] range indicating how much has been downloaded so far.
    double GetPercent() const 
        { return m_rDownloadTotal == 0 ? 0 : (100.0 * (m_rDownloadNow/m_rDownloadTotal)); }

    //! Returns the current download speed in bytes/second.
    double GetSpeed() const;


protected:
    double m_rDownloadTotal, m_rDownloadNow;

private:
    DECLARE_DYNAMIC_CLASS(wxCurlDownloadEvent);
};

typedef void (wxEvtHandler::*wxCurlDownloadEventFunction)(wxCurlDownloadEvent&);

#define wxCurlDownloadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxCurlDownloadEventFunction, &func)

#define EVT_CURL_DOWNLOAD(id, fn) \
    wx__DECLARE_EVT1(wxCURL_DOWNLOAD_EVENT, id, wxCurlDownloadEventHandler(fn))


//! This event gets posted by wxCURL with a frequent interval during operation
//! (roughly once per second) no matter if data is being transfered or not.
//! Unknown/unused argument values passed to the callback will be set to zero 
//! (like if you only download data, the upload size will remain 0).
//! Use the EVT_CURL_PROGRESS(id, function) macro to intercept this event.
class WXDLLIMPEXP_CURL wxCurlUploadEvent : public wxCurlProgressEvent
{
public:
    wxCurlUploadEvent();
    wxCurlUploadEvent(int id, wxCurlBase *originator,
                        const double& rUploadTotal, const double& rUploadNow, 
                        const wxString& szURL = wxEmptyString);
    wxCurlUploadEvent(const wxCurlUploadEvent& event);

    virtual wxEvent* Clone() const { return new wxCurlUploadEvent(*this); }


    //! Returns the number of bytes uploaded so far.
    double GetUploadedBytes() const { return m_rUploadNow; }

    //! Returns the total number of bytes to upload.
    double GetTotalBytes() const { return m_rUploadTotal; }

    //! Returns a number in [0;100] range indicating how much has been uploaded so far.
    double GetPercent() const 
        { return m_rUploadTotal == 0 ? 0 : (100.0 * (m_rUploadNow/m_rUploadTotal)); }

    //! Returns the current upload speed in bytes/second.
    double GetSpeed() const;

protected:
    double m_rUploadTotal, m_rUploadNow;

private:
    DECLARE_DYNAMIC_CLASS(wxCurlUploadEvent);
};

typedef void (wxEvtHandler::*wxCurlUploadEventFunction)(wxCurlUploadEvent&);

#define wxCurlUploadEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxCurlUploadEventFunction, &func)

#define EVT_CURL_UPLOAD(id, fn) \
    wx__DECLARE_EVT1(wxCURL_UPLOAD_EVENT, id, wxCurlUploadEventHandler(fn))



// base.h: interface for the wxCurlBeginPerformEvent class.
//
//////////////////////////////////////////////////////////////////////

//! This event get posted before the beginning of any tranfer operation.
//! Use the EVT_CURL_BEGIN_PERFORM(id, function) macro to intercept this event.
class WXDLLIMPEXP_CURL wxCurlBeginPerformEvent : public wxEvent
{
public:
    wxCurlBeginPerformEvent();
    wxCurlBeginPerformEvent(int id, const wxString& szURL);
    wxCurlBeginPerformEvent(const wxCurlBeginPerformEvent& event);

    virtual wxEvent* Clone() const { return new wxCurlBeginPerformEvent(*this); }

    //! Returns the URL you are going to transfering from/to.
    wxString GetURL() const { return m_szURL; }

protected:
    wxString m_szURL;

private:
    DECLARE_DYNAMIC_CLASS(wxCurlBeginPerformEvent);
};

typedef void (wxEvtHandler::*wxCurlBeginPerformEventFunction)(wxCurlBeginPerformEvent&);

#define wxCurlBeginPerformEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxCurlBeginPerformEventFunction, &func)

#define EVT_CURL_BEGIN_PERFORM(id, fn) \
    wx__DECLARE_EVT1(wxCURL_BEGIN_PERFORM_EVENT, id, wxCurlBeginPerformEventHandler(fn))



// base.h: interface for the wxCurlEndPerformEvent class.
//
//////////////////////////////////////////////////////////////////////

//! This event get posted at the end of any tranfer operation.
//! Use the EVT_CURL_END_PERFORM(id, function) macro to intercept this event.
class WXDLLIMPEXP_CURL wxCurlEndPerformEvent : public wxEvent
{
public:
    wxCurlEndPerformEvent();
    wxCurlEndPerformEvent(int id, const wxString& szURL, const long& iResponseCode);
    wxCurlEndPerformEvent(const wxCurlEndPerformEvent& event);

    virtual wxEvent* Clone() const { return new wxCurlEndPerformEvent(*this); }

    //! Returns the URL you are going to transfering from/to.
    wxString GetURL() const { return m_szURL; }

    //! Returns the response code for the operation.
    long GetResponseCode() const { return m_iResponseCode; }

protected:
    wxString	m_szURL;
    long		m_iResponseCode;

private:
    DECLARE_DYNAMIC_CLASS(wxCurlEndPerformEvent);
};

typedef void (wxEvtHandler::*wxCurlEndPerformEventFunction)(wxCurlEndPerformEvent&);

#define wxCurlEndPerformEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxCurlEndPerformEventFunction, &func)

#define EVT_CURL_END_PERFORM(id, fn) \
    wx__DECLARE_EVT1(wxCURL_END_PERFORM_EVENT, id, wxCurlEndPerformEventHandler(fn))


// C Function Declarations for LibCURL
//
//////////////////////////////////////////////////////////////////////

extern "C"
{
    int wxcurl_evt_progress_func(void* ptr, double rDlTotal, double rDlNow, 
                                double rUlTotal, double rUlNow);
    int wxcurl_verbose_stream_write (CURL * crlptr , curl_infotype info, char * cStrMessage, 
                                    size_t msgSize, void * buffer);
    size_t wxcurl_header_func(void *ptr, size_t size, size_t nmemb, void *stream);
    size_t wxcurl_str_write(void* ptr, size_t size, size_t nmemb, void* stream);
    size_t wxcurl_stream_write(void* ptr, size_t size, size_t nmemb, void* stream);
    size_t wxcurl_str_read(void* ptr, size_t size, size_t nmemb, void* stream);
    size_t wxcurl_stream_read(void* ptr, size_t size, size_t nmemb, void* stream);
}

// base.h: interface for the wxCurlBase class.
//
//////////////////////////////////////////////////////////////////////

//! Tells wxCurlBase to send wxCurlDownloadEvent events
#define wxCURL_SEND_PROGRESS_EVENTS     0x01

//! Tells wxCurlBase to send wxCurlBeginPerformEvent and wxCurlEndPerformEvent events
#define wxCURL_SEND_BEGINEND_EVENTS     0x02

//! By default wxCurlBase won't send events
#define wxCURL_DEFAULT_FLAGS            (0)

//! The "easy" unspecialized interface to libCURL.
//! You may want to look at wxCurlFTP, wxCurlHTTP, wxCurlDAV if you want to have a specialized
//! interface for respectively the FTP, HTTP and WebDAV protocols.
//!
//! wxCurlBase represents a libCURL handle to a "session".
//! To use this interface you should:
//! - create an instance of wxCurlBase
//! - use #SetOpt to set libCURL options you're interested to
//!   or alternatively the other various Set*() functions
//! - call #Perform to perform the operation
class WXDLLIMPEXP_CURL wxCurlBase
{
public:
    wxCurlBase(const wxString& szURL = wxEmptyString, 
               const wxString& szUserName = wxEmptyString,
               const wxString& szPassword = wxEmptyString, 
               wxEvtHandler* pEvtHandler = NULL, int id = wxID_ANY,
               long flags = wxCURL_DEFAULT_FLAGS);

    virtual ~wxCurlBase();


    // LibCURL Abstraction Methods - Wrapping curl_easy calls...

    //! Sets a transfer option for this libCURL session instance.
    //! See the curl_easy_setopt() function call for more info.
    bool SetOpt(CURLoption option, ...);

    //! Gets an info from this libCURL session instance.
    //! See the curl_easy_getinfo() function call for more info.
    bool GetInfo(CURLINFO info, ...) const;

    //! Start the operation as described by the options set previously with #SetOpt.
    //! If you set CURLOPT_UPLOAD to zero and the CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA
    //! options to suitable values, a download will be performed.
    //! If you set CURLOPT_UPLOAD to nonzero and the CURLOPT_READFUNCTION and CURLOPT_READDATA
    //! options to suitable values, an upload will be performed.
    //! See the curl_easy_perform() function call for more info.
    bool Perform();


    // Internal handle management:

    //! Initializes the internal libCURL handle. This function is automatically called by
    //! the constructor.
    bool InitHandle();

    //! Closes this libCURL session. This will effectively close all connections this handle 
    //! has used and possibly has kept open until now.
    //! This function is automatically called by the destructor.
    bool CleanupHandle();

    //! Reinit the handle of this libCURL session. Equivalent to call #CleanupHandle and then #InitHandle.
    bool ReInitHandle();

    //! Re-initializes all options previously set on this libCURL session to the default values.
    bool ResetHandle();

    //! Is the underlying libCURL handle valid?
    bool IsOk() const { return m_pCURL != NULL; }


    // Member Data Access Methods (MDA)

    //! Sets the event handler to which the wxCurlDownloadEvent, wxCurlBeginPerformEvent and
    //! wxCurlEndPerformEvent will be sent if they are enabled (see #SetFlags).
    bool			SetEvtHandler(wxEvtHandler* pParent, int id = wxID_ANY);
    wxEvtHandler*	GetEvtHandler() const;
    int             GetId() const;

    //! Sets the "event policy" of wxCURL: if you pass zero, then no events will ever be sent.
    //! The wxCURL_SEND_PROGRESS_EVENTS and wxCURL_SEND_BEGINEND_EVENTS flags instead tell
    //! wxCURL to send respectively the wxCurlDownloadEvent and wxCurlBeginPerformEvent,
    //! wxCurlEndPerformEvent events.
    void		SetFlags(long flags);
    long        GetFlags() const;

    //! Sets the base URL. This allows you to specify a 'base' URL if you
    //! are performing multiple actions.
    void		SetBaseURL(const wxString& szBaseURL);
    wxString	GetBaseURL() const;

    //! Returns the current 'full' URL. I.e. the real URL being used for the transfer.
    wxString    GetCurrentFullURL() const { return m_szCurrFullURL; }

    //! Sets the host Port.  This allows you to specify a specific (non-
    //! default port) if you like.  The value -1 means that the default port
    //! will be used.
    void		SetPort(const long& iPort);
    long		GetPort() const;

    //! Sets the Username. If no username is
    //! needed, simply assign an empty string (which is the default).
    void		SetUsername(const wxString& szUsername);
    wxString	GetUsername() const;

    //! Sets the Password. If no password is
    //! needed, simply assign an empty string (which is the default).
    void		SetPassword(const wxString& szPassword);
    wxString	GetPassword() const;

    //! Returns the header of the response.
    wxString	GetResponseHeader() const;
    wxString	GetResponseBody() const;		// May only contain data on NON-GET calls.
    long		GetResponseCode() const;
    wxString	GetErrorString() const;

    //! Should the proxy be used?
    void		UseProxy(const bool& bUseProxy);
    bool		UseProxy() const;

    //! Sets proxy host.
    void		SetProxyHost(const wxString& szProxyHost);
    wxString	GetProxyHost() const;

    //! Sets the username for proxy access (if needed).
    void		SetProxyUsername(const wxString& szProxyUsername);
    wxString	GetProxyUsername() const;

    //! Sets the password for proxy access (if needed).
    void		SetProxyPassword(const wxString& szProxyPassword);
    wxString	GetProxyPassword() const;

    //! Sets the port for proxy access.
    void		SetProxyPort(const long& iProxyPort);
    long		GetProxyPort() const;

    //! Sets verbose mode on/off. Note that in verbose mode a lot of info
    //! will be printed into an internal memory stream which can be queried
    //! using #GetVerboseStream and #GetVerboseString.
    void		SetVerbose(const bool& bVerbose);
    bool		IsVerbose() const;

    //! Writes into the given stream the verbose messages collected so far.
    bool		GetVerboseStream(wxOutputStream& destStream) const;

    //! Appends to the given stream the verbose messages collected so far.
    bool		GetVerboseString(wxString& szStream) const;

    // Static LibCURL Initialization Methods - Call At Program Init and Close...

    //! Initializes the libCURL. Call this only once at the beginning of your program.
    static void Init();

    //! Clean up libCURL. Call this only once at the end of your program.
    static void Shutdown();

    // Static LibCURL Utility Methods

    static wxDateTime GetDateFromString(const wxString& szDate);
    static wxString GetURLEncodedString(const wxString& szData);
    static wxString GetStringFromURLEncoded(const wxString& szData);

    static wxString GetCURLVersion();

protected:

    // The internal pointer to the libCURL session.
    CURL*                   m_pCURL;

    // VERY IMPORTANT: all these wxString are passed to curl_easy_setopt()
    //                 which does not take ownership of them. Thus we need
    //                 to keep them alive here for all the time m_pCURL is valid:

    // basic connection settings:

    wxString				m_szBaseURL;
    wxString				m_szCurrFullURL;
    wxString				m_szUsername;
    wxString				m_szPassword;
    wxString				m_szUserPass;

    long					m_iHostPort;

    // about received headers:

    wxString				m_szResponseHeader;
    wxString				m_szResponseBody;
    long					m_iResponseCode;

    // about headers to send:

    wxArrayString           m_arrHeaders;
    struct curl_slist*      m_pHeaders;

    // proxy:

    bool					m_bUseProxy;
    wxString				m_szProxyHost;
    wxString				m_szProxyUsername;
    wxString				m_szProxyPassword;
    wxString				m_szProxyUserPass;
    long					m_iProxyPort;

    // debugging/verbose mode:

    bool					m_bVerbose;
    wxMemoryOutputStream	m_mosVerbose;

    char					m_szErrorBuffer[CURL_ERROR_SIZE];

    // for events:
    wxEvtHandler*			m_pEvtHandler;
    int                     m_nId;
    long                    m_nFlags;


protected:      // internal functions

    // CURL Handle Initialization Helper Method
    virtual void	SetCurlHandleToDefaults();
    virtual void	SetHeaders();
    virtual void	ResetHeaders();
    virtual void	ResetResponseVars();

    // Output additional warnings/errors when in verbose mode.
    void DumpErrorIfNeed(CURLcode error) const;
};

#endif // _WXCURLBASE_H__INCLUDED_
