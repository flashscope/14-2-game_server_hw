#include "stdafx.h"
#include "Guard.h"


Guard::Guard( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 배트맨
	m_MeshKey = MESH_KEY_UNIT_GUARD;
	
	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}


Guard::~Guard()
{
}
