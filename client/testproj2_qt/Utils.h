#ifndef UTILS_H
#define UTILS_H

#define SINGLETON_CLASS_DEFINE(class_name)	\
public:\
    static class_name& Instance()\
    {\
        if( m_pInstance == NULL )\
        {\
            m_pInstance = new class_name();\
        }\
        return *m_pInstance;\
    }\
    ~class_name(){ delete m_pInstance; };\
private:\
    class_name(){};\
    static class_name *m_pInstance;\

#define SIGNLETON_CLASS_INIT(class_name)	\
class_name* class_name::m_pInstance = NULL;

#endif // UTILS_H
