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


#include <wx/sizer.h>

#include "wx/SizerUtils.h"

int GetOrientation( wxSizer *ps ){
	int dir = -1;
    wxBoxSizer *cbp = wxDynamicCast(ps,wxBoxSizer);
    if( cbp ) dir = cbp->GetOrientation();
    return dir;
}


int GetPropSum( wxSizerItem *psi ){
	wxASSERT( psi );
	if( !psi->GetSizer() ) return psi->GetProportion();
	int ps = 0;
	for( SizerIter sit(psi->GetSizer()); sit.Get(); sit.Step() )
		if( sit.Get()->IsShown() )
			ps += sit.Get()->GetProportion();
	return ps;
}

wxRect GetRectWithBorders( wxSizerItem *psi ){
    wxASSERT( psi );
	wxRect r = psi->GetRect();
	int flag = psi->GetFlag();
	int b = psi->GetBorder();
	if( flag&wxLEFT ) r.x-=b, r.width+=b;
	if( flag&wxRIGHT ) r.width+=b;
	if( flag&wxTOP ) r.y-=b, r.height+=b;
	if( flag&wxBOTTOM ) r.height+=b;
	return r;
}


// We want to grow one item and shrink another one by 'delta' size
bool TradeSpace( wxBoxSizer *pbsz, int ix_grow, int ix_shrink, int delta ){

    // Check incoming args, convert to sizeritems
    int ix = ix_grow;
    if( ix_grow>ix_shrink ) ix_grow=ix_shrink, ix_shrink=ix;
    wxSizerItem *itg = pbsz->GetItem( ix_grow );
    wxSizerItem *its = pbsz->GetItem( ix_shrink );
    if( itg==its || !itg || !its ) return false;
    if( !delta ) return true;

    // For now, this algo needs that the ones we're trading space between
    // have proportion>0.
    if( !itg->GetProportion() || !its->GetProportion() )
        return false;				

    // retrieve some basic values to work with
    wxSize sz0, sz2, msz0, msz2;
    bool vert = pbsz->GetOrientation()==wxVERTICAL;
    msz0 = itg->GetMinSizeWithBorder( );
    msz2 = its->GetMinSizeWithBorder( );
    sz0 = itg->GetSize( );
    sz2 = its->GetSize( );

    // If we're going past min sizes of components,
    // then adjust delta size value
    if( vert && sz0.GetHeight()+delta<msz0.GetHeight() )
        delta = msz0.GetHeight()-sz0.GetHeight();
    if( vert && sz2.GetHeight()-delta<msz2.GetHeight() )
        delta = -(msz2.GetHeight()-sz2.GetHeight());
    if( !vert && sz0.GetWidth()+delta<msz0.GetWidth() )
        delta = msz0.GetWidth()-sz0.GetWidth();
    if( !vert && sz2.GetWidth()-delta<msz2.GetWidth() )
        delta = -(msz2.GetWidth()-sz2.GetWidth());
    if( !delta ) return true;

    // Change sizes and coords
    wxPoint pos0 = itg->GetPosition();
    wxPoint pos2 = its->GetPosition();
    if( vert ){
        sz0.SetHeight( sz0.GetHeight()+delta );
        sz2.SetHeight( sz2.GetHeight()-delta );
        pos2.y += delta;
    } else {
        sz0.SetWidth( sz0.GetWidth()+delta );
        sz2.SetWidth( sz2.GetWidth()-delta );
        pos2.x += delta;
    }

    // Now actually move/resize parts
    itg->SetDimension( pos0, sz0 );
    its->SetDimension( pos2, sz2 );

    // Now adjust proportions
    int p0 = itg->GetProportion();
    int p2 = its->GetProportion();
    
    
    // Make sure we get fresh values on fixed size
    wxSize bs_sz_min = pbsz->CalcMin();
    wxSize bs_sz = pbsz->GetSize();
    
    // calculate proportion sum
    int prop_sum = 0;
    for ( wxSizerItemList::const_iterator i = pbsz->GetChildren().begin();
          i != pbsz->GetChildren().end();
          ++i )
    {
        wxSizerItem * const item = *i;
        if ( !item->IsShown() ) continue;
        prop_sum += item->GetProportion();
    }    
    
    int free_sum = vert ? bs_sz.y-bs_sz_min.y : bs_sz.x-bs_sz_min.x;

    if( free_sum>0 ){
            int np0 = (int)((vert ? sz0.GetHeight()-msz0.GetHeight() :
                        sz0.GetWidth()-msz0.GetWidth()) * (float)prop_sum/free_sum + 0.5);
            int np2 = (int)((vert ? sz2.GetHeight()-msz2.GetHeight() :
                        sz2.GetWidth()-msz2.GetWidth()) * (float)prop_sum/free_sum + 0.5);
            // Never let them drop down to zero, since they are used as flags
            if( !np0 ) np0 = 1;
            if( !np2 ) np2 = 1;
            itg->SetProportion( np0 );
            its->SetProportion( np2 );
    }

    // Last, we must change position for all items between the two
    wxSizerItem *si;
    wxSize sz;
    wxPoint pt;
    ix = 0;
    for( wxSizerItemList::compatibility_iterator node = pbsz->GetChildren().GetFirst();
         node && (si=node->GetData());
         node=node->GetNext(), ix++ ){
        if( !si->IsShown() ) continue;
        if( ix>ix_grow && ix<ix_shrink ){
            pt = si->GetPosition( );
            sz = si->GetSize( );
            if( vert ) pt.y += delta;
            else pt.x += delta;
            si->SetDimension( pt, sz );
        }
    }

    return true;
}


SizerIter::SizerIter( wxSizer *sizer, bool forward ){
    wxASSERT( sizer );
    m_sizer = sizer;
    InitSetDirection( forward );
}

void SizerIter::InitSetDirection( bool forward ){
    if( forward ){
        m_llnode = m_sizer->GetChildren().GetFirst();
        m_ix = 0;
    }else{
        m_llnode = m_sizer->GetChildren().GetLast();
        m_ix = m_sizer->GetChildren().GetCount()-1;
    }
}

wxSizerItem *SizerIter::Get(){
    if( !m_llnode ) return NULL;
    return m_llnode->GetData();
}

int SizerIter::GetIndex(){
    return m_ix;
}

void SizerIter::Step(){
    if( !m_llnode ) return;
    m_llnode = m_llnode->GetNext();
    m_ix++;
}

void SizerIter::StepBack(){
    if( !m_llnode ) return;
    m_llnode = m_llnode->GetPrevious();
    m_ix--;
}

wxSizerItem *SizerIter::GetNext(){
    if( !m_llnode ) return NULL;
    return m_llnode->GetNext() ? m_llnode->GetNext()->GetData() : NULL;
}

wxSizerItem *SizerIter::GetPrevious(){
    if( !m_llnode ) return NULL;
    return m_llnode->GetPrevious() ? m_llnode->GetPrevious()->GetData() : NULL;
}


