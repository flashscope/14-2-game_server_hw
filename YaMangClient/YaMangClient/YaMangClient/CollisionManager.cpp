#include "stdafx.h"
#include "CollisionManager.h"
#include "Collision.h"

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
	for ( auto iter : m_CollisionList )
	{
		auto toBeDelete = iter;
		SafeDelete( toBeDelete );
	}
	m_CollisionList.clear();
}

void CollisionManager::AddCollision( Collision* collision )
{
	if ( collision )
	{
		m_CollisionList.push_back( collision );
	}
}

void CollisionManager::RemoveColiision( Collision* collision )
{
	if ( collision && !m_CollisionList.empty() )
	{
		for ( auto iter = m_CollisionList.begin(); iter != m_CollisionList.end(); )
		{
			if ( collision == *iter )
			{
				iter = m_CollisionList.erase( iter );
			}
			else
			{
				++iter;
			}
		}
	}
}

void CollisionManager::CheckCollision( Collision* collision ) const
{
	for ( auto iter : m_CollisionList )
	{
		if ( nullptr == iter || collision == iter )
		{
			continue;
		}

		if ( collision->IsCollideWith( *iter ) )
		{
			break;
		}
	}
}
