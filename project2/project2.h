#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <thread>          
#include <string>
#include <regex>
#include <map>
#include <fstream>
#include <stdio.h>
#include <ctime>
#include <vector>

#define DEFAULT_BUFFER_LENGTH 256

using namespace std;

class SServer
{
public:
	SServer();
	~SServer();
	void startServer();
	void closeServer();
	void handle();
	unsigned short port;
private:
	SOCKET this_s;
	WSAData wData;
};

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
	char* c_buffer[1024];


	static DWORD WINAPI ThreadF(void *arg)
	{

		return ((SClient*)arg)->CalculationThread();
	}
protected:
};

