#ifndef _COMM_DEFINE_H_
#define _COMM_DEFINE_H_

#define WIN_SIZE_W	960
#define WIN_SIZE_H	640

#define RES_ROOT	".\\"

#define CHAT_FONT_NAME	"arial"
#define CHAT_FONT_SIZE	12

typedef int				MsgHeadType;
#define MSG_ID_LEN		sizeof(MsgHeadType)
#define MSG_HEAD_LEN	MSG_ID_LEN
#define MAX_MSG_LEN		4096

#define SYS_HEAD_SIZE_W	85
#define SYS_HEAD_SIZE_H	85

#define CHAT_MESSAGE_LIMIT	10
	
enum ESex
{
	E_Sex_Male,
	E_Sex_Female,
};
	
enum EMoneyType
{
	E_Money_Gold,
	E_Money_Diamond,

	E_Money_Type_Count,
};

#include <QDebug>
#include <assert.h>
#define CCLog qDebug
#define CCAssert(x,y) assert(x)

//#ifdef _USE_WIN32
//#pragma comment(lib,"ws2_32.lib")
//#endif

#endif
