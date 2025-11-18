#include "Server.h"

// 게임 레벨에 사용하는 변수
Player Players[3]; // 최대 3명 접속 가능
std::vector<Bullet> vecBullets; // 총알들
std::vector<ItemBox> vecItemBoxes; // 아이템 박스들
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
    //            // === 이동 로직 구현 ===
    //            int id = next.iPlayerNum;
    //            PLAYER_ACTION act = next.eAct;

    //            // 일단 가속도와 이동 상태를 초기화
    //            Players[id].acceleration = 0.0f;
    //            Players[id].isMoving = false;

    //            // 왼쪽 입력 시
    //            if (act.left) {
    //                Players[id].acceleration = -ACCELERATION; // 왼쪽 가속도 적용
    //                Players[id].isMoving = true;              // 이동 중 플래그 On
    //            }
    //            // 오른쪽 입력 시
    //            if (act.right) {
    //                Players[id].acceleration = ACCELERATION;  // 오른쪽 가속도 적용
    //                Players[id].isMoving = true;              // 이동 중 플래그 On
    //            }
    //            if (act.left && act.right) {
    //                Players[id].acceleration = 0.0f;          // 양쪽 다 누르면 가속도 0
				//}

    //            // 점프 입력 시 (땅에 있을 때만)
    //            //if (act.space) {
    //            //    if (!Players[id].jumping && !Players[id].falling) {
    //            //        Players[id].jumping = true;
    //            //        Players[id].jumpTime = 0.f;
    //            //        Players[id].jstartY = Players[id].info.vPosition.y; // 현재 Y위치 저장
    //            //        // Players[id].jumpCount = 0; // 필요하다면 초기화
    //            //    }
    //            //}

    //            //if (act.left)  Players[id].move(-1, 0.f);
    //            //if (act.right) Players[id].move(1, 0.f);
    //            //if (act.up)    Players[id].move(0.f, -1); // (Y좌표계에 따라 +)
    //            //if (act.down)  Players[id].move(0.f, 1); // (Y좌표계에 따라 -)

    //            // 1. 항상 마찰력 적용.
    //            if (!Players[id].jumping && !Players[id].falling)
    //            {
    //                if (Players[id].speed > 0.0f)
    //                {
    //                    Players[id].speed = max(0.0f, Players[id].speed - FRICTION);
    //                }
    //                else if (Players[id].speed < 0.0f)
    //                {
    //                    Players[id].speed = min(0.0f, Players[id].speed + FRICTION);
    //                }
    //            }

    //            // 2. 가속도 적용
    //            if (Players[id].isMoving) {
    //                Players[id].speed += Players[id].acceleration;
    //            }

    //            // 3. 속도 제한
    //            Players[id].speed = max(-MAX_SPEED, min(Players[id].speed, MAX_SPEED));

    //            // 4. 최종 속도를 위치에 적용
    //            Players[id].move(Players[id].speed, 0.f);

    //            // jumping or falling
    //            if (Players[id].jumping == TRUE) {
    //                Players[id].jumpHeight = (Players[id].jumpTime * Players[id].jumpTime - Players[id].jumpPower * Players[id].jumpTime) * 4.f;
    //                Players[id].jumpTime += 0.2f;
    //                Players[id].move(0.f, (Players[id].jstartY + (int)Players[id].jumpHeight) - Players[id].info.vPosition.y);
    //            }
    //            else if(Players[id].falling == TRUE) { 
    //                Players[id].downHeight = (Players[id].downTime * (Players[id].downTime / 2)) * 4.f;
    //                Players[id].downTime += 0.2f;
    //                Players[id].move(0.f, (Players[id].fstartY + (int)Players[id].downHeight) - Players[id].info.vPosition.y);
				//}

				//// 다운카운트 -> 0이면 증가 / 0이 아닐 때 증가 시작 / N 도달 시 0으로 초기화
    //            if (Players[id].downCount > 0) {
				//	Players[id].downCount++;
    //                if (Players[id].downCount > 30) {
    //                    Players[id].downCount = 0;
				//	}
    //            }

            }
            // === 2. 월드 상태 업데이트 ===
            UpdatePlayer();
            
            // === 충돌 처리 ===
            Collision();

            // === 3. 모든 클라이언트에게 상태 브로드캐스트 ===
			SendData dataToSend;
            //MovementData dataToSend;
            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                dataToSend.playerInfo[i].isConnected = Players[i].info.isConnected;
				dataToSend.playerInfo[i].looking = Players[i].iLooking;
                dataToSend.playerInfo[i].vPosition.x = Players[i].info.vPosition.x;
                dataToSend.playerInfo[i].vPosition.y = Players[i].info.vPosition.y;

				dataToSend.arrItemBoxs = {};
                dataToSend.arrItemBoxs[0].exist = TRUE;
            }

            for (int i = 0; i < MAX_PLAYERS; i++)
            {
                if (Players[i].info.isConnected)
                {
                    int retval = send(Players[i].socket, (char*)&dataToSend, sizeof(dataToSend), 0);
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

			int pX = 100 + (g_player_count * 200);
            Players[g_player_count].move(pX, 70);

			// 플레이어 이동 및 점프 상태 초기화
            /*Players[g_player_count].acceleration = 0.0f;
			Players[g_player_count].speed = 0.0f;
			Players[g_player_count].isMoving = false;*/

            // 뭐로 초기화해야하는지 모르겠음
			//Players[g_player_count].jstartY = ?
            //Players[g_player_count].fstartY = ?
            //Players[g_player_count].downTime = ?
            //Players[g_player_count].downHeight = ?

			/*Players[g_player_count].downCount = 0;
            Players[g_player_count].falling = 0;
			Players[g_player_count].jumping = false;
			Players[g_player_count].jumpCount = 0;
			Players[g_player_count].jumpPower = 12.f;*/

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
    // PLAYER_ACTION clientPlay;
	// PLAYER_ACTION nullPlay = { false, false, false, false, false };

    //Action act;
    //act.iPlayerNum = my_id;
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
        //act.eAct = clientPlay;
        ActionQue.push(act);

        // 데이터 출력 테스트
       /* printf("[Player %d] Action - L:%d R:%d U:%d D:%d S:%d\n", my_id,
			   clientPlay.left, clientPlay.right, clientPlay.up, clientPlay.down, clientPlay.space);*/
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

    // 3. 플레이어 vs 아이템 박스
}

void Shooting(int id)
{
    for (int j = 0; j < Players[id].Act.space; ++j)
    {
        // 스페이스바 누른 만큼 총알 생성
    }
    Players[id].Act.space = 0;
}

void UpdatePlayer()
{
    for (int i = 0; i < MAX_PLAYERS; ++i)
    {
        if (Players[i].info.isConnected)
        {
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
            
            // 실제 이동
            MovePlayer(i);
        }
    }
}

void MovePlayer(int id)
{
    Players[id].move(Players[id].fAcc / 6.f, Players[id].fGravity);
}
