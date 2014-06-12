#ifndef _SG_CONFIG_H_
#define _SG_CONFIG_H_

#define WAIT_WORK_THREAD_QUIT_TIME	8000		// 超时
#define DEFAULT_MS_TIME_OUT			1000		// IO默认的等待超时时间
#define MAX_CLIENT_COUNT		    5000		// 最大连接数
#define MAX_RECV_BUFFER_LEN			(1<<16)		// 接收缓冲区的最大长度 64kb
#define MAX_SEND_BUFFER_LEN			(1<<16)		// 发送缓冲区的最大长度 64kb
#define MAX_EVENT_COUNT				(1<<13)     // 事件队列长度
#define WAIT_EVENT_TIME_OUT			1000		// 等待事件超时
#define MAX_RECV_PKG_LEN			(1<<12)     // 接受的一个数据包的最大长度
#define LOG_BUFFER_MAX_LEN          (1<<12)     // 日志最大长度
#define MAX_DISPATCH_EVENT_NUM		16			// 事件处理线程的个数, 设置为1时即单线程处理
#define CONNECTING_QUEUE_SIZE		16			// 同时未处理的主动连接请求个数
//#define EPOLL_MAX_EVENT_COUNT       MAX_CLIENT_COUNT // epoll等待时间最大个数
#define STR_IP_LEN					64			// 字符串ip的长度

#include "Define.h"

class CNetConfig
{
public:
	s32 m_workThreadQuitTimeOut;
	s32 m_ioTimeOut;
	s32 m_maxClientCount;
	s32 m_eventQueueSize;
	s32 m_waitEventTimeOut;
	s32 m_maxPackageLen;
	s32 m_eventWorkerCount;
	s32 m_connectingQueueSize;
	s32 m_sendBufferSize;
	s32 m_recvBufferSize;
};

class CNetConfigMgr
{
public:
	CNetConfigMgr(const char *netcfg_file, const char *cfg_name);
	~CNetConfigMgr();
	const CNetConfig& GetNetConfig(); 

private:
	bool m_bInit;
	CNetConfig m_netCfg;
	void _Init(const char *netcfg_file, const char *cfg_name);
	void _LoadNetConfig(const char *netcfg_file, const char *cfg_name);
};

#endif
