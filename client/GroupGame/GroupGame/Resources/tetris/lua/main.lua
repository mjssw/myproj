require "Cocos2d"
require "Cocos2dConstants"

-- cclog
cclog = function(...)
    print(string.format(...))
end

function _processMessageHistroy(msgid, msglen, data)
	print( "[LUA][_processMessageHistroy] : ", msgid, ":", msglen );
	local msg = DPB_CreateMessage("group.proto", "sglib.groupproto.SCGroupMessageHistoryRsp");
	local ret = DPB_ParseFromArray(msg, data, msglen);
	print( "[LUA][_processMessageHistroy] ParseFromArray ret=", ret );
	local size = DPB_GetFieldSize(msg, "messages");
	print( "[LUA][_processMessageHistroy] MessageSize=", size );
	for i=0,size-1,1 do
		local g = DPB_GetRepeatedMessage(msg, "messages", i);
		print( "[HISTORY][user]:", DPB_GetString(g, "user"),"[msg]:",DPB_GetString(g,"msg"),"[time]:",DPB_GetInt32(g,"time"),"[idx]:",DPB_GetInt64(g,"idx"));
	end
	DPB_DeleteMessage(msg);
end

function ProcessGameMessage(msgid, msglen, data)
	print( "in lua ProcessGameMessage : ", msgid, ":", msglen );
	if msgid==932 then
		_processMessageHistroy(msgid, msglen, data);
		return
	end
end

function enterGame()
    collectgarbage("collect")
    collectgarbage("setpause", 100)
    collectgarbage("setstepmul", 5000)
	
	DPB_Init("../../../../protomsg/proto");
	
	cc.FileUtils:getInstance():addSearchResolutionsOrder("tetris/lua");
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
            --GobackHome()
			local msg = DPB_CreateMessage("group.proto", "sglib.groupproto.CSGroupMessageHistoryReq");
			local ret = DPB_SetUInt64(msg, "groupid", 1);
			print("DPB_SetUInt64 groupid ret:", ret);
			ret = DPB_SetInt64(msg, "idxfrom", 0);
			print("DPB_SetInt64 idxfrom ret:" , ret);
			ret = DPB_SetInt32(msg, "limit", 10);
			print("DPB_SetInt32 limit ret:" , ret);
			SendGameMessage( msg, 931 )
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
