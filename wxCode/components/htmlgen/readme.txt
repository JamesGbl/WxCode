Name:     libhtgen.a - A library to generate HTML documents
Author:   Matthias Gondan (matthias-gondan@gmx.de)
Licence:  wxWindows licence
Requires: wxWindows (wxGTK-linux, wxMSW-mingw32)

Compilation
-----------

make


Installation
------------

cp libhtgen.a /usr/local/lib
cp htgen.cpp /usr/local/include


Usage
---

#include <htgen.cpp> // not h!
and "-lhtgen"


int main()
{
  HTMLDocument doc("Document title -- as shown in the titlebar") ;

  doc.text(html_mask_info, "text with linebreak\n", html_normal) ;
  doc.text(html_mask_info, "the mask is used to classify the element\n", html_normal) ;
  doc.text(html_mask_info, "later on, you can filter out elements you want to display\n", html_normal) ;
  doc.text(html_mask_info, "emphasis\n", html_emph) ;
  doc.section(html_mask_info, "A section") ;
  doc.image(html_mask_info, "An image with filename.", "geometry.xpm") ;

  wxArrayString _cols ;
  _cols.Add("col1") ; // Three columns
  _cols.Add("col2") ;
  _cols.Add("col3") ;
  
  wxArrayString _rows ;
  _rows.Add("row1") ; // Three rows, however, the first row head is already "col1"
  _rows.Add("row2") ;
  
  wxArrayString _cells ;
  _cells.Add("cell1") ; // the remaining cells
  _cells.Add("cell2") ;
  _cells.Add("cell3") ;
  _cells.Add("emphasis") ;
  int info[4] = { html_normal, html_normal, html_normal, html_emph } ;
  
  doc.table(html_mask_info, "Table header", _cols, _rows, _cells, info) ;

  cout << doc.html(html_mask_all) << endl ;
  cout << doc.text(html_mask_all) << endl ;
  return 0 ;
} // main

