#include "Server.h"

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) printf("err - socket()\n");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) printf("err - bind()\n");

    //listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) printf("err - listen()\n");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    HANDLE hThread;

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            printf("err - accept()\n");
            break;
        }

        // 접속한 클라이언트 정보 출력
        char addr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("Client connected: %s:%d\n", addr, ntohs(clientaddr.sin_port));

        //// 쓰레드 생성
        //hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
        //if (hThread == NULL) { closesocket(client_sock); } else { CloseHandle(hThread); }

        // 접속한 클라이언트에게 액션 받기 테스트
        while(1)
        {
            PLAYER_ACTION clientPlay;
            retval = recv(client_sock, (char*)&clientPlay, sizeof(clientPlay), 0);
            if (retval == SOCKET_ERROR) {
                printf("err - recv()\n");
                break;
            }
            // 받은 액션 출력
            switch (clientPlay) {
            case ACTION_NONE:
                printf("Client Action: NONE\n");
                break;
            case ACTION_MOVE_R:
                printf("Client Action: MOVE RIGHT\n");
                break;
            case ACTION_MOVE_L:
                printf("Client Action: MOVE LEFT\n");
                break;
            case ACTION_JUMP_UP:
                printf("Client Action: JUMP UP\n");
                break;
            case ACTION_JUMP_DOWN:
                printf("Client Action: JUMP DOWN\n");
                break;
            case ACTION_SHOOT:
                printf("Client Action: FIRE\n");
                break;
            default:
                printf("Client Action: UNKNOWN\n");
                break;
            }
        }
    }

    // 소켓 닫기 및 윈속 종료
    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();

    return 0;
}