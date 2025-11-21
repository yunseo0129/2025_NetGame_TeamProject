#include "Server.h"

// 게임 레벨에 사용하는 변수
Player Players[3]; // 최대 3명 접속 가능
std::array<BulletInfo,100> arrBullets; // 총알들
std::array<ItemBoxInfo,10> arrItemBoxes; // 아이템 박스들
std::queue<Player_input> ActionQue;
RECT block[5];

bool g_running = true;
double timedelta = 0.0;

DWORD WINAPI AcceptThread(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

bool Initializer();
void Collision();
void Shooting(int id);
void UpdatePlayer();
void MovePlayer(int id);

void UpdateBullets();
void UpdateItemBoxes();

const float MAX_SPEED = 5.0f;      // 최대 속도
const float ACCELERATION = 0.2f;   // 가속도
const float FRICTION = 0.02f;      // 마찰력(감속)

int main(int argc, char* argv[])
{
    auto pre = std::chrono::high_resolution_clock::now();

    if (Initializer())
    {
        printf("초기화 오류");
        return 0;
    }

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
        // 메인스레드가 60fps를 고정으로 갖게하는 코드임
        int i = 0;
        auto now = std::chrono::high_resolution_clock::now();
        timedelta += std::chrono::duration<double>(now - pre).count();
        pre = now;

        if (timedelta >= (1.0 / 60.0))
        {
            // === 1. 입력 처리 (큐에 쌓인 모든 액션 처리) ===
            // (주의: 락을 걸지 않았으므로, 100% 스레드 세이프하지는 않으나 
            //  간단한 이동 구현을 위해 임시로 사용)
            while (!ActionQue.empty())
            {
                Player_input next = ActionQue.front();
                ActionQue.pop();

                // 키입력 처리
                switch (next.key)
                {
                case KEY_LEFT:
                    Players[next.id].Act.left = next.isDown;
                    break;
                case KEY_RIGHT:
                    Players[next.id].Act.right = next.isDown;
                    break;
                case KEY_JUMP:
                    Players[next.id].Act.up = next.isDown;
                    break;
                case KEY_DOWNJUMP:
                    Players[next.id].Act.down = 20;
                    break;
                case KEY_SHOOT:
                    Players[next.id].Act.space += 1;
                    break;
                default:
                    break;
                }
				Players[next.id].iLooking = (Players[next.id].Act.left) ? 0 : (Players[next.id].Act.right) ? 1 : Players[next.id].iLooking;
				Players[next.id].info.looking = Players[next.id].iLooking;
            }
            // === 2. 월드 상태 업데이트 ===
            UpdatePlayer();
            UpdateItemBoxes();
			UpdateBullets();

            // === 충돌 처리 ===
            Collision();

            // === 3. 모든 클라이언트에게 상태 브로드캐스트 ===
			SendData dataToSend;
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
				dataToSend.playerInfo[i] = Players[i].info;

				dataToSend.arrItemBoxs = arrItemBoxes;
                dataToSend.arrBullets = arrBullets;
                dataToSend.isChanged = true;
            }

            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if (Players[i].info.isConnected)
                {
                    int retval = send(Players[i].socket, (char*)&dataToSend, sizeof(dataToSend), 0);
                    if (dataToSend.arrBullets[0].exist)
                        int k = 0;
                    if (retval == SOCKET_ERROR) {
						err_quit("send()");
                        closesocket(Players[i].socket);
                    }
                }
            }

            // === 4. 루프 탈출 ===
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
            // 플레이어 초기화
            Players[g_player_count].socket = client_sock[g_player_count];
            Players[g_player_count].info.isConnected = true;
            Players[g_player_count].info.iLife = 3;
            Players[g_player_count].info.eItemType = ITEM_PISTOL;
            Players[g_player_count].info.eState = STATE_IDLE;
            Players[g_player_count].info.iBullet = 20;

			int pX = 100 + (g_player_count * 200);
            Players[g_player_count].move(pX, 70);

            // 접속한 클라이언트에게 ID 부여
            int idToSend = g_player_count; // 현재 할당할 ID
            int retval = send(client_sock[g_player_count], (char*)&idToSend, sizeof(int), 0);

            if (retval == SOCKET_ERROR) {
                printf("[오류] 클라이언트에게 ID 전송 실패\n");
                closesocket(client_sock[g_player_count]);
                Players[g_player_count].info.isConnected = false; // 초기화 취소
                continue; // 스레드 생성하지 않고 다음 접속 대기
            }
            printf("[접속] ID: %d번 플레이어 접속 성공\n", idToSend);

            
            // 스레드에 소켓과 ID를 넘겨주기 위해 구조체 사용
            ThreadParam* pArgs = new ThreadParam;
            pArgs->hClientSock = client_sock[g_player_count];
            pArgs->iPlayerID = g_player_count++; // 0, 1, 2 순서로 ID 할당

            hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)pArgs, 0, NULL);
            if (hThread == NULL) {
                closesocket(client_sock[g_player_count - 1]);
                delete pArgs; // 스레드 생성 실패 시 메모리 해제
                g_player_count--; // 카운트 복구
                Players[g_player_count].info.isConnected = false;
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
    Player_input act;

    while (true)
    {
        retval = recv(client_sock, (char*)&act, sizeof(act), 0);

        // === 접속 종료 처리 ===
        if (retval == SOCKET_ERROR || retval == 0)
        {
            printf("[Player %d] 클라이언트 접속 종료.\n", my_id);
            Players[my_id].info.isConnected = false; // 플래그 설정
            closesocket(client_sock);
            break; // 스레드 종료
        }

        // === 데이터 수신 성공: 큐에 삽입 ===
        ActionQue.push(act);

    }

    return 0;
}

