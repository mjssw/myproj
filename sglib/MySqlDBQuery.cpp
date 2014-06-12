#include "MySqlDBQuery.h"
//#include "define.h"

#include <string.h>
#include <stdio.h>
using namespace SGLib;

/////////////////////////////////MySqlDBQuery//////////////////////////////////////////

MySqlDBQuery::~MySqlDBQuery()
{
	if( m_MySqlConnection )
	{
		mysql_close( m_MySqlConnection );
	}
}

bool MySqlDBQuery::SetConnection(void *dbinfo, char *CharacterSet)
{
	MySqlDBInfo *mysql_dbinfo = (MySqlDBInfo*)dbinfo;

	strncpy( this->m_DBName, mysql_dbinfo->DBName, 24 );
	strncpy( this->m_Password, mysql_dbinfo->Password, 24 );
	strncpy( this->m_ServerAddr, mysql_dbinfo->ServerAddr, 24 );
	strncpy( this->m_User, mysql_dbinfo->User, 24 );
	this->m_ServerPort = mysql_dbinfo->ServerPort;

	///2009-12-08 设置字符集
	if( CharacterSet && CharacterSet[0]!=0 )
	{
		strncpy( this->m_CharacterSet, CharacterSet, 128 );
	}
	else
	{
		memset( (void*)m_CharacterSet, 0, 128 );
	}

	return true;
}

bool MySqlDBQuery::Connect()
{
	CGuardLock<CLock> g(m_Lock);
	if( m_MySqlConnection == NULL )
	{
		m_MySqlConnection = mysql_init(NULL);
	}

	if( m_MySqlConnection == NULL )
	{
		return false;
	}

	// 2014-06-21 注意最后一个参数, 保证了存储过程能返回一个结果集
	MYSQL *mCnn = mysql_real_connect(
		m_MySqlConnection, 
		this->m_ServerAddr, 
		this->m_User,
		this->m_Password,
		this->m_DBName,
		this->m_ServerPort, 
		NULL, 
		CLIENT_MULTI_STATEMENTS 
	) ;

	if( !mCnn )
	{
		printf("%s\n",  mysql_error(m_MySqlConnection));

		Close();

		return false;
	}

	m_IsConnected = true;

	return true;
}
	
bool MySqlDBQuery::Close()
{
	CGuardLock<CLock> g(m_Lock);
	mysql_close( m_MySqlConnection );
	m_MySqlConnection = NULL;
	m_IsConnected = false;

	return true;
}
	
bool MySqlDBQuery::IsConnected()
{
	CGuardLock<CLock> g(m_Lock);
	if( !m_IsConnected )
	{
		return false;
	}

	return ( mysql_ping(m_MySqlConnection) == 0 );
}
	
bool MySqlDBQuery::Execute(const char *sql, char *ErrMsg)
{
	CGuardLock<CLock> g(m_Lock);
	/////////////////////////////////////////////
	///2009-12-15 注意这里在调用前先设置字符集///
	/////////////////////////////////////////////
	///这里是根据 质量控制部门 的需求加入的
	if( this->m_CharacterSet[0] != 0 )
	{
		char tmp_sql[1024];
		sprintf(tmp_sql, "SET NAMES '%s';", m_CharacterSet);
		mysql_query(m_MySqlConnection, tmp_sql);
	}
	//mysql_query(m_MySqlConnection, "SET NAMES 'gb2312';");

	bool ret = (mysql_query(m_MySqlConnection, sql) == 0);
	if( ret )
	{
		if( ErrMsg != 0 )
		{
			ErrMsg[0] = 0;
		}
	}
	else
	{
		if( ErrMsg != 0 )
		{
			sprintf( ErrMsg, "%s", mysql_error(m_MySqlConnection) );
		}
	}

	return ret;
}

