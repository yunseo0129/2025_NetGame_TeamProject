#include "CPlayLevel.h"
#include "KeyMgr.h"

const char* SERVERIP = (char*)"127.0.0.1";

CPlayLevel::CPlayLevel()
{
}

void CPlayLevel::Initialize()
{
	// === 윈속 초기화 ===
	OutputDebugString(L"CPlayLevel::Initialize()\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		exit(1);

	// 소켓 생성
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

// 플레이어 물리/충돌 처리
void CPlayLevel::ProcessPlayerPhysics(CPlayer* player)
{
	if (player->jumping) {
		// 점프 중일 때 땅에 닿았는지 검사
		if (player->jumpHeight <= (player->jumpTime * player->jumpTime - player->jumpPower * player->jumpTime) * 4.f) { // 점프 정점 찍고 하강 중일 때만
			const auto& maps = GetGroupObject(OBJ_MAP);
			RECT playerFeet = player->GetRect();
			playerFeet.top = playerFeet.bottom - 10; // 발밑 10픽셀

			for (auto* pMapObj : maps) {
				CMap* pMap = static_cast<CMap*>(pMapObj);
				if (CheckRectCollision(playerFeet, pMap->GetRect())) {
					player->SetOnGround(pMap->GetTopY());
					return; // 땅 찾음
				}
			}
		}
	} else if (player->falling) {
		// 낙하 중일 때 땅에 닿았는지 검사
		if (player->downCount == 0) { // 아래점프 무적 아닐 때만
			const auto& maps = GetGroupObject(OBJ_MAP);
			RECT playerFeet = player->GetRect();
			playerFeet.top = playerFeet.bottom - 10;

			for (auto* pMapObj : maps) {
				CMap* pMap = static_cast<CMap*>(pMapObj);
				if (CheckRectCollision(playerFeet, pMap->GetRect())) {
					player->SetOnGround(pMap->GetTopY());
					return; // 땅 찾음
				}
			}
		}
	} else {
		// 점프/낙하 중이 아닐 때 (땅 위에 서 있을 때)
		// 발밑에 땅이 사라졌는지 검사
		const auto& maps = GetGroupObject(OBJ_MAP);
		RECT playerFeet = player->GetRect();
		playerFeet.top = playerFeet.bottom; // 발 바로 밑
		playerFeet.bottom += 5;             // 5픽셀 아래까지

		bool bOnGround = false;
		for (auto* pMapObj : maps) {
			CMap* pMap = static_cast<CMap*>(pMapObj);
			if (CheckRectCollision(playerFeet, pMap->GetRect())) {
				bOnGround = true;
				break;
			}
		}
		if (!bOnGround) {
			player->SetFalling(); // 밟고 있던 땅이 없어짐 -> 낙하
		}
	}
}

void CPlayLevel::Update()
{
	// 서버로부터 데이터 수신
	// 정보를 받으면 각 객체(플레이어)들은 그 정보를 바탕으로 자신의 상태를 각각 업데이트
	// 정보를 수정하기 위해 CPlayer에서 playerInfo 멤버 변수를 추가

	//retval = recv(sock, (char*)&recvData, sizeof(recvData), 0);
	//if (retval == SOCKET_ERROR) {
	//	OutputDebugString(L"err - recv()\n");
	//}
	//else {
	//	OutputDebugString(L"recv() 됨\n");
	//	m_pPlayer1->pInfo = recvData.playerInfo[0];
	//	m_pPlayer2->pInfo = recvData.playerInfo[1];
	//}

	// === 입력 처리 (Input) ===
	// -- Player 1 --
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT)) {
		myAction = ACTION_MOVE_L;

		m_pPlayer1->looking = 0;
		m_pPlayer1->acceleration = -ACCELERATION; 
		m_pPlayer1->isMoving = TRUE;            

	} 
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT)) {
		myAction = ACTION_MOVE_R;

		m_pPlayer1->looking = 1;
		m_pPlayer1->acceleration = ACCELERATION;
		m_pPlayer1->isMoving = TRUE;          

	} 
	else {
		myAction = ACTION_NONE;
		
		m_pPlayer1->isMoving = FALSE; // 키를 뗐을 때만 '이동 멈춤'으로 설정
	}
	
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP)) { // 점프
		myAction = ACTION_JUMP_UP;

		if (m_pPlayer1->jumpCount < 2) {
			m_pPlayer1->jumpCount++;
			m_pPlayer1->jumpTime = 0.f;
			m_pPlayer1->jumpHeight = 0;
			m_pPlayer1->jstartY = m_pPlayer1->y;
			m_pPlayer1->jumping = TRUE;
			m_pPlayer1->falling = FALSE;
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN)) { // 아래 점프
		myAction = ACTION_JUMP_DOWN;

		if (m_pPlayer1->downCount == 0 && !m_pPlayer1->falling && !m_pPlayer1->jumping) {
			m_pPlayer1->downCount = 1;
			m_pPlayer1->downTime = 0;
			m_pPlayer1->downHeight = 0;
			m_pPlayer1->fstartY = m_pPlayer1->y;
			m_pPlayer1->falling = TRUE;
		}
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE)) { 
		myAction = ACTION_SHOOT;
		
		m_pPlayer1->gunFire(); 
	}

	// === TCP/IP로 액션 전송 ===
	retval = send(sock, (const char*)&myAction, sizeof(myAction), 0);
	if (retval == SOCKET_ERROR) {
		OutputDebugString(L"err - send()\n");
	}

	// === 부모 클래스의 Update 호출 ===
	CLevel::Update();

	// 플레이어 vs 맵
	ProcessPlayerPhysics(m_pPlayer1);
	ProcessPlayerPhysics(m_pPlayer2);

	if (m_pPlayer1->y > rt.bottom + 100) m_pPlayer1->regen();
	if (m_pPlayer2->y > rt.bottom + 100) m_pPlayer2->regen();

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
