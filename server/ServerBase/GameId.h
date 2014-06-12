#ifndef _GAME_ID_H_
#define _GAME_ID_H_

// 游戏ID
#define GAME_ID_NOUSE		0
#define GAME_ID_TETRIS		10


// 游戏标识
// 如果游戏被分区，如俄罗斯方块分为1区、2区，
// 则由两个不同的GameManager管理自己分区的GameServer,
// 因此每个GameManager有自己的GameId, 即 GAME_ID_TETRIS_1, GAME_ID_TETRIS_2,
// 但是他们是同种游戏，因此他们都是 GAME_TETRIS.
#define GAME_NOUSE			0
#define GAME_TETRIS			10

#endif
