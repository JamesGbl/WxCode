#ifndef htgen_h
#define htgen_h

#include <io.h>

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/textfile.h>
#include <wx/listimpl.cpp>
#include <wx/html/htmlwin.h>
#include <wx/html/m_templ.h>

const int html_mask_debug   = 0x0001 ;
const int html_mask_info    = 0x0002 ;
const int html_mask_status  = 0x0004 ;
const int html_mask_warning = 0x0008 ;
const int html_mask_error   = 0x0010 ;
const int html_mask_code    = 0x0020 ;

const int html_mask_user1   = 0x0100 ;
const int html_mask_user2   = 0x0200 ;
const int html_mask_user3   = 0x0400 ;
const int html_mask_user4   = 0x0800 ;
const int html_mask_user5   = 0x1000 ;

const int html_mask_all     = 0xFFFF ;

const int html_normal       = 0 ;
const int html_emph         = 1 ;

class HTMLElement
{
protected :

  wxString _content ;
  int _mask ;

public :

  HTMLElement(const int amask, const wxString& acontent) ;
  virtual int save(const wxString& apath) ;

  int mask(const int amask) const ;

  virtual wxString html(const int amask) const = 0 ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const = 0 ;

  virtual ~HTMLElement() ;
} ; // HTMLElement

class HTMLText : public HTMLElement
{
  int _info ;

public :
  HTMLText(const int amask, const wxString& atext, const int ainfo) ;

