//
// Copyright (C) 2007 Arne Steinarson <arst at users dot sourceforge dot net>
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute
// it freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must
//    not claim that you wrote the original software. If you use this
//    software in a product, an acknowledgment in the product
//    documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must
//    not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source
//    distribution.
//


#ifndef WX_SIZER_UTILS_H
#define WX_SIZER_UTILS_H


bool TradeSpace( wxBoxSizer *pbsz, int ix_grow, int ix_shrink, int delta );

// Vert or Horz sizer? 
int GetOrientation( wxSizer *ps );

// Return sum of proportions, or proportion for item self
int GetPropSum( wxSizerItem *psi );

// Include borders with rectangle
wxRect GetRectWithBorders( wxSizerItem *psi );


class SizerIter{
public:
    SizerIter( wxSizer *sizer, bool forward=true );
    void InitSetDirection( bool forward=true );

    wxSizerItem *Get();
    void        Step();         // Step forward
    void        StepBack();     // Step backwards in list
    wxSizerItem *GetNext();     // Get without stepping
    wxSizerItem *GetPrevious(); // Get without stepping
    int         GetIndex();

public:
    wxSizer *m_sizer;
    int     m_ix;
    wxSizerItemList::compatibility_iterator m_llnode;
};



#endif // WX_SIZER_UTILS_H


