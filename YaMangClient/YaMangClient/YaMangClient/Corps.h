#pragma once
#include "EnumSet.h"
#include "Interface3D.h"

struct PositionInfo;
struct BreadBoard;
class Unit;

class Corps: public Interface3D
{
public:
	Corps( int corpsId, int playerId, PositionInfo pos );
	~Corps();

	void	Update();
	void	Render() const;
	
	void	Create(UINT num, UnitType unitType );
	void	SetVisible( bool visible );

	int		GetCorpsID() const { return m_CorpsID; }
	int		GetCorpsHP() const { return m_UnitList.size(); }
	void	SetCorpsHP( int unitNum );

	int		GetOwnPlayerID() const { return m_OwnerPlayerID; }
	
	UnitType		GetUnitType() const { return m_UnitType; }
	void			SetFormation( FormationType formation );
	D3DXVECTOR3		GetFormation( int unitId ) const;

	typedef std::function<void( Corps& )>	ActionFunc;

	void	ChangeAction( ActionFunc action ) { m_Action = action; }
	void	DoAction() { m_Action( *this ); }
	void	ClearAction();

	typedef std::function<void( Unit&, Corps& )> ActFunc;

	void	SetAct( ActFunc act );
	void	SetActStatus( ActionStatusType actionStatus );
	void	GoFoward();
	void	LookForward();

	void	InterpolatePosition();
	
	bool	IsSelected() const { return m_IsSelected; }
	void	SetSelected( bool selected ) { m_IsSelected = selected; }

	bool	IsEnemy() const { return m_IsEnemy; }
	void	SetEnemy( bool enemy ) { m_IsEnemy = enemy; }

	bool	IsContain( float x, float z );

	bool	GetMoved() const { return m_IsMoved; }
	void	SetMoved( bool isMoved ) { m_IsMoved = isMoved; }

	bool	IsFight() const { return m_IsFight; }
	bool	SetFight( bool fight ) { m_IsFight = fight; }
	void	StartFight();

	CorpsTextureType		GetCorpsTextureType() { return m_TextureType; }

private:
	Corps();
	Corps( const Corps& );
	Corps& operator=( const Corps& );

	MeshKeyType				m_MeshKey = MESH_KEY_CORPS_RUSH_MINE;
	CorpsTextureType		m_TextureType = CORPS_TEXTURE_NONE;

	std::vector<Unit*>		m_UnitList;
	int						m_CorpsID = -1;
	int						m_OwnerPlayerID = -1;
	UnitType				m_UnitType = UnitType::UNIT_NONE;

	FormationType			m_NowFormation = FormationType::FORMATION_RUSH;
	FormationType			m_TargetFormation = FormationType::FORMATION_RUSH;
	std::array<BreadBoard*, static_cast<size_t>(FormationType::FORMATION_MAX)> m_FormationArray;

	bool	m_IsSelected = false;
	bool	m_IsMoved = false;
	bool	m_IsInterpolated = false;
	bool	m_IsFight = false;
	bool	m_IsEnemy = false;

	UINT	m_AttacktedTime = 0;

	ActionFunc	m_Action;
};
