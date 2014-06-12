#ifndef _MYSQLDBQUERY_H_
#define _MYSQLDBQUERY_H_

#include "Lock.h"
#ifdef _USE_WIN32
#include <winsock.h>
#endif
#include "DBQuery.h"

#include "mysql.h"

//MySQL 5.1

//Disable deprecation warnings
#ifdef _USE_WIN32
#pragma warning(disable: 4996)
#endif

namespace SGLib
{

#define MAX_MYSQL_BUFFER_LEN	32*1024
#define MYDELETE(x)		if(x!=NULL){ delete x; x=NULL; }

class MySqlDBRecordSet;

class MySqlDBQuery : public IDBQuery
{
public:
	~MySqlDBQuery();
	bool SetConnection(void *dbinfo, char *CharacterSet=0);
	bool Connect();
	bool Close();
	bool IsConnected();
	bool Execute(const char *sql, char *ErrMsg);
	bool Execute(const char *sql, IDBRecordSet **RecordSet, char *ErrMsg);
	unsigned int GetLastError();
	const char* GetLastErrorInfo();
	unsigned int AppendStream(char *pszDest, const char *pszSrc, int size);
	bool GetRecordSetByLimit(const char *table_name, int line, int num, IDBRecordSet **RecordSet, char *clause);
	bool GetTableKeyInfo(const char *table_name, char *key_name, char *key_type);
	unsigned int GetRecordCount(const char *table_name);

public:
	MySqlDBQuery();

private:
	bool Execute1(const char *sql, MySqlDBRecordSet *RecordSet, char *ErrMsg);
	size_t EscapeString(char *pszDest, const char *pszSrc, int size);

private:
	char m_ServerAddr[24];
	char m_DBName[24];
	char m_User[24];
	char m_Password[24];
	unsigned int  m_ServerPort;

	MYSQL *m_MySqlConnection;
	bool m_IsConnected;

	//调用mysql接口执行的SQL语句
	char m_sql[MAX_MYSQL_BUFFER_LEN];

	///2009-12-08 使用的字符集
	char m_CharacterSet[128];

	CLock m_Lock;
};


class MySqlDBRecordSet : public IDBRecordSet
{
	friend class MySqlDBQuery;

public:
	~MySqlDBRecordSet();
	unsigned int GetRecordCount();
	unsigned int GetFieldCount();
	bool GetRecord();
	bool GetLinedRecord(unsigned int line);
	const char* GetFieldValue(unsigned int index);
	const char* GetFieldValue(char *field_name);
	//2010-03-17
	unsigned long GetFieldBits(unsigned int index);

public:
	MySqlDBRecordSet();


private:
	MYSQL_RES *m_res;
	MYSQL_ROW  m_row;
};

struct MySqlDBInfo
{
	char ServerAddr[24];
	char DBName[24];
	char User[24];
	char Password[24];
	unsigned int  ServerPort;
	//char Charset[24];

	MySqlDBInfo(
		const char *addr=NULL, const char *dbname=NULL, 
		const char *user=NULL, const char *pass=NULL, 
		unsigned int port=3306
	//	, char *charset=NULL
	)
	{
		strcpy(ServerAddr,addr);
		strcpy(DBName, dbname);
		strcpy(User, user);
		strcpy(Password, pass);
		//strcpy(Charset, charset);
		ServerPort = port;
	}

};

};
#endif
