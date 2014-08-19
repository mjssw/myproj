#include "AppDelegate.h"
#include "CCLuaEngine.h"
#include "scene/LoginScene.h"
#include "CommDef.h"
#include "res/ResManager.h"
#include "net/NetManager.h"
#include "user/UserManager.h"
#include "scene/SceneManager.h"
#include "lua/ExportProtobuf.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    //SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();

    GLView *glview = pDirector->getOpenGLView();
    if( !glview )
	{
        glview = GLView::create( "GroupGame");
    }
    pDirector->setOpenGLView( glview );

	glview->setDesignResolutionSize( WIN_SIZE_W, WIN_SIZE_H, kResolutionShowAll );

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

	CUserManager::Instance().GetViewData().LoadFromConfig();
	// 各组件初始化
	if( !CResManager::Instance().Init( RES_ROOT ) ||
		!CNetManager::Instance().Init() )
	{
		return false;
	}
	CSceneManager::Instance();
    
	LuaEngine* engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
	register_dynamicproto( engine->getLuaStack()->getLuaState() );
	lua_register(
		LuaEngine::getInstance()->getLuaStack()->getLuaState(), "SendGameMessage", CNetManager::SendGameMessage);

    // create a scene. it's an autorelease object
	CCScene *pScene = CLoginScene::scene();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    //SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    //SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
