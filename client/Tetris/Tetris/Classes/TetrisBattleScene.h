#ifndef _TETRIS_BATTLE_SCENE_H_
#define _TETRIS_BATTLE_SCENE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "SimpleAudioEngine.h"
#include "GameWorldBase.h"
#include "Block.h"

//#define _TEST_SIMPLE_BLOCK_

class CTetrisBattleScene : public cocos2d::CCLayer, public CGameWorldBase
{
public:
	enum
	{
		E_TetrisBattleScene_Loading,
		E_GameEnd_Logo,
		E_GameEnd_Btn,

	};

    virtual bool init();  
    virtual void onExit();

    static cocos2d::CCScene* scene();

	virtual void DisplayGameWorld(){ cocos2d::CCLog("CTetrisBattleScene"); }
	virtual void GameEnd(bool bWin);
	virtual void EnemyClearRows(std::vector<int> &clearRows);
	virtual void LeaveRoom(const std::string& user);

	void menuBackCallback(CCObject* pSender);
	void menuUpCallback(CCObject* pSender);
	void menuDownCallback(CCObject* pSender);
	void menuRightCallback(CCObject* pSender);
	void menuLeftCallback(CCObject* pSender);
	void menuQuickDownCallback(CCObject* pSender);
	void BlockDown(float t);

    CREATE_FUNC(CTetrisBattleScene);

private:
	void _GameStart();
	void _NextBlock();
	cocos2d::CCPoint _BuildPoint(s32 row, s32 col);
	void _Draw();
	void _MoveTo(s32 row, s32 col);
	void _DoPutBlock();
	void _CheckCleanUp();
	void _GameOver();
	void ClearBattleScene();

private:
	s32 m_nCurBlockId;
	s32 m_nCurRow, m_nCurCol;
	s32 m_nNextBlockId;
	CBlock *m_pCurBlock;
	CBlock *m_pNextBlock;
	bool m_bStart;
#ifdef _TEST_SIMPLE_BLOCK_
	cocos2d::CCSprite *m_pSp[2];
#endif
};

#endif
