#include "utils.h"
using namespace cocos2d;

#ifdef _WINDOWS
#include "iconv.h"

static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd;
    const char *temp = inbuf;
    const char **pin = &temp;
    char **pout = &outbuf;
    memset( outbuf, 0, outlen );
    cd = iconv_open( to_charset, from_charset );
	if( cd == 0 )
	{
		return -1;
	}
	if( iconv(cd,pin,&inlen,pout,&outlen) == -1 )
	{
		return -1;
	}
	iconv_close( cd );
    return 0;
}

// UTF8 To GB2312
std::string u2a( const char *inbuf )
{
    size_t inlen = strlen( inbuf );
    char * outbuf = new char[inlen * 2 + 2];
    std::string strRet( "" );
    if( code_convert("utf-8", "gb2312", inbuf, inlen, outbuf, inlen * 2 + 2) == 0 )
    {
        strRet = outbuf;
    }
    delete [] outbuf;
    return strRet;
}

// GB2312 To UTF8
std::string a2u(const char *inbuf)
{
    size_t inlen = strlen( inbuf );
    char *outbuf = new char[inlen * 2 + 2];
    std::string strRet( "" );
    if( code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, inlen * 2 + 2) == 0 )
    {
        strRet = outbuf;
    }
    delete [] outbuf;
    return strRet;
}

#else

std::string u2a( const char *inbuf )
{
	return std::string( inbuf );
}

std::string a2u(const char *inbuf)
{
	return std::string( inbuf );
}

#endif

Size chineseSizeWithFont(const char *font, int size)
{
	if( !font || size<=0 )
	{
		return Size(0, 0);
	}

	Label *labelText = Label::createWithSystemFont( a2u("жа"), font, size );
	if( !labelText )
	{
		return Size(0, 0);
	}
	Size sz = labelText->getContentSize();
	// TODO

	return sz;
}

Size asciiSizeWithFont(const char *font, int size)
{
	if( !font || size<=0 )
	{
		return Size(0, 0);
	}

	Label *labelText = Label::createWithSystemFont( "a", font, size );
	if( !labelText )
	{
		return Size(0, 0);
	}
	Size sz = labelText->getContentSize();
	// TODO

	return sz;
}
