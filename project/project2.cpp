#include "project2.h"

using namespace std;

typedef map<string, int>  mapT;
mapT tables;


int save_tables() {
	string fname = "Tables.bin";
	FILE* fp = fopen(fname.c_str(), "w");
	if (!fp)
		return -errno;
	mapT::iterator it = tables.begin();
	for (int i = 0; it != tables.end(); it++, i++) {
		fprintf(fp, "%s=%s\n", it->first.c_str(), to_string(it->second).c_str());
	}
	fclose(fp);
	return 0;
}

int read_tables() 
{
	string fname = "Tables.bin";
	ifstream input(fname);
	string line;
	tables.clear();
	char* buf = 0;
	size_t buflen = 0;
	regex wrd("\\w+");
	while (getline(input, line)) {
		sregex_iterator wrd_next(line.begin(), line.end(), wrd);
		//wrd_next++;
		smatch match = *wrd_next;
		string tn = match.str();
		wrd_next++;
		match = *wrd_next;
		int tl = stoi(match.str());
		tables[tn] = tl;
	}
	return 0;
}

int create_table(string s, int n) {
	mapT::iterator  it = tables.find(s);
	if (it == tables.end()) {
		tables[s] = n;
		ofstream file;
		file.open(s + ".dat", std::ofstream::out | std::ofstream::app);
		file.close();
		save_tables();
		return 0;
	}
	else
	{
		//cout << "error of table creating 1" << endl;
		return 1;
	}
}

int delete_table(string s) {
	mapT::iterator  it = tables.find(s);
	if (it != tables.end()) {
		tables.erase(s);
		if (remove((s + ".dat").c_str())) {
			//cout << "Error removing file" << endl;
			return 2;
		}	
		else {
			save_tables();
			return 0;
		}		
	}
	else
	{
		//cout << "error of table deleting 2" << endl;
		return 2;
	}
}

string str_toupper(string s) {
	transform(s.begin(), s.end(), s.begin(),
		[](unsigned char c) { return toupper(c); }
	);
	return s;
}

SClient::SClient(SOCKET s, SOCKADDR_IN sock_in)
{
	c_sock = s;
	c_addr = sock_in;
	printf("Client created\n");
	HandleThread = CreateThread(NULL, 0, ThreadF, this, 0, &IdThread);
}

SClient::~SClient()
{
	TerminateThread(HandleThread, NULL);
	CloseHandle(HandleThread);
}

