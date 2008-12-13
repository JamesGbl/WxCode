/////////////////////////////////////////////////////////////////////////////
// Name:       wxActiveRecordGenerator.cpp
// Purpose:
// Author:      Matías Szeftel
// Modified by:
// Created:     01/10/2006 16:36:21
// RCS-ID:
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

#include "../include/wxActiveRecordGenerator.h"

wxActiveRecordGenerator::wxActiveRecordGenerator(){
	m_database=0;
	m_readOnly=false;
}
wxActiveRecordGenerator::wxActiveRecordGenerator(DatabaseLayer* database){
	if(m_database!=0){
		delete m_database;
		m_database=database;
	}
	m_readOnly=false;
}
wxActiveRecordGenerator::~wxActiveRecordGenerator(){
	Clean();
	if(m_database!=NULL)
		delete m_database;

}

wxActiveRecordGenerator::wxActiveRecordGenerator(int dbType,const wxString& name,const wxString& server,const wxString& user,const wxString& password){
	Create(dbType,name,server,user,password);
}

bool wxActiveRecordGenerator::Create(int dbType,const wxString& name,const wxString& server,const wxString& user,const wxString& password){
	m_dbType=dbType;
	m_readOnly=false;
	try{
		if(dbType==AR_SQLITE){
#ifdef AR_USE_SQLITE
			m_database=new SqliteDatabaseLayer(name);
			return true;
#else
			return false;
#endif
		}
		else if(dbType==AR_POSTGRESQL){
#ifdef AR_USE_POSTGRESQL
			m_database=new PostgresDatabaseLayer(server,name,user,password);
			return true;
#else
			return false;
#endif
		}
		else if(dbType==AR_MYSQL){
#ifdef AR_USE_MYSQL
			m_database=new MysqlDatabaseLayer(server,name,user,password);
			return true;
#else
			return false;
#endif
		}
		else if(dbType==AR_FIREBIRD){
#ifdef AR_USE_FIREBIRD
			m_database=new FirebirdDatabaseLayer(server,name,user,password);
			return true;
#else
			return false;
#endif
		}
		else if(dbType==AR_FIREBIRD_EMBEDDED){
#ifdef AR_USE_FIREBIRD
			m_database=new FirebirdDatabaseLayer(name,user,password);
			return true;
#else
			return false;
#endif
		}
		else
			return false;
	}
	catch(DatabaseLayerException& e){
		throw(e);
		return false;
	}
	return false;
}

bool wxActiveRecordGenerator::Prepare(const wxString& table,const wxString& arClassName,const wxString& idField,bool autoIncr){
	Clean();
	if(!TableExists(table))
		return false;
	try{
		m_tableName=table;
		m_className=arClassName;
		m_idField=idField;
		m_autoIncr=autoIncr;
		
		wxString query=wxString::Format(wxT("SELECT * FROM %s LIMIT 1"),table.c_str());
		DatabaseResultSet* result=m_database->ExecuteQuery(query);
		result->Next();
		ResultSetMetaData* metaData=result->GetMetaData();

		// 1-based
		for(int i=1;i<=metaData->GetColumnCount();i++){
			wxString md_name=metaData->GetColumnName(i);
			m_varHash[metaData->GetColumnName(i)]=metaData->GetColumnType(i);
		}
		result->CloseMetaData(metaData);
		m_database->CloseResultSet(result);
		return true;
	}
	catch(DatabaseLayerException& e){
		throw(e);
		return false;
	}
}

bool wxActiveRecordGenerator::Prepare(arNode node){
	Clean();
	if(node.m_type.CmpNoCase(wxT("table"))==0){
		if(!TableExists(node.m_tableName))
			return false;
	}
	else if(node.m_type.CmpNoCase(wxT("view"))==0){
		if(!ViewExists(node.m_tableName))
			return false;
	}
	else
		return false;


	try{
		m_tableName=node.m_tableName;
		m_className=node.m_className;
		m_idField=node.m_idField;
		m_autoIncr=node.m_autoIncr;
		m_readOnly=node.m_readOnly;
		for(unsigned int i=0;i<node.m_belongsToRelations.Count();i++)
			BelongsTo(node.m_belongsToRelations.Item(i)->itemName,node.m_belongsToRelations.Item(i)->table,node.m_belongsToRelations.Item(i)->foreignId,node.m_belongsToRelations.Item(i)->keyId,node.m_belongsToRelations.Item(i)->arClassName);
		for(unsigned int i=0;i<node.m_hasManyRelations.Count();i++)
			HasMany(node.m_hasManyRelations.Item(i)->itemName,node.m_hasManyRelations.Item(i)->table,node.m_hasManyRelations.Item(i)->foreignId,node.m_hasManyRelations.Item(i)->keyId,node.m_hasManyRelations.Item(i)->arClassName);

		wxString query=wxString::Format(wxT("SELECT * FROM %s LIMIT 1"),node.m_tableName.c_str());
		DatabaseResultSet* result=m_database->ExecuteQuery(query);
		result->Next();
		ResultSetMetaData* metaData=result->GetMetaData();
		// 1-based
		for(int i=1;i<=metaData->GetColumnCount();i++){
			wxString md_name=metaData->GetColumnName(i);
			m_varHash[metaData->GetColumnName(i)]=metaData->GetColumnType(i);
		}

		result->CloseMetaData(metaData);
		m_database->CloseResultSet(result);
		return true;
	}
	catch(DatabaseLayerException& e){
		throw(e);
		return false;
	}
}

bool wxActiveRecordGenerator::TableExists(const wxString& table)const{
	return m_database->TableExists(table);
}

bool wxActiveRecordGenerator::ViewExists(const wxString& table)const{
	return m_database->ViewExists(table);
}

wxArrayString wxActiveRecordGenerator::GetTables()const{
	wxArrayString arr=m_database->GetTables();
	return arr;
}

