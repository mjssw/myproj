#ifndef _UTILS_H_
#define _UTILS_H_

#define WORLD_WIDTH			320
#define WORLD_HEIGHT		480	
#define GAME_ICON_WIDTH		200
#define GAME_ICON_HEIGHT	100
#define GROUP_ICON_WIDTH	100
#define GROUP_ICON_HEIGHT	50
#define ROOM_W				60
#define ROOM_H				60
#define ROOM_GRAP			20
#define ROOM_LEFR_FROM		10
#define ROOM_ONE_ROW		6
#define ROOM_ID_FROM		1
#define BG_SCALE			0.6
#define BLOCK_W				16
#define BLOCK_H				16
#define BATTLE_AREA_X		120
#define BATTLE_AREA_Y		110
#define NEXT_BLOCK_COL		(-5)

//#define PATCH_RES_DIR(res)	("..\\Resources\\" ## res)
#define PATCH_RES_DIR(res)	(res)

#define DEFINE_SINGLETON_CLASS(class_name)	\
public:\
	static class_name& Instance()\
	{\
		if( m_pInstance == NULL )\
		{\
			m_pInstance = new class_name();\
		}\
		return *m_pInstance;\
	}\
private:\
	class_name(){}\
	static class_name *m_pInstance;\

#define INIT_SIGNLETON_CLASS(class_name)	\
class_name* class_name::m_pInstance = NULL;

#define NET_MSG			"NetMsg"

#define GAME_ID_TETRIS	10

#include <string>

// UTF8 To GB2312
std::string u2a(const char *inbuf);

// GB2312 To UTF8
std::string a2u(const char *inbuf);

#endif
