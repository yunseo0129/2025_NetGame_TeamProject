#pragma once
#include "CObject.h"

//총 길이(이미지)
const int gunWidth = 80;
const int gunHeight = 30;
const int BULLET_SIZE = 2;		
const int BULLET_SIZE2 = 4;		// 그라데이션 길이

class CBullet final : public CObject {
public:
	CBullet();
	~CBullet();

public:
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;
	virtual bool Update() override;

private:
	int m_exist;	// 존재여부 (TRUE/FALSE)
	int m_x;		// 현재 x좌표 (픽셀 상)
	int m_y;		// 현재 y좌표 (픽셀 상)

	int m_gradationCount = 3;	// 그라데이션을 위한 카운트
	int m_maxGradation = 15;	// 타입에 따른 최대 그라데이션 수 (권총 15, 저격총 25)

public:
	BulletInfo bInfo;
};

