#ifndef _IO_SESSION_EX_H_
#define _IO_SESSION_EX_H_

#include "IoSocketEx.h"
#include "EventWorkerEx.h"

namespace SGLib
{
	class CIoAdvSocketEx : public CIoSocketEx
	{
	public:
		CIoAdvSocketEx(s32 id);
		~CIoAdvSocketEx();

		// work event call back
		virtual void HandleEvent(s32 paramLen, char *paramData);
		virtual void Close();
	protected:
		virtual void  _ProcessRecvData();
	};
};

#endif
