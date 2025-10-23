#include "CMap.h"

CMap::CMap()
{
}

CMap::~CMap()
{
}

void CMap::Draw(HDC mDC)
{

}

bool CMap::Update()
{
	return false;
}

void CMap::MapSelect(bool _isMap1)
{
	MAPS map;
	if (_isMap1)
	{
		map = { 330, 170 };	//1단
		m_vMaps.push_back(map);

		map = { 160, 300 };	//2단
		m_vMaps.push_back(map);
		map = { 510, 300 };
		m_vMaps.push_back(map);
		 
		map = { 70, 420 };	//3단
		m_vMaps.push_back(map);
		map = { 600, 420 };
		m_vMaps.push_back(map);
	}
	else
	{
		map = { 160, 170 };	//1단
		m_vMaps.push_back(map);
		map = { 510, 170 };
		m_vMaps.push_back(map);
		
		map = { 330, 300 }; //2단
		m_vMaps.push_back(map);
		
		map = { 70, 420 };	//3단
		m_vMaps.push_back(map);
		map = { 600, 420 };
		m_vMaps.push_back(map);
	}
}

void CMap::Free()
{
	m_vMaps.clear();
}
