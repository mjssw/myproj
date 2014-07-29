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
	 * ��һ��BLOB�ṹת����һ���Ϸ�SQL�ַ���
	 * ���ӵ�pszDest(SQL)��
	 */
	virtual unsigned int AppendStream(char *pszDest, const char *pszSrc, int size) = 0;

	/*
	 * ��ҳ��ȡ��¼
	 */
	virtual bool GetRecordSetByLimit(const char *table_name, int line, int num, IDBRecordSet **RecordSet, char *clause=0) = 0;

	/*
	 * �õ�һ�����������������
	 */
	virtual bool GetTableKeyInfo(const char *table_name, char *key_name, char *key_type) = 0;

	/*
	 * �õ�һ�����¼��
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
	 * ���ַ�������ʽ����ֵ
	 */
	virtual const char* GetFieldValue(unsigned int index) = 0;

	virtual const char* GetFieldValue(char *field_name) = 0;

	//�õ���ǰ�е�ĳ���еĳ��� 2010-03-17
	//index = 1, ... ,
	virtual unsigned long GetFieldBits(unsigned int index) = 0;
};

}

#endif