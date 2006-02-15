/////////////////////////////////////////////////////////////////////////////
// Name:        src/msw/mediactrl_realplayer.cpp
// Purpose:     RealPlayer Media Backend for Windows
// Author:      Ryan Norton <wxprojects@comcast.net>
// Modified by:
// Created:     01/29/05
// RCS-ID:      $Id: mediactrl_realplayer.cpp,v 1.1 2006-02-15 19:21:01 ryannpcs Exp $
// Copyright:   (c) Ryan Norton
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

//------------------------Introduction---------------------------------------
// This is the backend for the RealPlayer API. 
//
// The main thing here is that NOTHING is immediate - I.E. when Play()
// returns real hasn't actually switched to the play state. Instead,
// it switches when a few WinAPI events have been shot to it.
//
// This presents a bit of a problem with SetPosition as we have no wx
// there so we use wxYieldIfNeeded() which may not work and could freeze
// an app...
//
// Docs are at
// http://service.real.com/help/library/guides/extend/embed.htm

//===========================================================================
//  DECLARATIONS
//===========================================================================

//---------------------------------------------------------------------------
// Pre-compiled header stuff
//---------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

//disable "cast truncates constant value" for VARIANT_BOOL values
//passed as parameters in VC6
#ifdef _MSC_VER
#pragma warning (disable:4310)
#endif

//---------------------------------------------------------------------------
// MediaCtrl include
//---------------------------------------------------------------------------
#include "wx/mediactrl.h"

//---------------------------------------------------------------------------
// Compilation guard
//---------------------------------------------------------------------------
#if wxUSE_MEDIACTRL

//---------------------------------------------------------------------------
// WX Includes
//---------------------------------------------------------------------------
#include "wx/log.h"         //wxLogDebug
#include "wx/math.h"        //log10 & pow
#include "wx/msw/private.h" //user info and wndproc setting/getting
#include "wx/dcclient.h"
#include "wx/timer.h"
#include "wx/dynlib.h"
#include "wx/stopwatch.h"
#include "wx/msw/ole/activex.h" //wxActiveXContainer/COM includes

//---------------------------------------------------------------------------
//
// wxRealPlayerMediaBackend
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Stuff dumped from "Real Player ActiveX Control Library" (rmoc3260.idl)
//---------------------------------------------------------------------------

// IID/CLSIDS
const IID IID_IRealAudio = {0xCFCDAA01,0x8BE4,0x11CF,{0xB8,0x4B,0x00,0x20,0xAF,0xBB,0xCC,0xFA}};
const CLSID CLSID_RealAudio = {0xCFCDAA03,0x8BE4,0x11CF,{0xB8,0x4B,0x00,0x20,0xAF,0xBB,0xCC,0xFA}};

// IRealAudio
struct IRealAudio : public IDispatch
{
public:
    virtual HRESULT STDMETHODCALLTYPE get_Source(BSTR __RPC_FAR *path) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Source( 
        /* [in] */ BSTR path) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_Console( 
        /* [retval][out] */ BSTR __RPC_FAR *Console) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Console( 
        /* [in] */ BSTR Console) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_Controls( 
        /* [retval][out] */ BSTR __RPC_FAR *Controls) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_Controls( 
        /* [in] */ BSTR Controls) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_NoLabels( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *NoLabels) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_NoLabels( 
        /* [in] */ VARIANT_BOOL NoLabels) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_AutoStart( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *AutoStart) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoStart( 
        /* [in] */ VARIANT_BOOL AutoStart) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_AutoGotoURL( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *AutoStart) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_AutoGotoURL( 
        /* [in] */ VARIANT_BOOL AutoStart) = 0;

    virtual HRESULT STDMETHODCALLTYPE get_WindowName( 
        /* [retval][out] */ BSTR __RPC_FAR *WindowName) = 0;

