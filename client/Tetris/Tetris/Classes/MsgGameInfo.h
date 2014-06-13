#ifndef _MSG_GAMEINFO_H_
#define _MSG_GAMEINFO_H_

#include "MsgBase.h"
#include "GameManager.h"

class CMsgGameInfo : public CMsgBase
{
public:
	CMsgGameInfo(CGameInfo **info, s32 count) : m_GameInfo(info), m_Count(count)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGameInfo" );
		for( s32 i=0; i<m_Count; ++i )
		{
			CGameManager::Instance().SetGameInfo( m_GameInfo[i]->m_Id, m_GameInfo[i] );
		}
		SAFE_DELETE_ARRAY( m_GameInfo );
	}

private:
	CGameInfo **m_GameInfo;
	s32 m_Count;
};

#endif
