#include "CPlayLevel.h"
#include "KeyMgr.h"

//const char* SERVERIP = (char*)"192.168.72.31";
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
		m_pPlayer[i]->playerType = i;
		AddObject(m_pPlayer[i], OBJ_PLAYER); // 레벨에 등록
	}

	// === 맵 생성 ===
	AddObject(new CMap(330, 170), OBJ_MAP);  // 1단
	AddObject(new CMap(160, 300), OBJ_MAP);  // 2단
	AddObject(new CMap(510, 300), OBJ_MAP);
	AddObject(new CMap(70, 420), OBJ_MAP);  // 3단
	AddObject(new CMap(600, 420), OBJ_MAP);

	// === 아이템 박스 생성 ===
	for (int i = 0; i < 10; ++i) { 
		AddObject(new CItem(), OBJ_ITEMBOX);
	}
	
	// === 총알 생성 ===
	for (int i = 0; i < 100; ++i) {
		AddObject(new CBullet(), OBJ_BULLET);
	}
}

void CPlayLevel::update_camera()
{
	// 아직 ID를 못 받았거나, 플레이어가 생성되지 않았으면 리턴
	if (m_myPlayerID == -1) return;
	camId = m_myPlayerID;

	if (!m_pPlayer[camId]->pInfo.iLife)
	{
		switch (camId)
		{
		case 0:
			if (m_pPlayer[1]->pInfo.iLife)
				camId = 1;
			else
				camId = 2;
			break;
		case 1:
			if (m_pPlayer[2]->pInfo.iLife)
				camId = 2;
			else
				camId = 0;
			break;
		case 2:
			if (m_pPlayer[0]->pInfo.iLife)
				camId = 0;
			else
				camId = 1;
			break;
		default:
			break;
		}
	}
	int targetX = m_pPlayer[camId]->x;
	int targetY = m_pPlayer[camId]->y;

	cameraX = targetX - cameraWidth / 2 + (pWidth / 2);
	cameraY = targetY - cameraHeight / 2 + 100;
}

void CPlayLevel::Update()
{
	// 네트워크 패킷 처리
	SendData recvData;
	bool bPacketProcessed = false;

	EnterCriticalSection(&m_cs); 
	// if -> while 로 변경
	// 즉, 한 프레임에 해당 큐에 있는 데이터 중 가장 마지막 데이터만 처리
	while (!m_recvQueue.empty()) {
		recvData = m_recvQueue.front(); // 데이터를 꺼내서 복사
		m_recvQueue.pop();              // 큐에서 제거
		bPacketProcessed = true;        // 데이터가 있었음을 표시
	}
	LeaveCriticalSection(&m_cs);

	if (bPacketProcessed) {
		// 수신된 데이터로 플레이어 상태 업데이트
		for (int i = 0; i < 3; ++i) {
			if (m_pPlayer[i] != nullptr) {
				m_pPlayer[i]->pInfo = recvData.playerInfo[i];
			}
		}

		// 아이템 박스 상태 업데이트
		auto& itemList = GetGroupObject(OBJ_ITEMBOX);
		auto itemIter = itemList.begin();
		for (int i = 0; i < 10; ++i) {
			if (itemIter == itemList.end()) break;

			CItem* pItem = static_cast<CItem*>(*itemIter);
			if (pItem != nullptr) {
				pItem->iInfo = recvData.arrItemBoxs[i];
			}

			// 다음 아이템으로 이동
			++itemIter;
		}

		// 총알 상태 업데이트
		auto& bulletList = GetGroupObject(OBJ_BULLET);
		auto bulletIter = bulletList.begin();
		for (int i = 0; i < 10; ++i) {
			// 리스트 끝에 도달하면 중단
			if (bulletIter == bulletList.end()) break;

			CBullet* pBullet = static_cast<CBullet*>(*bulletIter);
			if (pBullet != nullptr) {
				pBullet->bInfo = recvData.arrBullets[i];
			}

			// 다음 총알로 이동
			++bulletIter;
		}
	}

	// 키 입력 처리
	ProcessInput(); 
	if (b_keyAct && m_sock != INVALID_SOCKET) {
		// 입력을 서버로 전송
		for (const auto& myAction : m_vecInputActions) {
			int retval = send(m_sock, (const char*)&myAction, sizeof(myAction), 0);
			if (retval == SOCKET_ERROR) {
				OutputDebugString(L"[CPlayLevel] : err - send()\n");
			}
		}
	}

	// 부모 클래스의 Update 호출 
	CLevel::Update();

	// 카메라 업데이트 
	update_camera();
}

