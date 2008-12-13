/////////////////////////////////////////////////////////////////////////////
// Name:        wxActiveRecordGenerator.h
// Purpose:
// Author:      Matías Szeftel
// Modified by:
// Created:     01/10/2006 19:12:13
// RCS-ID:
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_ACTIVE_RECORD_GENERATOR_H_
#define _WX_ACTIVE_RECORD_GENERATOR_H_

#include <wx/wx.h>
#include <wx/string.h>
#include <wx/fileconf.h>
#include <wx/wfstream.h>
#include <wx/sstream.h>
#include <wx/filename.h>
#include <wx/ffile.h>
#include <wx/hashmap.h>
#include <wx/regex.h>

#include <DatabaseLayer.h>
#include <DatabaseErrorCodes.h>
#include <DatabaseLayerException.h>
#include <ResultSetMetaData.h>

struct arRelation;
struct arNode;

enum{AR_BELONGS_TO=0,AR_HAS_MANY};

//hash of variable, maps variables names with variable types.
WX_DECLARE_STRING_HASH_MAP( int, VarHash );
//array of relations.
WX_DEFINE_ARRAY(arRelation*,RelationsArray);
//
WX_DECLARE_STRING_HASH_MAP(arNode*, ArNodeHash);

#ifdef AR_USE_SQLITE
	#include <SqliteDatabaseLayer.h>
#endif
#ifdef AR_USE_POSTGRESQL
	#include <PostgresDatabaseLayer.h>
#endif
#ifdef AR_USE_MYSQL
	#include <MysqlDatabaseLayer.h>
#endif
#ifdef AR_USE_FIREBIRD
	#include <FirebirdDatabaseLayer.h>
#endif

enum{AR_SQLITE=0,AR_POSTGRESQL,AR_MYSQL,AR_FIREBIRD,AR_FIREBIRD_EMBEDDED};

class wxActiveRecordGenerator{
	VarHash m_varHash;
	RelationsArray m_relations;
	int m_dbType;
	wxString m_tableName;
	wxString m_className;
	DatabaseLayer* m_database;
	wxString m_idField;
	bool m_autoIncr;
	bool m_readOnly;
	wxString m_version;

	wxString m_customArClass;
	wxString m_customArRowClass;
	wxString m_customArRowSetClass;
	wxString m_customImpls;
public:
	wxActiveRecordGenerator();
	wxActiveRecordGenerator(DatabaseLayer* database);
	~wxActiveRecordGenerator();
	/**
	 \param dbType Is the database type from enum{AR_SQLITE=0,AR_POSTGRESQL,AR_MYSQL};
     \param name Is the filename for sqlite3 or the database name for postgres and mysql

	*/
	wxActiveRecordGenerator(int dbType,const wxString& name,const wxString& server=wxT(""),const wxString& user=wxT(""),const wxString& password=wxT(""));
	/**
	 \param dbType Is the database type from enum{AR_SQLITE=0,AR_POSTGRESQL,AR_MYSQL};
     \param name Is the filename for sqlite3 or the database name for postgres and mysql

	*/
	bool Create(int dbType,const wxString& name,const wxString& server=wxT(""),const wxString& user=wxT(""),const wxString& password=wxT(""));
	/**
		\param table The name of the table that this activeRecord models.
		\param arClassName The name of the active record class that will be or was generated.

		\return False if the table doesn't exist.
	*/
	bool Prepare(const wxString& table,const wxString& arClassName,const wxString& idField=wxT("id"),bool autoIncr=true);
	/**
		\param node

		\return False if the table doesn't exist.
	*/
	bool Prepare(arNode node);
	/**
		This will generate a method that returns derivated wxActiveRecordRowSet. It will asume that the name of the class will be "{arClassName}RowSet".

		\param itemName The name of the items that this active record has many of, usually it's a plural noun.
		\param table The table of objects that this activeRecord has many.
		\param foreignid The columnname of the active record id on the foreign table that points to this active record
		\param keyId The id field of the foreign table.
		\param arClassName The name of the active record class that will be or was generated. This will also include the class header

		\return False if the table or the foreign id doesn't exist.
	*/
	bool HasMany(const wxString& itemName,const wxString& table,const wxString& foreignId,const wxString& keyId,const wxString& arClassName);
	/**
		This will generate a method that returns derivated wxActiveRecordRow. It will asume that the name of the class will be "{arClassName}Row".

		\param itemName The name of the items that this active record belong to, usually it's a single noun.
		\param table The table of objects that this activeRecord belongs to.
		\param foreingid The columnname of the foreing id on this active record that points to the owner active record.
		\param keyId The id field of the foreign table.
		\param arClassName The name of the active record class that will be or was generated. This will also include the class header

		\return False if the table or the foreign id doesn't exist.
	*/
	bool BelongsTo(const wxString& itemName,const wxString& table,const wxString& foreignId,const wxString& keyId,const wxString& arClassName);