int foo(string stroka, SOCKET &d_sock)
{
	string stroke = str_toupper(stroka);
	string chislo = "(\\-*\\d+(.\\d+){0,1})";
	string datar = "\\d\\d\\.\\d\\d\\.\\d\\d\\d\\d\\s\\d\\d\\:\\d\\d\\:\\d\\d";
	regex insrt("I\\s+\\w+\\s+\\(" + chislo + "(\\s*\\,\\s+" + chislo + ")*\\)");
	regex insrt2("\\(" + chislo + "(\\s*\\,\\s+" + chislo + ")*\\)");
	regex crt("C\\s+\\w+\\s+\\d+");
	regex dlt("D\\s+\\w+");
	regex slt("S\\s+\\w+\\s+" + datar);
	regex chi(chislo);
	regex wrd("\\w+");
	regex dtr(datar);
	try {
		sregex_iterator insrt_next(stroke.begin(), stroke.end(), insrt);
		sregex_iterator end;
		if (insrt_next != end) {
			sregex_iterator wrd_next(stroke.begin(), stroke.end(), wrd);
			wrd_next++;
			smatch match = *wrd_next;
			string table_name = match.str();
			sregex_iterator chisla_next(stroke.begin(), stroke.end(), insrt2);
			match = *chisla_next;
			string chisla = match.str();
			sregex_iterator next(chisla.begin(), chisla.end(), chi);
			int nn = 0;
			float chisla2[50];
			while (next != end) {
				nn += 1;
				smatch match = *next;
				chisla2[nn - 1] = stof(match.str());
				next++;
			}
			mapT::iterator  it = tables.find(table_name);
			if (it != tables.end()) {
				if (tables[table_name] == nn) {
					time_t t = time(0);
					ofstream file;
					file.open(table_name + ".dat", ios::binary | ofstream::out | ofstream::in);
					file.seekp(0, ios_base::end);
					file.write((char*)(&t), sizeof(time_t));
					file.write((char*)(&chisla2), sizeof(float) * nn);
					file.close();
				}
				else {
					//cout << "error 4" << endl;
					return 4;
				}
			}
			else {
				///cout << "error 5" << endl;
				return 5;
			}
		}
		else {
			sregex_iterator crt_next(stroke.begin(), stroke.end(), crt);
			if (crt_next != end) {
				sregex_iterator wrd_next(stroke.begin(), stroke.end(), wrd);
				wrd_next++;
				smatch match = *wrd_next;
				string table_name = match.str();
				wrd_next++;
				match = *wrd_next;
				int nn = stoi(match.str());
				if (nn > 0) {
					int err = create_table(table_name, nn);
					if (err > 0) {
						///cout << "error 2" << endl;
						return 2;
					}
				}
				else {
					///cout << "error 1" << endl;
					return 1;
				}
			}
			else {
				sregex_iterator dlt_next(stroke.begin(), stroke.end(), dlt);
				if (dlt_next != end) {
					sregex_iterator wrd_next(stroke.begin(), stroke.end(), wrd);
					wrd_next++;
					smatch match = *wrd_next;
					string table_name = match.str();
					int err = delete_table(table_name);
					if (err > 0) {
						///cout << "error 3" << endl;
						return 3;
					}
				}
				else {
					sregex_iterator slt_next(stroke.begin(), stroke.end(), slt);
					if (slt_next != end) {
						sregex_iterator wrd_next(stroke.begin(), stroke.end(), wrd);
						wrd_next++;
						smatch match = *wrd_next;
						string table_name = match.str();
						sregex_iterator dtr_next(stroke.begin(), stroke.end(), dtr);
						match = *dtr_next;
						string startdata = match.str();
						mapT::iterator  it = tables.find(table_name);
						if (it != tables.end()) {
							tm tt1;
							try
							{
								tt1.tm_sec = (startdata[17] - 48) * 10 + startdata[18] - 48;
								tt1.tm_min = (startdata[14] - 48) * 10 + startdata[15] - 48;
								tt1.tm_hour = (startdata[11] - 48) * 10 + startdata[12] - 48;
								tt1.tm_mday = (startdata[0] - 48) * 10 + startdata[1] - 48;
								tt1.tm_mon = (startdata[3] - 48) * 10 + startdata[4] - 48 - 1;
								tt1.tm_year = (startdata[6] - 48) * 1000 + (startdata[7] - 48) * 100 + (startdata[8] - 48) * 10 + startdata[9] - 48 - 1900;
								time_t tt2 = mktime(&tt1);
								ifstream in(table_name + ".dat", ios::binary);
								// Создаем вектор с соответствующим размером (запрашиваем его через
								// выставление указателя на конец файла)
								vector<unsigned char> v(in.seekg(0, ios::end).tellg());
								// Возвращаем указатель на место
								in.seekg(0, ios::beg);
								// Читаем файл в вектор
								in.read((char*)v.data(), v.size());
								int params = tables[table_name];
								int dl_zapis = 8 + params * 4;
								int zapis = v.size() / dl_zapis;
								if (zapis > 0) {
									time_t* tt0 = (time_t*)&v[0];
									time_t* ttn = (time_t*)&v[(zapis - 1) * dl_zapis];
									if (tt2 > * ttn){
										v.clear();
										return 10;
									}
									else {
										int startpos = 0;
										if (tt2 > * tt0) {
											int pos1 = 0;
											int posn = zapis - 1;
											while (pos1 + 1 < posn) {
												int pos3 = (pos1 + posn) / 2;
												time_t* tt3 = (time_t*)&v[pos3 * dl_zapis];
												if (tt2 > * tt3) {
													pos1 = pos3;
												}
												else {
													posn = pos3;
												}
											}
											startpos = posn;
										}
										char mm[1024];
										memcpy(mm, v.data() + startpos*dl_zapis, 1024);
										v.clear();
										char mm1[4];
										mm1[0] = 0;
										mm1[1] = 0;
										mm1[2] = params;
										int ter = 1024 / dl_zapis;
										if (startpos + ter > zapis) {
											ter = zapis - startpos;
										}
										mm1[3] = ter;
										send(d_sock, mm1, sizeof(mm1), NULL);
										Sleep(10);
										send(d_sock, mm, sizeof(mm), NULL);
										//cout << asctime(localtime(tt0)) << endl;
										//cout << asctime(localtime(ttn)) << endl;
										//cout << params << " " << dl_zapis << " " << zapis << endl;




										//v.clear();
										return 0;
									}
								}
								else {
									v.clear();
									return 9;
								}								
							}
							catch (...)
							{
								//cout << "error " << endl;
								return 8;
							}
						}
						else {
							///cout << "error 5" << endl;
							return 5;
						}
					}
					else {
						///cout << "error 6" << endl;
						return 6;
					}
				}
			}
		}
	}
	catch (regex_error& e) {
		cout << "regex error" << endl;
		return 7;
	}
	return 0;
}

