
local gamedir_table = {
	[10] = "tetris",
}

local gameicon_table = {
	[10] = "res/tetris.png",
}

local gamename_table = {
	[10] = "俄罗斯方块",
}

function GetGameInfo(id)
	local dir = gamedir_table[id]
	if dir==nil then dir="" end
	local icon = gameicon_table[id]
	if icon==nil then icon="" end
	local name = gamename_table[id]
	if name==nil then name="" end
	print( "dir:", dir, "name:", name, "icon:", icon );
	return dir,icon,name
end
