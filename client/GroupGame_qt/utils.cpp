#include "utils.h"
#include "CommDef.h"
#include <time.h>
/*
void CalcUserHead(const std::string &head, int sex, std::string &realhead, cocos2d::Rect &rc, bool &useRc)
{

	if( head == "" )
	{
		rc = cocos2d::Rect(0, 0, 0, 0);
		useRc = false;
		if( sex == E_Sex_Male )
		{
			realhead = "boy.png";
		}
		else
		{
			realhead = "girl.png";
		}
	}
	else if( head.at(0) == '#' )
	{
		std::string real = head.substr( 1 );
		int offset = atoi( real.c_str() );
		int offx = (offset >> 16);
		int offy = (offset & 0xFFFF );
		rc.origin = ccp( offx * SYS_HEAD_SIZE_W, offy * SYS_HEAD_SIZE_H );
		rc.size = Size( SYS_HEAD_SIZE_W, SYS_HEAD_SIZE_H );
		realhead = "syshead.png";
		useRc = true;
	}
	else
	{
		rc = cocos2d::Rect(0, 0, 0, 0);
		useRc = false;
		realhead = head;
    }
}//*/

std::string GetCurTime()
{
	time_t now;
	time( &now );
	char strtime[64] = {0};
	strftime( strtime, sizeof(strtime), "%Y-%m-%d %X", localtime(&now) );
	return std::string( strtime );
}

std::string FormatTime(int timeval)
{
	time_t t = timeval;
	char strtime[64] = {0};
	strftime( strtime, sizeof(strtime), "%Y-%m-%d %X", localtime(&t) );
	return std::string( strtime );
}
