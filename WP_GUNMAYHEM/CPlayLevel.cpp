#include "CPlayLevel.h"
#include "KeyMgr.h"

const char* SERVERIP = (char*)"127.0.0.1";

CPlayLevel::CPlayLevel()
{
}


void CPlayLevel::Initialize()
{

	// === 소켓 생성 및 서버 연결 ===
	HANDLE hThread = CreateThread(NULL, 0, ClientThread, NULL, 0, NULL);
	if (hThread == NULL)
	{
		OutputDebugString(L"스레드 생성 실패\n");
	}
	else
	{
		CloseHandle(hThread);
	}

	// === 타이머 초기화 ===
	m_itemSpawnTimer = 0.f;

	// === 플레이어 생성 ===
	m_pPlayer1 = new CPlayer();
	m_pPlayer1->x = 150;                // P1 시작 위치
	m_pPlayer1->playerType = 1;
	AddObject(m_pPlayer1, OBJ_PLAYER1); // 레벨에 등록

	m_pPlayer2 = new CPlayer();
	m_pPlayer2->x = 650;                // P2 시작 위치
	m_pPlayer2->playerType = 2;
	AddObject(m_pPlayer2, OBJ_PLAYER2); // 레벨에 등록

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
	if (m_pPlayer1) {
		int playerCenterX = (m_pPlayer1->x + 0);
		int playerCenterY = (m_pPlayer1->y + 0);

		cameraX = playerCenterX - cameraWidth / 2;
		cameraY = playerCenterY - cameraHeight / 2 + 100;
	}
}

void CPlayLevel::Update()
{
	
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
	closesocket(sock);
	// 부모 클래스의 Free()를 호출하여
	// m_ObjList에 있는 모든 객체 (CMap, CItem)를 delete 합니다.


	CLevel::Free();
}

void CPlayLevel::ProcessInput()
{
	b_keyAct = false;						// 키 입력이 있었는지 여부
	myAction = PLAYER_ACTION { false, };	// 보낼 액션 초기화

	// 키 다운 처리
	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT)) {
		b_keyAct = true;
		myAction.left = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT)) {
		b_keyAct = true;
		myAction.right = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP)) {
		b_keyAct = true;
		myAction.up = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN)) {
		b_keyAct = true;
		myAction.down = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)) {
		b_keyAct = true;
		myAction.space = true;
	}

	// 키 업 처리
	if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT)) {
		b_keyAct = true;
		myAction.left = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT)) {
		b_keyAct = true;
		myAction.right = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_UP)) {
		b_keyAct = true;
		myAction.up = false;
	}
	if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN)) {
		b_keyAct = true;
		myAction.down = false;
	}
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	int retval;
	SOCKET sock = INVALID_SOCKET;

	// 소켓 생ㅡ
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		OutputDebugString(L"err - socket()\n");
	// connect()
	sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
		OutputDebugString(L"err - connect()\n");
	else
		OutputDebugString(L"connect 성공\n");

	while (1) {
		// 서버로부터 데이터 수신
		// 정보를 받으면 각 객체(플레이어)들은 그 정보를 바탕으로 자신의 상태를 각각 업데이트
		SendData recvData
		retval = recv(sock, (char*)&recvData, sizeof(recvData), 0);
		if (retval == SOCKET_ERROR) {
			OutputDebugString(L"err - recv()\n");
		} else {
			// 플레이어 정보 업데이트
			/*CPlayLevel* pPlayLevel = (CPlayLevel*)CLevelManager::GetInstance()->GetCurrentLevel();
			if (pPlayLevel) {
				if (pPlayLevel->m_pPlayer1)
					pPlayLevel->m_pPlayer1->pInfo = recvData.playerInfo[0];
				if (pPlayLevel->m_pPlayer2)
					pPlayLevel->m_pPlayer2->pInfo = recvData.playerInfo[1];
			}*/
		}
	}
}