wxArrayString wxActiveRecordGenerator::GetViews()const{
	return m_database->GetViews();
}

wxArrayString wxActiveRecordGenerator::GetColumns(const wxString& table)const{
	return m_database->GetColumns(table);
}

bool wxActiveRecordGenerator::HasMany(const wxString& itemName,const wxString& table,const wxString& foreignId,const wxString& keyId,const wxString& arClassName){
	if(!TableExists(table))
		return false;
	arRelation* newRelation=new arRelation();
	newRelation->relationType=AR_HAS_MANY;
	newRelation->itemName=itemName;
	newRelation->table=table;
	newRelation->foreignId=foreignId;
	newRelation->keyId=keyId;
	newRelation->arClassName=arClassName;

	m_relations.Add(newRelation);

	return true;
}

bool wxActiveRecordGenerator::BelongsTo(const wxString& itemName,const wxString& table,const wxString& foreignId,const wxString& keyId,const wxString& arClassName){
	if(!TableExists(table))
		return false;
	arRelation* newRelation=new arRelation();
	newRelation->relationType=AR_BELONGS_TO;
	newRelation->itemName=itemName;
	newRelation->table=table;
	newRelation->foreignId=foreignId;
	newRelation->keyId=keyId;
	newRelation->arClassName=arClassName;

	m_relations.Add(newRelation);

	return true;
}

