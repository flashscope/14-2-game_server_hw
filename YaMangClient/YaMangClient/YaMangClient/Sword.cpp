#include "stdafx.h"
#include "Sword.h"


Sword::Sword( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 호랑이
	m_MeshKey = MESH_KEY_UNIT_SWORD;
	
	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}


Sword::~Sword()
{
}