  virtual wxString html(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLText

class HTMLCode : public HTMLText
{
public :
  HTMLCode(const int amask, const wxString& atext) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLCode

class HTMLTitle : public HTMLText
{
public :
  HTMLTitle(const wxString& atitle) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLTitle

class HTMLHead : public HTMLElement
{
  HTMLTitle _title ;

public :
  HTMLHead(const wxString& atitle) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLHead

WX_DECLARE_LIST(HTMLElement, HTMLList) ;

HTMLList* HTMLList_(const int amask, const wxArrayString& alist) ;

class HTMLBody : public HTMLElement
{
  HTMLList _list ;

public :
  HTMLBody() ;

  virtual int save(const wxString& apath) ;

  void add(HTMLElement* aelem) ;
  void clear() ;
  int empty() const ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLBody

class HTMLSection : public HTMLText
{
public :
  HTMLSection(const int amask,
    const wxString& asection) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLSection

class HTMLSubSection : public HTMLText
{
public :
  HTMLSubSection(const int amask,
    const wxString& asection) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
} ; // HTMLSubSection

class HTMLTable : public HTMLElement
{
  HTMLText _title ;
  unsigned int ncols ;
  HTMLList* _cols ;
  unsigned int nrows ;
  HTMLList* _rows ;
  HTMLList* _cells ;

public :
  HTMLTable(const int amask,
    const HTMLText& atitle,
    HTMLList* acols,
    HTMLList* arows,
    HTMLList* acells) ;

  HTMLTable(const int amask,
    const HTMLText& atitle,
    const unsigned int acols,
    const unsigned int arows,
    HTMLList* acells) ;

  virtual int save(const wxString& apath) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;

  virtual ~HTMLTable() ;
} ; // HTMLTable

class HTMLImage : public HTMLElement
{
  HTMLText _title ;
  wxString _filename ;
  int _delete ;

public :

  HTMLImage(const int amask,
    const wxString& acaption, const wxString& afilename,
    const int adelete = FALSE) ;

  virtual int save(const wxString& apath) ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;
  
  virtual ~HTMLImage() ;
} ; // HTMLImage

class HTMLDocument : public HTMLElement
{
  HTMLHead _head ;
  HTMLBody _body ;

public :

  HTMLDocument(const wxString& atitle) ;

  virtual int save(const wxString& apath) ;

  void add(HTMLElement* aelem) ;

  void text(const int amask, const wxString& atextelem, const int ainfo) ;

  void code(const int amask, const wxString& atextelem) ;

  void section(const int amask, const wxString& asection) ;
  void subsection(const int amask, const wxString& asection) ;

  void table(const int amask, 
             const wxString& atitle,
	     const unsigned int acols,
	     const unsigned int arows,
             const wxArrayString& acells,
             const int* ainfo) ;

  void table(const int amask, 
             const wxString& atitle,
             const wxArrayString& acols,
             const wxArrayString& arows,
             const wxArrayString& acells,
             const int* ainfo) ;

  void image(const int amask, 
    const wxString& aname, const wxString& afilename) ;

  void image(const int amask, 
    const wxString& aname, wxImage& aimage) ;

  void clear() ;
  int empty() const ;

  virtual wxString html(const int amask) const ;
  virtual wxString screen(const int amask) const ;
  virtual wxString text(const int amask) const ;

  int SaveHTML(const wxString& afile) ;
  int SaveTXT(const wxString& afile) const ;
} ; // HTMLDocument

class HtmlCtrl : public wxHtmlWindow
{
protected :
  HTMLDocument html ;
  int _refresh ;
  int _mask ;

public :
  
  HtmlCtrl(wxWindow* aparent, const int aid, 
	   const wxPoint& aposition, const wxSize& asize) ;

  wxString GetHTML() const ;
  wxString GetScreen() const ;
  wxString GetTXT() const ;

  void text(const int amask, const wxString& astring, const int ainfo) ;

  void code(const int amask, const wxString& astring) ;

  void section(const int amask, const wxString& astring) ;
  void subsection(const int amask, const wxString& astring) ;

  void table(const int amask,
             const wxString& atitle,
	     const unsigned int acols,
	     const unsigned int arows,
             const wxArrayString& acells,
             const int* ainfo) ;

  void table(const int amask,
             const wxString& atitle,
             const wxArrayString& acols,
             const wxArrayString& arows,
             const wxArrayString& acells,
             const int* ainfo) ;

  void image(const int amask,
    const wxString& acaption, const wxString& afilename) ;

  void image(const int amask,
    const wxString& acaption, wxImage& aimage) ;

  void mask(const int amask, const int aset) ;
 
  void refreshlog(const int arefresh) ;
  void OnCode(const int acode) ;

  void Clear() ;
  int empty() const ;

  int save(const wxString& afile) ;
  int SaveHTML(const wxString& afile) ;
  int SaveTXT(const wxString& afile) const ;
} ; // HtmlCtrl

#endif // htgen_h

#ifdef htgen_cpp

HTMLElement::HTMLElement(const int amask, const wxString& acontent) 
  : _content(acontent),
    _mask(amask)
{
} // HTMLElement

int HTMLElement::mask(const int amask) const
{
  return amask & _mask ;
} // code

int HTMLElement::save(const wxString& /* apath */)
{
  return 0 ;
} // save

wxString HTMLElement::screen(const int amask) const
{
  return html(amask) ;
} // screen

HTMLElement::~HTMLElement()
{
} // ~HTMLElement
    
HTMLText::HTMLText(const int amask, const wxString& atext, const int ainfo)
  : HTMLElement(amask, atext),
    _info(ainfo)
{
} // HTMLText

wxString HTMLText::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  wxString content_ = _content ;
  
  content_.Replace("ä", "&auml;") ;
  content_.Replace("ö", "&ouml;") ;
  content_.Replace("ü", "&uuml;") ;
  content_.Replace("ß", "&szlig;") ;
  content_.Replace("Ä", "&Auml;") ;
  content_.Replace("Ö", "&Ouml;") ;
  content_.Replace("Ü", "&Uuml;") ;
  content_.Replace("\n", wxString("<BR>\n")) ;

  if(_info == html_emph)
    content_ = "<I>" + content_ + "</I>" ;

  return content_ ;
} // html

wxString HTMLText::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return _content ;
} // text


HTMLCode::HTMLCode(const int amask, const wxString& atext)
  : HTMLText(amask, atext, html_normal)
{
} // HTMLCode

wxString HTMLCode::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<CODE>" + HTMLText::html(amask) + "</CODE>\n" ;
} // html

wxString HTMLCode::screen(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<CODE>" + HTMLText::screen(amask) + "</CODE>\n" ;
} // screen

wxString HTMLCode::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return HTMLText::text(amask) + "\n" ;
} // code

HTMLTitle::HTMLTitle(const wxString& atitle)
  : HTMLText(html_mask_all, atitle, html_normal)
{
} // HTMLTitle

wxString HTMLTitle::html(const int amask) const
{
  return "<TITLE>"
    + HTMLText::html(amask)
    + "</TITLE>\n" ;
} // html

wxString HTMLTitle::screen(const int amask) const
{
  return "<TITLE>"
    + HTMLText::screen(amask)
    + "</TITLE>\n" ;
} // screen

wxString HTMLTitle::text(const int amask) const 
{
  return HTMLText::text(amask) + "\n" ;
} // text

HTMLHead::HTMLHead(const wxString& atitle)
  : HTMLElement(html_mask_all, ""),
    _title(atitle)
{
} // HTMLHead

wxString HTMLHead::html(const int amask) const
{
  return _title.html(amask) ;
} // html

wxString HTMLHead::screen(const int amask) const
{
  return _title.screen(amask) ;
} // screen

wxString HTMLHead::text(const int amask) const
{
  return _title.text(amask) ;
} // text

WX_DEFINE_LIST(HTMLList) ;

HTMLList* HTMLList_(const int amask, const wxArrayString& alist, const int* ainfo)
{
  HTMLList* list = new HTMLList ;
  
  for(unsigned int i=0 ; i<alist.GetCount() ; i++)
    list->Append(new HTMLText(amask, alist[i], ainfo ? ainfo[i] : html_normal)) ;

  return list ;
} // HTMLList_


HTMLBody::HTMLBody()
  : HTMLElement(html_mask_all, "")
{
} // HTMLBody

int HTMLBody::save(const wxString& apath)
{
  if(HTMLElement::save(apath))
    return -1 ;
  
  for(unsigned int i=0 ; i<_list.GetCount() ; i++)
    if(_list[i]->save(apath))
      return -1 ;
  
  return 0 ;
} // save

void HTMLBody::add(HTMLElement* aelem)
{
  _list.Append(aelem) ;
} // add

void HTMLBody::clear()
{
  _list.Clear() ;
} // clear

int HTMLBody::empty() const
{
  return _list.GetCount() == 0 ;
} // empty

wxString HTMLBody::html(const int amask) const
{
  wxString content_ = "<BODY>\n" ;
  
  for(unsigned int i=0 ; i<_list.GetCount() ; i++)
    content_ += _list[i]->html(amask) ;
  
  return content_ + "\n</BODY>\n" ;
} // html

wxString HTMLBody::screen(const int amask) const
{
  wxString content_ = "<BODY>\n" ;
  
  for(unsigned int i=0 ; i<_list.GetCount() ; i++)
    content_ = content_ + _list[i]->screen(amask) ;
  
  return content_ + "\n</BODY>\n" ;
} // screen

wxString HTMLBody::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  wxString content_ ;
  
  for(unsigned int i=0 ; i<_list.GetCount() ; i++)
    content_ = content_ + _list[i]->text(amask) ;
  
  return content_ ;
} // text


HTMLSection::HTMLSection(const int amask,
    const wxString& asection)
  : HTMLText(amask, asection, html_normal)
{
} // HTMLSection

wxString HTMLSection::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<H1>" + HTMLText::html(amask) + "</H1>\n" ;
} // html

wxString HTMLSection::screen(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<H1>" + HTMLText::screen(amask) + "</H1>\n" ;
} // screen

wxString HTMLSection::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return HTMLText::text(amask) + "\n" ;
} // html

