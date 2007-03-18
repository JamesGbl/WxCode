
#include "multiLang.h"

multiLang::multiLang()
{
   wxLocale local(wxLANGUAGE_DEFAULT);
   m_systemLangName = local.GetLanguageName(local.GetSystemLanguage());
   m_systemLangName = m_systemLangName.Mid(0,m_systemLangName.Find("(")-1);
   m_langsDir = "";
   m_actualLang = m_systemLangName;
}


multiLang::multiLang(const multiLang& copyLang)
{
   langMap=copyLang.langMap;
   langList=copyLang.langList;
   m_langsDir=copyLang.m_langsDir;
   m_systemLangName=copyLang.m_systemLangName;
   m_actualLang=copyLang.m_actualLang;   
}

multiLang::~multiLang()
{
}

void multiLang::setActualLang(wxString language)
{
   m_actualLang = language;  
}

void multiLang::setLangsDir(wxString langDir)
{
    m_langsDir = langDir;
}

wxString multiLang::getSystemLanguage()
{
   return m_systemLangName;
}

wxString multiLang::getActualLang()
{
   return m_actualLang;
}

wxArrayString multiLang::getListLangs()
{
   wxDir dir;
   wxString filename;
   langList.Clear();

   if (!dir.Exists(m_langsDir)) return langList;
    dir.Open(m_langsDir);
    if (!dir.IsOpened()) return langList;

    bool cont = dir.GetFirst(&filename);
    while (cont)
    {
       if (filename.AfterLast(wxT('.')) == "lng")
       {
          langList.Add(filename.BeforeLast(wxT('.')));
       }   
       cont = dir.GetNext(&filename);
    }
   return langList;
}

bool multiLang::updateLangMap(wxString langDir, wxString language)
{
   m_langsDir = langDir;
   m_actualLang = language;  
   return updateLangMap();
}

bool multiLang::updateLangMap()
{
   langMap.clear();
   wxTextFile file(m_langsDir + "\\" + m_actualLang + ".lng");
   bool fileOpen = false;
   if (file.Exists()) fileOpen = file.Open(m_langsDir + "\\" + m_actualLang + ".lng");
   if (!fileOpen && m_actualLang!="English")
   {
      setActualLang("English");
      wxTextFile engFile(m_langsDir + "\\" + m_actualLang + ".lng");
      if (engFile.Exists()) fileOpen = file.Open(m_langsDir + "\\" + m_actualLang + ".lng");
   }
   if (!fileOpen)
      return false;
   for (wxString line = file.GetFirstLine(); !file.Eof(); line = file.GetNextLine())
   {
     if (!line.IsEmpty())
     {
       wxString name = line.Mid(0,line.Find("="));
       wxString value = line.Mid(line.Find("=")+1,line.Length()-(line.Find("=")));
       langMap[name] = value;
     }  
   }  
   return true;
}

multiLang& multiLang::operator=(const multiLang& copyLang)
{
   if (this != &copyLang)
   {
      langMap=copyLang.langMap;
      langList=copyLang.langList;
      m_langsDir=copyLang.m_langsDir;
      m_systemLangName=copyLang.m_systemLangName;
      m_actualLang=copyLang.m_actualLang;
   }
   return *this;
}


wxString multiLang::operator[](wxString index)
{
   wxString result = langMap[index];
   if (!result) result = index;
   return result;
}
