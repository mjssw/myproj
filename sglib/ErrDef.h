#ifndef _SG_ERR_DEFINE_H_
#define _SG_ERR_DEFINE_H_

namespace SGLib
{
	enum E_ERROR_CODE_DEF
	{
		E_Error_Success = 0,		                     // û�д���

		E_Error_CreateCompletePortFailed	= -1001,	 // ������ɶ˿�ʧ��
		E_Error_CreateSocketFailed			= -1002,	 // ����SOCKET���ʧ��
		E_Error_BindListenSocketFailed		= -1003,	 // �󶨼����˿�ʧ��
		E_Error_ListenSocketFailed			= -1004,	 // ����ʧ��
		E_Error_BindToCompletePortFailed	= -1005,	 // �󶨵���ɶ˿�ʧ��
		E_Error_GetAcceptExPtrFailed		= -1006,	 // ��ȡAcceptEx����ָ��ʧ��
		E_Error_GetAcceptExSockAddrFailed	= -1007,     // ��ȡAcceptExSockAddrs����ָ��ʧ��
		E_Error_CreateWorkThreadFailed		= -1008,	 // ���������߳�ʧ��
		E_Error_CreateIoSocketFailed		= -1009,     // ����IoSocket����ʧ��
		E_Error_SendFailed					= -1011,	 // ����ʧ��
        E_Error_EpollCreateFailed           = -1012,     // epoll_create����ʧ��
        E_Error_SetSocketNonBlockFailed     = -1013,     // ����socket������ʧ��
        E_Error_EpollCtlFailed              = -1014,     // epoll_ctl����ʧ��
		E_Error_CreateDispatchEventFailed	= -1015,	
		E_Error_LoadGloablConfigFailed      = -1016,	 // ����global.xml ʧ��
		E_Error_CanNotFoundServerConfig     = -1017, // û���ҵ�server����
	};

};

#endif
