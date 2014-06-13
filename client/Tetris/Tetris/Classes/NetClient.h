#ifndef _NET_CLIENT_H_
#define _NET_CLIENT_H_

#include "ClientLib.h"
using namespace SGLib;

#include "login.pb.h"
#include "client.pb.h";
#include "msgid.pb.h"
#include "common.pb.h"
#include "errno.pb.h"
#include "tetris.pb.h"

#include <map>
#include <vector>
#include <string>
using namespace std;


class CNetClient : public SGLib::CClientBase
{
public:
	enum
	{
		MSG_ID_LEN = sizeof(s32),
		MSG_HEAD_LEN = MSG_ID_LEN,
		MAX_MSG_LEN = 4096,
	};

	CNetClient(int id): SGLib::CClientBase(id) {}

	virtual int OnRecv(char *buf, int len);
	virtual void OnConnect();
	virtual void OnClose();
	virtual void OnConnectError();

	void UserLogin(const std::string &user);
	void UserEnterGame(int gameid);
	void LoginGame(const std::string &userId);
	void EnterRoom(s32 roomId);
	void LeaveRoom();
	void Ready();
	void Lost();
	void PutBlock(s32 blockid, s32 column);
	void ClearBlock(vector<s32> &vecRows);

private:
	void _UserLoginRsp(const sglib::loginproto::SCUserLoginRsp &rsp);
	void _GameInfoNotify(const sglib::loginproto::SCGameInfoNotify &ntf);
	void _EnterGameRsp(const sglib::commonproto::SCEnterGameRsp &rsp);
	void _LoginGameRsp(const sglib::commonproto::SCLoginGameRsp &rsp);
	void _EnterRoomRsp(const sglib::commonproto::SCEnterRoomRsp &rsp);
	void _LeaveRoomRsp(const sglib::commonproto::SCLeaveRoomRsp &rsp);
	void _EnterRoomNtf(const sglib::commonproto::SCEnterRoomNtf &ntf);
	void _LeaveRoomNtf(const sglib::commonproto::SCLeaveRoomNtf &ntf);
	void _ReadyRsp(const sglib::tetrisproto::SCMsgReadyRsp &rsp);
	void _GameStart(const sglib::tetrisproto::SCMsgGameStartNtf &ntf);
	void _GameEnd(const sglib::tetrisproto::SCMsgGameEndNtf &ntf);
	void _EnemyReady(const string &enemy);
	void _RoomInfoNotify(const sglib::tetrisproto::SCMsgRoomInfoNtf &ntf);
	void _EnemyPutBlockNotify(const sglib::tetrisproto::SCMsgPutBlockNtf &ntf);
	void _EnemyClearBlockNotify(const sglib::tetrisproto::SCMsgClearBlockNtf &ntf);

private:
	void SendMsg(const ::google::protobuf::Message &Msg, s32 nMsgId);

};

#endif
