#pragma once

class Interface3D;
extern const float NOT_A_NUMBER_FLOAT;

class Collision
{
public:
	Collision( Interface3D* owner, float radius ): m_Owner(owner), m_Radius(radius) {}
	~Collision();

	float	GetRadius() const { return m_Radius; }
	void	SetRadius( float radius ) { m_Radius = radius; }
	
	bool	IsCollide() const { return m_IsCollide; }
	void	SetCollide( bool isCollide = true ) { m_IsCollide = isCollide; }

	D3DXVECTOR3		GetPosition() const;
	D3DXVECTOR3		GetReverseVector() { return m_ReverseVector; }

	Interface3D*	GetOwner() const { return m_Owner; }
	Interface3D*	GetCompetitor() const { return m_Competitor; }

	bool	IsCollideWith( Collision& collision );
	
private:
	Collision();

	D3DXVECTOR3		m_ReverseVector = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };

	bool			m_IsCollide = false;
	float			m_Radius = 0;
	Interface3D*	m_Owner = nullptr;
	Interface3D*	m_Competitor = nullptr;
};
