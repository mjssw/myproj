#ifndef _UTILS_H_
#define _UTILS_H_

#include "Define.h"
#include <string>

#define DEFINE_SINGLETON_CLASS(class_name)	\
public:\
	static class_name& Instance()\
	{\
		if( m_pInstance == NULL )\
		{\
			m_pInstance = new class_name();\
		}\
		return *m_pInstance;\
	}\
private:\
	class_name(){}\
	static class_name *m_pInstance;\

#define INIT_SIGNLETON_CLASS(class_name)	\
class_name* class_name::m_pInstance = NULL;

#define DECLARE_IOSERVICEEX_CLONE_FUNC(class_name)	\
virtual CIoSocketEx* Clone(s32 id)\
{\
	return new class_name( id );\
}

class CUtils
{
public:

	static std::wstring UTF8toUnicode(const char *utf8Str, s32 len)
	{
		std::wstring unicodeStr(L"");

		if( !utf8Str )
		{
			return unicodeStr;
		}

		if( len == 0 )
		{
			return unicodeStr;
		}

		wchar_t chr = 0;
		for( s32 i=0; i<len; )
		{
			if( (0x80 & utf8Str[i]) == 0 ) // ASCII
			{
				chr = utf8Str[i];
				i++;
			}
			else if( (0xE0 & utf8Str[i]) == 0xC0 ) // 110xxxxx 10xxxxxx
			{
				chr = (utf8Str[i+0] & 0x3F) << 6;
				chr |= (utf8Str[i+1] & 0x3F);
				i += 2;
			}
			else if( (0xF0 & utf8Str[i]) == 0xE0 ) // 1110xxxx 10xxxxxx 10xxxxxx
			{
				chr = (utf8Str[i+0] & 0x1F) << 12;
				chr |= (utf8Str[i+1] & 0x3F) << 6;
				chr |= (utf8Str[i+2] & 0x3F);
				i += 3;
			}
			/*
			else if() // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			{}
			else if() // 111110xx 10xxxxxx 10xxxxxx 10xxxxxx  10xxxxxx
			{}
			else if() // 1111110x 10xxxxxx 10xxxxxx 10xxxxxx  10xxxxxx 10xxxxxx 
			{}
			*/
			else // ²»ÊÇUTF-8×Ö·û´®
			{
				return unicodeStr;
			}
			unicodeStr += chr;
		}

		return unicodeStr;
	}

	static std::wstring UTF8toUnicode(const char *utf8Str)
	{
		s32 len = (s32)strlen( utf8Str );
		return UTF8toUnicode( utf8Str, len );
	}

	static std::string ws2s(const std::wstring &ws)
	{
		std::string curLocale = setlocale( LC_ALL, NULL );        // curLocale = "C";
		setlocale( LC_ALL, "chs" );
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		char *_Dest = new char[_Dsize];
		memset( _Dest, 0, _Dsize );
		wcstombs( _Dest, _Source, _Dsize );
		std::string result = _Dest;
		delete [] _Dest;
		setlocale( LC_ALL, curLocale.c_str() );
		return result;
	}

	static std::string utf82s(const char *utf8Str)
	{
		std::wstring uniStr = UTF8toUnicode( utf8Str );
		return ws2s( uniStr );
	}

	static std::string GetCurYear()
	{
		time_t now;
		time( &now );
		char strtime[64] = {0};
		strftime( strtime, sizeof(strtime), "%Y",localtime(&now) );
		return std::string( strtime );
	}

	static std::string GetCurMonth()
	{
		time_t now;
		time( &now );
		char strtime[64] = {0};
		strftime( strtime, sizeof(strtime), "%m",localtime(&now) );
		return std::string( strtime );
	}

	static std::string GetCurDay()
	{
		time_t now;
		time( &now );
		char strtime[64] = {0};
		strftime( strtime, sizeof(strtime), "%d",localtime(&now) );
		return std::string( strtime );
	}

	static s32 GetTimeNow()
	{
		time_t now;
		time( &now );
		return (s32)(now);
	}
};

#endif