bool Initializer()
{
    // 맵 충돌체 위치 초기화
    float blockx = 200.f;
    float blocky = 30.f;

    block[0].left = 330;
    block[0].right = 330 + blockx;
    block[0].top = 170;
    block[0].bottom = 170 + blocky;

    block[1].left = 160;
    block[1].right = 160 + blockx;
    block[1].top = 300;
    block[1].bottom = 300 + blocky;

    block[2].left = 510;
    block[2].right = 510 + blockx;
    block[2].top = 300;
    block[2].bottom = 300 + blocky;

    block[3].left = 70;
    block[3].right = 70 + blockx;
    block[3].top = 420;
    block[3].bottom = 420 + blocky;

    block[4].left = 600;
    block[4].right = 600 + blockx;
    block[4].top = 420;
    block[4].bottom = 420 + blocky;

    // 플레이어 충돌체 위치 초기화
    for (int i = 0; i < 3; ++i)
    {
        Players[i].colBox.left = 10.f;
        Players[i].colBox.right = 35.f;
        Players[i].colBox.top = 0.f;
        Players[i].colBox.bottom = 67.f;
    }
   
    return false;
}

void Collision()
{
    // 1. 플레이어 vs 맵
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Players[i].info.isConnected)
        {
            bool isCollided = false;

            if (Players[i].Act.down > 0)
            {
                Players[i].Act.down -= 1;
            }
            else
            {
                RECT Pbox = Players[i].colBox;
                for (int j = 0; j < 5; ++j)
                {
                    RECT Mbox = block[j];
                    if (Pbox.right >= Mbox.left && Pbox.left <= Mbox.right)
                    {
                        if (Pbox.bottom >= Mbox.top && Pbox.bottom <= Mbox.bottom)
                        {
                            if (Players[i].fGravity < 0.f) 
								continue; // 점프 중일 때는 무시

                            isCollided = false;
							// printf("플레이어 %d가 블록 %d에 착지함. 블록의 left:%d, right:%d\n", i, j, Mbox.left, Mbox.right);

                            Players[i].move(0.f, -(Pbox.bottom - Mbox.top));
                            Players[i].isOnBlock = true;
                            Players[i].fGravity = 0.f;
                            Players[i].iJump = 0;
                        }
                    }
                }
            }
			Players[i].isOnBlock = isCollided;
        }
    }
    // 2. 플레이어 vs 총알
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        RECT Pbox = Players[i].colBox;
        for (int j = 0; j < arrBullets.size(); ++j)
        {
			if (arrBullets[j].exist && arrBullets[j].id != i) // 자신의 총알은 무시
            {
                RECT Bbox = arrBullets[j].colBox;
                if (Pbox.right >= Bbox.left && Pbox.left <= Bbox.right)
                {
                    if (Pbox.bottom >= Bbox.top && Pbox.top <= Bbox.bottom)
                    {
                        Players[i].fAcc += (arrBullets[j].eType == ITEM_PISTOL) ? ((arrBullets[j].vPosition.x >= arrBullets[j].vStarting.x) ? 5.f : -5.f) : ((arrBullets[j].vPosition.x >= arrBullets[j].vStarting.x) ? 10.f : -10.f);
                        arrBullets[j].exist = FALSE;
                    }
                }
            }
        }
    }
    // 3. 플레이어 vs 아이템 박스
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        if (Players[i].info.isConnected)
        {
            RECT Pbox = Players[i].colBox;
            for (ItemBoxInfo& itm : arrItemBoxes)
            {
                if (itm.exist)
                {
                    RECT Bbox = itm.colBox;
                    if (Pbox.right >= Bbox.left && Pbox.left <= Bbox.right)
                    {
                        if (Pbox.bottom >= Bbox.top && Pbox.top <= Bbox.bottom)
                        {
                            ITEMTYPE type = ITEMTYPE((rand() % 2) + 1);
                            Players[i].info.eItemType = type;
                            if (type == ITEM_PISTOL)
                                Players[i].info.iBullet = 20;
                            else
                                Players[i].info.iBullet = 10;

                            itm.exist = FALSE;
                        }
                    }
                }
            }
        }
    }
}

