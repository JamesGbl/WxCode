// persist-mdi.h

#ifndef _WX_PERSIST_MDI_H_
#define _WX_PERSIST_MDI_H_

#ifdef skip

#include "wx/persist/window.h"

#include "wx/mdi.h"

// ----------------------------------------------------------------------------
// string constants used by wxPersistentMDI
// ----------------------------------------------------------------------------

#define wxPERSIST_MDI_KIND "Book"

#define wxPERSIST_MDI_SELECTION "Selection"

// ----------------------------------------------------------------------------
// wxPersistentMDI: supports saving/restoring book control selection
// ----------------------------------------------------------------------------

class wxPersistentMDI : public wxPersistentWindow<wxMDIParentFrame>
{
public:
    wxPersistentMDI(wxMDIParentFrame *book)
        : wxPersistentWindow<wxMDIParentFrame>(book)
    {
    }

    virtual void Save() const
    {
        //SaveValue(wxPERSIST_MDI_SELECTION, Get()->GetSelection());
    }

    virtual bool Restore()
    {
        long sel;
        if ( RestoreValue(wxPERSIST_MDI_SELECTION, &sel) )
        {
            wxMDIParentFrame * const book = Get();
            /*
            if ( sel >= 0 && (unsigned)sel < book->GetPageCount() )
            {
                book->SetSelection(sel);
                return true;
            }
            */
        }

        return false;
    }

    virtual wxString GetKind() const { return wxPERSIST_MDI_KIND; }
};

inline wxPersistentObject *wxCreatePersistentObject(wxMDIParentFrame *book)
{
    return new wxPersistentMDI(book);
}

#endif // skip

#endif // _WX_PERSIST_MDI_H_
