#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include "cocos2d.h"

// UTF8 To GB2312
std::string u2a(const char *inbuf);

// GB2312 To UTF8
std::string a2u(const char *inbuf);

// 单个中文汉字/英文 在对应字体下的宽度
cocos2d::Size chineseSizeWithFont(const char *font, int size);
cocos2d::Size asciiSizeWithFont(const char *font, int size);

// 计算头像相关
void CalcUserHead(const std::string &head, int sex, std::string &realhead, cocos2d::Rect &rc, bool &useRc);

std::string GetCurTime();
std::string FormatTime(int timeval);

#endif
