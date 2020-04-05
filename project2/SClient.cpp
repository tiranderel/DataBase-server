#include "SClient.h"




SClient::SClient(SOCKET s, SOCKADDR_IN sock_in)
{

	c_sock = s;
	c_addr = sock_in;
	printf("Client created\n");
	//handle();
	HandleThread = CreateThread(NULL, 0, ThreadF, this, 0, &IdThread);

}


SClient::~SClient()
{
	TerminateThread(HandleThread, NULL);
	CloseHandle(HandleThread);
}

unsigned long SClient::CalculationThread()
{
	// Здесь все нужно переписывать!!!!!!

	while (true)
	{
		int k = recv(c_sock, buffer, sizeof(buffer), NULL);
		printf("%u\n", k);
		Sleep(30);
	}

	/////////
}
