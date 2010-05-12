//! \file mmDbase.h
//
// Name     : mmDbase
// Purpose  : Wrapper around wxDb/wxDbTable stuff.
// Author   : Arne Morken
// Copyright: (C) 1999-2002 MindMatters, www.mindmatters.no
// Licence  : wxWindows licence.
//

#ifndef _WXDBASE_H_INCLUDED
#define _WXDBASE_H_INCLUDED

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/string.h>
#include <wx/db.h>
#include <wx/dbtable.h>
#include <wx/list.h>
#include <wx/datetime.h>
#include <wx/mmwxdef.h>
#define __SQL
#include <odbcinst.h>


/*! \brief Class for holding table column info.
 */
class WXDLLIMPEXP_MMWX mmDbaseColumn
{
    public:
        mmDbaseColumn(int type, int size);

        /*! \brief Destructor.
         *
         *
         */
        ~mmDbaseColumn()
        {
            if(mData) {
                delete mData;
                mData = NULL;
            }
        }

        //void Update(wxString& str);
        wxString Str();

        void	*mData;	//!< The column data.
        int   	mSize;		//!< The data langth.
        int   	mCType;	//!< The SQL C type.
        int   	mType;		//!< The data type.
};

WX_DECLARE_LIST(mmDbaseColumn, mmDbaseRow);


/*! \brief Wrapper around a database table.
 */
class mmDbaseTable : public wxDbTable
{
    public:
        mmDbaseTable(wxDb *db, wxString &tabName, int ncols, wxDbColInf *col_inf, wxString &defDir);
        ~mmDbaseTable();
        void           SetColumn(int colNo, wxString    val);
        void           SetColumn(int colNo, int         val);
        void           SetColumn(int colNo, long        val);
        void           SetColumn(int colNo, short       val);
        void           SetColumn(int colNo, bool        val);
        void           SetColumn(int colNo, float       val);
        void           SetColumn(int colNo, double      val);
        void           SetColumn(int colNo, wxDateTime &val);
        /*! \brief Get column information.
         *
         * \param i 					int	The number of the column to fetch.
         * \return wxDbColInf&	The column info.
         *
         */
        wxDbColInf    &GetColumnInfo(int i)
        {
            return mColInf[i];
        }
        /*! \brief Get the current row.
         *
         * \return mmDbaseRow&	The row.
         *
         */
        mmDbaseRow &GetRow(){
            return mRow;
        }
        /*! \brief Get a column.
         *
         * \param i 							int	The number of the column to fetch.
         * \return mmDbaseColumn*	The column.
         *
         */
        mmDbaseColumn *GetColumn(int i)
        {
            if(mRow.GetCount() <= 0)
                return NULL;
            return mRow[i];
        };
        /*! \brief Get the number of columns.
         *
         * \return unsigned int	The number of columns.
         *
         */
        unsigned int GetNumColumns()
        {
            return mRow.GetCount();
        }
        void DoQuery(wxString whereStr, wxString orderStr);

    private:
        wxDbColInf 		*mColInf;	//!< Column info.
        mmDbaseRow  	mRow;		//!< The current row.
}; // mmDbaseTable

// A list of tables
WX_DECLARE_LIST(mmDbaseTable, mmDbaseTableList);


/*! \brief Wrapper around some database stuff.
 */
class mmDbase
{
    public:
        mmDbase();
        ~mmDbase();
        bool CreateDataSource(wxString &odbcDriver, wxString &regCmd);
        bool Init(wxString dsn      	= wxEmptyString,
                  wxString user     		= wxEmptyString,
                  wxString password 	= wxEmptyString,
                  wxString dbaseDir 	= wxEmptyString);
        //mmDbaseTable* CreateTable  (wxString tableName,mmDbaseRow& row); // UNTESTED
        mmDbaseTable *OpenTable(wxString tableName);
        /*! \brief Get the DB connection.
         *
         * \return wxDb*	The DB connection.
         *
         */
        wxDb *GetDBConn()
        {
            return mDB;
        }
        /*! \brief Get an error message.
         *
         * \return wxString	The message.
         *
         */
        wxString GetErrMsg()
        {
            return mDbaseErr;
        }

    private:
        wxDbConnectInf	mDbConInf;	//!< DB connection info.
        wxDb    			*mDB; 			//!< The connection to the database.
        wxString 			mDbaseErr;	//!< Error string.
}; // mmDbase

// Temporary solution for Updating an Interbase database
extern bool UpdateDbase(mmDbase *db, mmDbaseTable *tab, int col, wxString id_name, wxString id);

#endif