HTMLSubSection::HTMLSubSection(const int amask,
    const wxString& asection)
  : HTMLText(amask, asection, html_normal)
{
} // HTMLSubSection

wxString HTMLSubSection::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<H1>" + HTMLText::html(amask) + "</H1>\n" ;
} // html

wxString HTMLSubSection::screen(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<H1>" + HTMLText::screen(amask) + "</H1>\n" ;
} // screen

wxString HTMLSubSection::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return HTMLText::text(amask) + "\n" ;
} // html

HTMLTable::HTMLTable(const int amask,
    const HTMLText& atitle,
    HTMLList* acols,
    HTMLList* arows,
    HTMLList* acells)

  : HTMLElement(amask, ""),
    _title(atitle),
    ncols(acols->GetCount()),
    _cols(acols),
    nrows(arows->GetCount()),
    _rows(arows),
    _cells(acells)
{
} // HTMLTable

HTMLTable::HTMLTable(const int amask,
    const HTMLText& atitle,
    const unsigned int acols,
    const unsigned int arows,
    HTMLList* acells)

  : HTMLElement(amask, ""),
    _title(atitle),
    ncols(acols),
    _cols(NULL),
    nrows(arows),
    _rows(NULL),
    _cells(acells)
{
} // HTMLTable

int HTMLTable::save(const wxString& apath)
{
  if(HTMLElement::save(apath))
    return -1 ;
  
  if(_title.save(apath))
    return -1 ;

  if(_cols)
    for(unsigned int i=0 ; i<_cols->GetCount() ; i++)
      if(_cols->operator[](i)->save(apath))
        return -1 ;
  
  if(_rows)
    for(unsigned int i=0 ; i<_rows->GetCount() ; i++)
      if(_rows->operator[](i)->save(apath))
        return -1 ;
  
  for(unsigned int i=0 ; i<_cells->GetCount() ; i++)
    if(_cells->operator[](i)->save(apath))
      return -1 ;
  
  return 0 ;
} // save

