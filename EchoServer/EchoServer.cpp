#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 9000
#define BUFFER_SIZE 512

unsigned __stdcall ClientThread(void* arg) {
    SOCKET clientSocket = (SOCKET)arg;
    char buffer[BUFFER_SIZE];
    int nRecv;

    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    getpeername(clientSocket, (SOCKADDR*)&clientAddr, &addrLen);
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

    std::cout << "[스레드 " << GetCurrentThreadId() << "] 클라이언트 접속: "
        << clientIP << ":" << ntohs(clientAddr.sin_port) << "\n";

    while (true) {
        nRecv = recv(clientSocket, buffer, BUFFER_SIZE, 0);

        if (nRecv <= 0) {
            if (nRecv == 0) {
                std::cout << "[스레드 " << GetCurrentThreadId() << "] 클라이언트 접속 종료.\n";
            }
            else {
                std::cerr << "[스레드 " << GetCurrentThreadId() << "] recv 오류: " << WSAGetLastError() << "\n";
            }
            break;
        }

        buffer[nRecv] = '\0';
        std::cout << "[스레드 " << GetCurrentThreadId() << "] 수신: " << buffer << "\n";

        send(clientSocket, buffer, nRecv, 0);
    }

    closesocket(clientSocket);
    return 0;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup 실패.\n";
        return 1;
    }


    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "소켓 생성 실패.\n";
        WSACleanup();
        return 1;
    }

    SOCKADDR_IN serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "바인드 실패.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, 5) == SOCKET_ERROR) {
        std::cerr << "리슨 실패.\n";
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "서버가 " << PORT << " 포트에서 대기 중입니다...\n";

    while (true) {
        SOCKET clientSocket = accept(listenSocket, NULL, NULL);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "accept 실패.\n";
            continue;
        }


        HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ClientThread, (void*)clientSocket, 0, NULL);
        if (hThread == 0) {
            std::cerr << "스레드 생성 실패.\n";
            closesocket(clientSocket);
        }
        else {
            CloseHandle(hThread);
        }
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}