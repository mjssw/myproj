#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "GameClient.h"

class CGameMgr
{
public:
	std::string m_groupIp;
	s32 m_groupPort;
	bool m_bHaveGroupInfo;
	std::string m_user;
	std::string m_token;

	CGameMgr() : m_LoginConn(1), m_GroupConn(1), m_GameConn(1)
	{
		m_groupIp = "";
	}
	~CGameMgr()
	{
		m_LoginConn.Stop();
		m_GroupConn.Stop();
		m_GameConn.Stop();
	}
	void Start(const char *loginIp, s32 loginPort)
	{
		bool ret = m_LoginConn.Start( loginIp, loginPort );
		if( !ret )
		{
			printf( "Start [%s:%d] failed\n", loginIp, loginPort );
			return;
		}

		ctCLient *cts[10] = { 0 };
		s32 count = 0;
		m_LoginConn.DumpAllClients( cts, (sizeof(cts)/sizeof(ctCLient*)), count );
		m_loginClient = cts[0];
	
		while(true)
		{
			char cmd[64] = {0};
			gets(cmd);
			vector<string> vCmd;
			SplitCmd(cmd, vCmd );
			if( vCmd.size() == 0 )
			{
				printf("no cmd here\n");
				continue;
			}

			if( vCmd[0] == "quit" )
			{
				break;
			}

			else if( vCmd[0] == "login" )
			{
				if( vCmd.size() == 4 )
				{
					m_loginClient->UserLogin(vCmd[1], vCmd[2], vCmd[3]);
				}
			}
			else if( vCmd[0] == "entergame" )
			{
				if( vCmd.size() == 2 )
				{
					m_loginClient->UserEnterGame( atoi(vCmd[1].c_str()) );
				}
			}
			else if( vCmd[0] == "leavelogin" )
			{
				/*
				m_groupIp = m_loginClient->m_groupIp; 
				m_groupPort = m_loginClient->m_groupPort; 

				m_LoginConn.Stop();
				m_loginClient = NULL;
				//*/
			}

			else if( vCmd[0] == "startgroup" )
			{
				m_groupIp = m_loginClient->m_groupIp; 
				m_groupPort = m_loginClient->m_groupPort;
				m_bHaveGroupInfo = m_loginClient->m_bHaveGroupInfo; 
				m_user = m_loginClient->m_User;
				m_token = m_loginClient->m_token;

				if( m_bHaveGroupInfo )
				{
					m_LoginConn.Stop();
					m_loginClient = NULL;

					bool ret = m_GroupConn.Start( m_groupIp.c_str(), m_groupPort );
					if( !ret )
					{
						printf( "Start Group [%s:%d] failed\n", m_groupIp.c_str(), loginPort );
					}
					else
					{
						ctCLient *cts[10] = { 0 };
						s32 count = 0;
						m_GroupConn.DumpAllClients( cts, (sizeof(cts)/sizeof(ctCLient*)), count );
						m_groupClient = cts[0];
					}
				}
				else
				{
					printf( "have not get group info" );
				}
			}
			else if( vCmd[0] == "logingroup" )
			{
				if( vCmd.size() == 1 )
				{
					m_groupClient->LoginGroup( m_user, m_token );
				}
			}
			else if( vCmd[0] == "creategroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_groupClient->CreateGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "addgroupmember" )
			{
				if( vCmd.size() >= 3 )
				{
					m_groupClient->AddMemberToGroup( vCmd );
				}
			}
			else if( vCmd[0] == "agree" )
			{
				if( vCmd.size() == 2 )
				{
					m_groupClient->AgreeJoinGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "groupmsg" )
			{
				if( vCmd.size() == 3 )
				{
					m_groupClient->GroupMessage( vCmd[1], vCmd[2] );
				}
			}
			else if( vCmd[0] == "leavegroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_groupClient->LeaveGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "deletegroup" )
			{
				if( vCmd.size() == 2 )
				{
					m_groupClient->DeleteGroup( vCmd[1] );
				}
			}
			else if( vCmd[0] == "createroom" )
			{
				if( vCmd.size() == 3 )
				{
					m_groupClient->AskCreateGameRoom( vCmd[1], vCmd[2] );
				}
			}
		}
	}

	ctCLient *m_loginClient;
	ctCLient *m_groupClient;
	ctCLient *m_gameClient;

	CClientManager<ctCLient> m_LoginConn;
	CClientManager<ctCLient> m_GroupConn;
	CClientManager<ctCLient> m_GameConn;
};

#endif
