#pragma once

#include <queue>           
#include "CLevel.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CItem.h"

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
	// 플레이어 
	int m_myPlayerID = -1;					// 내 플레이어 ID
	CPlayer* m_pPlayer[3] = { nullptr, };	// 플레이어 객체 배열 (최대 3명 고정)

	// 네트워크 관련
	SOCKET m_sock = INVALID_SOCKET;
	std::queue<SendData> m_recvQueue;
	CRITICAL_SECTION m_cs;

	// 스레드 관련
	bool m_bIsRunning = false;		// 스레드 실행 플래그
	HANDLE m_hThread = NULL;        // 스레드 핸들
	static DWORD WINAPI ClientThread(LPVOID);

	// 키 입력 관련
	bool b_keyAct = false;
	std::vector<Player_input> m_vecInputActions;

	// 게임 시작 여부
	bool m_bGameStarted = false;		
};