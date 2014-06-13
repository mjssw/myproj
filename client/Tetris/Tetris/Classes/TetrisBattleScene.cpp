#include "TetrisBattleScene.h"
#include "utils.h"
#include "NetManager.h"
#include "GameManager.h"
#include "TetrisRoomScene.h"
using namespace cocos2d;

CCScene* CTetrisBattleScene::scene()
{
    CCScene *scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        CTetrisBattleScene *layer = CTetrisBattleScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool CTetrisBattleScene::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

		// 背景
		cocos2d::CCSprite *pSpriteBg = CCSprite::create( PATCH_RES_DIR("tetris_back.png") );
		pSpriteBg->setAnchorPoint( ccp(0, 0) );
		int y = (visibleSize.height -  pSpriteBg->getContentSize().height) / 2; 
		pSpriteBg->setPosition( ccp(0, y) );
		addChild( pSpriteBg, 0 );

		// left
		int fx = 5;
		int fy = 50;
		float fscale = 0.125;
		{
			CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
				PATCH_RES_DIR("left.png"),
				PATCH_RES_DIR("left2.png"),
				this,
				menu_selector(CTetrisBattleScene::menuLeftCallback) );
			CC_BREAK_IF( !pMenuItem );
			pMenuItem->setScale( fscale );
			pMenuItem->setAnchorPoint( ccp(0, 0) );
			pMenuItem->setPosition( ccp(fx, fy) );

			CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}
		{
			CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
				PATCH_RES_DIR("down.png"),
				PATCH_RES_DIR("down2.png"),
				this,
				menu_selector(CTetrisBattleScene::menuDownCallback) );
			CC_BREAK_IF( !pMenuItem );
			pMenuItem->setAnchorPoint( ccp(0, 0) );
			pMenuItem->setScale( fscale );
			CCSize sz = pMenuItem->getContentSize() * fscale; 
			pMenuItem->setPosition( ccp(fx + sz.width + 5, fy) );

			CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}
		{
			CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
				PATCH_RES_DIR("right.png"),
				PATCH_RES_DIR("right2.png"),
				this,
				menu_selector(CTetrisBattleScene::menuRightCallback) );
			CC_BREAK_IF( !pMenuItem );
			pMenuItem->setAnchorPoint( ccp(0, 0) );
			pMenuItem->setScale( fscale );
			CCSize sz = pMenuItem->getContentSize() * fscale; 
			pMenuItem->setPosition( ccp(fx + (sz.width + 5)*2, fy) );

			CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}
		{
			CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
				PATCH_RES_DIR("up.png"),
				PATCH_RES_DIR("up2.png"),
				this,
				menu_selector(CTetrisBattleScene::menuUpCallback) );
			CC_BREAK_IF( !pMenuItem );
			pMenuItem->setAnchorPoint( ccp(0, 0) );
			pMenuItem->setScale( fscale );
			CCSize sz = pMenuItem->getContentSize() * fscale; 
			pMenuItem->setPosition( ccp(fx + (sz.width + 5), fy + sz.height + 5) );

			CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}
		{
			CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
				PATCH_RES_DIR("kdown1.png"),
				PATCH_RES_DIR("kdown2.png"),
				this,
				menu_selector(CTetrisBattleScene::menuQuickDownCallback) );
			CC_BREAK_IF( !pMenuItem );
			pMenuItem->setAnchorPoint( ccp(0, 0) );
			pMenuItem->setScale( fscale );
			CCSize sz = pMenuItem->getContentSize() * fscale; 
			pMenuItem->setPosition( ccp(visibleSize.width - sz.width - 10, fy - sz.height/4) );

			CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
			pMenu->setPosition( CCPointZero );
			CC_BREAK_IF( !pMenu );
			addChild( pMenu, 1 );
		}

		// 方块
