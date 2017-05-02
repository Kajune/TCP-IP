#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
	WSADATA data;
	int error = WSAStartup(MAKEWORD(2, 0), &data);
	if (error) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		std::cerr << WSAGetLastError() << std::endl;
		return -1;
	}

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(12345);
	inet_pton(AF_INET, "10.200.12.86", &server.sin_addr);
	
	error = connect(sock, reinterpret_cast<const sockaddr*>(&server), sizeof(server));
	if (error) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	char buf[32];
	std::fill(std::begin(buf), std::end(buf), 0);
	int n = recv(sock, buf, sizeof(buf), 0);

	std::cout << n << ", " << buf << std::endl;

	WSACleanup();

	return 0;
}

