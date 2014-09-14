#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

class PlayerManager : public Singleton<PlayerManager>
{
public:
	PlayerManager();
	~PlayerManager();

	bool	IsSelectedCorps() { return !( m_SelectedCorpsList.empty() ); }
	bool	IsCorpsInIdList( int corpsId ) const;

	void	AddToSelectedCorps( int corpsID ) { m_SelectedCorpsList.push_back( corpsID ); }
	void	ClearSelectedCorps() { m_SelectedCorpsList.clear(); }

	void	AttackCorpsById( int corpsID ) const;
	void	MoveCorpsToPosition( float x, float z ) const;
	void	ChangeCorpsFormation( FormationType formation ) const;

	void	SetWin( bool win ) { m_IsWinner = win; }
	bool	GetWin() const { return m_IsWinner; }

private:
	std::list<int>	m_SelectedCorpsList;

	bool	m_IsWinner = false;
};

