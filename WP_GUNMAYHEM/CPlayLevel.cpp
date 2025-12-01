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

	// 큰 폰트 초기화
	m_hFont = CreateFont(60, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Arial");

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
		m_pPlayer[i]->playerType = i; // 플레이어 타입 설정
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
	if (m_myPlayerID == -1) return;

	// 살아있는 플레이어들의 좌표 평균 구하기
	int playerCenterX = 0;
	int playerCenterY = 0;
	int aliveCount = 0;

	for (int i = 0; i < 3; ++i) {
		if (m_pPlayer[i] && m_pPlayer[i]->pInfo.isConnected && m_pPlayer[i]->pInfo.iLife > 0) {
			playerCenterX += m_pPlayer[i]->Get_X();
			playerCenterY += m_pPlayer[i]->Get_Y();
			aliveCount++;
		}
	}

	if (aliveCount == 0) return; // 예외 처리 (0으로 나누기 방지)

	// 중심점 계산
	playerCenterX /= aliveCount;
	playerCenterY /= aliveCount;

	// 카메라 위치 설정 (화면 중앙에 중심점이 오도록)
	// targetX - (화면가로/2) + (플레이어가로/2)
	cameraX = playerCenterX - cameraWidth / 2 + (pWidth / 2);
	cameraY = playerCenterY - cameraHeight / 2 + 100;

	// ---------------------------------------------------------
	// 맵 경계 기준 Clamping 및 Centering
	const int mapMinX = 70;
	const int mapMaxX = 800;
	const int mapMinY = 170;
	const int mapMaxY = 450;

	const int offset = 400; // 여유 공간
	cameraX = max(mapMinX - offset, min(cameraX, mapMaxX + offset - cameraWidth));
	cameraY = max(mapMinY - offset, min(cameraY, mapMaxY + offset - cameraHeight));
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

		// 총알 상태 업데이트
		const std::list<CObject*>& bulletList = GetGroupObject(OBJ_BULLET);
		int i = 0;
		for (CObject* pObj : bulletList) {
			if (i >= 100) break;

			CBullet* pBullet = static_cast<CBullet*>(pObj);
			if (pBullet) {
				pBullet->bInfo = recvData.arrBullets[i];
			}
			++i;
		}

		// 아이템 박스 상태 업데이트
		const std::list<CObject*>& itemBoxList = GetGroupObject(OBJ_ITEMBOX);
		int j = 0; 
		for (CObject* pObj : itemBoxList) {
			if (j >= 10) break;
			CItem* pItemBox = static_cast<CItem*>(pObj);
			if (pItemBox) {
				pItemBox->iInfo = recvData.arrItemBoxs[j];
			}
			++j;
		}
	}

	// 키 입력 처리
	if (m_myPlayerID != -1 && m_pPlayer[m_myPlayerID]->pInfo.iLife > 0) {
		ProcessInput();
		if (b_keyAct && m_sock != INVALID_SOCKET) {
			// 입력을 서버로 전송
			for (const Player_input& myAction : m_vecInputActions) {
				int retval = send(m_sock, (const char*)&myAction, sizeof(myAction), 0);
				if (retval == SOCKET_ERROR) {
					OutputDebugString(L"[CPlayLevel] : err - send()\n");
				}
			}
		}
	}

	CLevel::Update();	// __super::Update();

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
		// 큰 폰트를 DC에 선택(Select), 이전 폰트 저장
		HFONT hOldFont = (HFONT)SelectObject(mDC, m_hFont);

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
	}
}

void CPlayLevel::Free()
{
	DeleteObject(m_hFont);
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
	setsockopt(pThis->m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&NODELAY, sizeof(NODELAY));

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
	retval = recv(pThis->m_sock, (char*)&myID, sizeof(myID), 0);
	if (retval == SOCKET_ERROR || retval == 0) {
		OutputDebugString(L"ID 수신 실패\n");
		pThis->m_bIsRunning = false;
	} else {
		pThis->m_myPlayerID = myID; // 내 ID 저장
		pThis->m_pPlayer[myID]->isMyPlayer = true; // 내 플레이어 객체 표시

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
				} else break;
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