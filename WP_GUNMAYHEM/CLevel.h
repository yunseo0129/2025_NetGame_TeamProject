#pragma once
#include "Default.h"
#include "CObject.h"
#include "CBase.h"

class CLevel abstract : CBase
{
public:
	CLevel() = default;
	virtual ~CLevel() = default;

public:
    virtual void Initialize() = 0;

    virtual void Update(HWND hWnd);                     // 입력 처리 포함
    virtual void Draw(HDC hDC, HDC mDC, HDC BMPmDC);

    virtual void Free();

public:
	void AddObject(CObject* _pObj, OBJ_ID _eID)
	{
		if (OBJ_END <= _eID || nullptr == _pObj)
			return;
		m_ObjList[_eID].push_back(_pObj);
	}
	const std::list<CObject*>& GetGroupObject(OBJ_ID _eID)
	{
		return m_ObjList[_eID];
	}
protected:
	std::list<CObject*>		m_ObjList[OBJ_END];
};