#pragma once
#include "Unit.h"

class Corps;

class Sword:
	public Unit
{
public:
	Sword( Corps* owner, UINT unitId );
	virtual ~Sword();

	// virtual void	Update();

private:
	Sword();
	Sword( const Sword& );
	Sword& operator=( const Sword& );
};

