#include "Server.h"

// 게임 레벨에 사용하는 변수
Player g_Players[3]; // 최대 3명 접속 가능
std::vector<Bullet> vecBullets; // 총알들
std::vector<ItemBox> vecItemBoxes; // 아이템 박스들
bool g_running = true;

DWORD WINAPI AcceptThread(LPVOID arg);

DWORD WINAPI ProcessClient(LPVOID arg);

int main(int argc, char* argv[])
{
    auto pre = std::chrono::high_resolution_clock::now();
    double timedelta = 0.0;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // 크리티컬 섹션 초기화
    InitializeCriticalSection(&g_cs_Inputs);
    InitializeCriticalSection(&g_World_CS);

    // AcceptThread 생성
    int retval;
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

    HANDLE hAcceptThread;
    hAcceptThread = CreateThread(NULL, 0, AcceptThread, LPVOID(listen_sock), 0, NULL);
    printf("[알림] 게임 서버가 시작되었습니다. (최대 %d명)\n", MAX_PLAYERS);
    printf("====================================================\n");

    bool isGameEnd = false;
    while (!isGameEnd)
    {
        // 게임 루프
        int i = 0;
        auto now = std::chrono::high_resolution_clock::now();
        timedelta += std::chrono::duration<double>(now - pre).count();
        pre = now;
        if (timedelta >= (1.0 / 30.0))
        {
            // 게임 루프
            if (GetAsyncKeyState(VK_ESCAPE))
            {
                break;
            }
            
            timedelta = 0.0;
        }
    }

    // 모든 생성한 소켓과 스레드를 종료 해야함
    // 근데 그 방법 좀 어려움
    // 각자 스레드가 특정 루프에 묶여있고
    // 특히 accept스레드는 accept에서 못빠져나옴

    DeleteCriticalSection(&g_cs_Inputs);
    DeleteCriticalSection(&g_World_CS);
    WSACleanup();

    return 0;
}

DWORD WINAPI AcceptThread(LPVOID arg)
{
    // 데이터 통신에 사용할 변수
    SOCKET listen_sock = (SOCKET)arg;
    SOCKET client_sock[4];
    struct sockaddr_in clientaddr;
    int addrlen;
    HANDLE hThread;
    static int g_player_count = 0; // 접속한 플레이어 수 (ID 할당용)

    while (g_running) {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock[g_player_count] = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
        if (client_sock[g_player_count] == INVALID_SOCKET) {
            if (g_running == false)
                break;
            err_quit("accept()");
            break;
        }

        // 3명까지만 접속 허용
        if (g_player_count < MAX_PLAYERS)
        {
            // 스레드에 소켓과 ID를 넘겨주기 위해 구조체 사용
            ThreadParam* pArgs = new ThreadParam;
            pArgs->hClientSock = client_sock[g_player_count];
            pArgs->iPlayerID = g_player_count++; // 0, 1, 2 순서로 ID 할당

            hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)pArgs, 0, NULL);
            if (hThread == NULL) {
                closesocket(client_sock[g_player_count - 1]);
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
            closesocket(client_sock[g_player_count]);
        }
    }

    return 0;
}

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

    return 0;
}