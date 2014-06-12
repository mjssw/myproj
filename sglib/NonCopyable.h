#ifndef _SG_NONCOPYABLE_H_
#define _SG_NONCOPYABLE_H_

namespace SGLib
{
    class NonCopyable 
    {
    protected:
        NonCopyable(){}
        ~NonCopyable(){}

    private:
        NonCopyable(const NonCopyable&);
        const NonCopyable& operator=(const NonCopyable&);
    };

}



#endif