wxString HTMLTable::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  wxString html_ ;
  html_ = wxString("<CENTER><BR><BR><BR>\n") +
    _title.html(amask) + "<BR><BR>\n" + 
    "<TABLE cellpadding=\"3\" cellspacing=\"0\" border=\"1\">\n" + 
    "<THEAD>\n"
    "<TR>" ;
  
  if(_cols)
    for(unsigned int i=0 ; i<_cols->GetCount() ; i++)
      html_ = html_ + "<TH>" + _cols->operator[](i)->html(amask) ;
  
  html_ = html_ + "\n</THEAD>\n<TBODY>\n" ;
  
  int index=0 ;
  for(unsigned int i=0 ; i<nrows ; i++)
    {
      html_ = html_ + "<TR>" ;
      if(_rows)
      {
        html_ += "<TH>" + _rows->operator[](i)->html(amask) ;
      
        for(unsigned int j=1 ; j<ncols ; j++)
	  html_ += "<TH>" + _cells->operator[](index++)->html(amask) ;      
      } // if
      else
        for(unsigned int j=0 ; j<ncols ; j++)
	  html_ += "<TH>" + _cells->operator[](index++)->html(amask) ;

      html_ += "\n" ;
    } // for
  
  html_ += "</TBODY></TABLE></CENTER>\n" ;
  return html_ ;
} // html

wxString HTMLTable::screen(const int amask) const
{
  if(!mask(amask))
    return "" ;

  wxString html_ ;
  html_ = wxString("<CENTER><BR><BR><BR>\n") +
    _title.screen(amask) + "<BR><BR>\n" + 
    "<TABLE cellpadding=\"3\" cellspacing=\"0\" border=\"1\">\n" + 
    "<THEAD>\n"
    "<TR>" ;
  
  if(_cols)
    for(unsigned int i=0 ; i<_cols->GetCount() ; i++)
      html_ = html_ + "<TH>" + _cols->operator[](i)->screen(amask) ;
  
  html_ = html_ + "\n</THEAD>\n<TBODY>\n" ;
  
  int index=0 ;
  for(unsigned int i=0 ; i<nrows ; i++)
    {
      html_ += "<TR>" ;

      if(_rows)
      {
        html_ += "<TH>" + _rows->operator[](i)->screen(amask) ;
      
        for(unsigned int j=1 ; j<ncols ; j++)
          html_ += "<TH>" + _cells->operator[](index++)->screen(amask) ;
      
      } // if
      else
        for(unsigned int j=0 ; j<ncols ; j++)
          html_ += "<TH>" + _cells->operator[](index++)->screen(amask) ;

      html_ += "\n" ;
    } // for
  
  html_ += "</TBODY></TABLE></CENTER>\n" ;
  return html_ ;
} // html

