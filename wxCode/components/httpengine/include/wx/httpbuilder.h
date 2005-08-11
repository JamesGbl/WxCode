/////////////////////////////////////////////////////////////////////////////
// Name:        httpbuilder.h
// Purpose:     HTTP builder assists with building data to send to a web server
//				including the query string, cookies, urlencoded post variables, and multipart post data.
// Author:      Angelo Mandato
// Created:     2005/08/10
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Angelo Mandato (http://www.spaceblue.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_HTTPBUILDER_H_
#define _WX_HTTPBUILDER_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "httpbuilder.h"
#endif

#define HTTPBUILDER_BASE64 // Use base 64 for HTTP Authentication
#define HTTPBUILDER_BOUNDARY_LENGTH 40 // Boundary length for multipart posts
#define HTTPBUILDER_VERSION wxT("1.2") // Version of wxHTTPBuilder

#define HTTPBUILDER_NEWLINE wxT("\r\n") // For future use

#include <wx/protocol/http.h>
#include "wx/httpenginedef.h"


class WXDLLIMPEXP_HTTPENGINE wxHTTPBuilder : public wxHTTP
{
public:

  // Constructor
  wxHTTPBuilder();

  // Destructor
  ~wxHTTPBuilder();

  // variable and authentication types
  enum wxHTTP_Type
  {
    wxHTTP_TYPE_ANY = 0, // Any except cookie
    wxHTTP_TYPE_GET,
    wxHTTP_TYPE_POST,
    wxHTTP_TYPE_FILE,
    wxHTTP_TYPE_MULTIPARTDATA,
    wxHTTP_TYPE_COOKIE,
    wxHTTP_TYPE_COOKIERAW
  };

  enum wxHTTP_Auth
  {
    wxHTTP_AUTH_NONE = 0,
    wxHTTP_AUTH_BASIC
  };

  // Content type functions:
	void InitContentTypes(void);
	wxString GetContentType( const wxString &filename );
  bool RemoveContentType( const wxString &abv );
  void AddContentType( const wxString &abv, const wxString &desc ) { RemoveContentType(abv); m_ContentTypeAbvs.Add(abv); m_ContentTypeDescs.Add(desc); };
	
  // Proxy server settings
  void HttpProxyAuth( const wxString &user, const wxString &password ) { m_HttpProxyUser = user, m_HttpProxyPassword = password; };
	void HttpProxy( const wxString &host, const int &port );
  void UseHttpProxy(const bool &use ) { m_UseHttpProxy = use; m_HttpProxyUsed = false; };
  virtual void HttpProxyExceptions(const wxString &exceptions, const wxString &delim = wxT(",")); // { m_HttpProxyExceptions = exceptions; m_HttpProxyExceptionsDelim = delim; };
  // virtual because other implementations may wish to parse the host names differently.

  // Basic authenfication:
  void Authenticate( const wxString &user, const wxString &password, wxHTTP_Auth type = wxHTTP_AUTH_BASIC);
	
	// Stop what we are doing gracefully, let it read the last received bytes then stop
  void Stop(const bool &stop = true) { m_Stop = stop; };

  // Get the collected values url encoded
  wxString GetPostBuffer(void);
	wxString GetQueryString(bool includeAny = false);
  wxString GetCookieString(void);

	// Handle the name/value pairs for the GET/POST/COOKIEs
	wxString GetValue( const wxString &name, wxHTTP_Type type = wxHTTP_TYPE_ANY );
	bool SetValue( const wxString &name, const wxString &value, wxHTTP_Type type = wxHTTP_TYPE_ANY );
  bool RemoveValue( wxString &name, wxHTTP_Type type );

  // Same as SetValue except it also verifies no invalid characters are used for the cookie name or value
	bool SetRequestCookie( const wxString &name, const wxString &value, const bool &urlencode = false );

  
  ////////////////////////////////////////////////////////////
  // Static methods that may be used for other purposes
  ////////////////////////////////////////////////////////////

  // URL coding and characters:
	static wxString URLEncode( const wxString &value );
	static wxString URLDecode( const wxString &value );
  
  // Convert chars to HTML special characters
  static wxString HTMLSpecialChars( const wxString &value, const bool &SingleQuotes = false, const bool &DoubleQuotes = true);

  // Base 64 coding:
	static wxString Base64Decode( const wxString &value );
	static wxString Base64Encode( const wxString &value );
  
  // Parsing URLs:
  static bool ParseURL( const wxString &url, wxString &protocol, wxString &host, int &port, wxString &request, wxString &username, wxString &password );
  
  ////////////////////////////////////////////////////////////
  // End static methods
  ////////////////////////////////////////////////////////////

  // Send built HTTP request and save the returned information as a file
	bool SaveFile(const wxString& filename, const wxString& path, const wxString& tempDirOrPrefix = wxEmptyString);

  // Send the built HTTP request and return a stream of the returned data from server.
  wxInputStream* GetInputStream(const wxString& url, const wxString& tempDirOrPrefix = wxEmptyString);

  // Send the build HTTP requeset and return a string returned from the server.
  wxString GetInputString(const wxString &url, const wxString& tempDirOrPrefix = wxEmptyString);


  // Error message
  wxString GetLastError(void) { return m_error; };
  wxString GetStatusCode(void);
  wxString GetResponseString(void);

  int GetBytesRead(void) { return m_bytesRead; };
  int GetBytesSent(void) { return m_bytesSent; };

protected:
  
  // helper functions that may be accessed by an extended class
  bool SendRequest(const wxString& path, const wxString& tempDirOrPrefix = wxEmptyString );
  wxString CreateBoundary( const int &length );

  // Bytes Read 
  int           m_bytesRead;
  int           m_bytesSent;

  // Stop what we are doing flag
  bool          m_Stop;

private:
	bool UseProxyForHost( const wxString &host);

  // Class specific funtions:
  int FindNamePosition( const wxString &name, wxHTTP_Type type = wxHTTP_TYPE_ANY );
  wxInputStream* GetPostBufferStream(const wxString &szTempFile);
  static wxString HexFromInt( const int &value );
	static int IntFromHex( const wxString &chars);
  static int Rand(void);
	static void SRand(void);

  // Set of virtual functions that a thread may want to use
  virtual void AddBytesRead(const int &bytes) { m_bytesRead += bytes; };
  virtual void AddBytesSent(const int &bytes) { m_bytesSent += bytes; };
  virtual bool StopCheck(void) { return m_Stop; };

  // GET/POST/COOKIE/FILE names and values:
  wxArrayString m_Fields;
  wxArrayString m_Values;
  wxArrayInt    m_Types;
  int           m_MultiPartCount; // Future use

  // Basic Authentication
  wxString      m_authUser;
  wxString      m_authPassword;
  wxHTTP_Auth   m_authType;

  // Proxy server settings
  bool          m_UseHttpProxy;
  wxString      m_HttpProxyHost;
  int           m_HttpProxyPort;
  wxString      m_HttpProxyUser;
  wxString      m_HttpProxyPassword;
  wxArrayString m_HttpProxyExceptionsArray;
  bool          m_HttpProxyUsed;


  // Content types
  wxArrayString m_ContentTypeDescs;
  wxArrayString m_ContentTypeAbvs;
  
  // Error string
  wxString      m_error;
};

#endif
