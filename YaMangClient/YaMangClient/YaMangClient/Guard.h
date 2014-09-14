#pragma once
#include "Unit.h"
class Guard:
	public Unit
{
public:
	Guard( Corps* owner, UINT unitId );
	virtual ~Guard();

	// virtual void	Update();

private:
	Guard();
	Guard( const Guard& );
	Guard& operator=( const Guard& );
};