wxString HTMLTable::text(const int amask) const
{
  if(!mask(amask))
    return "" ;

  wxString text_ ;
  text_ = _title.text(amask) + "\n\n" ;
  
  if(_cols)
  {
    text_ += _cols->operator[](0)->text(amask) ;
  
    for(unsigned int i=1 ; i<_cols->GetCount() ; i++)
      text_ += " " + _cols->operator[](i)->text(amask) ;

    text_ += "\n" ;
  } // if
  
  int index=0 ;
  for(unsigned int i=0 ; i<nrows ; i++)
    {
      if(_rows)
        text_ += _rows->operator[](i)->text(amask) ;
      
      for(unsigned int j=1 ; j<ncols ; j++)
        text_ += " " + _cells->operator[](index++)->text(amask) ;
      
      text_ += "\n" ;
    } // for
  
  text_ += "\n" ;
  return text_ ;
} // text

HTMLTable::~HTMLTable()
{
  if(_cols)
    delete _cols ;

  if(_rows)
    delete _rows ;

  delete _cells ;
} // ~HTMLTable


HTMLImage::HTMLImage(const int amask,
    const wxString& acaption, const wxString& afilename,
    const int adelete = FALSE)
  : HTMLElement(amask, acaption),
    _title(amask, acaption, html_normal),
    _filename(afilename),
    _delete(adelete)
{
} // HTMLImage

int HTMLImage::save(const wxString& apath)
{
  if(!_delete)
    return 0 ;
  
  char _new[1000] ;
  strcpy(_new, apath) ;
  strcat(_new, "-XXXXXX") ;
#ifdef __WXGTK__
  int fd = mkstemp(_new) ;
  if(fd == -1)
    {
      wxLogLastError("html/save-image: %s") ;
      return -1 ;
    } // if
  ::close(fd) ;
#else
  if(!mktemp(_new))
    {
      wxLogLastError("html/save-image: %s") ;
      return -1 ;
    } // if
#endif
  
  if(!wxCopyFile(_filename, _new))
    {
      wxLogLastError(wxString("html/save-image " + _filename + " " + _new).c_str()) ;
      return -1 ;
    } // if
  
  if(_delete)
    {
      wxRemoveFile(_filename) ;
      _delete = FALSE ;
    } // if

  wxString name, ext ;
  wxSplitPath(_new, NULL, &name, &ext) ;

#ifdef __WXMSW__
  _filename = (ext == "" ? name : name + wxFILE_SEP_EXT + ext) ;
#else
  _filename = name + wxFILE_SEP_EXT + ext ;
#endif 
              
  return 0 ;
} // save

wxString HTMLImage::html(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<CENTER><BR><BR>\n" + _title.html(amask) + "<BR>\n"
    + "<IMG SRC=\"" + _filename + "\" "
    + "ALT=\"" + _title.html(amask) + "\"></CENTER><BR>\n" ;
} // html

wxString HTMLImage::screen(const int amask) const
{
  if(!mask(amask))
    return "" ;

  return "<CENTER><BR><BR>\n" + _title.screen(amask) + "<BR>\n"
    + "<IMG SRC=\"" + _filename + "\" "
    + "ALT=\"" + _title.screen(amask) + "\"></CENTER><BR>\n" ;
} // screen

wxString HTMLImage::text(const int /* amask */) const
{
  return "\n(image)\n" ;
} // text

HTMLImage::~HTMLImage()
{
  if(_delete)
    wxRemoveFile(_filename) ;
} // ~HTMLImage


HTMLDocument::HTMLDocument(const wxString& atitle)
  : HTMLElement(html_mask_all, ""),
    _head(atitle),
    _body()
{
} // HTMLDocument

int HTMLDocument::save(const wxString& apath)
{
  if(HTMLElement::save(apath))
    return -1 ;
  
  if(_head.save(apath))
    return -1 ;
  
  if(_body.save(apath))
    return -1 ;
  
  return 0 ;
} // save

void HTMLDocument::add(HTMLElement* aelem)
{
  _body.add(aelem) ;
} // add

void HTMLDocument::text(const int amask, const wxString& atextelem, const int ainfo)
{
  add(new HTMLText(amask, atextelem, ainfo)) ;
} // text

void HTMLDocument::code(const int amask, const wxString& atextelem)
{
  add(new HTMLCode(amask, atextelem)) ;
} // code

void HTMLDocument::section(const int amask, const wxString& asection)
{
  add(new HTMLSection(amask, asection)) ;
} // section

void HTMLDocument::subsection(const int amask, const wxString& asection)
{
  add(new HTMLSubSection(amask, asection)) ;
} // subsection