#ifdef _TEST_SIMPLE_BLOCK_
		CCRect rc;
		rc.setRect( 0, 0, BLOCK_W, BLOCK_H );
		for(int i=0; i<2; ++i )
		{
			m_pSp[i] = CCSprite::create( PATCH_RES_DIR("tetris_brick.png"), rc );
			m_pSp[i]->setAnchorPoint( ccp(0, 0) );
			m_pSp[i]->setPosition( ccp(BATTLE_AREA_X+i*BLOCK_W, BATTLE_AREA_Y) );
			addChild( m_pSp[i], 0 );
		}
#endif
		/*
		CCRect rc;
		rc.setRect( 0, 0, BLOCK_W, BLOCK_H );
		int bx, by;
		for( int i=0; i<12; ++i )
		{
			bx = BATTLE_AREA_X + BLOCK_W * i;
			by = BATTLE_AREA_Y;
			cocos2d::CCSprite *pSpriteBlock = CCSprite::create( PATCH_RES_DIR("tetris_brick.png"), rc );
			pSpriteBlock->setAnchorPoint( ccp(0, 0) );
			pSpriteBlock->setPosition( ccp(bx, by) );
			addChild( pSpriteBlock, 0 );
		}
		rc.setRect( BLOCK_W, 0, BLOCK_W, BLOCK_H );
		for( int i=0; i<20; ++i )
		{
			bx = BATTLE_AREA_X;
			by = BATTLE_AREA_Y + BLOCK_H * (i+1);
			cocos2d::CCSprite *pSpriteBlock = CCSprite::create( PATCH_RES_DIR("tetris_brick.png"), rc );
			pSpriteBlock->setAnchorPoint( ccp(0, 0) );
			pSpriteBlock->setPosition( ccp(bx, by) );
			addChild( pSpriteBlock, 0 );
		}
		//*/
		/*
        // 关闭按钮
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            PATCH_RES_DIR("close2.png"),
            PATCH_RES_DIR("close1.png"),
            this,
            menu_selector(CTetrisBattleScene::menuCloseCallback) );
        CC_BREAK_IF( !pCloseItem );
		pCloseItem->setAnchorPoint( ccp(0, 0) );
		float w = visibleSize.width - pCloseItem->getContentSize().width - 5;
		float h = 5;
		pCloseItem->setPosition( ccp(w, h) );

        CCMenu *pMenu = CCMenu::create( pCloseItem, NULL );
        pMenu->setPosition( CCPointZero );
        CC_BREAK_IF( !pMenu );
        addChild( pMenu, 1 );
		//*/


		CGameManager::Instance().SetGameWorld( this );

		m_bStart = false;
		_GameStart();
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(\
			schedule_selector(CTetrisBattleScene::BlockDown), this, 1.0f, false);

		CNetManager::Instance().ResumeProcMsg();
        
		bRet = true;
    } while (0);

    return bRet;
}

void CTetrisBattleScene::onExit()
{
	CCLayer::onExit();
}

void CTetrisBattleScene::GameEnd(bool bWin)
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	cocos2d::CCSprite *pSpriteBg = NULL;
	if( bWin )
	{
		pSpriteBg = CCSprite::create( PATCH_RES_DIR("win.png") );
	}
	else
	{
		pSpriteBg = CCSprite::create( PATCH_RES_DIR("lost.png") );
	}
	pSpriteBg->setAnchorPoint( ccp(0, 0) );
	int y = (visibleSize.height - pSpriteBg->getContentSize().height) * 2 / 3;
	int x = (visibleSize.width - pSpriteBg->getContentSize().width) / 2;
	pSpriteBg->setPosition( ccp(x, y) );
	addChild( pSpriteBg, 2, E_GameEnd_Logo );
	
	{
		CCMenuItemImage *pMenuItem = CCMenuItemImage::create(
			PATCH_RES_DIR("btn_common_normal.png"),
			PATCH_RES_DIR("btn_common_over.png"),
			this,
			menu_selector(CTetrisBattleScene::menuBackCallback) );
		pMenuItem->setAnchorPoint( ccp(0, 0) );
		y -= (pMenuItem->getContentSize().height + 20);
		x = (visibleSize.width - pMenuItem->getContentSize().width) / 2;
		pMenuItem->setPosition( ccp(x, y) );

		CCMenu *pMenu = CCMenu::create( pMenuItem, NULL );
		pMenu->setPosition( CCPointZero );
		addChild( pMenu, 2, E_GameEnd_Btn );
	}

	m_bStart = false;
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(CTetrisBattleScene::BlockDown), this );
}

