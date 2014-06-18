#include "ServerManager.h"
#include "LoginClient.h"
#include "LoginRpcClient.h"
#include "LoginManager.h"
#include "common.h"

struct _DB_PARAM
{
	void *data;
};

class CTestClient : public CRpcClient
{
public:
	CTestClient(s32 id) : CRpcClient(id){}
	void _mysql_callback1(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
	{
		_DB_PARAM *pdbparam = (_DB_PARAM*)param; 
		CTestClient *pt = (CTestClient*)pdbparam->data;

		printf( "[%llu]run here\n", CServerEx<CLoginClient, CLoginRpcClient, CServerConfig>::GetProcessId() );
		s32 count = (s32)RecordSet->GetRecordCount();
		for( s32 i=0; i<count; ++i )
		{
			if( RecordSet->GetRecord() )
			{
				printf( "Record:%d\n", i );
				s32 _num = (s32)RecordSet->GetFieldCount();
				for( s32 j=0; j<_num; ++j )
				{
					printf( "\tField:[%d]=%s\n", j, RecordSet->GetFieldValue( j+1 ) );
				}
				printf( "\n" );
			}
		}
	}
	void _mysql_callback2(SGLib::IDBRecordSet *RecordSet, char *ErrMsg, void *param, s32 len)
	{
		_DB_PARAM *pdbparam = (_DB_PARAM*)param; 
		CTestClient *pt = (CTestClient*)pdbparam->data;

		printf( "[%llu]run here2\n", CServerEx<CLoginClient, CLoginRpcClient, CServerConfig>::GetProcessId() );
		if( RecordSet->GetRecord() )
		{
			s32 count = (s32)RecordSet->GetRecordCount();
			printf( "count=%d resutl=%s\n", 
				count, RecordSet->GetFieldValue(1) );
		}
	}
};


int main(int argc, char *argv[])
{
	/*
	SEventObj obj;
	obj.type = SEventObj::E_Event_ConnectError;
	CTestClient test(0);
	//test.HandleEvent( sizeof(obj), (char*)&obj);
	IEventEx *pev = (IEventEx*)&test;
	pev->HandleEvent(sizeof(obj), (char*)&obj);//*/

	CTestClient test100(100);

	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if( argc >= 5 )
	{
		CServerManager::Instance();
		CLoginManager::Instance();
		bool bInit = CServerManager::Instance().InitServer<CLoginClient, CLoginRpcClient, CServerConfig>(
			argv[1], 
			argv[2], 
			argv[3], 
			argv[4] );
		if( bInit && CServerManager::Instance().Start() )
		{
			while( true )
			{
				char cmd[32] = {0};
				gets( cmd );
				if( strcmp( cmd, "quit" ) == 0 )
				{
					break;
				}
				else if( strcmp(cmd, "testdb") == 0 )
				{
					CTestClient _testct(0);
					CMysqlManager *mgr = CServerManager::Instance().GetMysqlManager(1);
					_DB_PARAM dbparam = { &test100 };
					mgr->Execute( "select * from user;", &_testct, &CTestClient::_mysql_callback1, &dbparam, sizeof(dbparam) ); 
				}
				else if( strcmp(cmd, "testdb1") == 0 )
				{
					CTestClient _testct(0);
					CMysqlManager *mgr = CServerManager::Instance().GetMysqlManager(1);
					//string sql = "insert into user(User,Password,Name) values('sjj3','123456','sjj3');";
					//string sql = "call TestProc();";
					string sql = "call UserRegister('sjj6','123456','sjj6');";
					_DB_PARAM dbparam = { &test100 };
					mgr->Execute( sql.c_str(), &_testct, &CTestClient::_mysql_callback2, &dbparam, sizeof(dbparam) ); 
				}
				
			}

			CServerManager::Instance().Stop();
		}
		else
		{
			printf("start failed\n");
		}
	}

	google::protobuf::ShutdownProtobufLibrary();
}
