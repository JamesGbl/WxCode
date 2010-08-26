
/*
 * SciDocManager.cpp
 * ______________________________________________________________
 * This software is dual licensed under either the GNU GPL
 * version 2 or at your option any other version
 * ______________________________________________________________
 * or the wxWindows license.
 * Just a quick reminder of what that means:
 *
 * This software is released under the GNU GPL licence
 * with a few exceptins applied, check the wxWindows licence
 * to see what those are.
 * ______________________________________________________________
 *
 * Visit: http://opensource.org/ to see both-
 * the GNU GPL and wxWindows licences.
 *
 * This software has absolutely no warranty, express or implied.
 *
 * Just so you know, I don't care if you change the code
 * don't email me if you did someting to it.
 *
 * If you edit a function to change its behavior, it would be courtious
 * to others to let them know that the file is not an official release,
 * but you don't have to do that either.
 *
 * You must not misrepresent the origins of this software, if you distribute
 * it, let the user know where to get it and that you where not the original
 * creator. (Except for any code you add obviously)
 *
 * This notice may not be changed in any way and must remain at the top of every
 * source file.
 *
 * Scintilla Document Manager is part of XSTC
 * XSTC was developed by Nuklear Zelph
 * copyright (C) 2009
 */
 
#include <wx/xstc/SciDocManager.h>

SciDocManager::SciDocHandler::SciDocHandler(void* document, int id)
{
    this->document = document;
    this->id = id;
    data = NULL;
}

SciDocManager::SciDocHandler::~SciDocHandler()
{
    delete data;
    data = 0;
}

int SciDocManager::SciDocHandler::GetID()
{
    return id;
}

void* SciDocManager::SciDocHandler::GetDocument()
{
    return document;
}

void SciDocManager::SciDocHandler::SetData(extras* data)
{
    this->data = data;
}

extras* SciDocManager::SciDocHandler::GetData()
{
    return data;
}


SciDocManager::SciDocManager(XSTC* editor)
{
    this->editor = editor;
    current = 0;
    lastid = 0;
    SciDocHandler* mydoc = new SciDocHandler(editor->GetDocPointer(), current);
    documents.push_back(*mydoc);
}

SciDocManager::~SciDocManager()
{
    ClearAll();
}

int SciDocManager::AddDocument(wxString filename, int wrapmode, wxString theme)
{
    SciDocHandler* mydoc = new SciDocHandler(editor->CreateDocument(), ++lastid);
    extras* extr;
    extr->foldheads.clear();
    extr->selection = wxPoint(-1,-1);
    extr->filename = filename;
    extr->wrap = wrapmode;
    extr->cursorpos = 0;
    extr->lexer = XSTC_DEF(LEX_NULL);
    extr->theme = theme;
    extr->overtype = false;
    mydoc->SetData(extr);
    documents.push_back(*mydoc);
    return lastid;
}

int SciDocManager::GetCurrentDocument()
{
    return current;
}

bool SciDocManager::SetDocument(int id)
{
    itr myitr;
    void* docptr;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == id)
        {
            StoreEX();
            docptr = editor->GetDocPointer();
            editor->AddRefDocument(docptr);
            editor->SetDocPointer(myitr->GetDocument());
            SetEX();
            current = id;
            return true;
        }
    }
    return false;
}

int SciDocManager::GetTotal()
{
    return documents.size();
}

bool SciDocManager::DeleteDocument(int id)
{
    itr myitr;
    void* docptr;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == id)
        {
            if(id != current)
            {
                docptr = myitr->GetDocument();
                documents.erase(myitr);
                editor->ReleaseDocument(docptr);
                myitr = documents.end();
            }
            else
            {
                void* newptr;
                int nid;
                newptr = editor->CreateDocument();
                docptr = editor->GetDocPointer();
                editor->SetDocPointer(newptr);
                nid = myitr->GetID();
                editor->ReleaseDocument(docptr);
                documents.erase(myitr);
                SciDocHandler* mydoc = new SciDocHandler(newptr, nid);
                documents.push_back(*mydoc);
                current = nid;
                myitr = documents.end();
                return false;
            }
        }
    }
    return true;
}

void SciDocManager::ClearDocument(int id)
{
    itr myitr;
    void* docptr;
    void* newptr;
    int nid;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == id)
        {
            newptr = editor->CreateDocument();
            nid = myitr->GetID();
            if(id == current)
            {
                editor->SetDocPointer(newptr);
                current = nid;
            }
            docptr = myitr->GetDocument();
            editor->ReleaseDocument(docptr);
            documents.erase(myitr);
            SciDocHandler* mydoc = new SciDocHandler(newptr, nid);
            documents.push_back(*mydoc);
            myitr = documents.end();
        }
    }
}

void SciDocManager::ClearAll()
{
    itr myitr;
    void* thedoc = editor->CreateDocument();
    void* temp;
    editor->SetDocPointer(thedoc);
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        temp = myitr->GetDocument();
        editor->ReleaseDocument(temp);
    }
    documents.clear();
    current = 0;
    lastid = 0;
    SciDocHandler* mydoc = new SciDocHandler(thedoc, current);
    documents.push_back(*mydoc);
}

wxString SciDocManager::GetDocFilename(int id)
{
    itr myitr;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == id)
        {
            extras* extr = myitr->GetData();
            if(extr)
                return extr->filename;
            else
                return wxT("");
        }
    }
    return wxT("");
}

bool SciDocManager::SetDocFilename(int id, wxString filename)
{
    itr myitr;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == id)
        {
            extras* extr = myitr->GetData();
            if(extr)
            {
                extr->filename = filename;
                return true;
            }
            else
                return false;
        }
    }
    return false;
}

void SciDocManager::SetEX()
{
    itr myitr;

    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == current)
        {
            break;
        }
    }
    extras* extr = myitr->GetData();
    if(extr)
    {
        if(editor->GetLexer() != extr->lexer)
        {
            editor->SetLexerX(extr->lexer);
        }
		
		if(extr->selection == wxPoint(-1,-1))
        {
            editor->SetCurrentPos(extr->cursorpos);
        }
        else
        {
            editor->SetSelection(extr->selection.x, extr->selection.y);
        }
        editor->SetWrapMode(extr->wrap);
        if(editor->colorstyle != extr->theme)
        {
            editor->ConfigStyle(extr->theme);
        }
        if(editor->GetOvertype() != extr->overtype)
        {
            editor->SetOvertype(extr->overtype);
        }
		
		if(extr->foldheads.size() > 0)
        {
            //update folding
        }
    }
}

void SciDocManager::StoreEX()
{
    itr myitr;
    
    for (myitr=documents.begin();myitr<documents.end();myitr++)
    {
        if(myitr->GetID() == current)
        {
            break;
        }
    }
    extras* extr = myitr->GetData();
    if(extr)
    {
        /*if(extr->foldheads.size() > 0)
        {
            //update folding
        }*/
        
        if(editor->GetSelectionStart() != editor->GetSelectionEnd())
        /* TODO (NZ#1#): fix selection logic */
        {
            extr->selection.x = editor->GetSelectionStart();
            extr->selection.y = editor->GetSelectionEnd();
        }
        else
        {
            extr->selection = wxPoint(-1,-1);
        }
        extr->cursorpos = editor->GetCurrentPos();
        extr->wrap = editor->GetWrapMode();
        extr->lexer = editor->GetLexer();
        extr->theme = editor->colorstyle;
        extr->overtype = editor->GetOvertype();
    }
}
