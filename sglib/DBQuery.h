#ifndef _DBQUERY_H_
#define _DBQUERY_H_

namespace SGLib
{
class IDBRecordSet;

class IDBQuery
{
public:
	virtual ~IDBQuery(){}

	virtual bool SetConnection(void *dbinfo, char *CharacterSet=0) = 0;

	virtual bool Connect() = 0;

	virtual bool Close() = 0;

	virtual bool IsConnected() = 0;

	virtual bool Execute(const char *sql, char *ErrMsg=0) = 0;

	virtual bool Execute(const char *sql, IDBRecordSet **RecordSet, char *ErrMsg=0) = 0;

	virtual unsigned int GetLastError() = 0;

	virtual const char* GetLastErrorInfo() = 0;

	/*
	 * 将一个BLOB结构转化成一个合法SQL字符串
	 * 连接到pszDest(SQL)中
	 */
	virtual unsigned int AppendStream(char *pszDest, const char *pszSrc, int size) = 0;

	/*
	 * 分页获取记录
	 */
	virtual bool GetRecordSetByLimit(const char *table_name, int line, int num, IDBRecordSet **RecordSet, char *clause=0) = 0;

	/*
	 * 得到一个表的主键名和类型
	 */
	virtual bool GetTableKeyInfo(const char *table_name, char *key_name, char *key_type) = 0;

	/*
	 * 得到一个表记录数
	 */
	virtual unsigned int GetRecordCount(const char *table_name) = 0;

	virtual int Ping() = 0;
};

class IDBRecordSet
{
public:
	virtual ~IDBRecordSet(){}

	virtual unsigned int GetRecordCount() = 0;

	virtual unsigned int GetFieldCount() = 0;

	virtual bool GetRecord() = 0;

	//line = 1, ... , 
	virtual bool GetLinedRecord(unsigned int line) = 0;

	/*
	 * index = 1, ... ,
	 * 以字符串的形式表现值
	 */
	virtual const char* GetFieldValue(unsigned int index) = 0;

	virtual const char* GetFieldValue(char *field_name) = 0;

	//得到当前行的某个列的长度 2010-03-17
	//index = 1, ... ,
	virtual unsigned long GetFieldBits(unsigned int index) = 0;
};

}

#endif