void HTMLDocument::table(const int amask, 
                         const wxString& atitle,
			 const unsigned int acols,
			 const unsigned int arows,
                         const wxArrayString& acells,
                         const int* ainfo)
{
  add(new HTMLTable(amask,
                    HTMLText(amask, atitle, html_normal),
                    acols,
                    arows, 
                    HTMLList_(amask, acells, ainfo))) ;
} // table

void HTMLDocument::table(const int amask, 
                         const wxString& atitle,
                         const wxArrayString& acols,
                         const wxArrayString& arows,
                         const wxArrayString& acells,
                         const int* ainfo)
{
  add(new HTMLTable(amask,
                    HTMLText(amask, atitle, html_normal),
                    HTMLList_(amask, acols, NULL), 
                    HTMLList_(amask, arows, NULL), 
                    HTMLList_(amask, acells, ainfo))) ;
} // table

void HTMLDocument::image(const int amask,
  const wxString& aname, const wxString& afilename)
{
  add(new HTMLImage(amask, aname, afilename)) ;
} // image

void HTMLDocument::image(const int amask,
  const wxString& aname, wxImage& aimage)
{
  wxString temp ;
  if(!wxGetTempFileName("", temp))
    {
      wxLogError("Error creating temp file.") ;
      return ;
    } // if
  
  //    aimage.SetOption("quality", 100) ;
  if(!aimage.SaveFile(temp, wxBITMAP_TYPE_JPEG))
    {
      wxLogLastError(wxString("Error saving " + temp).c_str()) ;
      return ;
    } // if
  
  add(new HTMLImage(amask, aname, temp, TRUE)) ;
} // image

void HTMLDocument::clear()
{
  _body.clear() ;
} // clear

int HTMLDocument::empty() const
{
  return _body.empty() ;
} // empty

wxString HTMLDocument::html(const int amask) const
{
  wxString s = "<HTML>\n" ;
  s += _head.html(amask) ;
  s += _body.html(amask) ;
  s += "</HTML>\n" ;
  return s ;
} // html

wxString HTMLDocument::screen(const int amask) const
{
  wxString s = "<HTML>\n" ;
  s += _head.screen(amask) ;
  s += _body.screen(amask) ;
  s += "</HTML>\n" ;
  return s ;
} // screen

wxString HTMLDocument::text(const int amask) const
{
  return _head.text(amask) + _body.text(amask) ;
} // text

int HTMLDocument::SaveHTML(const wxString& afile)
{
  if(save(afile))
    return -1 ;
  
  FILE* f = fopen(afile, "wt") ;
  if(!f)
  {
    wxLogLastError(afile.c_str()) ;
    return -1 ;
  } // if
  
  fputs(html(_mask).c_str(), f) ;
  int err = ferror(f) ;
  fclose(f) ;
  return err ;
} // SaveHTML

int HTMLDocument::SaveTXT(const wxString& afile) const
{
  FILE* f = fopen(afile, "wt") ;
  if(!f)
  {
    wxLogLastError(afile.c_str()) ;
    return -1 ;
  } // if
  
  fputs(text(_mask).c_str(), f) ;
  int err = ferror(f) ;
  fclose(f) ;
  return err ;
} // SaveTXT


HtmlCtrl::HtmlCtrl(wxWindow* aparent, const int aid, 
		   const wxPoint& aposition, const wxSize& asize)
  : wxHtmlWindow(aparent, aid, aposition, asize),
    html("EASI output"),
    _refresh(TRUE),
    _mask(html_mask_all)
{
} // HTMLCtrl

wxString HtmlCtrl::GetHTML() const
{
  return html.html(_mask) ;
} // GetHTML

wxString HtmlCtrl::GetScreen() const
{
  return html.screen(_mask) ;
} // GetScreen

wxString HtmlCtrl::GetTXT() const
{
  return html.text(_mask) ;
} // GetTXT

void HtmlCtrl::text(const int amask, const wxString& astring, const int ainfo)
{
  html.text(amask, astring, ainfo) ;
  if(_refresh)
    SetPage(GetScreen()) ;
} // text

void HtmlCtrl::code(const int amask, const wxString& astring)
{
  html.code(amask, astring) ;
  if(_refresh)
    SetPage(GetScreen()) ;
} // text

