#include "Room.h"
#include "TetrisRoomScene.h"
#include "NetManager.h"

void CRoom::Enter()
{
	CNetManager::Instance().EnterRoom( Id() );
	//cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisRoomScene::scene() );
}

void CRoom::EnemyEnter()
{
}
