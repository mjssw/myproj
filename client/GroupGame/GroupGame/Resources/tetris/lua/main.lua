require "Cocos2d"
require "Cocos2dConstants"

-- cclog
cclog = function(...)
    print(string.format(...))
end

sceneGame = nil;
labelState = nil;
local scheduler = cc.Director:getInstance():getScheduler()
local timer = nil;
local user = nil;
local readybtn = nil;
local lostbtn = nil;

local function LoginGame()
	user = GetUser();
	local token = GetToken();
	print( "LoginGame user:", user, " token:", token );
	local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.CSLoginGameReq");
	local ret = DPB_SetString(msg, "userid", user);
	print("DPB_SetString userid ret:", ret);
	ret = DPB_SetString(msg, "token", token);
	print("DPB_SetString token ret:" , ret);
	SendGameMessage( msg, 503 );
	print( "send LoginGame request" );
end

local function EnterRoom()
    local roomid = GetRoomId();
	print( "try enter room:", roomid );
	local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.CSEnterRoomReq");
	local ret = DPB_SetInt32(msg, "roomid", roomid);
	SendGameMessage( msg, 505 );
    local strret = labelState:getString() .. "\n正在进入房间(" .. roomid .. ")..."
    print( strret );
    labelState:setString(strret);
end

local function Ready()
	local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.CSMsgReadyReq");
	SendGameMessage( msg, 1001 );
    print( "self Ready" );
    readybtn:setVisible(false)
end

local function GameLost()
	local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.CSMsgGameLostReq");
	SendGameMessage( msg, 1004 );
    print( "self Lost" );
    lostbtn:setVisible(false)
end

local function createReadyMenu()
        if lostbtn ~= nil then
            lostbtn:setVisible(true)
            return lostbtn;
        end
        local visibleSize = cc.Director:getInstance():getVisibleSize()
        local function menuCallbackReady()
		    Ready()	
        end

        local menuPopupItem = cc.MenuItemImage:create("btn2_normal.png", "btn2_hover.png")
        menuPopupItem:setPosition(0, 0)
        menuPopupItem:registerScriptTapHandler(menuCallbackReady)
        readybtn = cc.Menu:create(menuPopupItem)
        readybtn:setPosition(visibleSize.width / 2, visibleSize.height / 4 * 3)

        local btntext = cc.Label:createWithSystemFont("准备", "arial", 20)
        local sz = menuPopupItem:getContentSize();
        btntext:setPosition( cc.p(sz.width / 2, sz.height / 2 ));
        menuPopupItem:addChild(btntext)

        print( "createReadyMenu" )
        return readybtn;
end

local function createLostMenu()
        local visibleSize = cc.Director:getInstance():getVisibleSize()
        local function menuCallbackReady()
		    GameLost()	
        end

        local menuPopupItem = cc.MenuItemImage:create("btn2_normal.png", "btn2_hover.png")
        menuPopupItem:setPosition(0, 0)
        menuPopupItem:registerScriptTapHandler(menuCallbackReady)
        lostbtn = cc.Menu:create(menuPopupItem)
        lostbtn:setPosition(visibleSize.width / 4 * 3, visibleSize.height / 4 * 3)

        local btntext = cc.Label:createWithSystemFont("认输", "arial", 20)
        local sz = menuPopupItem:getContentSize();
        btntext:setPosition( cc.p(sz.width / 2, sz.height / 2 ));
        menuPopupItem:addChild(btntext)

        return lostbtn;
end

local function _processMessageHistroy(msgid, msglen, data)
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

local function _processLoginGameResult(msgid, msglen, data)
	print( "[LUA][_processLoginGameResult] : ", msgid, ":", msglen );
	local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.SCLoginGameRsp");
	local ret = DPB_ParseFromArray(msg, data, msglen);
	print( "[LUA][_processLoginGameResult] ParseFromArray ret=", ret );
	ret = DPB_GetInt32(msg, "result");
	print( "[LUA][_processLoginGameResult] loginGameResult=", ret );
	DPB_DeleteMessage(msg);
    local strret = labelState:getString()
    if ret~=0 then
        strret = strret .. "失败!"
    else
        strret = strret .. "成功!"
    end
    labelState:setString(strret);
    if ret==0 then
        EnterRoom();
    end
end

local function _processEnterRoomResult(msgid, msglen, data)
	local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.SCEnterRoomRsp");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    ret = DPB_GetInt32(msg, "result");
    local roomid = DPB_GetInt32(msg, "roomid");
    print( "enter room:", roomid, "result:", ret );
    local strret = labelState:getString()
    if ret==0 then
        strret = strret .. "成功!"
    else
        strret = strret .. "失败!"
    end
    labelState:setString(strret);
    DPB_DeleteMessage(msg);
    if ret==0 then
        sceneGame:addChild(createReadyMenu());
    end
end

local function _processRoomInfoNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n房间内玩家信息:\n"
    local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.SCMsgRoomInfoNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    local size = DPB_GetFieldSize(msg, "players");
    print( "[LUA][_processRoomInfoNotify] players=", size );
    for i=0,size-1,1 do
        local g = DPB_GetRepeatedMessage(msg, "players", i);
        --strret = strret .. "玩家:" .. DPB_GetString(g, "user") .. " 是否准备完毕:" .. DPB_GetInt32(g, "isready") .. "\n"
    end
    DPB_DeleteMessage(msg);
    --labelState:setString(strret);
