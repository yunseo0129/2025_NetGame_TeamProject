#include "Server.h"

DWORD WINAPI ProcessClient(LPVOID arg)
{
    // 인자(소켓, ID) 받기 (헤더의 ThreadParam 구조체 사용)
    ThreadParam* pArgs = (ThreadParam*)arg;
    SOCKET client_sock = pArgs->hClientSock;
    int my_id = pArgs->iPlayerID;
    delete pArgs; // 인자 전달 완료 후 메모리 해제

    // 클라이언트 정보 출력
    struct sockaddr_in clientaddr;
    int addrlen = sizeof(clientaddr);
    char addr[INET_ADDRSTRLEN];
    getpeername(client_sock, (struct sockaddr*)&clientaddr, &addrlen);
    inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
    printf("[Player %d] 클라이언트 접속: %s:%d\n", my_id, addr, ntohs(clientaddr.sin_port));

    int retval;
    PLAYER_ACTION clientPlay;

    while (true)
    {
        retval = recv(client_sock, (char*)&clientPlay, sizeof(clientPlay), 0);

        // 입력 수신 로그
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

    closesocket(client_sock);
    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 크리티컬 섹션 초기화
    InitializeCriticalSection(&g_cs_Inputs);
    InitializeCriticalSection(&g_World_CS);

    // 소켓 생성
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

    //listen()
    retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    HANDLE hThread;
    static int g_player_count = 0; // 접속한 플레이어 수 (ID 할당용)

    printf("[알림] 게임 서버가 시작되었습니다. (최대 %d명)\n", MAX_PLAYERS);
    printf("====================================================\n");

    while (1) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
            break;
        }

        // 3명까지만 접속 허용
        if (g_player_count < MAX_PLAYERS)
        {
            // 스레드에 소켓과 ID를 넘겨주기 위해 구조체 사용
            ThreadParam* pArgs = new ThreadParam;
            pArgs->hClientSock = client_sock;
            pArgs->iPlayerID = g_player_count++; // 0, 1, 2 순서로 ID 할당

            hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)pArgs, 0, NULL);
            if (hThread == NULL) {
                closesocket(client_sock);
                delete pArgs; // 스레드 생성 실패 시 메모리 해제
                g_player_count--; // 카운트 복구
            }
            else {
                CloseHandle(hThread); // 스레드 핸들 정리
            }
        }
        else
        {
            // 서버가 꽉 찼을 때
            printf("[알림] 서버가 가득 찼습니다. (Full)\n");
            closesocket(client_sock);
        }
    }

    // 소켓 닫기 및 윈속 종료
    closesocket(client_sock);
    closesocket(listen_sock);
    DeleteCriticalSection(&g_cs_Inputs);
    DeleteCriticalSection(&g_World_CS);
    WSACleanup();

    return 0;
}