#pragma once
#include "Unit.h"
class Pike:
	public Unit
{
public:
	Pike( Corps* owner, UINT unitId );
	virtual ~Pike();

	// virtual void	Update();

private:
	Pike();
	Pike( const Pike& );
	Pike& operator=( const Pike& );
};

