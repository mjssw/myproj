#ifndef _SG_SETUP_SOCKET_H_
#define _SG_SETUP_SOCKET_H_

#include "Define.h"

namespace SGLib
{
    class CSetupSocket
    {
    public:
        ~CSetupSocket(){};

		static CSetupSocket& Instance()
        {
            static CSetupSocket SetupSock;
            return SetupSock;
        }

        void Setup()
        {
            #ifdef _USE_WIN32
                if( m_bSetup == false )
                {
			        WSADATA wsaData;
			        WSAStartup( MAKEWORD(2,2), &wsaData );
                    m_bSetup = true;
                }
            #endif
        }

        void Clean()
        {
            #ifdef _USE_WIN32
                if( m_bSetup == true )
                {
                    WSACleanup();
                    m_bSetup = false;
                } 
            #endif
        }

    private:
        bool m_bSetup;
        CSetupSocket() : m_bSetup(false)
        {}
    };
};

#endif

