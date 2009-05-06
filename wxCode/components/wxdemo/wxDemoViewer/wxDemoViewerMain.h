/***************************************************************
 * Name:      wxDemoViewerMain.h
 * Purpose:   Defines main application window class
 * Author:    Ján Chudý (lenintech@gmail.com)
 * Created:   2009-04-19
 * Copyright: Ján Chudý
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#ifndef WXDEMOVIEWERMAIN_H
#define WXDEMOVIEWERMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <wx/wxhtml.h>
#include <wx/html/helpctrl.h>
#include <wx/fs_arc.h>
#include <wx/process.h>
#include <wx/mimetype.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include "logoPanel.h"
#include "ownerDrawnStatic.h"
#include "wxDemoViewerData.h"
#include "ownerDrawnButton.h"
#include "wxSTC.h"
#include "ownerDrawnSearchCtrl.h"
#include "ownerDrawnComboBox.h"
#include "ownerDrawnSplitter.h"
#include "aboutDialog.h"
#include "skinDialog.h"
#include "wxDemoViewerApp.h"


/*! \brief Class encapsulating main application window. */
class wxDemoViewerFrame: public wxFrame
{
    public:
        /*!
         * \brief Default contructor.
         * \param frame Pointer to a parent window
         * \param title Window title
         * \param ewd Path to a directory where the main executable is located
         */
        wxDemoViewerFrame(wxFrame *frame, const wxString& title, wxString ewd);

        /*! \brief Destructor */
        ~wxDemoViewerFrame();

    private:
        enum
        {
            idMenuQuit = 1000, idMenuAbout, idMenuShowLog, idMenuShowDoc, idMenuSearch, idChangeSkin,
            idDescriptionTab, idSourceTab, idDocTab, idLaunchButton, idSourceSearchNext, idSourceSearchPrev, idKeywordsSearchCode, idKeywordsSearchDocs,
            idMainSearchCtrl, idSourceSearchCtrl,
            idTreeList, idSourceFiles, idKeywords,
            idLaunchedDemo
        };

        /*! \brief Main application data - loaded from xml file. */
        wxDemoViewerData m_appData;

        /*! \brief Pointer to owner drawn wxSplitterWindow. */
        ownerDrawnSplitter *m_pSplitterWnd;

        /*! \brief Pointer to tree control. */
        wxTreeCtrl *m_pTreeCtrl;

        /*! \brief Pointer to wxHtmlWindow used for displaying demo's description. */
        wxHtmlWindow *m_pDescrTab;

        /*! \brief Pointer to panel with logo image. */
        logoPanel *m_pLogoImg;

        /*! \brief Pointer to panel which hold source code viewer related controls. */
        ownerDrawnPanel *m_pSourceCodeTab;

        /*! \brief Pointer to search control used to search demos in tree control. */
        ownerDrawnSearchCtrl *m_pSearchTree;

        /*! \brief Pointer to search control used to search in source code. */
        ownerDrawnSearchCtrl *m_pSearchSourceCode;

        /*! \brief Pointer to sizer used to simulate tabs behaviour. */
        wxBoxSizer *m_pSizerContent;

        /*! \brief Pointer to sizer which hold tabs buttons. */
        wxBoxSizer *m_pSizerTabs;

        /*! \brief Pointer to sizer which holds combo boxes on the source code page. */
        wxBoxSizer *m_pSizerSourceLists;

        /*! \brief Pointer to sizer which holds keywords combo box. */
        wxBoxSizer *m_pSizerKeywordsSub;

        /*! \brief Pointer to description tab button. */
        ownerDrawnButton *m_pTabDescription;

        /*! \brief Pointer to source tab button. */
        ownerDrawnButton *m_pTabSourceCode;

        /*! \brief Pointer to documentation tab button. */
        ownerDrawnButton *m_pTabDoc;

        /*! \brief Pointer to launch button. */
        ownerDrawnButton *m_pLaunchButton;

        /*! \brief Pointer to "search previous" button. */
        ownerDrawnButton *m_pSourceCodeSearchPrev;

        /*! \brief Pointer to "search next" button. */
        ownerDrawnButton *m_pSourceCodeSearchNext;

        /*! \brief Pointer to "search keyword in source code" button. */
        ownerDrawnButton *m_pKeywordSearchCode;

        /*! \brief Pointer to "search keyword in documentation" button. */
        ownerDrawnButton *m_pKeywordSearchDocs;

        /*! \brief Pointer to documentation controler window. */
        wxHtmlHelpWindow *m_pDocWindowTab;

        /*! \brief Pointer to owner draw combo box which holds source code files. */
        ownerDrawnComboBox *m_pSourceFiles;

        /*! \brief Pointer to owner draw combo box which holds keywords. */
        ownerDrawnComboBox *m_pSourceKeywords;

