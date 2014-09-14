#include "stdafx.h"
#include "Pike.h"


Pike::Pike( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 중국
	m_MeshKey = MESH_KEY_UNIT_PIKE;
	
	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}


Pike::~Pike()
{
}
