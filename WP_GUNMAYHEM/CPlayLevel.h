#pragma once

#include <queue>           
#include "CLevel.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CItem.h"

// 플레이 레벨 ====
// 캐릭터, 아이템, 맵, 총알 오브젝트들
// Draw - 맵, 캐릭터, 캐릭터 정보, 총알은 그라데이션 신경쓰기
// Update - 입력처리, 충돌처리, 아이템 생성, 캐릭터 이동, 총알 이동

class CPlayLevel final : public CLevel {
public:
	CPlayLevel();
	virtual ~CPlayLevel() = default;

public:
	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;

	void ProcessInput();
	void update_camera();

private:
	CPlayer* m_pPlayer1 = nullptr;
	CPlayer* m_pPlayer2 = nullptr;

private:
	// 네트워크 관련
	SOCKET m_sock = INVALID_SOCKET;
	//std::queue<SendData> m_recvQueue;
	std::queue<MovementData> m_recvQueue; // SendData -> MovementData
	CRITICAL_SECTION m_cs;

	bool m_bIsRunning = false;		// 스레드 실행 플래그
	HANDLE m_hThread = NULL;        // 스레드 핸들
	static DWORD WINAPI ClientThread(LPVOID);

	// 키 입력 관련
	bool b_keyAct = false;
	PLAYER_ACTION myAction { false, };

	int m_myPlayerID = -1;
};