void Shooting(int id)
{
    for (int j = 0; j < Players[id].Act.space; ++j)
    {
        if (Players[id].info.iBullet <= 0)
			break;

        Players[id].info.iBullet -= 1;
        Players[id].Act.space -= 1;
        if (Players[id].info.iBullet >= 0)
        {
            for (int i = 0; i < arrBullets.size(); ++i)
            {
                if (!arrBullets[i].exist)
                {
                    arrBullets[i].exist = TRUE;
                    arrBullets[i].id = id;
                    arrBullets[i].eType = Players[id].info.eItemType;
                    arrBullets[i].vStarting.x = Players[id].info.vPosition.x + ((Players[id].iLooking == 1) ? 30.f : -10.f);
                    arrBullets[i].vStarting.y = Players[id].info.vPosition.y + 30.f;

                    // 왼쪽을 볼 때
                    arrBullets[i].vPosition.x = arrBullets[i].vStarting.x - 1.0f;       // 시작점보다 1픽셀 왼쪽
                    // 오른쪽을 볼 때
                    if (Players[id].iLooking == 1)
                        arrBullets[i].vPosition.x = arrBullets[i].vStarting.x + 1.0f;   // 시작점보다 1픽셀 오른쪽
                    arrBullets[i].vPosition.y = arrBullets[i].vStarting.y;              // Y좌표는 동일

                    // 총알 충돌박스 초기화
                    arrBullets[i].colBox.left = (int)arrBullets[i].vPosition.x;
                    arrBullets[i].colBox.right = (int)arrBullets[i].vPosition.x + 5;
                    arrBullets[i].colBox.top = (int)arrBullets[i].vPosition.y;
                    arrBullets[i].colBox.bottom = (int)arrBullets[i].vPosition.y + 5;
                    break; // 하나 생성했으면 다음 총알 생성 대기
                }
            }
        }
    }
}

