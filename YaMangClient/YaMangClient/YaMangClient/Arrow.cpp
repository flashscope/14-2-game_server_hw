#include "stdafx.h"
#include "Arrow.h"
#include "Timer.h"

Arrow::Arrow( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 토로
	m_MeshKey = MESH_KEY_UNIT_ARROW;
	
	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}

Arrow::~Arrow()
{
}