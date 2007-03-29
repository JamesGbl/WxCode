/////////////////////////////////////////////////////////////////////////////
// Name:        fifo.cpp
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id$
// Copyright:   (c) 2006 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "wx/ctb/fifo.h"

fifo::fifo(size_t size) :
    m_size(size)
{
    m_begin = new char[size];
    m_end = m_begin + m_size;
    m_rdptr = m_wrptr = m_begin;
};

fifo::~fifo()
{
    delete m_begin;
};

void fifo::clear()
{
    m_rdptr = m_wrptr = m_begin;
};

int fifo::get(char* ch)
{
    if(m_rdptr != m_wrptr) {
	   *ch = *m_rdptr++;
	   if(m_rdptr >= m_end) {
		  m_rdptr = m_begin;
	   }
	   return 1;
    }
    return 0;
};

size_t fifo::items()
{
    char* tmp_wrptr = m_wrptr;
    // der rdptr wird sich normalerweise waehrend Aufruf
    // von ncount() nicht aendern, da ncount() nur vom
    // Lesetask aufgerufen wird.
    char* tmp_rdptr = m_rdptr;

    // beide gleich, kein Zeichen in der fifo
    if(tmp_wrptr == tmp_rdptr) {
	   return 0;
    }
    // wrptr steht vor rdptr, einfache Differenz ergibt
    // Anzahl der noch in der fifo stehenden Zeichen
    if(tmp_wrptr > tmp_rdptr) {
	   return (tmp_wrptr - tmp_rdptr);
    }
    // wrptr steht hinter rdptr, ist also einmal rund
    // gelaufen, fifo Groesse abzueglich Differenz beider
    // Zeiger ist dann Anzahl der Zeichen in der fifo
    else {
	   return (m_size - (tmp_rdptr - tmp_wrptr));
    }
};

int fifo::put(char ch)
{
    // wrptr temporaerem Zeiger zuweisen, der dann in
    // Ruhe verglichen und vorallem MODIFIZIERT werden
    // kann, ohne get() zu beeinflussen
    char* tmp_wrptr = m_wrptr + 1;
    if(tmp_wrptr >= m_end) {
	   tmp_wrptr = m_begin;
    }
    if(tmp_wrptr == m_rdptr) {
	   return 0;
    }
    *m_wrptr = ch;
    // hier liegt der Trick, um fifo thread fest zu machen.
    // Aenderung des wrptr nach Vergleich und Schreiben mit
    // temporaerem Zeiger tmp_wrptr ist atomic und kann
    // dadurch von get() nicht mehr unterbrochen werden.
    m_wrptr = tmp_wrptr;
    return 1;
};
    
int fifo::read(char* data,int n)
{
    int nresult = 0;
    while(n--) {
	   if(m_rdptr != m_wrptr) {
		  *data = *m_rdptr++;
		  if(m_rdptr >= m_end) {
			 m_rdptr = m_begin;
		  }
	   }
	   else {
		  break;
	   }
	   nresult++;
	   data++;
    }
    return nresult;
};

int fifo::write(char* data,int n)
{
    int nresult = 0;
    while(n--) {
	   // wrptr temporaerem Zeiger zuweisen, der dann in
	   // Ruhe verglichen und vorallem MODIFIZIERT werden
	   // kann, ohne get() zu beeinflussen
	   char* tmp_wrptr = m_wrptr + 1;
	   if(tmp_wrptr >= m_end) {
		  tmp_wrptr = m_begin;
	   }
	   if(tmp_wrptr == m_rdptr) {
		  break;
	   }
	   *m_wrptr = *data++;
	   // hier liegt der Trick, um fifo thread fest zu machen.
	   // Aenderung des wrptr nach Vergleich und Schreiben mit
	   // temporaerem Zeiger tmp_wrptr ist atomic und kann
	   // dadurch von get() nicht mehr unterbrochen werden.
	   m_wrptr = tmp_wrptr;
	   nresult++;
    }
    return nresult;
};