void UpdatePlayer()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        if (Players[i].info.isConnected)
        {
            // 낙사 처리
            if (Players[i].info.vPosition.y > 1100)
            {
                Players[i].info.iLife--; // 생명력 감소
                if (Players[i].info.iLife > 0)
                {
                    // 초기 위치로 리스폰
                    int pX = 100 + (i * 200);
                    Players[i].info.vPosition.x = pX;
                    Players[i].info.vPosition.y = 70;
                    Players[i].fGravity = 0.f; // 중력 초기화
                    Players[i].fAcc = 0.f;     // 가속도 초기화

					Players[i].colBox.left = Players[i].info.vPosition.x + 10.f;  // 충돌 박스 초기화
                    Players[i].colBox.right = Players[i].info.vPosition.x + 35.f;
                    Players[i].colBox.top = Players[i].info.vPosition.y;
                    Players[i].colBox.bottom = Players[i].info.vPosition.y + 67.f;
                }
                else
                {
                    // 게임 오버 처리 (연결 끊김으로 처리)
                    Players[i].info.isConnected = false;
                    closesocket(Players[i].socket);
                }
                continue; // 아래 로직을 실행하지 않고 다음 플레이어로 넘어감
            }

            // 가속도 감소
            if (Players[i].fAcc > 0.f)
            {
                Players[i].fAcc -= 1.f;
                if (Players[i].fAcc < 0.f)
                    Players[i].fAcc = 0.f;
            }
            else if (Players[i].fAcc < 0.f)
            {
                Players[i].fAcc += 1.f;
                if (Players[i].fAcc > 0.f)
                    Players[i].fAcc = 0.f;
            }
            // 중력 가속도 증가
            if (!Players[i].isOnBlock)
            {
                Players[i].fGravity += 9.81f * 2.f * timedelta;
            }

            // 점프
            if (Players[i].Act.up)
            {
                if (Players[i].iJump < 2)
                {
                    Players[i].iJump += 1;
                    Players[i].fGravity = -10.f;
                }
                
                Players[i].Act.up = false;
            }

            // 이동 가속도 연산
            if (Players[i].Act.left)
            {
                if (Players[i].fAcc > -50.f)
                {
                    Players[i].fAcc -= 2.f;
                    if (Players[i].fAcc < -50.f)
                        Players[i].fAcc = -50.f;
                }
            }
            if (Players[i].Act.right)
            {
                if (Players[i].fAcc < 50.f)
                {
                    Players[i].fAcc += 2.f;
                    if (Players[i].fAcc > 50.f)
                        Players[i].fAcc = 50.f;
                }
            }
            if (Players[i].Act.space > 0)
            {
                Shooting(i);
            }
        }
        // 실제 이동
        MovePlayer(i);
    }
}

void MovePlayer(int id)
{
    Players[id].move(Players[id].fAcc / 6.f, Players[id].fGravity);
}

void UpdateBullets()
{
    // 총알 위치 업데이트
    for (int i = 0; i < arrBullets.size(); ++i)
    {
        if (arrBullets[i].exist)
        {
            // 총알 이동 로직 구현
            // 예: arrBullets[i].vPosition.x += 속도 * 방향 * timedelta;
			// 총알이 사거리 초과 또는 충돌 시 삭제 처리
			arrBullets[i].vPosition.x += (arrBullets[i].eType == ITEM_PISTOL ? 10.f : 5.f) * ((arrBullets[i].vPosition.x >= arrBullets[i].vStarting.x) ? 1.f : -1.f);
            if (abs(arrBullets[i].vPosition.x - arrBullets[i].vStarting.x) > 800.0f) 
            {
				arrBullets[i].exist = FALSE;
            }
        }
    }
}

void UpdateItemBoxes()
{
    // 아이템 박스 생성 및 업데이트
    for (int i = 0; i < 5; ++i)
    {
        // 아이템 박스가 존재하지 않으면 랜덤하게 생성
        if (!arrItemBoxes[i].exist)
        {
            // 1% 확률로 아이템 박스 생성
            if ((rand() % 100) < 1)
            {
                arrItemBoxes[i].exist = TRUE;
                // 발판 중앙에 위치
                arrItemBoxes[i].vPosition.x = (block[i].left + block[i].right) / 2.0f;
                // 낙하 시작 지점
                arrItemBoxes[i].vPosition.y = - 300.f;
            }
        }
        else // 아이템 박스가 존재하면 아래로 떨어뜨림
        {
            // 목표 y 위치 블록 위 
            float targetY = block[i].top - 42.f;

            // 아이템 박스가 목표 위치보다 위에 있으면 중력 적용
            if (arrItemBoxes[i].vPosition.y < targetY)
            {
                arrItemBoxes[i].vPosition.y += 9.81f * 100.f * timedelta; // 중력 적용

                // 목표 위치를 넘어가면 위치 고정
                if (arrItemBoxes[i].vPosition.y > targetY)
                {
                    arrItemBoxes[i].vPosition.y = targetY;
                }
            }
        }

        // 모든 아이템 박스의 충돌 박스 위치 업데이트
        arrItemBoxes[i].colBox.left = arrItemBoxes[i].vPosition.x;
        arrItemBoxes[i].colBox.top = arrItemBoxes[i].vPosition.y;
        arrItemBoxes[i].colBox.right = arrItemBoxes[i].colBox.left + 40;
        arrItemBoxes[i].colBox.bottom = arrItemBoxes[i].colBox.top + 40;
    }
}