    virtual /* [helpstring][propput][id] */ HRESULT STDMETHODCALLTYPE put_WindowName( 
        /* [in] */ BSTR WindowName) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetSource( 
        /* [retval][out] */ BSTR __RPC_FAR *path) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetSource( 
        /* [in] */ BSTR lpszNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetConsole( 
        /* [retval][out] */ BSTR __RPC_FAR *Console) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetConsole( 
        /* [in] */ BSTR lpszNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetControls( 
        /* [retval][out] */ BSTR __RPC_FAR *Controls) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetControls( 
        /* [in] */ BSTR lpszNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetNoLabels( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *NoLabels) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetNoLabels( 
        /* [in] */ VARIANT_BOOL bNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetAutoStart( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *AutoStart) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetAutoStart( 
        /* [in] */ VARIANT_BOOL bNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetAutoGotoURL( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *AutoStart) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetAutoGotoURL( 
        /* [in] */ VARIANT_BOOL bNewValue) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetVolume( 
        /* [retval][out] */ short __RPC_FAR *pnVol) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetVolume( 
        /* [in] */ short nVol) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetMute( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbMute) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetMute( 
        /* [in] */ VARIANT_BOOL bMute) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetLoop( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetLoop( 
        /* [in] */ VARIANT_BOOL bVal) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetImageStatus( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbEnable) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetImageStatus( 
        /* [in] */ VARIANT_BOOL bEnable) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsTotal( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsReceived( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsOutOfOrder( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsMissing( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsEarly( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPacketsLate( 
        /* [retval][out] */ long __RPC_FAR *plPackets) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetBandwidthAverage( 
        /* [retval][out] */ long __RPC_FAR *plBandwidth) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetBandwidthCurrent( 
        /* [retval][out] */ long __RPC_FAR *plBandwidth) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoPlayPause( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoStop( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoNextItem( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoPrevItem( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CanPlayPause( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CanStop( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE HasNextItem( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE HasPrevItem( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE HasNextEntry( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE HasPrevEntry( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoNextEntry( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoPrevEntry( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE AboutBox( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EditPreferences( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE HideShowStatistics( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsStatisticsVisible( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *value) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoGotoURL( 
        /* [in] */ BSTR url,
        /* [in] */ BSTR target) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoPlay( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE DoPause( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPosition( 
        /* [retval][out] */ long __RPC_FAR *plPosition) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPlayState( 
        /* [retval][out] */ long __RPC_FAR *plState) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLength( 
        /* [retval][out] */ long __RPC_FAR *plLength) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetTitle( 
        /* [retval][out] */ BSTR __RPC_FAR *bstrTitle) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetAuthor( 
        /* [retval][out] */ BSTR __RPC_FAR *bstrAuthor) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetCopyright( 
        /* [retval][out] */ BSTR __RPC_FAR *bstrCopyright) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetClipWidth( 
        /* [retval][out] */ long __RPC_FAR *plWidth) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetClipHeight( 
        /* [retval][out] */ long __RPC_FAR *plHeight) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CanPlay( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbCanPlay) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE CanPause( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbCanPause) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetPosition( 
        /* [in] */ long lPosition) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetNumLoop( 
        /* [retval][out] */ long __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetNumLoop( 
        /* [in] */ long lVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetCenter( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetCenter( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetNoLogo( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetNoLogo( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetMaintainAspect( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetMaintainAspect( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetBackgroundColor( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetBackgroundColor( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetStereoState( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLiveState( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetShowStatistics( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetShowStatistics( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetShowPreferences( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetShowPreferences( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetShowAbout( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetShowAbout( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetOriginalSize( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetOriginalSize( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetDoubleSize( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetDoubleSize( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetFullScreen( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetFullScreen( void) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEnableContextMenu( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetEnableContextMenu( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEnableOriginalSize( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetEnableOriginalSize( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEnableDoubleSize( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetEnableDoubleSize( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEnableFullScreen( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetEnableFullScreen( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEnableMessageBox( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetEnableMessageBox( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetTitle( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetAuthor( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetCopyright( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetWantKeyboardEvents( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbWantsEvents) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetWantKeyboardEvents( 
        /* [in] */ VARIANT_BOOL bWantsEvents) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetWantMouseEvents( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbWantsEvents) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetWantMouseEvents( 
        /* [in] */ VARIANT_BOOL bWantsEvents) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetNumEntries( 
        /* [retval][out] */ short __RPC_FAR *uNumEntries) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetCurrentEntry( 
        /* [retval][out] */ short __RPC_FAR *uCurrentEntry) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEntryTitle( 
        /* [in] */ short uEntryIndex,
        /* [retval][out] */ BSTR __RPC_FAR *pszTitle) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEntryAuthor( 
        /* [in] */ short uEntryIndex,
        /* [retval][out] */ BSTR __RPC_FAR *pszAuthor) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEntryCopyright( 
        /* [in] */ short uEntryIndex,
        /* [retval][out] */ BSTR __RPC_FAR *pszCopy) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetEntryAbstract( 
        /* [in] */ short uEntryIndex,
        /* [retval][out] */ BSTR __RPC_FAR *pszAbstract) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetCanSeek( 
        /* [in] */ VARIANT_BOOL bCanSeek) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetCanSeek( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pbCanSeek) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetBufferingTimeElapsed( 
        /* [retval][out] */ long __RPC_FAR *uElapsed) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetBufferingTimeRemaining( 
        /* [retval][out] */ long __RPC_FAR *uRemaining) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetConnectionBandwidth( 
        /* [retval][out] */ long __RPC_FAR *uBandwidthSetting) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPreferedLanguageString( 
        /* [retval][out] */ BSTR __RPC_FAR *pLangauge) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPreferedLanguageID( 
        /* [retval][out] */ long __RPC_FAR *nLangID) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetUserCountryID( 
        /* [retval][out] */ long __RPC_FAR *nCountryID) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetNumSources( 
        /* [retval][out] */ short __RPC_FAR *nNumSources) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetSourceTransport( 
        /* [in] */ short nSourceNum,
        /* [retval][out] */ BSTR __RPC_FAR *pszTransport) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetWantErrors( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetWantErrors( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetShuffle( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetShuffle( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetVersionInfo( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastMessage( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastErrorSeverity( 
        /* [retval][out] */ long __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastErrorRMACode( 
        /* [retval][out] */ long __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastErrorUserCode( 
        /* [retval][out] */ long __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastErrorUserString( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetLastErrorMoreInfoURL( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetPreFetch( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetPreFetch( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetRegion( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetRegion( 
        /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetIsPlus( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetConsoleEvents( 
        /* [retval][out] */ VARIANT_BOOL __RPC_FAR *pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetConsoleEvents( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE GetDRMInfo( 
        /* [in] */ BSTR pVal,
        /* [retval][out] */ BSTR __RPC_FAR *pRet) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetShowVideo( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE AddLicense( 
        /* [in] */ BSTR pVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetVideoOverlay( 
        /* [in] */ VARIANT_BOOL bVal) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE EnsureTranscryptionLicense( 
        /* [in] */ BSTR pVal) = 0;
    
};

//###########################################################################
//
//
//  wxRealPlayerMediaBackend
//
//
//###########################################################################

class WXDLLIMPEXP_MEDIA wxRealPlayerMediaBackend : public wxMediaBackendCommonBase
{
public:
    wxRealPlayerMediaBackend();
    virtual ~wxRealPlayerMediaBackend();

    virtual bool CreateControl(wxControl* ctrl, wxWindow* parent,
                                     wxWindowID id,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxValidator& validator,
                                     const wxString& name);

    virtual bool Play();
    virtual bool Pause();
    virtual bool Stop();

    virtual bool Load(const wxString& fileName);
    virtual bool Load(const wxURI& location);
    virtual bool Load(const wxURI& location, const wxURI& proxy);

    bool DoLoad(const wxString& location);
    void FinishLoad();

    virtual wxMediaState GetState();

    virtual bool SetPosition(wxLongLong where);
    virtual wxLongLong GetPosition();
    virtual wxLongLong GetDuration();

    virtual void Move(int x, int y, int w, int h);
    wxSize GetVideoSize() const;

    virtual double GetPlaybackRate();
    virtual bool SetPlaybackRate(double);

    virtual double GetVolume();
    virtual bool SetVolume(double);

    virtual bool ShowPlayerControls(wxMediaCtrlPlayerControls flags);

    void DoGetDownloadProgress(wxLongLong*, wxLongLong*);
    virtual wxLongLong GetDownloadProgress()
    {
        wxLongLong progress, total;
        DoGetDownloadProgress(&progress, &total);
        return progress;
    }
    virtual wxLongLong GetDownloadTotal()
    {
        wxLongLong progress, total;
        DoGetDownloadProgress(&progress, &total);
        return total;
    }

    wxActiveXContainer* m_pAX;
    IRealAudio* m_pRA;
    bool m_bInitial;
    class wxRealPlayerMediaEvtHandler* m_pEventHandler;
    wxSize m_bestSize;

    DECLARE_DYNAMIC_CLASS(wxRealPlayerMediaBackend)
};

class WXDLLIMPEXP_MEDIA wxRealPlayerMediaEvtHandler : public wxEvtHandler
{
public:
    wxRealPlayerMediaEvtHandler(wxRealPlayerMediaBackend *rpb) :
       m_rpb(rpb)
    {
        DoConnect();
    }

    void OnActiveX(wxActiveXEvent& event);
    void DoConnect()
    {
        m_rpb->m_pAX->Connect(m_rpb->m_pAX->GetId(),
            wxEVT_ACTIVEX, 
            wxActiveXEventHandler(wxRealPlayerMediaEvtHandler::OnActiveX),
            NULL, this
                              );
    }
    void DoDisconnect()
    {
        m_rpb->m_pAX->Disconnect(m_rpb->m_pAX->GetId(),
            wxEVT_ACTIVEX, 
            wxActiveXEventHandler(wxRealPlayerMediaEvtHandler::OnActiveX),
            NULL, this
                              );
    }

private:
    wxRealPlayerMediaBackend *m_rpb;

    DECLARE_NO_COPY_CLASS(wxRealPlayerMediaEvtHandler)
};

//===========================================================================
//  IMPLEMENTATION
//===========================================================================

//---------------------------------------------------------------------------
//
// wxRealPlayerMediaBackend
//
//---------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxRealPlayerMediaBackend, wxMediaBackend);

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend Constructor
//---------------------------------------------------------------------------
wxRealPlayerMediaBackend::wxRealPlayerMediaBackend()
                 :m_pRA(NULL), m_bInitial(true)
{
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend Destructor
//---------------------------------------------------------------------------
wxRealPlayerMediaBackend::~wxRealPlayerMediaBackend()
{
    if(m_pRA)
    {
        m_pAX->DissociateHandle();
        delete m_pAX;
        m_pRA->Release();
        m_ctrl->PopEventHandler(true);
    }
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::CreateControl
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::CreateControl(wxControl* ctrl, wxWindow* parent,
                                     wxWindowID id,
                                     const wxPoint& pos,
                                     const wxSize& size,
                                     long style,
                                     const wxValidator& validator,
                                     const wxString& name)
{
    // Determine if the IRealAudio interface is available
    // (I.E. RealPlayer is installed)
    if( ::CoCreateInstance(CLSID_RealAudio, NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_IRealAudio, (void**)&m_pRA) != 0 )
         return false;


    //
    // Create window
    // By default wxWindow(s) is created with a border -
    // so we need to get rid of those
    //
    // Since we don't have a child window like most other
    // backends, we don't need wxCLIP_CHILDREN
    //
    if ( !ctrl->wxControl::Create(parent, id, pos, size,
                            (style & ~wxBORDER_MASK) | wxBORDER_NONE,
                            validator, name) )
        return false;

    //
    // Now create the ActiveX container along with the media player
    // interface and query them
    //
    m_ctrl = wxStaticCast(ctrl, wxMediaCtrl);
    m_pAX = new wxActiveXContainer(ctrl,
                IID_IRealAudio,
                m_pRA);

    // Connect for events
    m_pEventHandler = new wxRealPlayerMediaEvtHandler(this);
    m_ctrl->PushEventHandler(m_pEventHandler);

    //by default true
    m_pRA->SetAutoStart(VARIANT_FALSE);
    m_pRA->SetAutoGotoURL(VARIANT_FALSE);
    //by default enabled
    wxRealPlayerMediaBackend::ShowPlayerControls(wxMEDIACTRLPLAYERCONTROLS_NONE);
    //TODO: Need this? AM is 0.5 by default...
    m_pRA->SetVolume(100); 

    //Should make sure we can get media information
    //before playing anything and put the media in pause 
    //position the first play
    m_pRA->SetPreFetch(VARIANT_TRUE);

    m_pRA->SetWantErrors(VARIANT_FALSE);
    m_pRA->SetEnableMessageBox(VARIANT_FALSE);

    // don't erase the background of our control window so that resizing is a
    // bit smoother
    m_ctrl->SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // success
    return true;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Load (file version)
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Load(const wxString& fileName)
{
    return DoLoad(wxString("file://") + fileName);
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Load (URL Version)
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Load(const wxURI& location)
{
    return DoLoad(location.BuildURI());
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Load (URL Version with Proxy)
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Load(const wxURI& WXUNUSED(location), 
                                    const wxURI& WXUNUSED(proxy))
{
        return false;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::DoLoad
//
// Called by all functions - this actually renders
// the file
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::DoLoad(const wxString& location)
{
    HRESULT hr;    

    //Stop the media first, otherwise
    //CanPlay will return false

    //Disconnect ActiveX handler first so client
    //does not get an extra stop event
    m_pEventHandler->DoDisconnect();

    if( SUCCEEDED(m_pRA->DoStop()) )
    {
        //FORCE STOP - real will fail at DoPlay unless we stop 
        //completely
        //TODO: Maybe connect, catch the stop event,
        //      then finishload and disconnect?

        //wait for the interface to reach the desired state
        long lState;
        m_pRA->GetPlayState(&lState);
        while(lState != 0 && wxYieldIfNeeded())
        {
          m_pRA->GetPlayState(&lState);
        }
        //
    }

    //Reconnect for ActiveX events
    m_pEventHandler->DoConnect();

    //Set the source of the IRealAudio interface
    //(real loading takes place in DoPlay)
    hr = m_pRA->SetSource(wxBasicString(location).Get());

    if(FAILED(hr))
    {
        wxLogSysError(wxT("Couldn't set source"));
        return false;
    }

    //Can we play the media? Note that 
    //you could play here and it would bring up the
    //codec dialog from realplayer. 
    VARIANT_BOOL vbCanPlay;
    hr = m_pRA->CanPlay(&vbCanPlay);

    if(FAILED(hr) || vbCanPlay == VARIANT_FALSE)
    {
        if(vbCanPlay == VARIANT_FALSE &&
            m_bInitial)
        {
            m_bInitial = false;
            //RealPlayer needs to process some MSW messags to 
            //"Start Up"
            //So empty the MSW thread message queue using 
            //::PeekMessage (TODO: Secondary thread?)
            MSG msg;
            while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }

            hr = m_pRA->CanPlay(&vbCanPlay);

            if(FAILED(hr) || vbCanPlay == VARIANT_FALSE)
            {
                wxLogSysError(wxT("Cannot play media"));
                return false;   
            }
        }
        else
        {
            wxLogSysError(wxT("Cannot play media"));
            return false;
        }
    }

    // Not on the initial file anymore
    m_bInitial = false;
    
    // Doesn't REALLY play - just loads the media
    // and pauses - we need to call DoPlay again
    // to actually play anything
    hr = m_pRA->DoPlay();

    if(FAILED(hr))
    {
        wxLogSysError(wxT("Couldn't initial play"));
        return false;
    }

    return true;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::FinishLoad
//
// Called by our wxRALoadTimer when the
// embedded RealPlayer tells its the media is ready to play.
//
// Here we get the original size of the video and
// send the loaded event to our watcher :).
//---------------------------------------------------------------------------
void wxRealPlayerMediaBackend::FinishLoad()
{
    //Get the original video size
    m_pRA->GetClipWidth((long*)&m_bestSize.x);
    m_pRA->GetClipHeight((long*)&m_bestSize.y);


    NotifyMovieLoaded();
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::ShowPlayerControls
//
// FIXME: This is really funky. For one thing sometimes
// the controls take up the whole screen (realplayer either
// chooses or the user presses something) and either way
// the video doesn't show up in the transition
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::ShowPlayerControls(wxMediaCtrlPlayerControls flags)
{
    if(!flags)
    {
        return SUCCEEDED(m_pRA->SetControls(
                            wxBasicString(wxT("ImageWindow")).Get()
                                            )
                        );
    }
    else
    {
        return SUCCEEDED(m_pRA->SetControls(
                            wxBasicString(wxT("simple")).Get()
                                            )
                        );
    }
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Play
//
// Plays the stream.  
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Play()
{
    //Actually try to play the movie
    HRESULT hr = m_pRA->DoPlay();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    wxLogSysError(wxT("Could not play media!"));
    return false;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Pause
//
// Pauses the stream.
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Pause()
{
    long lState;
    m_pRA->GetPlayState(&lState);
    if(lState == 4)
        return true;

    HRESULT hr = m_pRA->DoPause();
    if(SUCCEEDED(hr))
    {
        return true;
    }
    wxLogSysError(wxT("Could not pause media!"));
    return false;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Stop
//
// Stops the stream.
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::Stop()
{
    HRESULT hr = m_pRA->DoStop();
    if(SUCCEEDED(hr))
    {
        //Seek to beginning
        wxRealPlayerMediaBackend::SetPosition(0);
        return true;
    }
    wxLogSysError(wxT("Could not stop media!"));
    return false;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::SetPosition
//
// Sets the media position in the IRealAudio interface
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::SetPosition(wxLongLong where)
{
    HRESULT hr = m_pRA->SetPosition(where.GetValue());
    if(FAILED(hr))
    {
        wxLogSysError(wxT("Could not set position from realplayer"));
        return false;
    }

    //
    //FIXMEFIXMEFIXME!:We are blocking here
    //

    //Wait for the control to reach the state we want
    long lState;
    m_pRA->GetPlayState(&lState);
    while(lState == 5 && wxYieldIfNeeded())
    {
      m_pRA->GetPlayState(&lState);
    }
    //

    return true;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetPosition
//
// Obtain position in media from realplayer
//---------------------------------------------------------------------------
wxLongLong wxRealPlayerMediaBackend::GetPosition()
{
    long lPos;
    HRESULT hr = m_pRA->GetPosition(&lPos);
    if(FAILED(hr))
    {
        wxLogSysError(wxT("Could not obtain media position from realplayer"));
        return 0;
    }

    wxLongLong ll;
    ll.Assign(lPos);

    return ll;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetVolume
//
// Gets the volume through the IRealAudio interface - 
// realplayer volume is on a scale of 0-100
//---------------------------------------------------------------------------
double wxRealPlayerMediaBackend::GetVolume()
{
    short sVolume;
    HRESULT hr = m_pRA->GetVolume(&sVolume);
    if(FAILED(hr))
        {
        wxLogSysError(wxT("Could not obtain volume from RealPlayer"));
        return 0.0;
    }
    return ((double)sVolume) / 100.0;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::SetVolume
//
// Sets the volume through the IRealAudio interface - 
// realplayer volume is on a scale of 0-100
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::SetVolume(double dVolume)
{
    short sVolume = (short)(dVolume*100.0);
    HRESULT hr = m_pRA->SetVolume( sVolume );
    if(FAILED(hr))
    {
        wxLogSysError(wxT("Could not set volume from RealPlayer!"));
        return false;
        }
    return true;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetDuration
//
// Obtains the length of the media from realplayer
// (time signature is in milliseconds like ours)
//---------------------------------------------------------------------------
wxLongLong wxRealPlayerMediaBackend::GetDuration()
{
    long lDuration;
    HRESULT hr = m_pRA->GetLength(&lDuration);
    if(FAILED(hr))
    {
        wxLogSysError(wxT("Could not obtain length from RealPlayer!"));
        return 0;
    }

    //h,m,s,milli - outdur is in 1 second (double)
    wxLongLong ll;
    ll.Assign(lDuration);

    return ll;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetState
//
// Obtains the state of the movie from realplayer
//---------------------------------------------------------------------------
wxMediaState wxRealPlayerMediaBackend::GetState()
{
    long lState;
    HRESULT hr = m_pRA->GetPlayState(&lState);
    if(FAILED(hr))
    {
        wxLogSysError(wxT("Could not obtain state from RealPlayer!"));
        return wxMEDIASTATE_STOPPED;
    }

    switch(lState)
        {
    case 0: //stopped
    case 1: //contacting
    case 2: //loading
        return wxMEDIASTATE_STOPPED;
    case 3: //playing
    case 5: //seeking
        return wxMEDIASTATE_PLAYING;
    case 4: //paused
        return wxMEDIASTATE_PAUSED;

        default:
        return wxMEDIASTATE_STOPPED;
    }
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetPlaybackRate
//
// Not implemented
//---------------------------------------------------------------------------
double wxRealPlayerMediaBackend::GetPlaybackRate()
{
    return 0.0;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::SetPlaybackRate
//
// Not implemented
//---------------------------------------------------------------------------
bool wxRealPlayerMediaBackend::SetPlaybackRate(double WXUNUSED(dRate))
{
    return false;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetDownloadXXX
//
// Not implemented
//---------------------------------------------------------------------------
void wxRealPlayerMediaBackend::DoGetDownloadProgress(
                                             wxLongLong* pLoadProgress,
                                             wxLongLong* pLoadTotal)
{
    *pLoadProgress = 0;
    *pLoadTotal = 0;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::GetVideoSize
//
// Obtains the cached original video size
//---------------------------------------------------------------------------
wxSize wxRealPlayerMediaBackend::GetVideoSize() const
{
    return m_bestSize;
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaBackend::Move
//
// We take care of this in our redrawing
//---------------------------------------------------------------------------
void wxRealPlayerMediaBackend::Move(int WXUNUSED(x), int WXUNUSED(y),
                            int WXUNUSED(w), int WXUNUSED(h))
{
}

//---------------------------------------------------------------------------
// wxRealPlayerMediaEvtHandler::OnActiveX
//
// Handle events sent from our activex control (IRealAudio).
//
// The weird numbers in the switch statement here are "dispatch ids"
// (the numbers in the id field like ( id(xxx) ) ) from rmoc3260.idl
//---------------------------------------------------------------------------
void wxRealPlayerMediaEvtHandler::OnActiveX(wxActiveXEvent& event)
{
    switch(event.GetDispatchId())
    {
    // OnStateChange - GOING to switch states
    // Param 1 -- old state
    // Param 2 -- new state
    case 0x00000412: 
        if(event.ParamCount() >= 2)
		{
            switch (event[1].GetInteger())
			{
            case 0: //stopping
                if(m_rpb->wxRealPlayerMediaBackend::GetPosition() ==
                   m_rpb->wxRealPlayerMediaBackend::GetDuration())
                {
                    if ( m_rpb->SendStopEvent() )
                    {
                        //Seek to beginning of movie
                        m_rpb->wxRealPlayerMediaBackend::SetPosition(0);

                        //send the event to our child
                        m_rpb->QueueFinishEvent();
					}
                }
                else
				{
                    m_rpb->QueueStopEvent();
                }
                break;
            case 3: //play
                m_rpb->QueuePlayEvent();
                break;
            case 4: //pause 
                m_rpb->QueuePauseEvent();
                break;
            default: //loading etc.
                break;
			}
		}
        else
            event.Skip();
        break;

    // OnPreFetchComplete - Initial loading done. (no params)
    case 0x000003fd: 
        m_rpb->FinishLoad();
        break;
    default:
        event.Skip();
        break;
    }
}

//---------------------------------------------------------------------------
// End of wxRealPlayerMediaBackend
//---------------------------------------------------------------------------

//in source file that contains stuff you don't directly use
//#include "wx/html/forcelnk.h"
//FORCE_LINK_ME(wxmediabackend_realplayer)

//---------------------------------------------------------------------------
//  End wxMediaCtrl Compilation Guard and this file
//---------------------------------------------------------------------------
#endif //wxUSE_MEDIACTRL