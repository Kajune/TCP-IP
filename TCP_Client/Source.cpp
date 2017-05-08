#include <iostream>
#include <string>
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

	std::string str;
	std::cin >> str;

	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(80);
	if (inet_pton(AF_INET, str.c_str(), &server.sin_addr) == 0) {
		addrinfo hints{ 0 }, *res;
		auto ret = getaddrinfo(str.c_str(), "http", &hints, &res);
		while (res) {
			server.sin_addr = ((sockaddr_in*)res->ai_addr)->sin_addr;
			if (connect(sock, reinterpret_cast<const sockaddr*>(&server), sizeof(server)) == 0) {
				break;
			}
			res = res->ai_next;
		}
	} else {
		error = connect(sock, reinterpret_cast<const sockaddr*>(&server), sizeof(server));
		if (error) {
			std::cerr << WSAGetLastError() << std::endl;
		}
	}

	std::string message = "GET / HTTP/1.0\r\n\r\n";
	send(sock, message.c_str(), message.size(), 0);

	char buf[32] = {};
	int n = 0;
	do{
		std::fill(std::begin(buf), std::end(buf), 0);
		n = recv(sock, buf, sizeof(buf) - 1, 0);
		buf[sizeof(buf) - 1] = '\0';
		std::cout << buf;
	}while (n > 0);

	closesocket(sock);

	WSACleanup();

	return 0;
}

