#include "CPlayLevel.h"
#include "KeyMgr.h"

//const char* SERVERIP = (char*)"192.168.71.174";
const char* SERVERIP = (char*)"127.0.0.1";

CPlayLevel::CPlayLevel()
{
}


void CPlayLevel::Initialize()
{
	InitializeCriticalSection(&m_cs);

	m_bIsRunning = true; // 스레드 실행 플래그 설정

	// === 소켓 생성 및 서버 연결 ===
	m_hThread = CreateThread(NULL, 0, ClientThread, this, 0, NULL);
	if (m_hThread == NULL)
	{
		OutputDebugString(L"[CPlayLevel] : 스레드 생성 실패\n");
		m_bIsRunning = false;
	}

	// === 플레이어 생성 ===
	for (int i = 0; i < 3; ++i) {
		m_pPlayer[i] = new CPlayer();
		m_pPlayer[i]->x = 100 + i * 200; // 초기 위치
		m_pPlayer[i]->playerType = i;
		AddObject(m_pPlayer[i], (i == 0) ? OBJ_PLAYER1 : OBJ_PLAYER2); // 레벨에 등록
	}

	if (g_mapType == 0) {
		AddObject(new CMap(330, 170), OBJ_MAP);  // 1단
		AddObject(new CMap(160, 300), OBJ_MAP);  // 2단
		AddObject(new CMap(510, 300), OBJ_MAP);
		AddObject(new CMap(70, 420), OBJ_MAP);  // 3단
		AddObject(new CMap(600, 420), OBJ_MAP);
	} else { // mapType == 1
		AddObject(new CMap(160, 170), OBJ_MAP);  // 1단
		AddObject(new CMap(510, 170), OBJ_MAP);
		AddObject(new CMap(330, 300), OBJ_MAP);  // 2단
		AddObject(new CMap(70, 420), OBJ_MAP);  // 3단
		AddObject(new CMap(600, 420), OBJ_MAP);
	}
}


void CPlayLevel::update_camera()
{
	// 아직 ID를 못 받았거나, 플레이어가 생성되지 않았으면 리턴
	if (m_myPlayerID == -1) return;

	int targetX = m_pPlayer[m_myPlayerID]->x;
	int targetY = m_pPlayer[m_myPlayerID]->y;

	cameraX = targetX - cameraWidth / 2 + (pWidth / 2); 
	cameraY = targetY - cameraHeight / 2 + 100;         
}

void CPlayLevel::Update()
{
	// 네트워크 패킷 처리
	//SendData recvData;
	MovementData recvData;
	bool bPacketProcessed = false;

	EnterCriticalSection(&m_cs); 
	if (!m_recvQueue.empty()) {
		recvData = m_recvQueue.front();
		m_recvQueue.pop();
		bPacketProcessed = true;
	}
	LeaveCriticalSection(&m_cs);

	if (bPacketProcessed) {
		// 수신된 데이터로 플레이어 상태 업데이트
		for (int i = 0; i < 3; ++i) {
			if (m_pPlayer[i] != nullptr) {
				m_pPlayer[i]->pInfo.vPosition.x = recvData.players[i].x;
				m_pPlayer[i]->pInfo.vPosition.y = recvData.players[i].y;
			}
		}
	}

	// 키 입력 처리
	ProcessInput(); 
	if (b_keyAct && m_sock != INVALID_SOCKET) {
		// 입력을 서버로 전송
		int retval = send(m_sock, (const char*)&myAction, sizeof(myAction), 0);
		if (retval == SOCKET_ERROR) {
			OutputDebugString(L"[CPlayLevel] : err - send()\n");
		}
	}

	// === 부모 클래스의 Update 호출 ===
	CLevel::Update();

	// === 카메라 업데이트 ===
	update_camera();
}

