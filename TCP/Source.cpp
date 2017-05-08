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

	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;

	int yes = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
	bind(sock, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));

	error = listen(sock, 5);
	if (error) {
		std::cerr << WSAGetLastError() << std::endl;
	}

	int count = 0;
	while (true) {
		sockaddr_in client;
		int length = sizeof(client);
		SOCKET sock1 = accept(sock, reinterpret_cast<sockaddr*>(&client), &length);

		char inbuf[2048] = {};
//		recv(sock1, inbuf, sizeof(inbuf), 0);
//		std::cout << inbuf << std::endl;

		std::string message = "Hello, world!";
		message += std::to_string(count + 1) + "l–Ú";
		send(sock1, message.c_str(), message.length(), 0);

		char addr[16];
		inet_ntop(AF_INET, &client.sin_addr, addr, sizeof(addr));
		std::cout << "accepted connection from " << addr
			<< ", port = " << ntohs(client.sin_port) << std::endl;

		closesocket(sock1);
		count++;
	}

	closesocket(sock);

	WSACleanup();

	return 0;
}

