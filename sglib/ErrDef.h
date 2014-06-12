#ifndef _SG_ERR_DEFINE_H_
#define _SG_ERR_DEFINE_H_

namespace SGLib
{
	enum E_ERROR_CODE_DEF
	{
		E_Error_Success = 0,		                     // 没有错误

		E_Error_CreateCompletePortFailed	= -1001,	 // 创建完成端口失败
		E_Error_CreateSocketFailed			= -1002,	 // 创建SOCKET句柄失败
		E_Error_BindListenSocketFailed		= -1003,	 // 绑定监听端口失败
		E_Error_ListenSocketFailed			= -1004,	 // 监听失败
		E_Error_BindToCompletePortFailed	= -1005,	 // 绑定到完成端口失败
		E_Error_GetAcceptExPtrFailed		= -1006,	 // 获取AcceptEx函数指针失败
		E_Error_GetAcceptExSockAddrFailed	= -1007,     // 获取AcceptExSockAddrs函数指针失败
		E_Error_CreateWorkThreadFailed		= -1008,	 // 创建工作线程失败
		E_Error_CreateIoSocketFailed		= -1009,     // 创建IoSocket对象失败
		E_Error_SendFailed					= -1011,	 // 发送失败
        E_Error_EpollCreateFailed           = -1012,     // epoll_create调用失败
        E_Error_SetSocketNonBlockFailed     = -1013,     // 设置socket非阻塞失败
        E_Error_EpollCtlFailed              = -1014,     // epoll_ctl调用失败
		E_Error_CreateDispatchEventFailed	= -1015,	
		E_Error_LoadGloablConfigFailed      = -1016,	 // 加载global.xml 失败
		E_Error_CanNotFoundServerConfig     = -1017, // 没有找到server配置
	};

};

#endif
