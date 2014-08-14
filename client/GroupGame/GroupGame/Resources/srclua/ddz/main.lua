require "Cocos2d"
require "Cocos2dConstants"

-- cclog
cclog = function(...)
    print(string.format(...))
end

function enterGame()
    collectgarbage("collect")
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
	
	cc.FileUtils:getInstance():addSearchResolutionsOrder("srclua");
	cc.FileUtils:getInstance():addSearchResolutionsOrder("srclua/ddz");
	cc.FileUtils:getInstance():addSearchResolutionsOrder("Resources");
	
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
	cclog("current platform is " .. targetPlatform)
    
    local visibleSize = cc.Director:getInstance():getVisibleSize()
    local origin = cc.Director:getInstance():getVisibleOrigin()

    local function createLayerFarm()
        local layerFarm = cc.Layer:create()

        -- add in farm background
        local bg = cc.Sprite:create("gamebg.png")
        bg:setPosition(visibleSize.width / 2, visibleSize.height / 2)
        layerFarm:addChild(bg)

        return layerFarm
    end

    local function createLayerMenu()
        local layerMenu = cc.Layer:create()
        local menuPopup
        local function menuCallbackOpenPopup()
			cclog("menuCallbackOpenPopup")
            GobackHome()
        end

        local menuPopupItem = cc.MenuItemImage:create("btn2_normal.png", "btn2_hover.png")
        menuPopupItem:setPosition(0, 0)
        menuPopupItem:registerScriptTapHandler(menuCallbackOpenPopup)
        menuPopup = cc.Menu:create(menuPopupItem)
        menuPopup:setPosition(visibleSize.width / 2, visibleSize.height / 2)
        layerMenu:addChild(menuPopup)

        return layerMenu
    end

    local sceneGame = cc.Scene:create()
    sceneGame:addChild(createLayerFarm())
    sceneGame:addChild(createLayerMenu())
	
	if cc.Director:getInstance():getRunningScene() then
		cc.Director:getInstance():replaceScene(sceneGame)
	else
		cc.Director:getInstance():runWithScene(sceneGame)
	end

    cclog("enterGame ddz return")
end

enterGame();
