#ifndef _MSG_GAME_END_H_
#define _MSG_GAME_END_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgGameEnd: public CMsgBase
{
public:
	CMsgGameEnd(const std::string &loser) : m_Loser(loser)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGameEnd" );
		CGameManager::Instance().GameEnd( m_Loser );
	}

private:
	std::string m_Loser;
};

#endif