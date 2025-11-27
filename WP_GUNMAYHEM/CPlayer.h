#pragma once
#include "CObject.h"
#include "CBullet.h"

const int pWidth = 45;
const int pHeight = 67;

class CPlayer final : public CObject 
{
public:
	CPlayer();
	~CPlayer() = default;

public:
	virtual void Draw(HDC mDC) override;
	virtual void Free() override;       
	virtual bool Update() override;     

	int Get_X() const { return x; }
	int Get_Y() const { return y; }
	
private:
	int exist;				// dead or alive
	int x;				   
	int y;				   
	int looking;			// left right -> paint & gunfire
	
	int gunType = 1;		// 가진 총 타입 ( 1:pistol / 2:snipe )

	TCHAR info_text[20];	// 정보창 출력용 텍스트

	// 애니메이션
	bool isMoving = false;	// 이동 중인지 여부
	int prevX = 0;			// 애니메이션 추론용
	int anim_frame = 0;
	int anim_timer = 0;

public:
	int playerType = -1;
	PlayerInfo pInfo;
};
