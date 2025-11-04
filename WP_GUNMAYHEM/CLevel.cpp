#include "CLevel.h"
#include "keymgr.h"
void CLevel::Update()
{
	CKeyMgr::Get_Instance()->Key_Update();

	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto iter = m_ObjList[i].begin(); iter != m_ObjList[i].end(); )
		{
			int iResult = (*iter)->Update();
			if (true == iResult)
			{
				delete (*iter); // 1. 객체 삭제 (메모리 누수 방지)
				iter = m_ObjList[i].erase(iter); // 2. 리스트에서 포인터 제거
			}
			else
				++iter;
		}
	}
}

void CLevel::Draw(HDC mDC)
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_ObjList[i])
		{
			obj->Draw(mDC);
		}
	}
}

void CLevel::Free()
{
	for (size_t i = 0; i < OBJ_END; ++i)
	{
		for (auto& obj : m_ObjList[i])
		{
			if (obj != nullptr) {
				delete obj;			// 포인터가 가리키는 객체를 삭제
				obj = nullptr;		// 리스트에 저장된 포인터를 nullptr로 변경
			}
		}
		m_ObjList[i].clear();
	}
}