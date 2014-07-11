#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

// UTF8 To GB2312
std::string u2a(const char *inbuf);

// GB2312 To UTF8
std::string a2u(const char *inbuf);

#endif