string str(char* a)
{
	int i;
	string s = "";
	int ii = 0;
	for (i = 0; i < DEFAULT_BUFFER_LENGTH; i++) {
		if(ii==0)
			s = s + a[i];
		if (a[i] == 0)
			ii = 1;
	}
	return s;
}

unsigned long SClient::CalculationThread()
{
	//d_sock = &c_sock;
	int er;
	char recvstr[DEFAULT_BUFFER_LENGTH];
	er = recv(c_sock, recvstr, DEFAULT_BUFFER_LENGTH, NULL);
	if (er > 0) {
		string stroka = str(recvstr);
		cout << stroka << endl;
		int res=-foo(stroka, c_sock);
		char* c = (char*)&res;

		char msg1[4];
		msg1[0] = *c;
		msg1[1] = *(c + 1);
		msg1[2] = *(c + 2);
		msg1[3] = *(c + 3);


		send(c_sock, msg1, sizeof(msg1), NULL);
		Sleep(10);


		return res;
	}
	else if (er == 0) {
		cout << "connection terminated\n";
		return 1;
	}
	else {
		cout << "connection error" << endl;
		return 2;
	}
	///int k = recv(c_sock, buffer, sizeof(buffer), NULL);
	///printf("%u\n", k);
	Sleep(30);
}

SServer::SServer()
{
}

SServer::~SServer()
{
}

void SServer::startServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &wData) == 0)
	{
		printf("WSA Startup succes\n");
	}
	SOCKADDR_IN addr;
	int addrl = sizeof(addr);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	cin.clear();
	addr.sin_family = AF_INET;
	SOCKADDR ssdf;
	this_s = socket(AF_INET, SOCK_STREAM, NULL);
	if (this_s == SOCKET_ERROR) {
		printf("Socket not created\n");
	}

	if (bind(this_s, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
		printf("Socket succed binded\n");
	}

	if (listen(this_s, SOMAXCONN) != SOCKET_ERROR)
	{
		printf("Start listenin at port%u\n", ntohs(addr.sin_port));
	}
	handle();
}

void SServer::closeServer() {
	closesocket(this_s);
	WSACleanup();
	cout << "Server was stoped. You can close app" << endl;
}

void SServer::handle() {
	while (true)
	{
		SOCKET acceptS;
		SOCKADDR_IN addr_c;
		int addrlen = sizeof(addr_c);
		char buffer[512];
		struct sockaddr_in from_struct;
		int froml = sizeof(from_struct);
		if ((acceptS = accept(this_s, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
			printf("send\n");
			printf("sended Client connected from 0  %u.%u.%u.%u:%u\n",
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b1,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b2,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b3,
				(unsigned char)addr_c.sin_addr.S_un.S_un_b.s_b4,
				ntohs(addr_c.sin_port));
			SClient* client = new SClient(acceptS, addr_c);

		}
		Sleep(200);
	}
}


int main()
{
	read_tables();
	SServer* server = new SServer();
	server->port = 3487;
	server->startServer();
	return 0;
}
