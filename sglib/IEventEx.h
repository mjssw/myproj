#ifndef _I_EVENT_EX_H_
#define _I_EVENT_EX_H_

#include "Define.h"

namespace SGLib
{
	class IEventEx
	{
	public:
		virtual ~IEventEx(){}
		virtual void HandleEvent(s32 paramLen, char *paramData) = 0;
	};
};

#endif