bool MySqlDBQuery::Execute(const char *sql, IDBRecordSet **RecordSet, char *ErrMsg)
{
	if( *RecordSet != NULL )
	{
		MYDELETE(*RecordSet);
	}

	CGuardLock<CLock> g(m_Lock);

	/////////////////////////////////////////////
	///2009-12-15 注意这里在调用前先设置字符集///
	/////////////////////////////////////////////
	///这里是根据 质量控制部门 的需求加入的
	if( this->m_CharacterSet[0] != 0 )
	{
		char tmp_sql[1024];
		sprintf(tmp_sql, "SET NAMES '%s';", m_CharacterSet);
		if( mysql_query(m_MySqlConnection, tmp_sql) != 0 )
		{
			//printf("set characterset error: %s\n", mysql_error(m_MySqlConnection));
			FILE *fp = fopen("MySqlErr.txt", "a");
			fprintf(fp, "set characterset error: %s\n", mysql_error(m_MySqlConnection));
			fclose(fp);
		}
	}
	//mysql_query(m_MySqlConnection, "SET NAMES 'gb2312';");

	*RecordSet = new MySqlDBRecordSet;
	if( *RecordSet == NULL ) return false;

	return Execute1(sql, (MySqlDBRecordSet*)(*RecordSet), ErrMsg);
}

bool MySqlDBQuery::Execute1(const char *sql, MySqlDBRecordSet *RecordSet, char *ErrMsg)
{
	if( RecordSet == NULL )
	{
		strcpy(ErrMsg, "With null record set!!");
		return false;
	}

	if( RecordSet->m_res )
	{
		mysql_free_result( RecordSet->m_res );
	}

	bool ret = (mysql_query(m_MySqlConnection, sql) == 0);
	if( ret )
	{
		if( ErrMsg != 0 )
		{
			ErrMsg[0] = 0;
		}

		//MySqlDBQuery is friend of MySqlDBRecordSet
		RecordSet->m_res = mysql_store_result( m_MySqlConnection );
	}
	else
	{
		if( ErrMsg != 0 )
		{
			sprintf( ErrMsg, "%s", mysql_error(m_MySqlConnection) );
		}
	}

	return ret;
}

size_t MySqlDBQuery::EscapeString(char *pszDest, const char *pszSrc, int size)
{
	if( m_MySqlConnection )
	{
		//return mysql_real_escape_string(m_MySqlConnection, pszDest, pszSrc, size);
		size_t msize = 0;

		char *end;
		end = pszDest + strlen(pszDest);
		//注意一定要加''
		*end++ = '\'';
		msize = mysql_real_escape_string(m_MySqlConnection, end, pszSrc, size);
		end += msize;
		*end++ = '\'';

		return msize+2;
	}

	return 0;
}
	
unsigned int MySqlDBQuery::GetLastError()
{
	CGuardLock<CLock> g(m_Lock);
	if( m_MySqlConnection )
	{
		return mysql_errno( m_MySqlConnection );
	}

	return 0;
}
	
const char* MySqlDBQuery::GetLastErrorInfo()
{	
	CGuardLock<CLock> g(m_Lock);
	if( m_MySqlConnection )
	{
		return mysql_error( m_MySqlConnection );
	}

	return NULL;
}

MySqlDBQuery::MySqlDBQuery() :
	m_MySqlConnection(NULL),
	m_IsConnected(false),
	m_Lock()
{
	m_sql[0] = 0;
}

unsigned int MySqlDBQuery::AppendStream(char *pszDest, const char *pszSrc, int size)
{
	CGuardLock<CLock> g(m_Lock);
	return (unsigned int)EscapeString(pszDest, pszSrc, size);
}

bool MySqlDBQuery::GetRecordSetByLimit(const char *table_name, int line, int num, IDBRecordSet **RecordSet, char *clause)
{
	//char msql[128] = {0};
	int clause_len = 0;
	if( clause ) clause_len = (int)strlen(clause);

	char *msql = new char[64+strlen(table_name)+clause_len+16];
	if( clause == NULL )
	{
		sprintf(msql, "select * from %s limit %d, %d", table_name, line, num);
	}
	else
	{
		sprintf(msql, "select * from %s where %s limit %d, %d", table_name, clause, line, num);
	}

	char msg[1024];
	bool ret = Execute(msql, RecordSet, msg);
	if( !ret ) printf("ErrMsg: %s\n", msg);
	
	delete [] msql;

	return ret;
}

