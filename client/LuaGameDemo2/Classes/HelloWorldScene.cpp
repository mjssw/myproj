#include "HelloWorldScene.h"
#include "base/CCScriptSupport.h"
#include "CCLuaEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = LabelTTF::create("TestLuaGame", "Arial", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("commonbg.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);


	// 
	auto luaItem = MenuItemImage::create(
		"btn2_normal.png",
		"btn2_hover.png",
		CC_CALLBACK_1(HelloWorld::menuTestCallback, this));
	luaItem->setPosition(
		Vec2( visibleSize.width/2, visibleSize.height/2) );
	auto luamenu = Menu::create(luaItem, NULL);
	luamenu->setPosition(Vec2::ZERO);
	this->addChild(luamenu, 1);

	lua_State *ls = LuaEngine::getInstance()->getLuaStack()->getLuaState();
    lua_register(ls, "GobackHome", GobackHome);

	return true;
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

static void _DumpLuaStack(lua_State *L)
{
	int nTop = lua_gettop(L); //得到栈的元素个数。栈顶的位置。
	CCLog( "The Length of stack is %d", nTop ); //输出栈顶位置
	for (int i = 1; i <= nTop; ++i)
	{
		int t = lua_type(L, i);
		CCLog("type:%d %s:", t, lua_typename(L, t)); //这里的typename是把类型的枚举变成字符串，是类型名。不是栈中的位置。
		switch(t)
		{
		case LUA_TNUMBER:
			CCLog("%f", lua_tonumber(L, i));
			break;
		case LUA_TSTRING:
			CCLog("%s", lua_tostring(L, i));
			break;
		case LUA_TTABLE:
			CCLog("%s", lua_tostring(L,i));
			break;
		case LUA_TFUNCTION:
			CCLog("%s", lua_tostring(L,i));
			break;
		case LUA_TNIL:
			CCLog("Is NULL");
			break;
		case LUA_TBOOLEAN:
			CCLog("%s", lua_toboolean(L, i) ? "true" : "false");
			break;
		default:
			CCLog( "unknow type:%d", t );
			break;
		}
	}
}

void HelloWorld::menuTestCallback(cocos2d::Ref* pSender)
{
	CCLog( "menuTestCallback" );

	LuaEngine::getInstance()->reload("srclua/main.lua");
}

int HelloWorld::GobackHome(lua_State *ls)
{
    auto home = HelloWorld::createScene();
    auto director = Director::getInstance();
	if( director->getRunningScene() )
	{
		director->replaceScene( home );
	}
	else
	{
		director->runWithScene( home );
	}

	return 0;
}