void CPlayLevel::Draw(HDC mDC)
{
	// 1. 배경 그리기 
	// 맵 하나만 사용
	SelectObject(g_BMPmDC, BMP_map1);
	BitBlt(mDC, 0, 0, 900, 650, g_BMPmDC, 900 - cameraX / 5, 650 - cameraY / 5, SRCCOPY);

	// 2. 부모 클래스의 Draw 호출
	// (이 코드가 m_ObjList의 모든 객체(CMap, CItem)의 Draw를 호출)
	CLevel::Draw(mDC);
}

void CPlayLevel::Free()
{
	closesocket(m_sock);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_cs);

	CLevel::Free();
}

void CPlayLevel::ProcessInput()
{
	b_keyAct = false;						// 키 입력이 있었는지 여부
	// myAction = PLAYER_ACTION { false, };	// 보낼 액션 초기화

	// 키 다운 처리
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_LEFT;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_RIGHT;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_JUMP;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_DOWNJUMP;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_SHOOT;
	}

	// 키 업 처리
	if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT)) {
		b_keyAct = true;
		myAction.isDown = false;
		myAction.key = KEY_LEFT;
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)) {
		b_keyAct = true;
		myAction.isDown = false;
		myAction.key = KEY_RIGHT;
	}
	/*if (CKeyMgr::Get_Instance()->Key_Up(VK_UP)) {
		b_keyAct = true;
		myAction.key = KEY_JUMP;
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN)) {
		b_keyAct = true;
		myAction.isDown = false;
		myAction.key = KEY_DOWNJUMP;
	}*/
}

DWORD WINAPI CPlayLevel::ClientThread(LPVOID pArg)
{
	CPlayLevel* pThis = static_cast<CPlayLevel*>(pArg);
	int retval;

	// 소켓 생성
	pThis->m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (pThis->m_sock == INVALID_SOCKET) {
		OutputDebugString(L"[ClientThread] : err - socket()\n");
		return 1;
	}

	// 소켓 옵션 설정 NODELAY
	DWORD NODELAY = 1;
	setsockopt(pThis->m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&NODELAY, sizeof(DWORD));

	// connect()
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(pThis->m_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		OutputDebugString(L"[ClientThread] : err - connect()\n");
		closesocket(pThis->m_sock);
		pThis->m_sock = INVALID_SOCKET;
		return 1;
	}
	OutputDebugString(L"[ClientThread] : connect 성공\n");

	// =================================================
	// 접속 직후 서버로부터 "내 ID"를 먼저 받는다.
	// 서버는 접속되자마자 int형 데이터(0 또는 1)를 보내줘야 함
	// =================================================

	int myID = -1;
	retval = recv(pThis->m_sock, (char*)&myID, sizeof(int), 0);

	if (retval == SOCKET_ERROR || retval == 0) {
		OutputDebugString(L"ID 수신 실패\n");
		pThis->m_bIsRunning = false;
	} else {
		pThis->m_myPlayerID = myID; // 내 ID 저장!

		// 디버그용 출력
		wchar_t buf[100];
		wsprintf(buf, L"내 ID 할당됨: %d\n", myID);
		OutputDebugString(buf);
	}

	while (pThis->m_bIsRunning) {
		// 서버로부터 데이터 수신
		// 정보를 받으면 각 객체(플레이어)들은 그 정보를 바탕으로 자신의 상태를 각각 업데이트
		MovementData recvData;
		retval = recv(pThis->m_sock, (char*)&recvData, sizeof(recvData), 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			OutputDebugString(L"[ClientThread] : err - recv() or connection closed\n");
			pThis->m_bIsRunning = false; // 루프 탈출
			break;
		} 

		// 데이터 수신 성공 - 큐에 저장
		EnterCriticalSection(&pThis->m_cs);
		pThis->m_recvQueue.push(recvData); // 2. 큐에 데이터 삽입
		LeaveCriticalSection(&pThis->m_cs);
	}

	// 스레드 종료
	OutputDebugString(L"[ClientThread] : 종료\n");
	return 0;
}