bool MySqlDBQuery::GetTableKeyInfo(const char *table_name, char *key_name, char *key_type)
{
	IDBRecordSet *recordset = NULL;

	char msql[128] = {0};
	sprintf(msql, "describe %s", table_name);
	if( Execute(msql, &recordset, NULL) )
	{
		while( recordset->GetRecord() )
		{
			const char *p = recordset->GetFieldValue(4);
			if( strcmp(p, "PRI") == 0 )
			{
				strcpy( key_name, recordset->GetFieldValue(1) );
				strcpy( key_type, recordset->GetFieldValue(2) );

				MYDELETE(recordset);

				return true;
			}
		}
	}

	MYDELETE(recordset);

	return false;
}

unsigned int MySqlDBQuery::GetRecordCount(const char *table_name)
{
	char msql[128] = {0};
	sprintf(msql, "select count(*) from %s", table_name);
	IDBRecordSet *recordset = NULL;

	char err_msg[1024];
	if( !Execute(msql, &recordset, err_msg) )
	{
		return 0;
	}

	if( recordset->GetRecordCount() != 1 )
	{
		return 0;
	}

	recordset->GetRecord();

	unsigned int count = atoi( recordset->GetFieldValue(1) );

	MYDELETE(recordset);

	return count;

}



/////////////////////////////////////////MySqlDBRecordSet//////////////////////////////////

MySqlDBRecordSet::~MySqlDBRecordSet()
{
	if( m_res )
	{
		mysql_free_result( m_res );
	}
}
	
unsigned int MySqlDBRecordSet::GetRecordCount()
{
	if( m_res )
	{
		return (unsigned int)mysql_num_rows( m_res );
	}

	return 0;
}
	
unsigned int MySqlDBRecordSet::GetFieldCount()
{
	if( m_res )
	{
		return mysql_num_fields( m_res );
	}

	return 0;
}

bool MySqlDBRecordSet::GetRecord()
{
	if( !m_res )
	{
		return false;
	}

	m_row = mysql_fetch_row( m_res );

	return (m_row != NULL);
}

bool MySqlDBRecordSet::GetLinedRecord(unsigned int line)
{
	if( !m_res )
	{
		return false;
	}

	if( line<1 || line>GetRecordCount() )
	{
		return false;
	}

	mysql_data_seek( m_res, line-1 );
	m_row = m_res->data_cursor->data;

	return (m_row != NULL);
}

const char* MySqlDBRecordSet::GetFieldValue(unsigned int index)
{
	if( !m_row || index==0 )
	{
		return NULL;
	}

	if( index > GetFieldCount() )
	{
		return NULL;
	}

	return m_row[index-1];
}
	
const char* MySqlDBRecordSet::GetFieldValue(char *field_name)
{
	if( !m_row )
	{
		return NULL;
	}

	unsigned int num_fields, i;
	MYSQL_FIELD *fields;
	num_fields = mysql_num_fields( m_res );
	fields = mysql_fetch_fields( m_res );
	for(i=0; i<num_fields; i++)
	{
		if( strcmp(field_name, fields[i].name) == 0 )
		{
			return m_row[i];
		}
	}

	return NULL;
}

unsigned long MySqlDBRecordSet::GetFieldBits(unsigned int index)
{
	if( !m_row || index==0 )
	{
		return 0;
	}

	unsigned int num_fields = mysql_num_fields( m_res );
	if( index > num_fields )
	{
		return 0;
	}

	unsigned long *lengths = mysql_fetch_lengths( m_res );
	if( lengths )
	{
		return lengths[index-1];
	}

	return 0;
}

MySqlDBRecordSet::MySqlDBRecordSet() : m_res(0), m_row(0)
{
}

