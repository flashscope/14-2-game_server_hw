#pragma once
#include "Unit.h"
class Arrow:
	public Unit
{
public:
	Arrow( Corps* owner, UINT unitId );
	virtual ~Arrow();

	// virtual void	Update();

private:
	Arrow();
	Arrow( const Arrow& );
	Arrow& operator=( const Arrow& );
};

