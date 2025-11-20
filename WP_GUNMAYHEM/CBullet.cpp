#include "CBullet.h"

CBullet::CBullet()
{
	m_exist = FALSE;
	m_x = 0;
	m_y = 0;
}

CBullet::~CBullet()
{
}

void CBullet::Draw(HDC mDC)
{
	if (m_exist)
	{
		HPEN mPen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(mDC, mPen);
		HBRUSH mBrush = CreateSolidBrush(RGB(0, 0, 0));
		HBRUSH oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

		Rectangle(mDC,
				  m_x - BULLET_SIZE - cameraX,
				  m_y + 2 - BULLET_SIZE - cameraY,
				  m_x + BULLET_SIZE - cameraX,
				  m_y + 2 + BULLET_SIZE - cameraY);

		SelectObject(mDC, oldBrush);
		DeleteObject(mBrush);
		SelectObject(mDC, oldPen);
		DeleteObject(mPen);

		// 그라데이션 (꼬리)
		int vx = (bInfo.vPosition.x - bInfo.vStarting.x < 0) ? -1 : 1; // 방향 벡터 x 성분
		for (int j = 0; j < m_gradationCount; j++) {
			mPen = CreatePen(PS_SOLID, 0, RGB(7 * j, 7 * j, 7 * j));
			oldPen = (HPEN)SelectObject(mDC, mPen);
			mBrush = CreateSolidBrush(RGB(7 * j, 7 * j, 7 * j));
			oldBrush = (HBRUSH)SelectObject(mDC, mBrush);

			Rectangle(mDC,
					  m_x - vx * BULLET_SIZE2 * (j + 1) - BULLET_SIZE - cameraX,
					  m_y + 2							- BULLET_SIZE - cameraY,
					  m_x - vx * BULLET_SIZE2 * (j + 1) + BULLET_SIZE - cameraX,
					  m_y + 2 							+ BULLET_SIZE - cameraY);

			SelectObject(mDC, oldBrush);
			DeleteObject(mBrush);
			SelectObject(mDC, oldPen);
			DeleteObject(mPen);

		}

		if (m_gradationCount < m_maxGradation)
			++m_gradationCount;
	}
}

void CBullet::Free()
{
}

bool CBullet::Update()
{
	if (true) {
		// 새로 생성될 때 그라데이션 초기화
		if (bInfo.exist == true && m_exist == FALSE) {
			m_gradationCount = 3; 
			m_maxGradation = (bInfo.eType == ITEM_PISTOL) ? 15 : 25;

			// 사운드 재생
			FMOD::Sound* gun_sound = (bInfo.eType == ITEM_PISTOL) ? sound1 : sound2;
			channel->stop();
			gun_sound->setMode(FMOD_LOOP_OFF);
			ssystem->playSound(gun_sound, 0, false, &channel);
		}

		m_exist = bInfo.exist;
		m_x = bInfo.vPosition.x;
		m_y = bInfo.vPosition.y;

		
	}
	return false;
}