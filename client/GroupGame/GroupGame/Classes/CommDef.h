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

#endif