void CPlayLevel::Draw(HDC mDC)
{
	// 배경 그리기 - 맵 하나만 사용
	SelectObject(g_BMPmDC, BMP_map1);
	BitBlt(mDC, 0, 0, 900, 650, g_BMPmDC, 900 - cameraX / 5, 650 - cameraY / 5, SRCCOPY);

	// 부모 클래스의 Draw 호출
	CLevel::Draw(mDC);

	// 종료 검사 및 게임 결과 텍스트 출력
	if (m_bGameStarted) {
		
		// 큰 폰트 생성
		HFONT hFont = CreateFont(60, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 
								 0, 0, 0, 0, L"Arial");

		// 생성한 폰트를 DC에 선택(Select), 이전 폰트 저장
		HFONT hOldFont = (HFONT)SelectObject(mDC, hFont);

		// 텍스트 배경을 투명하게 설정
		int nOldBkMode = SetBkMode(mDC, TRANSPARENT);

		// 종료 검사	및 텍스트 출력
		if (m_pPlayer[m_myPlayerID]->pInfo.iLife <= 0) {
			TCHAR EndText[100];
			wsprintf(EndText, L"Game Over");
			TextOut(mDC, 320, 150, EndText, lstrlen(EndText));
		}
		else {
			// 승리 판정 및 텍스트 출력
			bool bWin = false;
			switch (m_myPlayerID) {
			case 0:
				if (m_pPlayer[1]->pInfo.iLife <= 0 && m_pPlayer[2]->pInfo.iLife <= 0) {
					bWin = true;
				}
				break;
			case 1:
				if (m_pPlayer[0]->pInfo.iLife <= 0 && m_pPlayer[2]->pInfo.iLife <= 0) {
					bWin = true;
				}
				break;
			case 2:
				if (m_pPlayer[0]->pInfo.iLife <= 0 && m_pPlayer[1]->pInfo.iLife <= 0) {
					bWin = true;
				}
				break;
			}
			if (bWin) {
				TCHAR WinText[100];
				wsprintf(WinText, L"You Win!");
				TextOut(mDC, 320, 150, WinText, lstrlen(WinText));
			}
		}

		// 뒷정리 
		SelectObject(mDC, hOldFont);
		SetBkMode(mDC, nOldBkMode);		// 원래 폰트와 배경 모드로 되돌림
		DeleteObject(hFont);			// 다 쓴 폰트 객체 삭제
	}
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
	m_vecInputActions.clear();
	b_keyAct = false;			// 키 입력이 있었는지 여부
	Player_input myAction;		// 현재 키 이벤트를 저장할 임시 변수
	myAction.id = m_myPlayerID;

	// 키 다운 처리
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_LEFT;
		m_vecInputActions.push_back(myAction);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_RIGHT;
		m_vecInputActions.push_back(myAction);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_JUMP;
		m_vecInputActions.push_back(myAction);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_DOWNJUMP;
		m_vecInputActions.push_back(myAction);
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)) {
		b_keyAct = true;
		myAction.isDown = true;
		myAction.key = KEY_SHOOT;
		m_vecInputActions.push_back(myAction);
	}

	// 키 업 처리
	if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT)) {
		b_keyAct = true;
		myAction.isDown = false;
		myAction.key = KEY_LEFT;
		m_vecInputActions.push_back(myAction);
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)) {
		b_keyAct = true;
		myAction.isDown = false;
		myAction.key = KEY_RIGHT;
		m_vecInputActions.push_back(myAction);
	}
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

	// 접속 직후 서버로부터 "내 ID"를 먼저 받는다.
	int myID = -1;
	retval = recv(pThis->m_sock, (char*)&myID, sizeof(int), 0);
	if (retval == SOCKET_ERROR || retval == 0) {
		OutputDebugString(L"ID 수신 실패\n");
		pThis->m_bIsRunning = false;
	} else {
		pThis->m_myPlayerID = myID; // 내 ID 저장

		// 디버그용 출력
		wchar_t buf[100];
		wsprintf(buf, L"내 ID 할당됨: %d\n", myID);
		OutputDebugString(buf);
	}

	while (pThis->m_bIsRunning) {
		// 서버로부터 데이터 수신
		// 정보를 받으면 각 객체(플레이어)들은 그 정보를 바탕으로 자신의 상태를 각각 업데이트
		SendData recvData;
		retval = recv(pThis->m_sock, (char*)&recvData, sizeof(recvData), 0);
		if (retval == SOCKET_ERROR || retval == 0) {
			OutputDebugString(L"[ClientThread] : err - recv() or connection closed\n");
			pThis->m_bIsRunning = false; // 루프 탈출
			break;
		} 

		// 게임 시작 검사 (플레이어 세명 연결 성공)
		if (!pThis->m_bGameStarted)
		{
			int connectedCount = 0;
			for (int i = 0; i < 3; ++i) {
				if (recvData.playerInfo[i].isConnected == true) {
					connectedCount++;
				}
			}
			if (connectedCount == 3) {
				pThis->m_bGameStarted = true;
			}
		}

		if (recvData.isChanged)
		{
			// 데이터 수신 성공 - 큐에 저장
			EnterCriticalSection(&pThis->m_cs);
			OutputDebugString(L"[ClientThread] : 데이터 수신 성공 - 큐에 삽입\n");
			pThis->m_recvQueue.push(recvData); 
			LeaveCriticalSection(&pThis->m_cs);
		}
	}

	// 스레드 종료
	OutputDebugString(L"[ClientThread] : 종료\n");
	return 0;
}