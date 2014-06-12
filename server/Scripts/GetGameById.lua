-- 注册 GameId 对应的 Game, 注意每次增加游戏或游戏新区都要对照 GameId.h 修改此文件
-- GameName			GameId			Game
-- 俄罗斯方块		10				10

local switch_table = {
	[10] = 10,
}

function GetGameById(id)
	local ret = switch_table[id]
	if(ret) then 
		return ret
	else
		return 0
	end
end