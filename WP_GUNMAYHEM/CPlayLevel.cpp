#include "CPlayLevel.h"

// 사각형 충돌 유틸리티 함수
bool CheckRectCollision(const RECT& rect1, const RECT& rect2)
{
    if (rect1.left < rect2.right && rect1.right > rect2.left &&
        rect1.top < rect2.bottom && rect1.bottom > rect2.top) {
        return true;
    }
    return false;
}

CPlayLevel::CPlayLevel()
{
}

void CPlayLevel::Initialize()
{
    // === 타이머 초기화 ===
    m_prevTime = GetTickCount64(); // 현재 시간
    m_itemSpawnTimer = 0.f;

    if (g_mapType == 0) {
        AddObject(new CMap(330, 170), OBJ_MAP);  // 1단
        AddObject(new CMap(160, 300), OBJ_MAP);  // 2단
        AddObject(new CMap(510, 300), OBJ_MAP);  
        AddObject(new CMap(70, 420),  OBJ_MAP);  // 3단
        AddObject(new CMap(600, 420), OBJ_MAP);
    } else { // mapType == 1
        AddObject(new CMap(160, 170), OBJ_MAP);  // 1단
        AddObject(new CMap(510, 170), OBJ_MAP);
        AddObject(new CMap(330, 300), OBJ_MAP);  // 2단
        AddObject(new CMap(70, 420),  OBJ_MAP);  // 3단
        AddObject(new CMap(600, 420), OBJ_MAP);
    }

    // === 아이템 초기화 ===
    // 아이템은 시작 시 생성하는 것이 아니라, Update에서 타이머로 생성
    // (기존 '총알1/2 초기화', '아이템 초기화' 로직은 CPlayer, CItem 생성자로 이동)
}

void CPlayLevel::Update()
{
    //OutputDebugString(L"PlayLevel Update\n");

    // === 1. DeltaTime 계산 (타이머 기반 로직을 위해) ===
    DWORD curTime = GetTickCount64();
    m_deltaTime = (float)(curTime - m_prevTime);
    m_prevTime = curTime;

    // === 2. 모든 객체(아이템 포함)의 기본 Update 호출 ===
    // (CItem::Update()가 여기서 호출되어 아이템이 낙하함)
    CLevel::Update();

    // === 3. 아이템 생성 타이머 (기존 WM_TIMER 2번) ===
    m_itemSpawnTimer += m_deltaTime;

    if (m_itemSpawnTimer >= m_itemSpawnDelay) {
        m_itemSpawnTimer = 0.f; // 타이머 리셋

        // 아이템 개수 제한 (MAX_ITEM)
        if (GetGroupObject(OBJ_ITEMBOX).size() < MAX_ITEM) {
            AddObject(new CItem(), OBJ_ITEMBOX); // 새 아이템 생성 및 레벨에 등록
        }
    }

    // === 4. 충돌 처리 ===
    // (기존 WP_FINAL의 update item create & move 로직 대체)

    // 4-1. 아이템 vs 맵 충돌
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

    // 4-2. 플레이어 vs 맵 충돌
    // (CPlayer를 객체로 만든 후 여기에 구현)

    // 4-3. 플레이어 vs 아이템 충돌
    // (CPlayer를 객체로 만든 후 여기에 구현)

    // 4-4. 총알 vs 플레이어 충돌
    // (CBullet, CPlayer를 객체로 만든 후 여기에 구현)
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