void HtmlCtrl::section(const int amask, const wxString& astring)
{
  html.section(amask, astring) ;    
  if(_refresh)
    SetPage(GetScreen()) ;
} // section

void HtmlCtrl::subsection(const int amask, const wxString& astring)
{
  html.subsection(amask, astring) ;    
  if(_refresh)
    SetPage(GetScreen()) ;
} // subsection

void HtmlCtrl::table(const int amask,
                     const wxString& atitle,
                     const wxArrayString& acols,
                     const wxArrayString& arows,
                     const wxArrayString& acells,
                     const int* ainfo)
{
  html.table(amask, atitle, acols, arows, acells, ainfo) ;    
  if(_refresh)
    SetPage(GetScreen()) ;
} // table

void HtmlCtrl::table(const int amask,
                     const wxString& atitle,
		     const unsigned int acols,
		     const unsigned int arows,
                     const wxArrayString& acells,
                     const int* ainfo)
{
  html.table(amask, atitle, acols, arows, acells, ainfo) ;
  if(_refresh)
    SetPage(GetScreen()) ;
} // table

void HtmlCtrl::image(const int amask,
  const wxString& acaption, const wxString& afilename)
{
  html.image(amask, acaption, afilename) ;    
  if(_refresh)
    SetPage(GetScreen()) ;
} // image

void HtmlCtrl::image(const int amask,
  const wxString& acaption, wxImage& aimage)
{
  html.image(amask, acaption, aimage) ;    
  if(_refresh)
    SetPage(GetScreen()) ;
} // image

void HtmlCtrl::mask(const int amask, const int aset)
{
  if(aset)
    _mask = _mask | amask ;
  else
    _mask = _mask & (0xFFFF-amask) ;

  if(_refresh)
    SetPage(GetScreen()) ;
} // mask

void HtmlCtrl::refreshlog(const int arefresh)
{
  _refresh = arefresh ;
  if(_refresh)
    SetPage(GetScreen()) ;
} // refreshlog

void HtmlCtrl::OnCode(const int acode)
{
  if(acode)
    _mask = _mask | html_mask_code ;
  else
    _mask = _mask & (html_mask_all-html_mask_code) ;

  if(_refresh)
    SetPage(GetScreen()) ;
} // OnCode

void HtmlCtrl::Clear()
{
  html.clear() ;
  SetPage(GetScreen()) ;
} // Clear

int HtmlCtrl::empty() const
{
  return html.empty() ;
} // empty

int HtmlCtrl::save(const wxString& afile)
{
  return html.save(afile) ;
} // save

int HtmlCtrl::SaveHTML(const wxString& afile)
{
  return html.SaveHTML(afile) ;
} // SaveHTML

int HtmlCtrl::SaveTXT(const wxString& afile) const
{
  return html.SaveTXT(afile) ;
} // SaveTXT

#endif // htgen_cpp

#ifdef htgen_test

int main()
{
  HTMLDocument doc("title") ;

  doc.text(html_mask_info, "häöüo1\n", html_normal) ;
  doc.text(html_mask_info, "hallo2\n", html_normal) ;
  doc.text(html_mask_info, "emphasis\n", html_emph) ;
  doc.section(html_mask_info, "hallo4") ;
  doc.subsection(html_mask_info, "hallo5") ;
  doc.image(html_mask_info, "Geometrie", "geometry.xpm") ;

  wxArrayString _cols ;
  _cols.Add("col1") ;
  _cols.Add("col2") ;
  _cols.Add("col3") ;
  
  wxArrayString _rows ;
  _rows.Add("row1") ;
  _rows.Add("row2") ;
  
  wxArrayString _cells ;
  _cells.Add("cell1") ;
  _cells.Add("cell2") ;
  _cells.Add("cell3") ;
  _cells.Add("emphasis") ;

  int info[4] = {html_normal, html_normal, html_normal, html_emph} ;
  doc.table(html_mask_info, "Table", _cols, _rows, _cells, info) ;

  cout << doc.html(html_mask_all) << endl ;
  cout << doc.text(html_mask_all) << endl ;
  return 0 ;
} ; // main

#endif // htgen_test
