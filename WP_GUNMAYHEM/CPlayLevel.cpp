#include "CPlayLevel.h"

// 더블 점프 안되는 오류 (GetKeyState)
// 점프 후 상단 플랫폼에 올라가지 못하는 오류
// 
// 승리/패배 결과 출력 필요

CPlayLevel::CPlayLevel()
{
}

void CPlayLevel::Initialize()
{
    // === 타이머 초기화 ===
    m_prevTime = GetTickCount64(); // 현재 시간
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
    if (m_pPlayer1 && m_pPlayer2) {
        int playerCenterX = (m_pPlayer1->x + m_pPlayer2->x) / 2;
        int playerCenterY = (m_pPlayer1->y + m_pPlayer2->y) / 2;

        cameraX = playerCenterX - cameraWidth / 2;
        cameraY = playerCenterY - cameraHeight / 2 + 100;
    }
}

// 플레이어 물리/충돌 처리
void CPlayLevel::ProcessPlayerPhysics(CPlayer* player)
{
    if (player->jumping) {
        // 점프 중일 때 땅에 닿았는지 검사
        if (player->jumpHeight > 0) { // 점프 정점 찍고 하강 중일 때만
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
    // === 1. DeltaTime 계산 ===
    ULONGLONG curTime = GetTickCount64();
    m_deltaTime = (float)(curTime - m_prevTime);
    m_prevTime = curTime;

    // === 2. 입력 처리 (Input) ===
    // -- Player 1 --
    if (GetAsyncKeyState('A') & 0x8000) {
        m_pPlayer1->looking = 0;
        if (m_pPlayer1->isMoving && m_pPlayer1->speed > 0) m_pPlayer1->speed -= FRICTION;
        else { m_pPlayer1->acceleration = -ACCELERATION; m_pPlayer1->isMoving = TRUE; }
    } 
    else if (GetAsyncKeyState('D') & 0x8000) {
        m_pPlayer1->looking = 1;
        if (m_pPlayer1->isMoving && m_pPlayer1->speed < 0) m_pPlayer1->speed += FRICTION;
        else { m_pPlayer1->acceleration = ACCELERATION; m_pPlayer1->isMoving = TRUE; }
    } 
    else {
        m_pPlayer1->isMoving = FALSE;
    }
    
    if (GetAsyncKeyState('W') & 0x8001) { // 점프
        if (m_pPlayer1->jumpCount < 2) {
            m_pPlayer1->jumpCount++;
            m_pPlayer1->jumpTime = 0.f;
            m_pPlayer1->jumpHeight = 0;
            m_pPlayer1->jstartY = m_pPlayer1->y;
            m_pPlayer1->jumping = TRUE;
            m_pPlayer1->falling = FALSE;
        }
    }
    if (GetAsyncKeyState('S') & 0x8001) { // 아래 점프
        if (m_pPlayer1->downCount == 0 && !m_pPlayer1->falling && !m_pPlayer1->jumping) {
            m_pPlayer1->downCount = 1;
            m_pPlayer1->downTime = 0;
            m_pPlayer1->downHeight = 0;
            m_pPlayer1->fstartY = m_pPlayer1->y;
            m_pPlayer1->falling = TRUE;
        }
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8001) { m_pPlayer1->gunFire(); }

    // -- Player 2 --
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        m_pPlayer2->looking = 0;
        if (m_pPlayer2->isMoving && m_pPlayer2->speed > 0) m_pPlayer2->speed -= FRICTION;
        else { m_pPlayer2->acceleration = -ACCELERATION; m_pPlayer2->isMoving = TRUE; }
    } 
    else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        m_pPlayer2->looking = 1;
        if (m_pPlayer2->isMoving && m_pPlayer2->speed < 0) m_pPlayer2->speed += FRICTION;
        else { m_pPlayer2->acceleration = ACCELERATION; m_pPlayer2->isMoving = TRUE; }
    } 
    else {
        m_pPlayer2->isMoving = FALSE;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8001) { 
        if (m_pPlayer2->jumpCount < 2) {
            m_pPlayer2->jumpCount++;
            m_pPlayer2->jumpTime = 0.f;
            m_pPlayer2->jumpHeight = 0;
            m_pPlayer2->jstartY = m_pPlayer2->y;
            m_pPlayer2->jumping = TRUE;
            m_pPlayer2->falling = FALSE;
        }
    }
    if (GetAsyncKeyState(VK_DOWN) & 0x8001) { 
        if (m_pPlayer2->downCount == 0 && !m_pPlayer2->falling && !m_pPlayer2->jumping) {
            m_pPlayer2->downCount = 1;
            m_pPlayer2->downTime = 0;
            m_pPlayer2->downHeight = 0;
            m_pPlayer2->fstartY = m_pPlayer2->y;
            m_pPlayer2->falling = TRUE;
        }
    }
    if (GetAsyncKeyState(VK_OEM_4) & 0x8001) { m_pPlayer2->gunFire(); } // '[' 키

	// === 3. 부모 클래스의 Update 호출 ===
    CLevel::Update();

    // === 4. 아이템 생성 타이머 ===
    m_itemSpawnTimer += m_deltaTime;
    if (m_itemSpawnTimer >= m_itemSpawnDelay) {
        m_itemSpawnTimer = 0.f;
        if (GetGroupObject(OBJ_ITEMBOX).size() < MAX_ITEM) {
            AddObject(new CItem(), OBJ_ITEMBOX);
        }
    }

    // ========== 5. 물리 및 충돌 처리 ==========
    // 아이템 vs 맵
    const auto& maps = GetGroupObject(OBJ_MAP);
    auto& items = GetGroupObject(OBJ_ITEMBOX); // (수정 가능해야 하므로 const 아님)
    for (auto* pItemObj : items) {
        CItem* pItem = static_cast<CItem*>(pItemObj); // CItem으로 캐스팅

        if (!pItem->IsFalling()) continue; // 이미 땅에 닿았으면 스킵

        RECT itemRect = pItem->GetRect();

        for (auto* pMapObj : maps) {
            CMap* pMap = static_cast<CMap*>(pMapObj);
            RECT mapRect = pMap->GetRect();

            // 아이템의 "발밑" 부분만 체크
            RECT itemFeet = { itemRect.left, itemRect.bottom - 10, itemRect.right, itemRect.bottom };

            if (CheckRectCollision(itemFeet, mapRect)) {
                pItem->StopFalling(pMap->GetTopY()); // 아이템 낙하 중지
                break; // 이 아이템은 땅을 찾았으므로 다음 아이템으로
            }
        }
    }

    // 플레이어 vs 맵
    ProcessPlayerPhysics(m_pPlayer1);
    ProcessPlayerPhysics(m_pPlayer2);

    // 플레이어 vs 아이템
    RECT p1Rect = m_pPlayer1->GetRect();
    RECT p2Rect = m_pPlayer2->GetRect();

    for (auto* pItemObj : items) {
        CItem* pItem = static_cast<CItem*>(pItemObj);

        // 이미 이번 프레임에 죽었으면 건너뛰기
        if (pItem->IsDead()) continue;

        RECT itemRect = pItem->GetRect();

        if (CheckRectCollision(p1Rect, itemRect)) {
            pItem->SetDead(); 
            m_pPlayer1->ApplyItem(pItem->GetGunType()); // P1에게 아이템 효과 적용
        }

        if (CheckRectCollision(p2Rect, itemRect)) {
            pItem->SetDead(); 
            m_pPlayer2->ApplyItem(pItem->GetGunType()); // P2에게 아이템 효과 적용
        }
    }

    // 총알 vs 플레이어
    m_pPlayer1->update_bullet(m_pPlayer2); // P1 총알이 P2를 맞추는지
    m_pPlayer2->update_bullet(m_pPlayer1); // P2 총알이 P1을 맞추는지

    if (m_pPlayer1->y > rt.bottom + 100) m_pPlayer1->regen();
    if (m_pPlayer2->y > rt.bottom + 100) m_pPlayer2->regen();

	// === 6. 카메라 업데이트 ===
	update_camera();
}

