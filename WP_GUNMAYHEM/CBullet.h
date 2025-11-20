#pragma once
#include "CObject.h"

//총 길이(이미지)
const int gunWidth = 80;
const int gunHeight = 30;
const int BULLET_SIZE = 2;		// 권총		가로 및 세로의 길이
const int BULLET_SIZE2 = 4;		// 저격총	가로길이 - 세로는 권총과 동일
const int MAX_BULLET = 20;		// 총알 최대 개수

class CBullet final : public CObject {
public:
	CBullet();
	~CBullet();

public:
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;
	virtual bool Update() override;

	void Init() { 
		m_exist = FALSE; 
		m_x = 0; 
		m_y = 0; 
		m_gradationCount = 7;
	}

public:
	int m_exist;	// 존재여부 (TRUE/FALSE)
	int m_x;		// 현재 x좌표 (픽셀 상)
	int m_y;		// 현재 y좌표 (픽셀 상)

	int m_gradationCount = 3;	// 그라데이션을 위한 카운트
	int m_maxGradation = 15;	// 타입에 따른 최대 그라데이션 수 (권총 15, 저격총 25)

public:
	BulletInfo bInfo;
};

