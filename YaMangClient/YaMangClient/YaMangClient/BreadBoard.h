#pragma once
#include "SharedDefine.h"

extern const int	MAX_UNIT_IN_CORPS;

struct BreadBoard
{
	BreadBoard()
	{
		ZeroMemory( &m_Position, sizeof( m_Position ) );
	}
	
	D3DXVECTOR3		m_Position[MAX_UNIT_IN_CORPS];
	D3DXVECTOR3		m_Scale = { 1.0f, 1.0f, 1.0f };
	
	D3DXVECTOR3		m_Min = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3		m_Max = { 0.0f, 0.0f, 0.0f };
};

struct DestroyBread: public BreadBoard
{
	DestroyBread()
	{
		m_Position[0] = { 0.0f, 0.0f, 0.0f };
		m_Position[1] = { -1.0f, 0.0f, 0.0f };
		m_Position[2] = { 0.0f, 0.0f, -1.0f };
		m_Position[3] = { -1.0f, 0.0f, -1.0f };
		m_Position[4] = { 0.0f, 0.0f, -2.0f };
		m_Position[5] = { -1.0f, 0.0f, -2.0f };
		m_Position[6] = { 0.0f, 0.0f, -3.0f };
		m_Position[7] = { -1.0f, 0.0f, -3.0f };
		m_Position[8] = { 0.0f, 0.0f, -4.0f };
		m_Position[9] = { -1.0f, 0.0f, -4.0f };
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//				1  0
	//				3  2
	//				5  4
	//				7  6
	//				9  8
	//
	//////////////////////////////////////////////////////////////////////////
};

struct DefenseBread: public BreadBoard
{
	DefenseBread()
	{
		m_Position[0] = { 0.0f, 0.0f, 0.0f };
		m_Position[1] = { 1.0f, 0.0f, 0.0f };
		m_Position[2] = { 1.0f, 0.0f, -1.0f };
		m_Position[3] = { 0.0f, 0.0f, -1.0f };
		m_Position[4] = { -1.0f, 0.0f, 0.0f };
		m_Position[5] = { -1.0f, 0.0f, -1.0f };
		m_Position[6] = { -1.0f, 0.0f, -2.0f };
		m_Position[7] = { 0.0f, 0.0f, -2.0f };
		m_Position[8] = { 1.0f, 0.0f, -2.0f };
		m_Position[9] = { 0.0f, 0.0f, -3.0f };
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//				4  0  1
	//				5  3  2
	//				6  7  8
	//				   9
	//
	//////////////////////////////////////////////////////////////////////////
};

struct RushBread: public BreadBoard
{
	RushBread()
	{
		m_Position[0] = { 0.0f, 0.0f, 0.0f };
		m_Position[1] = { 1.0f, 0.0f, 0.0f };
		m_Position[2] = { -1.0f, 0.0f, 0.0f };
		m_Position[3] = { 0.0f, 0.0f, -1.0f };
		m_Position[4] = { 1.0f, 0.0f, -1.0f };
		m_Position[5] = { -1.0f, 0.0f, -1.0f };
		m_Position[6] = { 2.0f, 0.0f, 0.0f };
		m_Position[7] = { -2.0f, 0.0f, 0.0f };
		m_Position[8] = { 2.0f, 0.0f, -1.0f };
		m_Position[9] = { -2.0f, 0.0f, -1.0f };
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//			7  2  0  1  6
	//			9  5  3  4  8
	//
	//////////////////////////////////////////////////////////////////////////
};