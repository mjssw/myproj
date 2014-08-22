#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

extern void TestForIoService(const char *ip, int port, int clientNum, int freq);
int main(int argc, char *argv[])
{
	if( argc == 5 )
	{
		TestForIoService( argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]) );
	}
	return 0;
}