bool wxActiveRecordGenerator::GenerateDecl(wxString& fileContent)const{
	{
		wxRegEx reg(wxT("////@@begin template arWrite(.*)////@@end template arWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}
	{
		wxRegEx reg(wxT("////@@begin template arRowWrite(.*)////@@end template arRowWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}
	{
		wxRegEx reg(wxT("////@@begin template arSetWrite(.*)////@@end template arSetWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}

	fileContent.Replace(wxT("%%IFDEF%%"),GetIfdef(),true);
	fileContent.Replace(wxT("%%ARROWSETNAME%%"),m_className+wxT("RowSet"),true);
	fileContent.Replace(wxT("%%ARROWNAME%%"),m_className+wxT("Row"),true);
	fileContent.Replace(wxT("%%ARNAME%%"),m_className,true);
	fileContent.Replace(wxT("%%TABLENAME%%"),m_tableName,true);
	
	fileContent.Replace(wxT("%%INCLUDES%%"),GetIncludes(),true);
	fileContent.Replace(wxT("%%FORWARDDECLS%%"),GetForwardDecls(),true);
	fileContent.Replace(wxT("%%IDFIELDDECL%%"),GetIdFieldDecl(),true);
	fileContent.Replace(wxT("%%DELETEIDDECL%%"),GetDeleteIdDecl(),true);
	
	
	fileContent.Replace(wxT("%%ARG_VERSION%%"),GetVersion(),true);
	if(m_dbType==AR_SQLITE)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxSqliteActiveRecord"),true);
	else if(m_dbType==AR_POSTGRESQL)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxPostgresActiveRecord"),true);
	else if(m_dbType==AR_MYSQL)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxMysqlActiveRecord"),true);
	else if(m_dbType==AR_FIREBIRD)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxFirebirdActiveRecord"),true);
	else if(m_dbType==AR_FIREBIRD_EMBEDDED)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxFirebirdEmbeddedActiveRecord"),true);

	//VARIABLE DECLARATION
	wxString vars=wxT("");
	VarHash::const_iterator it;
    for( it = m_varHash.begin() ; it != m_varHash.end(); ++it )
    {
		if(it!=m_varHash.begin()) vars+=wxT("\t");
		vars+=GetVarDecl(it->first,it->second)+wxT("\n");
    }

	fileContent.Replace(wxT("%%VARDECL%%"),vars,true);

	if(m_customArClass.IsEmpty())
		fileContent.Replace(wxT("%%CUSTOMAR%%"),wxT("public:"),true);
	else
		fileContent.Replace(wxT("%%CUSTOMAR%%"),m_customArClass,true);
	if(m_customArRowClass.IsEmpty())
		fileContent.Replace(wxT("%%CUSTOMARROW%%"),wxT("public:"),true);
	else
		fileContent.Replace(wxT("%%CUSTOMARROW%%"),m_customArRowClass,true);
	if(m_customArClass.IsEmpty())
		fileContent.Replace(wxT("%%CUSTOMARROWSET%%"),wxT("public:"),true);
	else
		fileContent.Replace(wxT("%%CUSTOMARROWSET%%"),m_customArRowSetClass,true);

	//FUNCTION DECLARATION

	fileContent.Replace(wxT("%%RELATIONSDECL%%"),GetRelationsDecl(),true);

	fileContent.Replace(wxT("%%CMPSDECL%%"),GetCmpsDecl(),true);

	return true;
}

wxString wxActiveRecordGenerator::GetIfdef() const{
	wxString ifdef=m_className;
	wxRegEx regEx(wxT("[A-Z]"));
	regEx.ReplaceAll(&ifdef,wxT("_&"));
	ifdef.MakeUpper();
	ifdef=wxT("_")+ifdef+wxT("_");

	return ifdef;
}

bool wxActiveRecordGenerator::GenerateImpl(wxString& fileContent)const{
	{
		wxRegEx reg(wxT("////@@begin template arWrite(.*)////@@end template arWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}
	{
		wxRegEx reg(wxT("////@@begin template arRowWrite(.*)////@@end template arRowWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}
	{
		wxRegEx reg(wxT("////@@begin template arSetWrite(.*)////@@end template arSetWrite"));
		bool matched=reg.Matches(fileContent);
		if(matched){
			if(m_readOnly)
				reg.Replace(&fileContent,wxT(""));
			else
				reg.Replace(&fileContent,wxT("\\1"));
		}
	}

	fileContent.Replace(wxT("%%ARROWSETNAME%%"),m_className+wxT("RowSet"),true);
	fileContent.Replace(wxT("%%ARROWNAME%%"),m_className+wxT("Row"),true);
	fileContent.Replace(wxT("%%ARNAME%%"),m_className,true);
	fileContent.Replace(wxT("%%IDFIELDIMPL%%"),GetIdFieldImpl(),true);
	fileContent.Replace(wxT("%%DELETEIDIMPL%%"),GetDeleteIdImpl(),true);
	fileContent.Replace(wxT("%%ROWDELETEIMPL%%"),GetRowDeleteImpl(),true);
	fileContent.Replace(wxT("%%ARG_VERSION%%"),GetVersion(),true);

	if(m_dbType==AR_SQLITE)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxSqliteActiveRecord"),true);
	else if(m_dbType==AR_POSTGRESQL)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxPostgresActiveRecord"),true);
	else if(m_dbType==AR_MYSQL)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxMysqlActiveRecord"),true);
	else if(m_dbType==AR_FIREBIRD)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxFirebirdActiveRecord"),true);
	else if(m_dbType==AR_FIREBIRD_EMBEDDED)
		fileContent.Replace(wxT("%%AR_BASE_CLASS%%"),wxT("wxFirebirdEmbeddedActiveRecord"),true);

	wxString vars=wxT("");
	VarHash::const_iterator it;
    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		if(it!=m_varHash.begin()) vars+=wxT("\t");
		vars+=it->first+wxT("=src.")+it->first+wxT(";\n");
    }

	fileContent.Replace(wxT("%%COPYCONST%%"),vars,true);
	fileContent.Replace(wxT("%%SAVE%%"),GetSave(),true);
	fileContent.Replace(wxT("%%INSERT%%"),GetInsert(),true);
	fileContent.Replace(wxT("%%ROWFROMRESULT%%"),GetRowFromResult(),true);
	fileContent.Replace(wxT("%%GETFROMRESULT%%"),GetGetFromResult(),true);
	fileContent.Replace(wxT("%%RELATIONSIMPL%%"),GetRelationsImpl(),true);
	fileContent.Replace(wxT("%%CMPSIMPL%%"),GetCmpsImpl(),true);
	fileContent.Replace(wxT("%%CUSTOMIMPLS%%"),m_customImpls,true);
	return true;
}


wxString wxActiveRecordGenerator::GetVarDecl(const wxString& key,int value){
	return GetTypeString(value)+wxT(" ")+key+wxT(";");
}

wxString wxActiveRecordGenerator::GetTypeString(int value){
	wxString str;
	if(value==ResultSetMetaData::COLUMN_INTEGER)
		str=wxT("int");
	else if(value==ResultSetMetaData::COLUMN_STRING)
		str=wxT("wxString");
	else if(value==ResultSetMetaData::COLUMN_DOUBLE)
		str=wxT("double");
	else if(value==ResultSetMetaData::COLUMN_BOOL)
		str=wxT("bool");
	else if(value==ResultSetMetaData::COLUMN_BLOB)
		str=wxT("wxString");
	else if(value==ResultSetMetaData::COLUMN_DATE)
		str=wxT("wxDateTime");
	else
		str=wxT("wxString");

	return str;
}

wxString wxActiveRecordGenerator::GetParamString(int value){
	wxString str;
	if(value==ResultSetMetaData::COLUMN_INTEGER)
		str=wxT("Int");
	else if(value==ResultSetMetaData::COLUMN_STRING)
		str=wxT("String");
	else if(value==ResultSetMetaData::COLUMN_DOUBLE)
		str=wxT("Double");
	else if(value==ResultSetMetaData::COLUMN_BOOL)
		str=wxT("Bool");
	else if(value==ResultSetMetaData::COLUMN_BLOB)
		str=wxT("Blob");
	else if(value==ResultSetMetaData::COLUMN_DATE)
		str=wxT("Date");
	else
		str=wxT("String");

	return str;
}

wxString wxActiveRecordGenerator::GetIdFieldDecl() const{
	wxString str=wxT("");

	str+=m_className+wxT("Row* ")+wxActiveRecordGenerator::CapitalizeTable(m_idField)+wxT("(");
	VarHash::const_iterator it=m_varHash.find(m_idField);
	str+=GetTypeString(it->second)+wxT(" key);\n");

	return str;
}

wxString wxActiveRecordGenerator::GetIdFieldImpl() const{
	wxString str=wxT("");

	str+=m_className+wxT("Row* ")+m_className+wxT("::")+wxActiveRecordGenerator::CapitalizeTable(m_idField)+wxT("(");
	VarHash::const_iterator it=m_varHash.find(m_idField);
	str+=GetTypeString(it->second)+wxT(" key){\n");


	str+=wxT("\ttry{\n");
	str+=wxString::Format(wxT("\t\tPreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT(\"SELECT * FROM %%s WHERE %s=?\"),m_table.c_str()));\n"),m_idField.c_str());
	str+=wxString::Format(wxT("\t\tpStatement->SetParam%s(1,key);\n"),GetParamString(it->second).c_str());
	str+=wxT("\t\tDatabaseResultSet* result= pStatement->ExecuteQuery();\n\n");

	str+=wxT("\t\tif(!result->Next())\n");
	str+=wxT("\t\t\treturn NULL;");
	str+=wxT("\t\t")+m_className+wxT("Row* ")+wxT("row=RowFromResult(result);\n");
	str+=wxT("\t\tgarbageRows.Add(row);\n");
	str+=wxT("\t\tm_database->CloseResultSet(result);\n");
	str+=wxT("\t\tm_database->CloseStatement(pStatement);\n");
	str+=wxT("\t\treturn row;\n");
	str+=wxT("\t}\n");
	str+=wxT("\tcatch (DatabaseLayerException& e)\n\t{\n");
	str+=wxT("\t\tProcessException(e);\n");
	str+=wxT("\t\treturn NULL;\n\t}\n");
	str+=wxT("}\n");

	return str;
}



wxString wxActiveRecordGenerator::GetDeleteIdDecl() const{
	//bool Delete(key_type key);
	wxString str=wxT("");
	VarHash::const_iterator it=m_varHash.find(m_idField);
	str+=wxT("bool Delete(")+GetTypeString(it->second)+wxT(" key);\n");

	return str;
}

wxString wxActiveRecordGenerator::GetDeleteIdImpl() const{

	wxString str=wxT("");
	VarHash::const_iterator it=m_varHash.find(m_idField);
	str+=wxT("bool ")+m_className+wxT("::Delete(")+GetTypeString(it->second)+wxT(" key){\n");
	str+=wxT("\ttry{\n");
	str+=wxString::Format(wxT("\t\tPreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT(\"DELETE FROM %%s WHERE %s=?\"),m_table.c_str()));\n"),m_idField.c_str());
	str+=wxT("\t\tpStatement->SetParam")+GetParamString(it->second)+wxT("(1,key);\n");
	str+=wxT("\t\tpStatement->ExecuteUpdate();\n");
	str+=wxT("\t\treturn true;\n");
	str+=wxT("\t}\n");
	str+=wxT("\tcatch(DatabaseLayerException& e){\n");
	str+=wxT("\t\tthrow(e);\n");
	str+=wxT("\t\treturn false;\n");
	str+=wxT("\t}\n");
	str+=wxT("}\n");

	return str;
}

wxString wxActiveRecordGenerator::GetRowDeleteDecl() const{
	//bool Delete(key_type key);
	wxString str=wxT("");
	VarHash::const_iterator ir=m_varHash.find(m_idField);
	str+=wxT("bool Delete();\n");

	return str;
}

wxString wxActiveRecordGenerator::GetRowDeleteImpl() const{
	wxString str=wxT("");
	VarHash::const_iterator it=m_varHash.find(m_idField);
	str+=wxT("bool ")+m_className+wxT("Row::Delete(){\n");
	str+=wxT("\ttry{\n");
	str+=wxString::Format(wxT("\t\tPreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT(\"DELETE FROM %%s WHERE %s=?\"),m_table.c_str()));\n"),m_idField.c_str());
	str+=wxT("\t\tpStatement->SetParam")+GetParamString(it->second)+wxString::Format(wxT("(1,%s);\n"),m_idField.c_str());
	str+=wxT("\t\tpStatement->ExecuteUpdate();\n");
	str+=wxT("\t\treturn true;\n");
	str+=wxT("\t}\n");
	str+=wxT("\tcatch(DatabaseLayerException& e){\n");
	str+=wxT("\t\tthrow(e);\n");
	str+=wxT("\t\treturn false;\n");
	str+=wxT("\t}\n");
	str+=wxT("}\n");

	return str;
}

wxString wxActiveRecordGenerator::GetSave() const{
	/*
		PreparedStatement* pStatement=m_activeRecord->GetDatabase()->PrepareStatement(wxString::Format(wxT("UPDATE %s SET titulo=?,instrucciones=?,descripcion=?,fecha=? WHERE id=?"),m_activeRecord->GetTable().c_str()));
		pStatement->SetParamString(1,titulo);
		pStatement->SetParamString(2,instrucciones);
		pStatement->SetParamString(3,descripcion);
		pStatement->SetParamDate(4,fecha);
		pStatement->SetParamInt(5,id);
	*/
	wxString str=wxT("PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT(\"UPDATE %s SET ");

	VarHash::const_iterator it;

    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		if(it->first==m_idField)
			continue;
		if(it!=m_varHash.begin())
			str+=wxT(",");
		str+=it->first+wxT("=?");

    }
	str+=wxString::Format(wxT(" WHERE %s=?\"),m_table.c_str()));\n"),m_idField.c_str());

	int i;
	wxString lastSetParam=wxT("");
	for( i=1,it = m_varHash.begin(); it != m_varHash.end(); ++it ,++i)
    {
		if(it->first==m_idField){
			--i;
			lastSetParam+=wxT("\t\t\tpStatement->SetParam")+GetParamString(it->second);
			lastSetParam+=wxString::Format(wxT("(%d,%s);\n"),m_varHash.size(),it->first.c_str());
			continue;
		}
		
		if(it->second==ResultSetMetaData::COLUMN_DATE){
			lastSetParam+=wxString::Format(wxT("\t\t\tif(%s.IsValid())\n"),it->first.c_str());
			lastSetParam+=wxT("\t\t\t\tpStatement->SetParam")+GetParamString(it->second);
			lastSetParam+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
			lastSetParam+=wxString::Format(wxT("\t\t\telse\n"));
			lastSetParam+=wxT("\t\t\t\tpStatement->SetParamNull");
			lastSetParam+=wxString::Format(wxT("(%d);\n"),i);
		}
		else{
			lastSetParam+=wxT("\t\t\tpStatement->SetParam")+GetParamString(it->second);
			lastSetParam+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
		}
		//str+=wxT("\t\t\tpStatement->SetParam")+GetParamString(it->second);
		//str+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
	}

	str+=lastSetParam;

	str+=wxT("\t\t\tpStatement->RunQuery();\n");
	str+=wxT("\t\t\tm_database->CloseStatement(pStatement);\n");

	return str;


}

wxString wxActiveRecordGenerator::GetForwardDecls() const{
	wxString str=wxT("");
	for(unsigned int i=0;i<m_relations.GetCount();i++){
		if(m_relations.Item(i)->arClassName.Cmp(this->m_className)==0)
			continue;
		str+=wxT("class ")+m_relations.Item(i)->arClassName+wxT(";\n");
		str+=wxT("class ")+m_relations.Item(i)->arClassName+wxT("Row;\n");
		str+=wxT("class ")+m_relations.Item(i)->arClassName+wxT("RowSet;\n");
	}

	return str;
}

wxString wxActiveRecordGenerator::GetInsert() const{
	/*
		PreparedStatement* pStatement=m_activeRecord->GetDatabase()->PrepareStatement(wxString::Format(wxT("INSERT INTO %s (titulo,instrucciones,descripcion,fecha) VALUES (?,?,?,?)"),m_activeRecord->GetTableName().c_str()));
		pStatement->SetParamString(1,titulo);
		pStatement->SetParamString(2,instrucciones);
		pStatement->SetParamString(3,descripcion);
		pStatement->SetParamDate(4,fecha);
		pStatement->RunQuery();
	*/

	wxString str=wxT("PreparedStatement* pStatement=m_database->PrepareStatement(wxString::Format(wxT(\"INSERT INTO %s (");

	VarHash::const_iterator it;

    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		if(it->first==m_idField && m_autoIncr)
			continue;
		if(it!=m_varHash.begin())
			str+=wxT(",");
		str+=it->first;
	}

	str+=wxT(") VALUES (");
	for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		if(it->first==m_idField && m_autoIncr)
			continue;
		if(it!=m_varHash.begin())
			str+=wxT(",");
		str+=wxT("?");
	}

	str+=wxT(")\"),m_table.c_str()));\n");

	int i;
	for( i=1,it = m_varHash.begin(); it != m_varHash.end(); ++it ,++i)
    {
		if(it->first==m_idField && m_autoIncr){
			--i;
			continue;
		}
		
		if(it->second==ResultSetMetaData::COLUMN_DATE){
			str+=wxString::Format(wxT("\t\t\tif(%s.IsValid())\n"),it->first.c_str());
			str+=wxT("\t\t\t\tpStatement->SetParam")+GetParamString(it->second);
			str+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
			str+=wxString::Format(wxT("\t\t\telse\n"));
			str+=wxT("\t\t\t\tpStatement->SetParamNull");
			str+=wxString::Format(wxT("(%d);\n"),i);
		}
		else{
			str+=wxT("\t\t\tpStatement->SetParam")+GetParamString(it->second);
			str+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
		}
		//str+=wxT("\t\t\tpStatement->SetParam")+GetParamString(it->second);
		//str+=wxString::Format(wxT("(%d,%s);\n"),i,it->first.c_str());
	}

	str+=wxT("\t\t\tpStatement->RunQuery();\n");
	str+=wxT("\t\t\tm_database->CloseStatement(pStatement);\n");

	return str;
}

wxString wxActiveRecordGenerator::GetIncludes() const{
	//includes for the relations.
	wxString str=wxT("");
	for(unsigned int i=0;i<m_relations.GetCount();i++)
		str+=wxString::Format(wxT("#include \"%s.h\"\n"),m_relations.Item(i)->arClassName.c_str());

	return str;
}



wxString wxActiveRecordGenerator::GetRowFromResult() const{
	wxString vars=wxT("");
	VarHash::const_iterator it;
    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		if(it!=m_varHash.begin()) vars+=wxT("\t");
		vars+=wxT("row->")+it->first+wxT("=result->GetResult")+GetParamString(it->second);
		vars+=wxT("(wxT(\"")+it->first+wxT("\"));\n");
    }
	return vars;
}

wxString wxActiveRecordGenerator::GetGetFromResult() const{
	wxString vars=wxT("");
	VarHash::const_iterator it;
    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		vars+=wxT("\t")+it->first+wxT("=result->GetResult")+GetParamString(it->second);
		vars+=wxT("(wxT(\"")+it->first+wxT("\"));\n");
    }
	return vars;
}

wxString wxActiveRecordGenerator::GetRelationsDecl() const{
	//if AR_HAS_MANY str like wxFooRowSet* GetFoos() const;
	//if AR_BELONGS_TO str like wxFooRow* GetFoo() const;

	wxString str=wxT("");
	for(unsigned int i=0;i<m_relations.GetCount();i++){
		if(i!=0) str+=wxT("\t");
		str+=m_relations.Item(i)->arClassName;
		if(m_relations.Item(i)->relationType==AR_HAS_MANY)
			str+=wxT("RowSet* Get")+m_relations.Item(i)->itemName+wxT("(const wxString& orderBy=wxEmptyString);\n");
		else
			str+=wxT("Row* Get")+m_relations.Item(i)->itemName+wxT("();\n");
		
	}

	return str;
}

wxString wxActiveRecordGenerator::GetRelationsImpl() const{
	wxString str=wxT("");
	for(unsigned int i=0;i<m_relations.GetCount();i++){
		wxString returntype;
		arRelation* testRel=m_relations.Item(i);
		if(m_relations.Item(i)->relationType==AR_HAS_MANY)
			returntype=m_relations.Item(i)->arClassName+wxT("RowSet* ");
		else
			returntype=m_relations.Item(i)->arClassName+wxT("Row* ");

		str+=returntype+m_className+wxT("Row::")+wxT("Get")+m_relations.Item(i)->itemName;

		if(m_relations.Item(i)->relationType==AR_HAS_MANY){
			str+=wxT("(const wxString& orderBy){\n");
			str+=wxT("\t")+returntype+wxT("set= new ")+m_relations.Item(i)->arClassName+wxString::Format(wxT("RowSet(m_database,wxT(\"%s\"));\n"),m_relations.Item(i)->table.c_str());
			str+=wxString::Format(wxT("\twxString str_statement =wxT(\"SELECT * FROM %s WHERE %s=?\");\n"),m_relations.Item(i)->table.c_str(),m_relations.Item(i)->foreignId.c_str());
			str+=wxString::Format(wxT("\tif(!orderBy.IsEmpty())\n\t\tstr_statement+=wxT(\" ORDER BY \")+orderBy;\n"));
			str+=wxString::Format(wxT("\tPreparedStatement* pStatement=m_database->PrepareStatement(str_statement);\n"));

			VarHash::const_iterator it=m_varHash.find(m_idField);
			str+=wxT("\tpStatement->SetParam")+GetParamString(it->second);
			str+=wxString::Format(wxT("(1,%s);\n"),m_idField.c_str());

			str+=wxT("\tDatabaseResultSet* result= pStatement->ExecuteQuery();\n\n");

			str+=wxT("\twhile(result->Next()){\n");
			str+=wxT("\t\t")+m_relations.Item(i)->arClassName+wxT("Row* toAdd=new ")+m_relations.Item(i)->arClassName+wxString::Format(wxT("Row(m_database,wxT(\"%s\"));\n"),m_relations.Item(i)->table.c_str());
			str+=wxT("\t\ttoAdd->GetFromResult(result);\n");
			str+=wxT("\t\tset->Add(toAdd);\n");
			str+=wxT("\t}\n");
			str+=wxT("\tgarbageRowSets.Add(set);\n");
			str+=wxT("\tm_database->CloseResultSet(result);\n");
			str+=wxT("\tm_database->CloseStatement(pStatement);\n");
			str+=wxT("\treturn set;\n");


		}
		else{
			str+=wxT("(){\n");
			str+=wxT("\t")+returntype+wxT("owner= new ")+m_relations.Item(i)->arClassName+wxString::Format(wxT("Row(m_database,wxT(\"%s\"));\n"),m_relations.Item(i)->table.c_str());
			str+=wxString::Format(wxT("\tPreparedStatement* pStatement=m_database->PrepareStatement(wxT(\"SELECT * FROM %s WHERE %s=?\"));\n"),m_relations.Item(i)->table.c_str(),m_relations.Item(i)->keyId.c_str());

			str+=wxString::Format(wxT("\tpStatement->SetParamInt(1,%s);\n"),m_relations.Item(i)->foreignId.c_str());

			VarHash::const_iterator it=m_varHash.find(m_idField);
			str+=wxT("\tpStatement->SetParam")+GetParamString(it->second);
			str+=wxString::Format(wxT("(1,%s);\n"),m_relations.Item(i)->foreignId.c_str());

			str+=wxT("\tDatabaseResultSet* result= pStatement->ExecuteQuery();\n\n");

			str+=wxT("\tresult->Next();\n");
			str+=wxT("\towner->GetFromResult(result);\n");
			str+=wxT("\tgarbageRows.Add(owner);\n");
			str+=wxT("\tm_database->CloseResultSet(result);\n");
			str+=wxT("\tm_database->CloseStatement(pStatement);\n");
			str+=wxT("\treturn owner;\n");

		}
		str+=wxT("}\n\n");
	}

	return str;
}

wxString wxActiveRecordGenerator::GetCustomArClass(const wxString& strFile){
	wxString matchedStr;

	wxRegEx reg(wxT("////@@begin custom arClass\n(.*)////@@end custom arClass"));
	bool matched=reg.Matches(strFile);

	if(!reg.IsValid()) return wxT("");
	if(!matched) return wxT("");

	matchedStr=reg.GetMatch(strFile,1);

	return matchedStr;

}

wxString wxActiveRecordGenerator::GetCustomArRowClass(const wxString& strFile){
	wxString matchedStr;

	wxRegEx reg(wxT("////@@begin custom arRow\n(.*)////@@end custom arRow"));
	bool matched=reg.Matches(strFile);

	if(!reg.IsValid()) return wxT("");
	if(!matched) return wxT("");

	matchedStr=reg.GetMatch(strFile,1);

	return matchedStr;

}

wxString wxActiveRecordGenerator::GetCustomArRowSetClass(const wxString& strFile){
	wxString matchedStr;

	wxRegEx reg(wxT("////@@begin custom arSet\n(.*)////@@end custom arSet"));
	bool matched=reg.Matches(strFile);

	if(!reg.IsValid()) return wxT("");
	if(!matched) return wxT("");

	matchedStr=reg.GetMatch(strFile,1);

	return matchedStr;
}

wxString wxActiveRecordGenerator::GetCustomImpls(const wxString& strFile){
	wxString matchedStr;

	wxRegEx reg(wxT("////@@begin custom implementations\n(.*)////@@end custom implementations"));
	bool matched=reg.Matches(strFile);

	if(!reg.IsValid()) return wxT("");
	if(!matched) return wxT("");

	matchedStr=reg.GetMatch(strFile,1);

	return matchedStr;
}



bool wxActiveRecordGenerator::GenerateH(bool overwrite, const wxString& dir,const wxString& fn){

	wxFileName fileName(dir,m_className+wxT(".h"));


	if(fn!=wxEmptyString)
		fileName=wxFileName(dir,fn);

	if(!overwrite){
		wxString oldStr=wxT("");
		if(fileName.FileExists()){
			wxFFile oldFile;
			if(oldFile.Open(fileName.GetFullPath())){
				oldFile.ReadAll(&oldStr);
				oldFile.Close();
			}
		}

		m_customArClass=GetCustomArClass(oldStr);
		m_customArRowClass=GetCustomArRowClass(oldStr);
		m_customArRowSetClass=GetCustomArRowSetClass(oldStr);
	}


	wxString fileContent;
	{
	wxFFile file(wxT("wxActiveRecord.htmp"));
	if(!file.IsOpened()){
		wxMessageBox(wxT("Couldn't find wxActiveRecord.htmp"),wxT("Error"));
		return false;
	}
	file.ReadAll(&fileContent);
	file.Close();
	}

	GenerateDecl(fileContent);

	{
		wxFFile file;
		if(file.Open(fileName.GetFullPath(),wxT("w"))){
			file.Write(fileContent);
			file.Close();
		}
	}

	return true;
}



bool wxActiveRecordGenerator::GenerateCpp(bool overwrite, const wxString& dir,const wxString& fn){
	wxFileName fileName(dir,m_className+wxT(".cpp"));
	wxString fileContent;
	if(fn!=wxEmptyString)
		fileName=wxFileName(dir,fn);

	if(!overwrite){
		wxString oldStr=wxT("");
		if(fileName.FileExists()){
			wxFFile oldFile;

			if(oldFile.Open(fileName.GetFullPath())){
				oldFile.ReadAll(&oldStr);
				oldFile.Close();
			}
		}

		m_customImpls=GetCustomImpls(oldStr);
	}

	{
	wxFFile file(wxT("wxActiveRecord.ctmp"));
	if(!file.IsOpened()){
		wxMessageBox(wxT("Couldn't find wxActiveRecord.ctmp"),wxT("Error"));
		return false;
	}
	file.ReadAll(&fileContent);
	file.Close();
	}

	GenerateImpl(fileContent);

	{
	wxFFile file;
	wxString a = fileName.GetFullPath();
	if(file.Open(fileName.GetFullPath(),wxT("w"))){
		file.Write(fileContent);
		file.Close();
	}
	}

	return true;
}

wxString wxActiveRecordGenerator::GetCmpsDecl() const{
	wxString str=wxT("");

	VarHash::const_iterator it;

    for( it = m_varHash.begin(); it != m_varHash.end(); ++it ){
		if(it!=m_varHash.begin()) str+=wxT("\t");
		str+=wxString::Format(wxT("static int CMPFUNC_%s(wxActiveRecordRow** item1,wxActiveRecordRow** item2);\n"),it->first.c_str());
	}
	str+=wxString::Format(wxT("\tstatic int CMPFUNC_global(wxActiveRecordRow** item1,wxActiveRecordRow** item2);\n"));

	//generate GetCmpFunc
	str+=wxString::Format(wxT("\tvirtual CMPFUNC_proto GetCmpFunc(const wxString& var) const;\n"));

    return str;
}

wxString wxActiveRecordGenerator::GetCmpsImpl() const{
	wxString str=wxT("");

	VarHash::const_iterator it;

    for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		str+=wxString::Format(wxT("int %s::CMPFUNC_%s(wxActiveRecordRow** item1,wxActiveRecordRow** item2){\n"),(m_className+wxT("RowSet")).c_str(),it->first.c_str());
		str+=wxT("\t")+m_className+wxT("Row** m_item1=(")+m_className+wxT("Row**)item1;\n");
		str+=wxT("\t")+m_className+wxT("Row** m_item2=(")+m_className+wxT("Row**)item2;\n");

		if(it->second==ResultSetMetaData::COLUMN_INTEGER){
			str+=wxString::Format(wxT("\tif((*m_item1)->%s<(*m_item2)->%s)\n\t"),it->first.c_str(),it->first.c_str());
			str+=wxT("\treturn -1;");
			str+=wxString::Format(wxT("\n\telse if((*m_item1)->%s>(*m_item2)->%s)\n\t"),it->first.c_str(),it->first.c_str());
			str+=wxT("\treturn 1;");
			str+=wxT("\n\telse\n\t\treturn 0;");
		}
		else if(it->second==ResultSetMetaData::COLUMN_STRING){
			str+=wxString::Format(wxT("\treturn (*m_item1)->%s.Cmp((*m_item2)->%s);"),it->first.c_str(),it->first.c_str());
		}
		else if(it->second==ResultSetMetaData::COLUMN_DOUBLE){
			str+=wxString::Format(wxT("\tif((*m_item1)->%s<(*m_item2)->%s)\n\t"),it->first.c_str(),it->first.c_str());
			str+=wxT("\treturn -1;");
			str+=wxString::Format(wxT("\n\telse if((*m_item1)->%s>(*m_item2)->%s)\n\t"),it->first.c_str(),it->first.c_str());
			str+=wxT("\treturn 1;");
			str+=wxT("\n\telse\n\t\treturn 0;");
		}
		else if(it->second==ResultSetMetaData::COLUMN_BOOL){
			str+=wxT("\treturn 0;");
		}
		else if(it->second==ResultSetMetaData::COLUMN_BLOB){
			str+=wxT("\treturn 0;");
		}
		else if(it->second==ResultSetMetaData::COLUMN_DATE){
			str+=wxT("\tint res;\n");
			str+=wxString::Format(wxT("\tif(!(*m_item1)->%s.IsValid()){\n"),it->first.c_str());
			str+=wxString::Format(wxT("\t\tif(!(*m_item2)->%s.IsValid())\n"),it->first.c_str());
			str+=wxT("\t\t\treturn 0;\n");
			str+=wxT("\t\treturn 1;\n");
			str+=wxT("\t}\n");
			str+=wxString::Format(wxT("\telse if(!(*m_item2)->%s.IsValid())\n"),it->first.c_str());
			str+=wxT("\t\treturn -1;\n");
			str+=wxT("\telse{\n");
			str+=wxString::Format(wxT("\t\tif ((*m_item1)->%s.IsEarlierThan((*m_item2)->%s))\n"),it->first.c_str(),it->first.c_str());
			str+=wxT("\t\t\treturn 1;\n");
			str+=wxString::Format(wxT("\t\telse if ((*m_item1)->%s.IsLaterThan((*m_item2)->%s))\n"),it->first.c_str(),it->first.c_str());
			str+=wxT("\t\t\treturn -1;\n");
			str+=wxT("\t\telse return 0;\n");
			str+=wxT("\t}\n\n");
		}
		else
			str+=wxString::Format(wxT("\treturn (*m_item1)->%s.Cmp((*m_item2)->%s);"),it->first.c_str(),it->first.c_str());

		str+=wxT("\n}\n\n");

	}

	//generate GetCmpFunc
	/**
	CMPFUNC_proto arRecetaRowSet::GetCmpFunc(const wxString& var) const{
		if(var==wxT("id"))
			return (CMPFUNC_proto)CMPFUNC_id;
		else if(var==wxT("titulo"))
			return (CMPFUNC_proto)CMPFUNC_titulo;
		else
			return (CMPFUNC_proto)CMPFUNC_default;
	}
	*/


	str+=wxString::Format(wxT("CMPFUNC_proto %s::GetCmpFunc(const wxString& var) const{\n"),(m_className+wxT("RowSet")).c_str());
	str+=wxT("\t");

	for( it = m_varHash.begin(); it != m_varHash.end(); ++it )
    {
		str+=wxT("if(var==wxT(\"")+it->first+wxT("\"))\n");
		str+=wxT("\t\treturn (CMPFUNC_proto)CMPFUNC_")+it->first+wxT(";\n");
		str+=wxT("\telse ");
	}
	str+=wxT("\n\treturn (CMPFUNC_proto)CMPFUNC_default;\n");
	str+=wxT("}\n\n");

	return str;
}

void wxActiveRecordGenerator::Clean(){
	m_varHash.clear();
	while(m_relations.Count()>0){
		arRelation* toDel=m_relations.Item(0);
		delete toDel;
		m_relations.RemoveAt(0);
	}
	m_relations.Clear();
	m_className=wxT("");
	m_customArClass=wxT("");
	m_customArRowClass=wxT("");
	m_customArRowSetClass=wxT("");
	m_customImpls=wxT("");
}

int wxActiveRecordGenerator::GetType() const{
	return m_dbType;
}

wxString wxActiveRecordGenerator::GetTypeStr() const{
	return TypeToStr(m_dbType);
}

wxString wxActiveRecordGenerator::Pluralize(const wxString& word){
	wxString plural=word;
	wxString end=word.Mid(word.Length()-1);

	if(end.Mid(1)==wxT("s")||end.Mid(1)==wxT("x")){//ends with s or x
		plural+=wxT("es");
	}
	else if(end==wxT("ss")||end==wxT("sh")||end==wxT("ch")){
		plural+=wxT("es");
	}
	else if(end.Mid(1)==wxT("y")){
		plural.RemoveLast();
		plural+=wxT("ies");
	}
	else
		plural+=wxT("s");

	return plural;
}

wxString wxActiveRecordGenerator::DePluralize(const wxString& word){
	wxString singular=word;
	wxString end=word.Mid(word.Length()-4);

	if(end.Mid(1)==wxT("ses")||end.Mid(1)==wxT("xes")){//ends with s or x
		singular.RemoveLast().RemoveLast();

	}
	else if(end==wxT("sses")||end==wxT("shes")||end==wxT("ches")){
		singular.RemoveLast().RemoveLast();
	}
	else if(end.Mid(1)==wxT("ies")){
		singular.RemoveLast().RemoveLast().RemoveLast();
		singular+=wxT("y");
	}
	else if(end.Mid(3)==wxT("s"))
		singular.RemoveLast();

	return singular;
}

wxString wxActiveRecordGenerator::CapitalizeTable(const wxString& word){
	wxString capital=word;
	size_t start,len;

	wxRegEx regEx(wxT("_[a-z]")); //REPLACE _l with L
	while(regEx.Matches(capital)){
		regEx.GetMatch(&start,&len);
		capital.Replace(capital.Mid(start,len),capital.Mid(start+1,1).Upper());
	}

	return capital.Mid(0,1).Upper()+capital.Mid(1);

}

arNode::~arNode(){
	while(m_hasManyRelations.Count()>0){
		arRelation* toDel=m_hasManyRelations.Item(0);
		delete toDel;
		m_hasManyRelations.RemoveAt(0);
	}
	while(m_belongsToRelations.Count()>0){
		arRelation* toDel=m_belongsToRelations.Item(0);
		delete toDel;
		m_belongsToRelations.RemoveAt(0);
	}
}


wxString wxActiveRecordGenerator::TypeToStr(int type){
	//enum{AR_SQLITE=0,AR_POSTGRESQL,AR_MYSQL};
	if(type==AR_SQLITE)
		return wxT("sqlite");
	else if(type==AR_POSTGRESQL)
		return wxT("postgresql");
	else if(type==AR_MYSQL)
		return wxT("mysql");
	else if(type==AR_FIREBIRD)
		return wxT("firebird");
	else if(type==AR_FIREBIRD_EMBEDDED)
		return wxT("firebird_embedded");
	else
		return wxT("");
}

int wxActiveRecordGenerator::StrToType(const wxString& strType){
	if(strType.CmpNoCase(wxT("sqlite"))==0)
		return AR_SQLITE;
	else if(strType.CmpNoCase(wxT("postgresql"))==0)
		return AR_POSTGRESQL;
	else if(strType.CmpNoCase(wxT("mysql"))==0)
		return AR_MYSQL;
	else if(strType.CmpNoCase(wxT("firebird"))==0)
		return AR_FIREBIRD;
	else if(strType.CmpNoCase(wxT("firebird_embedded"))==0)
		return AR_FIREBIRD_EMBEDDED;
	else return -1;
}


bool wxActiveRecordGenerator::IsSupported(int dbType){
	//enum{AR_SQLITE=0,AR_POSTGRESQL,AR_MYSQL,AR_FIREBIRD,AR_FIREBIRD_EMBEDDED};
#ifdef AR_USE_SQLITE
	if(dbType==AR_SQLITE)
		return true;
#endif

#ifdef AR_USE_POSTGRESQL
	if(dbType==AR_POSTGRESQL)
		return true;
#endif

#ifdef AR_USE_MYSQL
	if(dbType==AR_MYSQL)
		return true;
#endif

#ifdef AR_USE_FIREBIRD
	if(dbType==AR_FIREBIRD || dbType==AR_FIREBIRD_EMBEDDED)
		return true;
#endif
	return false;
}
