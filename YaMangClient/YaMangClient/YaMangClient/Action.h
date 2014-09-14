/**
*@file		Action.h
*@brief		Corps가 갖는 단일 행동을 정의합니다.
*@details	Corps의 Update() 내에서 호출(무한루프) 됩니다.
*/

#pragma once
#include "Logger.h"
#include "EnumSet.h"
#include "Corps.h"
#include "Act.h"

/**
*@brief		Action 추상 클래스
*@details	모든 Action은 이 함수 객체(struct)를 상속 받아 구현합니다.
*@author	zrma
*@date		2014-04-25
*/
struct Action
{
public:
	virtual void operator()( Corps& ) = 0;
};

struct ActionDefault: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActDefault act;
		corps.SetAct( act );
	}
};

struct ActionStoppingPosition: public Action
{
	virtual void operator() ( Corps& corps )
	{
		// 멈춰 있지만 서버에서 받은 위치와 클라 위치를 위치 보정함
		corps.InterpolatePosition();
	}
};

struct ActionHoldPosition: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActDefault act;
		corps.SetAct( act );
	}
};

struct ActionAttack: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActAttackSeries act;
		corps.SetAct( act );
	}
};

struct ActionTakeArea: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActAttackSeries act;
		corps.SetAct( act );
	}
};

struct ActionTransFormation: public Action
{
	virtual void operator() ( Corps& corps )
	{
		corps.GoFoward();
		
		ActMoveSeries act;
		corps.SetAct( act );
		corps.LookForward();
	}
};

struct ActionCorpsSpawn: public Action
{
	virtual void operator() ( Corps& corps )
	{
		ActMoveSeries act;
		corps.SetAct( act );
	}
};

struct ActionMovePosition: public Action
{
	virtual void operator() ( Corps& corps )
	{
		corps.GoFoward();
		// Log( "현 좌표 : %f %f ", corps.GetEyePoint().x, corps.GetEyePoint().z );

		ActMoveSeries act;
		corps.SetAct( act );
	}
};