	bool GenerateH(bool overwrite, const wxString& dir=wxEmptyString, const wxString& fn=wxEmptyString);
	bool GenerateCpp(bool overwrite, const wxString& dir=wxEmptyString, const wxString& fn=wxEmptyString);
	
	bool TableExists(const wxString& table)const;
	bool ViewExists(const wxString& table)const;
	wxArrayString GetTables() const;
	wxArrayString GetViews() const;
	wxArrayString GetColumns(const wxString& table) const;

	int GetType() const;
	wxString GetTypeStr() const;
	static wxString CapitalizeTable(const wxString& word);
	static wxString Pluralize(const wxString& word);
	static wxString DePluralize(const wxString& word);
	static wxString TypeToStr(int type);
	static int StrToType(const wxString& strType);

	static wxString GetCustomArClass(const wxString& strFile); //get the string between ////@@begin custom arClass markers
	static wxString GetCustomArRowClass(const wxString& strFile); //get the string between ////@@begin custom arRow markers
	static wxString GetCustomArRowSetClass(const wxString& strFile); //get the string between ////@@begin custom arRowSet markers
	static wxString GetCustomImpls(const wxString& strFile); //get the string between ////@@begin custom implementations markers
	void Clean();
	void SetVersion(const wxString& value){
		m_version=value;
	}
	wxString GetVersion()const{
		return m_version;
	}
	static bool IsSupported(int dbType);
protected:
	
	bool GenerateDecl(wxString& fileContent) const;
	bool GenerateImpl(wxString& fileContent) const;
	wxString GetIfdef() const;
	wxString GetSave() const;
	wxString GetInsert() const;
	wxString GetIncludes() const;
	wxString GetRowFromResult() const;
	wxString GetGetFromResult() const;
	wxString GetRelationsDecl() const;
	wxString GetRelationsImpl() const;
	wxString GetCmpsDecl() const;
	wxString GetCmpsImpl() const;
	wxString GetForwardDecls() const;
	wxString GetIdFieldDecl() const;
	wxString GetIdFieldImpl() const;
	wxString GetDeleteIdDecl() const;
	wxString GetDeleteIdImpl() const;
	wxString GetRowDeleteDecl() const;
	wxString GetRowDeleteImpl() const;


private:
	static wxString GetVarDecl(const wxString& key,int value);
	static wxString GetTypeString(int value);
	static wxString GetParamString(int value);


};

struct arRelation{

	int relationType;
	wxString itemName;
	wxString table;
	wxString foreignId;
	wxString arClassName;
	wxString keyId;

	arRelation(){
		relationType=0;
	}
	arRelation(const arRelation& src){
		relationType=src.relationType;
		itemName=src.itemName;
		table=src.table;
		foreignId=src.foreignId;
		arClassName=src.arClassName;
		keyId=src.keyId;
	}
	
	arRelation& arRelation::operator=(const arRelation& src){
		relationType=src.relationType;
		itemName=src.itemName;
		table=src.table;
		foreignId=src.foreignId;
		arClassName=src.arClassName;
		keyId=src.keyId;

		return *this;
	}
};

struct arNode{
	wxString m_tableName;
	wxString m_className;
	wxString m_idField;
	wxString m_type;
	bool m_autoIncr;
	bool m_overwrite;
	bool m_readOnly;
	RelationsArray m_hasManyRelations;
	RelationsArray m_belongsToRelations;

	arNode(){
		m_tableName=wxT("");
		m_type=wxT("table");
		m_idField=wxT("");
		m_autoIncr=true;
		m_overwrite=false;
		m_readOnly=false;
	}
	arNode(const wxString& table, const wxString& idField=wxT("")){
		m_idField=idField;;
		m_tableName=table;
		m_type=wxT("table");
		m_overwrite=false;
		m_autoIncr=true;
		m_readOnly=false;
		m_className=wxActiveRecordGenerator::DePluralize(wxActiveRecordGenerator::CapitalizeTable(table));
	}

	arNode(const arNode& src){
		m_tableName=src.m_tableName;
		m_type=src.m_type;
		m_className=src.m_className;
		m_idField=src.m_idField;
		m_autoIncr=src.m_autoIncr;
		m_overwrite=src.m_overwrite;
		m_readOnly=src.m_readOnly;
		for(unsigned int i=0;i<src.m_hasManyRelations.Count();++i)
			m_hasManyRelations.Add(new arRelation(*src.m_hasManyRelations.Item(i)));
		for(unsigned int i=0;i<src.m_belongsToRelations.Count();++i)
			m_belongsToRelations.Add(new arRelation(*src.m_belongsToRelations.Item(i)));
	}
	~arNode();
};


#endif /* _WX_ACTIVE_RECORD_GENERATOR_H_ */