void CTetrisBattleScene::EnemyClearRows(std::vector<int> &clearRows)
{
	if( !CGameManager::Instance().RiseBlock( (int)clearRows.size(), m_pCurBlock, this ) )
	{
		// Game Over
		_GameOver();
	}
}

void CTetrisBattleScene::LeaveRoom(const std::string& user)
{
	CGameManager::Instance().DelEnemy();
}

void CTetrisBattleScene::menuBackCallback(CCObject* pSender)
{
	if( !CGameManager::Instance().IsOfflineGame() )
	{
		CNetManager::Instance().PauseProcMsg();
		cocos2d::CCLog( "GameEnd change to tetris room scene" );
		cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisRoomScene::scene() );
	}
	else
	{
		/*
		removeChildByTag( E_GameEnd_Logo );
		removeChildByTag( E_GameEnd_Btn );
		ClearBattleScene();

		_GameStart();
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(\
			schedule_selector(CTetrisBattleScene::BlockDown), this, 1.0f, false);
			//*/
		
		cocos2d::CCDirector::sharedDirector()->replaceScene( CTetrisBattleScene::scene() );
	}
}

void CTetrisBattleScene::menuUpCallback(CCObject* pSender)
{
	//CCLog( "UP" );

#ifdef _TEST_SIMPLE_BLOCK_
	float y1 = m_pSp[0]->getPositionY();
	float x1 = m_pSp[0]->getPositionX();

	float y2 = m_pSp[1]->getPositionY();
	float x2 = m_pSp[1]->getPositionX();

	if( y1 == y2 )
	{
		m_pSp[1]->setPosition( ccp(x1, y1-BLOCK_H) );
	}
	else
	{
		m_pSp[1]->setPosition( ccp(x1+BLOCK_W, y1) );
	}

	/*
	for(int i=0; i<2; ++i )
	{
		float y = m_pSp[i]->getPositionY();
		float x = m_pSp[i]->getPositionX();
		m_pSp[i]->setPosition( ccp(x, y+BLOCK_H) );
	}
	//*/
#endif
	if( !m_bStart )
	{
		return;
	}

	s32 bid = CGameManager::Instance().TryRotateClockwise( m_pCurBlock, this );
	if( m_nCurBlockId != bid )
	{
		m_nCurBlockId = bid;
		m_nCurRow = m_pCurBlock->Row();	
		m_nCurCol = m_pCurBlock->Col();	
	}
	//*/
}

void CTetrisBattleScene::menuDownCallback(CCObject* pSender)
{
	//CCLog( "DOWN" );
#ifdef _TEST_SIMPLE_BLOCK_
	for(int i=0; i<2; ++i )
	{
		float y = m_pSp[i]->getPositionY();
		float x = m_pSp[i]->getPositionX();
		m_pSp[i]->setPosition( ccp(x, y-BLOCK_H) );
	}
#endif
	
	if( !m_bStart )
	{
		return;
	}

	BlockDown( 0 );
	/*
	vector<s32> vecRows;
	vecRows.push_back( 10 );
	EnemyClearRows( vecRows );
	//*/
}

void CTetrisBattleScene::menuRightCallback(CCObject* pSender)
{
	//CCLog( "RIGHT" );
#ifdef _TEST_SIMPLE_BLOCK_
	for(int i=0; i<2; ++i )
	{
		float y = m_pSp[i]->getPositionY();
		float x = m_pSp[i]->getPositionX();
		m_pSp[i]->setPosition( ccp(x+BLOCK_W, y) );
	}
#endif
	if( !m_bStart )
	{
		return;
	}

	if( CGameManager::Instance().TryRight( m_pCurBlock, this ) )
	{
		m_nCurCol += 1;
	}
	//*/
}

