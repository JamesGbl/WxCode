
/*
 * SciDocManager.h
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
 
 
#ifndef SCIDOCHANDLER_H
#define SCIDOCHANDLER_H

#include <wx/xstc/xstc.h>
#include <vector>

//to make this work with any version of Scintilla, the macro allows for the class
//to be set to whatever version you are using
#define SCICLASS XSTC

/*
This class aims to make multiple document handling with Scintilla easier to handle.
Rather than needing to keep track of documents and reference counting for them, this
class does that for you. It automatically replaces the document in the editor based
on the id you pass to it, corrosponding to the document. the editor instance cannot
be NULL.

to get the document id from the default document, call GetCurrentDocument() right
after you create an instance of the handler.

this class will not destroy your editor, it does destroy all documents except for the
one that is currently held in the editor.
*/

typedef std::vector<int> intarr;
typedef std::vector<int>::iterator intitr;

/**
	\brief stores some info for each doc that scintilla does not while doc exchanging
	
	\detailed since this class stores the scintilla document and swaps it into the editor some information
				is lost. This keeps track of that information so that the user is not always battling
				the interface losing settings on them.
	\struct
*/
typedef struct extras
{//this is to restore info and settings that scintilla does not automatically
    intarr foldheads;//used for remembering folding information
    wxPoint selection;//x=anchor y=end (-1, -1) = unused
	//int fcharlen;//for filename
	int wrap;
    int cursorpos;//if no selection the cursors last position
    int lexer;
    wxString theme;
    wxString filename;//document filename
    bool overtype;//overtype mode or insert
    
    extras():wrap(XSTC_DEF(WRAP_NONE)), cursorpos(0), lexer(XSTC_DEF(LEX_NULL)), overtype(false)
    {
        
    }
};

/* notes:
need to delete doc0 and move everything up when it is deleted with more than one instance in the 
manager.

rename lex to theme

impliment the extras

i can fix the id correlation error with the notebook pages by overloading the tab class
to store the document id.
*/

/**
	\brief this class manages reference counting and document swapping for scintilla. It will work with
			any version as long as the SCICLASS macro is set correctly.
	
	\detailed Scintilla can handle more than one document at once, but it will not edit more than one at
			once. this class manages document swapping and storage for that purpose. it also remembers
			each documents settings that are not saved my scintilla to keep the interface consistent.
			one example is selections are not remembered, this will reselect the text when the document
			is seitched back to. also wraping is remembered and color theme.
	\class
*/
class SciDocManager
{
    
/**
	\brief this class stored one document for scintilla and an unchanging id to look it up with. 
	\class
*/
	class SciDocHandler
    {
    public:
/**
	\brief constructor taked the document pointer and a uniqe id to keep track of it.
*/
    SciDocHandler(void* document, int id);
    ~SciDocHandler();
/**
	\brief return the id of this document for lookup later
*/
    int GetID();
/**
	\brief returns the document so it can be placed into the Scintilla editor
*/
    void* GetDocument();
/**
	\brief sets the extra information that scintilla does not keep track of for us
*/
    void SetData(extras* data);
/**
	\brief returns the extra informaiton that scintilla does not keep track of for us.
	
	\detailed this updates the information each time it is called so that it remaines correct.
*/
    extras* GetData();

    private:
/**
	\brief the document that is in storage
*/
    void* document;
/**
	\brief extra information
*/
    extras* data;
/**
	\brief uniqe id for the stored document
*/
    int id;
    };
/**
	\brief a vector to allow storage of multiple documents 
    \typedef
*/
    typedef std::vector<SciDocHandler> DocHandler;
    typedef std::vector<SciDocHandler>::iterator itr;

    public:
/**
	\brief to use the document manager pass the instance of scintilla you want managed
*/
    SciDocManager(SCICLASS* editor);
    ~SciDocManager();

/**
	\brief adds a fresh document and returns the id, optionally sets the filename
*/
    int AddDocument(wxString filename = wxT(""), int wrapmode = XSTC_DEF(WRAP_WORD), wxString theme = wxT(""));// adds a new document and returns its id
/**
	\brief returns the id of the document in the editor
*/
    int GetCurrentDocument();//returns the id for the current editor document
/**
	\brief change the document in the editor by id
	\detailed if the document is already in the editor instance or is invalid it is ignored
*/
    bool SetDocument(int id);//set the document in the editor unles it is already there or the id is invalid
/**
	\brief returns the total number of stored documents
*/
    int GetTotal();//total number of docs
/**
	\brief removes the document from the storage array using the correct reference counting method
*/
    bool DeleteDocument(int id);//remove doc from the array
/**
	\brief resets the document to its initial state
*/
    void ClearDocument(int id);//replace doc with fresh instance
/**
	\brief deletes all documents and clears the one in the editor
*/
    void ClearAll();//delete all docs, leaves editor with one fresh document
/**
	\brief returns the filename for the given document
*/
    wxString GetDocFilename(int id);
/**
	\brief sets the filename of the doc
*/
    bool SetDocFilename(int id, wxString filename);

    private:
/**
	\brief the document storage array
*/
    DocHandler documents;
/**
	\brief the id of the doc in the editor and the one that was just there
*/
    int current, lastid;
/**
	\brief the scintilla editor instance
*/
    SCICLASS* editor;
/**
	\brief updates the document with the extra info
*/
    void SetEX();
/**
	\brief stores the document extra info
*/
    void StoreEX();
};

#endif //SCIDOCHANDLER_H

