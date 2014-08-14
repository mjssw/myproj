require "Cocos2d"
require "Cocos2dConstants"

-- cclog
cclog = function(...)
    print(string.format(...))
end

function gameend_scene()
    collectgarbage("collect")
    -- avoid memory leak
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
	cc.FileUtils:getInstance():addSearchResolutionsOrder("srclua");
	cc.FileUtils:getInstance():addSearchResolutionsOrder("Resources");
	local schedulerID = 0
    --support debug
    local targetPlatform = cc.Application:getInstance():getTargetPlatform()
    if (cc.PLATFORM_OS_IPHONE == targetPlatform) or (cc.PLATFORM_OS_IPAD == targetPlatform) or 
       (cc.PLATFORM_OS_ANDROID == targetPlatform) or (cc.PLATFORM_OS_WINDOWS == targetPlatform) or
       (cc.PLATFORM_OS_MAC == targetPlatform) then
        cclog("result is ")
		--require('debugger')()
        
    end
    require "hello2"
    cclog("result is " .. myadd(1, 1))

    ---------------

    local visibleSize = cc.Director:getInstance():getVisibleSize()
    local origin = cc.Director:getInstance():getVisibleOrigin()

    -- create farm
    local function createLayerFarm()
        local layerFarm = cc.Layer:create()

        -- add in farm background
        local bg = cc.Sprite:create("bg.png")
        bg:setPosition(visibleSize.width / 2, visibleSize.height / 2)
        layerFarm:addChild(bg)

        return layerFarm
    end


    -- create menu
    local function createLayerMenu()
        local layerMenu = cc.Layer:create()

        local menuPopup

        local function menuCallbackClosePopup()
            cclog("menuCallbackClosePopup");
			GobackHome();
        end

        -- add a popup menu
        local menuPopupItem = cc.MenuItemImage:create("_s02.png", "_s01.png")
        menuPopupItem:setPosition(0, 0)
        menuPopupItem:registerScriptTapHandler(menuCallbackClosePopup)
        menuPopup = cc.Menu:create(menuPopupItem)
        menuPopup:setPosition(visibleSize.width / 2, visibleSize.height / 2)
        menuPopup:setVisible(true)
        layerMenu:addChild(menuPopup)
        
        return layerMenu
    end

    -- run
    local sceneGame = cc.Scene:create()
    sceneGame:addChild(createLayerFarm())
    sceneGame:addChild(createLayerMenu())
	
	if cc.Director:getInstance():getRunningScene() then
		cc.Director:getInstance():replaceScene(sceneGame)
	else
		cc.Director:getInstance():runWithScene(sceneGame)
	end

end

