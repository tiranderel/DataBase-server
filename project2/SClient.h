#pragma once
#include "includes.h"

class SClient
{
public:
	SClient(SOCKET s, SOCKADDR_IN sock_in);
	~SClient();
	unsigned long CalculationThread();

	//void handle();
private:
	void *HandleThread;
	unsigned long IdThread;

	SOCKET c_sock;
	SOCKADDR_IN c_addr;
	char buffer[1024];


	static DWORD WINAPI ThreadF(void *arg)
	{

		return ((SClient*)arg)->CalculationThread();
	}
protected:
	

};
