#pragma once
#include "CObject.h"

//총 길이(이미지)
const int gunWidth = 80;
const int gunHeight = 30;
const int BULLET_SIZE = 2;		// 권총		가로 및 세로의 길이
const int BULLET_SIZE2 = 4;		// 저격총	가로길이 - 세로는 권총과 동일
const int MAX_BULLET = 20;		// 총알 최대 개수

class CBullet final : CObject {
public:
	CBullet();
	~CBullet();

public:
	virtual void Draw(HDC BMPmDC, HDC mDC);

private:
	virtual void Free();

public:
	virtual bool Update();

public:
	int exist;	// 존재여부 (TRUE/FALSE)
	int x;		// 현재 x좌표 (픽셀 상)
	int y;		// 현재 y좌표 (픽셀 상)

	float vx;	// x좌표 이동방향
	float vy;	// y좌표 이동방향 // 0으로 고정

	int c;		// 그라데이션을 위한 카운트

	float travelDistance;   // 이동한 거리, 사거리 체크용
};

