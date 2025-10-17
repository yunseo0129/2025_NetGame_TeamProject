#pragma once
#include "CObject.h"
#include "CBullet.h"

const int pWidth = 45;
const int pHeight = 67;
const int MAXLIFE = 3;

const float MAX_SPEED = 5.0f;      // 최대 속도
const float ACCELERATION = 0.1f;   // 가속도
const float FRICTION = 0.05f;      // 마찰력(감속)

//const int anim_max_frame;

class CPlayer final : CObject 
{
public:
	CPlayer();
	~CPlayer() = default;

public:
	virtual void Draw(HDC BMPmDC, HDC mDC);

private:
	virtual void Free();

public:
	void regen();
	void update();

	void reload();
	void gunFire();
	void update_bullet(CPlayer* pTarget);

public:
	int exist;			// dead or alive
	int x;				   
	int y;				   
	int looking;		// left right -> paint & gunfire
	int life;			   
	int downCount;		// 10번의 타이머 동안 땅 부딪힘 검사 x

	// 낙하 및 점프 변수
	int fstartY;
	float downTime;
	float downHeight;
	BOOL falling;

	int jstartY;		// 점프 시작 위치
	float jumpTime;
	float jumpHeight;
	float jumpPower;
	BOOL jumping;
	int jumpCount = 0;	// 점프 횟수

	// 속도, 가속도 변수
	float acceleration;	// 현재 가속도
	float speed;		// 현재 속도
	bool isMoving;		// 이동 중인지 여부

	// 콤보
	int combo;			// 콤보가 쌓인 횟수에 따라 총을 맞았을 때 가속이 빨라짐
	int comboTime = 0;	// 콤보 지속시간 

	// 애니메이션
	int anim_frame;
	int anim_timer;

public:
	CBullet bullet[30];
	int bullet_count = 0;	// 현재 발사된 총알 수

	// 총기 관련 (먹은 아이템에 따라 변화)
	int gunType = 1;		// 1:pistol		2:snipe
	int range = 300;		// 플레이어1의 사거리 (아이템을 먹으면 변화)
	int maxBullet = 20;		// 1:pistol 20발, 2:snipe 10발
};

