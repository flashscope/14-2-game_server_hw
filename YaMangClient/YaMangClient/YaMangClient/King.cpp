﻿#include "stdafx.h"
#include "King.h"


King::King( Corps* owner, UINT unitId )
: Unit( owner, unitId )
{
	// 왕
	m_MeshKey = MESH_KEY_UNIT_KING;

	m_EyePoint = { 0.0f, 0.0f, 0.0f };
	m_LookAtPoint = { 0.0f, 0.0f, 1.0f };
	m_Scale = { 1.0f, 1.0f, 1.0f };

	SetStartPosition();
}


King::~King( )
{
}