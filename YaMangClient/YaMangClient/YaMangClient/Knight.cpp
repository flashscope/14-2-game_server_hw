#include "stdafx.h"
#include "Knight.h"


Knight::Knight( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 소녀
	m_MeshKey = MESH_KEY_UNIT_KNIGHT;
	
	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}


Knight::~Knight()
{
}
