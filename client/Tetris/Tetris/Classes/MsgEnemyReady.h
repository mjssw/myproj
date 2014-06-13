#ifndef _MSG_ENEMY_READY_RSP_H_
#define _MSG_ENEMY_READY_RSP_H_

#include "MsgBase.h"
#include "NetManager.h"
#include "TetrisScene.h"
#include "NetClient.h"
#include "GameManager.h"

class CMsgEnemyReady: public CMsgBase
{
public:
	CMsgEnemyReady(const std::string &enemy) : m_Enemy(enemy)
	{}

	virtual void Proc()
	{
		cocos2d::CCLog( "<===> CMsgEnemyReady" );
		CGameManager::Instance().EnemyReady();
	}

private:
	std::string m_Enemy;
};

#endif