#include <iostream>
#include <WinSock2.h>

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

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));

	error = listen(sock, 5);
	if (error) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	sockaddr_in client;
	int length = sizeof(client);
	SOCKET sock1 = accept(sock, reinterpret_cast<sockaddr*>(&client), &length);

	std::string message = "Hello, world!";
	send(sock1, message.c_str(), message.length(), 0);

	closesocket(sock1);

	WSACleanup();

	return 0;
}

