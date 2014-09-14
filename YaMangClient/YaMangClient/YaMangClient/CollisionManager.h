#pragma once
#include "MacroSet.h"

class Collision;

class CollisionManager:public Singleton<CollisionManager>
{
public:
	CollisionManager();
	~CollisionManager();

	void	AddCollision( Collision* collision );
	void	RemoveColiision( Collision* collision );
	void	CheckCollision( Collision* collision ) const;
	
	void	ClearList() { m_CollisionList.clear(); }

private:
	std::list<Collision*>	m_CollisionList;
};