        /*! \brief Pointer to Log window. */
        wxLogWindow *m_pLogWnd;

        /*! \brief Pointer to owner drawn wxStyledTextCtrl used to dispay highlighted source code. */
        wxSTC *m_pSourceCodeViewer;

        /*! \brief Path to a directory where demo files are located. */
        wxString m_sMainWorkingDir;

        /*! \brief Path to a directory where the main executable is located . */
        wxString m_sExecWorkingDir;

        /*! \brief True if demo has documentation. */
        bool m_bHasDoc;

        /*! \brief True if source code page is visible. Used to enable/disable menu item. */
        bool m_bCanSearchCode;

        /*! \brief Pointer to wxProcess used to watch running demo. */
        wxProcess *m_pProcessWatcher;

        /*! \brief Running demo identifier. */
        long m_lPID;


        /*! \brief Create window controls witch style loaded from xml file. */
        void CreateControlsWithStyle();

        /*!
         * \brief Inserts items into tree control
         * \param search If this parameter is given, only items which contains this text will be inserted
         */
        void BuildTree(wxString search = wxEmptyString);

        /*! \brief Selects the first source file in list. */
        void SelectSourceFile();

        /*!
         * \brief Event handler call when window is closed
         * \param event Close event
         */
        void OnClose(wxCloseEvent &event);

        /*!
         * \brief Event handler call when menu item "Quit" is pressed
         * \param event Command event
         */
        void OnQuit(wxCommandEvent &event);

        /*!
         * \brief Event handler call when menu item "About" is pressed
         * \param event Command event
         */
        void OnAbout(wxCommandEvent &event);

        /*!
         * \brief Event handler call when menu item "Show Log" is pressed
         * \param event Command event
         */
        void OnMenuShowLog(wxCommandEvent &event);

        /*!
         * \brief Event handler call when Description tab button is pressed
         * \param event Command event
         */
        void OnDescriptionTabClick(wxCommandEvent &event);

        /*!
         * \brief Event handler call when Source code tab button is pressed
         * \param event Command event
         */
        void OnSourceTabClick(wxCommandEvent &event);

        /*!
         * \brief Event handler call when Documentation tab button is pressed
         * \param event Command event
         */
        void OnDocTabClick(wxCommandEvent &event);

        /*!
         * \brief Event handler call when Search next button is pressed
         * \param event Command event
         */
        void OnSourceSearchNext(wxCommandEvent &event);

        /*!
         * \brief Event handler call when Search previous button is pressed
         * \param event Command event
         */
        void OnSourceSearchPrev(wxCommandEvent &event);

        /*!
         * \brief Event handler call when key is pressed inside code viewer window
         * \param event Command event
         */
        void OnSourceSearchKeyPress(wxCommandEvent &event);

        /*!
         * \brief Event handler call when tree item is selected
         * \param event Tree event
         */
        void OnTreeSelectionChanged(wxTreeEvent &event);

        /*!
         * \brief Event handler call when source file combo box is changed
         * \param event Command event
         */
        void OnSourceFileChanged(wxCommandEvent &event);

        /*!
         * \brief Event handler call when menu item "Documentation" is pressed
         * \param event Command event
         */
        void OnMenuShowDoc(wxCommandEvent &event);

        /*!
         * \brief Event handler call when "Search keyword in source code" button is pressed
         * \param event Command event
         */
        void OnKeywordsSearchCode(wxCommandEvent &event);

        /*!
         * \brief Event handler call when "search keyword in documentation" button is pressed
         * \param event Command event
         */
        void OnKeywordsSearchDocs(wxCommandEvent &event);

        /*!
         * \brief Event handler call when "Launch demo" button is pressed
         * \param event Command event
         */
        void OnLaunchDemo(wxCommandEvent &event);

        /*!
         * \brief Event handler call when key is pressed inside left search control
         * \param event Command event
         */
        void OnMainSearchKeyPress(wxCommandEvent &event);

        /*!
         * \brief Event handler call when menu item "Search" is pressed
         * \param event Command event
         */
        void OnMenuSearch(wxCommandEvent &event);

        /*!
         * \brief Event handler call when menu need to be updated
         * \param event Command event
         */
        void OnUpdateSearchMenu(wxUpdateUIEvent &event);

        /*!
         * \brief Event handler call menu item "Change skin" is pressed
         * \param event Command event
         */
        void OnChangeSkin(wxCommandEvent &event);

        /*!
         * \brief Event handler call when launched demo is terminated
         * \param event Process event
         */
        void OnTerminateDemo(wxProcessEvent &event);

        DECLARE_EVENT_TABLE()
};

#endif // WXDEMOVIEWERMAIN_H
