#pragma once
#include "Unit.h"

class King:
	public Unit
{
public:
	King( Corps* owner, UINT unitId );
	virtual ~King( );

	// virtual void	Update();

private:
	King( );
	King( const King& );
	King& operator=( const King& );
};