end
        
local function _processEnterRoomNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.SCEnterRoomNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    local player = DPB_GetString(msg, "user");
    strret = strret .. "玩家:" .. player .. "进入房间"
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
end

local function _processLeaveRoomNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("common.proto", "sglib.commonproto.SCLeaveRoomNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    local player = DPB_GetString(msg, "user");
    strret = strret .. "玩家:" .. player .. "离开房间"
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
end

local function _processRoomReady(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.SCMsgReadyRsp");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    ret = DPB_GetInt32(msg, "result");
    strret = strret .. "我准备好了"
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
end

local function _processRoomReadyNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.SCMsgReadyNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    local player = DPB_GetString(msg, "player");
    strret = strret .. "玩家:" .. player .. " 准备好了"
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
end

local function _processGameStartNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.SCMsgGameStartNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    strret = strret .. "游戏开始!"
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
    if lostbtn~=nil then
        lostbtn:setVisible(true);
    else
        sceneGame:addChild(createLostMenu())
    end
end

local function _processGameEndNotify(msgid, msglen, data)
    local strret = labelState:getString() .. "\n"
    local msg = DPB_CreateMessage("tetris.proto", "sglib.tetrisproto.SCMsgGameEndNtf");
	local ret = DPB_ParseFromArray(msg, data, msglen);
    local loser = DPB_GetString(msg, "loser");
    strret = strret .. "游戏结束! 失败者是:" .. loser
    DPB_DeleteMessage(msg);
    labelState:setString(strret);
    readybtn:setVisible(true)
    lostbtn:setVisible(false)
end

function ProcessGameMessage(msgid, msglen, data)
	print( "in lua ProcessGameMessage : ", msgid, ":", msglen );
	if msgid==932 then
		_processMessageHistroy(msgid, msglen, data);
		return
	end
	if msgid==504 then
		_processLoginGameResult(msgid, msglen, data);
	end
    if msgid==506 then
        _processEnterRoomResult(msgid, msglen, data);
    end
    if msgid==507 then
        _processEnterRoomNotify(msgid, msglen, data);
    end
    if msgid==510 then
        _processLeaveRoomNotify(msgid, msglen, data);
    end
    if msgid==1011 then
        _processRoomInfoNotify(msgid, msglen, data);
    end
    if msgid==1002 then
        _processRoomReady(msgid, msglen, data);
    end
    if msgid==1010 then
        _processRoomReadyNotify(msgid, msglen, data);
    end
    if msgid==1003 then
        _processGameStartNotify(msgid, msglen, data);
    end
    if msgid==1005 then
        _processGameEndNotify(msgid, msglen, data);
    end
end


local function TimerCallback()
    print( "TimerCallback" );
    LoginGame();
    scheduler:unscheduleScriptEntry(timer)
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
            GobackHome()
			--[[local msg = DPB_CreateMessage("group.proto", "sglib.groupproto.CSGroupMessageHistoryReq");
			local ret = DPB_SetUInt64(msg, "groupid", 1);
			print("DPB_SetUInt64 groupid ret:", ret);
			ret = DPB_SetInt64(msg, "idxfrom", 0);
			print("DPB_SetInt64 idxfrom ret:" , ret);
			ret = DPB_SetInt32(msg, "limit", 10);
			print("DPB_SetInt32 limit ret:" , ret);
			SendGameMessage( msg, 931 ) ]]--
        end

        local menuPopupItem = cc.MenuItemImage:create("btn2_normal.png", "btn2_hover.png")
        menuPopupItem:setPosition(0, 0)
        menuPopupItem:registerScriptTapHandler(menuCallbackOpenPopup)
        menuPopup = cc.Menu:create(menuPopupItem)
        menuPopup:setPosition(visibleSize.width / 4, visibleSize.height / 4 * 3)

        local btntext = cc.Label:createWithSystemFont("返回大厅", "arial", 20)
        local sz = menuPopupItem:getContentSize();
        btntext:setPosition( cc.p(sz.width / 2, sz.height / 2 ));
        menuPopupItem:addChild(btntext)
        layerMenu:addChild(menuPopup)

        return layerMenu
    end

    local function createStateLabel()
        labelState = cc.Label:createWithSystemFont("正在登录游戏...", "arial", 20)
        labelState:setPosition( cc.p(visibleSize.width / 2, visibleSize.height / 4 ));
        return labelState;
    end

    sceneGame = cc.Scene:create()
    sceneGame:addChild(createLayerFarm())
    sceneGame:addChild(createLayerMenu())
    sceneGame:addChild(createStateLabel())
	
	if cc.Director:getInstance():getRunningScene() then
		cc.Director:getInstance():replaceScene(sceneGame)
	else
		cc.Director:getInstance():runWithScene(sceneGame)
	end
	
    timer = scheduler:scheduleScriptFunc(TimerCallback, 0.5, false)

    cclog("enterGame tetris return")
end

enterGame();
