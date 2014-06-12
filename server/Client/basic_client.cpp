#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 4096

int basic_client_test(char *ip, int port)
{
    int sockfd;
    struct sockaddr_in servaddr;
    
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
    	printf( "create socket error\n" );
    	return -1;
    }

    memset( &servaddr, 0, sizeof(servaddr) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons( port );
    if ( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0 )
    {
    	printf( "inet_pton error for %s\n", ip );
		close( sockfd );
    	return -2;
    }

    if( connect( sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr) ) < 0 )
    {
    	printf( "connect error: %s(errno: %d)\n", strerror(errno), errno );
		close( sockfd );
    	return -3;
    }
	
	printf("connected, press enter to close\n");
    char buf[16]={0};
    gets(buf);

    close( sockfd );
    return 0;
	
}