void CPlayLevel::Draw(HDC mDC)
{
    // 1. 배경 그리기 (Test.cpp의 WM_PAINT에서 가져옴)
    // (g_BMPmDC, BMP_map1 등 전역 비트맵 핸들 사용)
    switch (g_mapType) {
    case 0:
        SelectObject(g_BMPmDC, BMP_map1);
        BitBlt(mDC, 0, 0, 900, 650, g_BMPmDC, 900 - cameraX / 5, 650 - cameraY / 5, SRCCOPY);
        break;
    case 1:
        SelectObject(g_BMPmDC, BMP_map2);
        BitBlt(mDC, 0, 0, 900, 650, g_BMPmDC, 900 - cameraX / 5, 650 - cameraY / 5, SRCCOPY);
        break;
    }

    // 2. 부모 클래스의 Draw 호출
    // (이 코드가 m_ObjList의 모든 객체(CMap, CItem)의 Draw를 호출)
    CLevel::Draw(mDC);

    // 3. 플레이어 정보창 등 UI 그리기
    // (추후 CPlayer 구현 후 여기에 추가)
}

void CPlayLevel::Free()
{
    // 부모 클래스의 Free()를 호출하여
    // m_ObjList에 있는 모든 객체 (CMap, CItem)를 delete 합니다.
    CLevel::Free();
}
