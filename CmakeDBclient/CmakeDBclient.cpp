// CmakeDBclient.cpp: определяет точку входа для приложения.
//

#include "CmakeDBclient.h"

using namespace std;

SOCKET podcluchenie;

int main(int argc, char* argv[]) {

	string fname = "adres.txt";
	ifstream input(fname);
	string address_s;
	string port_s;
	getline(input, address_s);

	getline(input, port_s);

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "oshibka" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int razmerada = sizeof(addr);
	//локалхост
	addr.sin_addr.s_addr = inet_addr(address_s.c_str());
	addr.sin_port = htons(stoi(port_s));
	addr.sin_family = AF_INET;

	
	

	//отправляет сообщение на сервер в переменную msg
	char msg1[256];
	while (true) {
		podcluchenie = socket(AF_INET, SOCK_STREAM, NULL);
		std::cout << "vvedite comandu: ";
		std::cin.getline(msg1, sizeof(msg1));
		if (connect(podcluchenie, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
			std::cout << "oshibka podkluchenia k serveru\n";
		}
		else {
			std::cout << "connected\n";
			send(podcluchenie, msg1, sizeof(msg1), NULL);
			Sleep(10);
			char msg2[4];
			int er = recv(podcluchenie, msg2, sizeof(msg2), NULL);
			if (er > 0) {
				int* dee = (int*)&msg2[0];
				//cout << *dee << endl;
				if (*dee == 0)
					cout << "it is OK" << endl;
				else if (*dee == -1)
					cout << "incorrect parameters of table" << endl;
				else if (*dee == -2)
					cout << "table exist" << endl;
				else if (*dee == -3)
					cout << "table not found" << endl;
				else if (*dee == -4)
					cout << "incorrect nuber of parameters in table" << endl;
				else if (*dee == -5)
					cout << "table not found" << endl;
				else if (*dee == -6)
					cout << "syntax error" << endl;
				else if (*dee == -8)
					cout << "table reading error" << endl;
				else if (*dee == -9)
					cout << "the table is empty" << endl;
				else if (*dee == -10)
					cout << "records not found" << endl;
				else if (*dee > 0) {
					int p1 = msg2[2];
					int p2 = msg2[3];
					//cout << p1 << " " << p2 << endl;
					char msg3[1024];
					int er1 = recv(podcluchenie, msg3, sizeof(msg3), NULL);
					if (er1 > 0) {
						int dlina = 8 + p1 * 4;
						for (int g = 0; g < p2; g++) {
							time_t* tt0 = (time_t*)&msg3[g*dlina];
							cout << asctime(localtime(tt0));
							for (int w = 0; w < p1; w++) {
								float* ff0 = (float*)&msg3[g * dlina + 8 + 4 * w];
								cout << " " << *ff0;
							}
							cout << endl;

						}
						///cout << msg3 << endl;
					}
				}
				else
					cout << "unknown error" << endl;

				//string stroka = str(recvstr);
				///cout << *dee << endl;
				//int res = -foo(stroka);
				//char* c = (char*)&res;

				///char msg1[4];
				///msg1[0] = *c;
				///msg1[1] = *(c + 1);
				///msg1[2] = *(c + 2);
				///msg1[3] = *(c + 3); 
			}
			else if (er == 0) {
				cout << "connection terminated\n"; 
			}
			else {
				cout << "connection error" << endl; 
			}
			closesocket(podcluchenie);
		}
		
	}
	system("pause");
	return 0;
}