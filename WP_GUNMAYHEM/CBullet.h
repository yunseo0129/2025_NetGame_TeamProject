#pragma once
#include "CObject.h"

//총 길이(이미지)
#define gunWidth 80
#define gunHeight 30

class CBullet final : CObject {
public:
	CBullet();
	~CBullet();

public:
	virtual void Draw(HDC BMPmDC, HDC mDC);

private:
	virtual void Free();

public:
	void update_bullet();

public:
	int exist;	// 존재여부 (TRUE/FALSE)
	int x;		// 현재 x좌표 (픽셀 상)
	int y;		// 현재 y좌표 (픽셀 상)

	float vx;	// x좌표 이동방향
	float vy;	// y좌표 이동방향 // 0으로 고정

	int c;		// 그라데이션을 위한 카운트

	float travelDistance;   // 이동한 거리, 사거리 체크용
};

