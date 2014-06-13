#ifndef _MSG_GAME_START_H_
#define _MSG_GAME_START_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisBattleScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgGameStart: public CMsgBase
{
public:
	CMsgGameStart(s32 seed) : m_BlockSeed(seed)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgGameStart" );
		CGameManager::Instance().GameStart( m_BlockSeed );

		CNetManager::Instance().PauseProcMsg();
		cocos2d::CCLog( "change to tetris battle scene" );
		cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisBattleScene::scene() );
	}

private:
	s32 m_BlockSeed;
};

#endif