#pragma once
#include "Unit.h"
class Knight:
	public Unit
{
public:
	Knight( Corps* owner, UINT unitId );
	virtual ~Knight();

	// virtual void	Update();

private:
	Knight();
	Knight( const Knight& );
	Knight& operator=( const Knight& );
};

