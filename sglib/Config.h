#ifndef _SG_CONFIG_H_
#define _SG_CONFIG_H_

#define WAIT_WORK_THREAD_QUIT_TIME	8000		// ��ʱ
#define DEFAULT_MS_TIME_OUT			1000		// IOĬ�ϵĵȴ���ʱʱ��
#define MAX_CLIENT_COUNT		    5000		// ���������
#define MAX_RECV_BUFFER_LEN			(1<<16)		// ���ջ���������󳤶� 64kb
#define MAX_SEND_BUFFER_LEN			(1<<16)		// ���ͻ���������󳤶� 64kb
#define MAX_EVENT_COUNT				(1<<13)     // �¼����г���
#define WAIT_EVENT_TIME_OUT			1000		// �ȴ��¼���ʱ
#define MAX_RECV_PKG_LEN			(1<<12)     // ���ܵ�һ�����ݰ�����󳤶�
#define LOG_BUFFER_MAX_LEN          (1<<12)     // ��־��󳤶�
#define MAX_DISPATCH_EVENT_NUM		16			// �¼������̵߳ĸ���, ����Ϊ1ʱ�����̴߳���
#define CONNECTING_QUEUE_SIZE		16			// ͬʱδ��������������������
//#define EPOLL_MAX_EVENT_COUNT       MAX_CLIENT_COUNT // epoll�ȴ�ʱ��������
#define STR_IP_LEN					64			// �ַ���ip�ĳ���

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
