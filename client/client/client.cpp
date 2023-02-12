// Client part for Server-Client chat. Developed by Mr_Dezz

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <cstring>
#include <string>
#include <nlohmann/json.hpp>
#include <filesystem>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

std::string ConfigName(const std::string& filename) 
{
	size_t lastdot = filename.find_last_of(".");
	if (lastdot == std::string::npos) 
		return filename + ".json";
	return filename.substr(0, lastdot) + ".json";
}

std::wstring ExePath() {

	TCHAR buffer[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
	return std::wstring(buffer).substr(0, pos)+std::filesystem::path::preferred_separator;
}

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "rus");
	//Key constants
	char SERVER_IP[20] = "127.0.0.1";
	short SERVER_PORT_NUM = 2323;				// Enter Listening port on Server side
	const short BUFF_SIZE = 1024;
	
	wstring pathB(ExePath());
	std::string current_path(pathB.begin(), pathB.end());

	using json = nlohmann::json;
	std::ifstream f(current_path + ConfigName(argv[0]), std::ifstream::in);

	if (f.is_open())
	{

	}
	else
	{
		f.close();
		std::cout << "create json\n";
		std::ofstream out(current_path + ConfigName(argv[0]), std::ofstream::out);
		json jdEmployees =
		{
			{"port",2323},
			{"ip","127.0.0.1"}
		};
		out << jdEmployees;
		out.close();
	}

	f.open(current_path + ConfigName(argv[0]), std::ifstream::in);
	json j; //create unitiialized json object
	f >> j; // initialize json object with what was read from file
	
	

	std::string ip=j.at("ip");
	SERVER_PORT_NUM = j.at("port");
	
	strncpy_s(SERVER_IP, ip.c_str(), sizeof(SERVER_IP));
	SERVER_IP[sizeof(SERVER_IP) - 1] = '\0';
	


	// Key variables for all program
	int erStat;										// For checking errors in sockets functions

	//IP in string format to numeric format for socket functions. Data is in "ip_to_num"
	in_addr ip_to_num;
	inet_pton(AF_INET, SERVER_IP, &ip_to_num);


	// WinSock initialization
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else
		cout << "winSock initialization is OK" << endl;

	// Socket initialization
	SOCKET ClientSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ClientSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
	}
	else
		cout << "Client socket initialization is OK" << endl;

	// Establishing a connection to Server
	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(SERVER_PORT_NUM);

	erStat = connect(ClientSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		cout << "Connection to Server is FAILED. Error # " << WSAGetLastError() << endl;
		closesocket(ClientSock);
		WSACleanup();
		return 1;
	}


	short packet_size = 0;												// The size of sending / receiving packet in bytes

		packet_size = send(ClientSock, argv[1], strlen(argv[1]), 0);


		if (packet_size == SOCKET_ERROR) {
			cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
			closesocket(ClientSock);
			WSACleanup();
			return 1;
		}


	closesocket(ClientSock);
	WSACleanup();

	return 0;
}