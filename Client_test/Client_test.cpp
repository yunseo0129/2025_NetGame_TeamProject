#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 9000
#define BUFFER_SIZE 512

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패.\n";
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패.\n";
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) { // connect()
        std::cerr << "서버 연결 실패.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "서버에 연결되었습니다. 메시지를 입력하세요 (종료: 'exit').\n";

    std::string message;
    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "보낼 메시지: ";
        std::getline(std::cin, message);

        if (message == "exit") {
            break;
        }

        send(clientSocket, message.c_str(), (int)message.length(), 0);

        int nRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (nRecv <= 0) {
            std::cout << "서버와의 연결이 끊겼습니다.\n";
            break;
        }

        buffer[nRecv] = '\0';
        std::cout << "서버로부터 에코: " << buffer << "\n";
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}