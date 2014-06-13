#ifndef _MSG_ENEMY_CLEAR_BLOCK_H_
#define _MSG_ENEMY_CLEAR_BLOCK_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnemyClearBlockNtf: public CMsgBase
{
public:
	CMsgEnemyClearBlockNtf(int count, int *rows)
	{
		for( int i=0; i<count; ++i )
		{
			m_ClearRows.push_back( rows[i] );
		}
	}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnemyClearBlockNtf" );
		CGameManager::Instance().EnemyClearRows( m_ClearRows );
	}

private:
	std::vector<int> m_ClearRows;
};

#endif