void CTetrisBattleScene::menuLeftCallback(CCObject* pSender)
{
	//CCLog( "LEFT" );
#ifdef _TEST_SIMPLE_BLOCK_
	for(int i=0; i<2; ++i )
	{
		float y = m_pSp[i]->getPositionY();
		float x = m_pSp[i]->getPositionX();
		m_pSp[i]->setPosition( ccp(x-BLOCK_W, y) );
	}
#endif
	if( !m_bStart )
	{
		return;
	}

	if( CGameManager::Instance().TryLeft( m_pCurBlock, this ) )
	{
		m_nCurCol -= 1;
	}
	//*/
}

void CTetrisBattleScene::menuQuickDownCallback(CCObject* pSender)
{
	//CCLog( "Quick DOWN" );
	if( !m_bStart )
	{
		return;
	}

	while( CGameManager::Instance().TryDown( m_pCurBlock, this ) )
	{
		m_nCurRow += 1;
	}
	
	_DoPutBlock();
}

void CTetrisBattleScene::BlockDown(float t)
{
	if( CGameManager::Instance().TryDown( m_pCurBlock, this ) )
	{
		m_nCurRow += 1;
	}
	else
	{
		_DoPutBlock();
	}
}

void CTetrisBattleScene::_DoPutBlock()
{
	bool ret = CGameManager::Instance().PutBlock( m_pCurBlock, m_nCurCol );
	if( ret )
	{
		// 网络消息
		CNetManager::Instance().PutBlock( m_pCurBlock->Id(), m_nCurCol );

		// 消方块
		_CheckCleanUp();

		// 下一个
		_NextBlock();
		m_pCurBlock->SetPosition( this, m_nCurRow, m_nCurCol );
	}
	else
	{
		// Game Over
		_GameOver();
	}
}

void CTetrisBattleScene::_CheckCleanUp()
{
	CGameManager::Instance().CheckCleanUp( m_pCurBlock, this );
}

void CTetrisBattleScene::_GameOver()
{
	m_bStart = false;
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(
		schedule_selector(CTetrisBattleScene::BlockDown), this );
	if( CGameManager::Instance().IsOfflineGame() )
	{
		GameEnd(false);
	}
	else
	{
		CNetManager::Instance().Lost();
	}
}

void CTetrisBattleScene::_GameStart()
{
	CGameManager::Instance().InitGameScene();

	m_nCurBlockId = CGameManager::Instance().NextBlockId();
	m_nCurRow = 0;
	m_nCurCol = (GAME_SCENE_COL_NUM / 2) - 1;
	m_pCurBlock = new CBlock( m_nCurBlockId );
	m_pCurBlock->AddScene( this, m_nCurRow, m_nCurCol );

	m_nNextBlockId = CGameManager::Instance().NextBlockId();
	m_pNextBlock = new CBlock( m_nNextBlockId );
	m_pNextBlock->AddScene( this, 0, NEXT_BLOCK_COL );
	
	m_bStart = true;
}

void CTetrisBattleScene::_NextBlock()
{
	m_nCurBlockId = m_nNextBlockId;

	m_nCurRow = 0;
	m_nCurCol = (GAME_SCENE_COL_NUM / 2) - 1;
	m_pCurBlock = m_pNextBlock;

	m_nNextBlockId = CGameManager::Instance().NextBlockId();
	m_pNextBlock = new CBlock( m_nNextBlockId );
	m_pNextBlock->AddScene( this, 0, NEXT_BLOCK_COL );
}

CCPoint CTetrisBattleScene::_BuildPoint(s32 row, s32 col)
{
	return CCPoint( col*BLOCK_W+BATTLE_AREA_X, row*BLOCK_H+BATTLE_AREA_Y);
}

void CTetrisBattleScene::ClearBattleScene()
{
}
