#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include "cocos2d.h"

// UTF8 To GB2312
std::string u2a(const char *inbuf);

// GB2312 To UTF8
std::string a2u(const char *inbuf);

// �������ĺ���/Ӣ�� �ڶ�Ӧ�����µĿ��
cocos2d::Size chineseSizeWithFont(const char *font, int size);
cocos2d::Size asciiSizeWithFont(const char *font, int size);

// ����ͷ�����
void CalcUserHead(const std::string &head, int sex, std::string &realhead, cocos2d::Rect &rc, bool &useRc);

std::string GetCurTime();
std::string FormatTime(int timeval);

#endif
