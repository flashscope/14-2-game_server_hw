#pragma once

extern const float NOT_A_NUMBER_FLOAT;
class Collision;

class Interface3D
{
public:
	Interface3D();
	virtual ~Interface3D();

	virtual void	Update() = 0;
	virtual void	Render() const = 0;

	virtual void	SetVisible( bool visible ) { m_Visible = visible; }
	D3DXMATRIXA16	GetMatrix( bool leftHand = true ) const;

	D3DXVECTOR3		GetEyePoint() const { return m_EyePoint; }
	void			SetEyePoint( D3DXVECTOR3 eyePoint ) { m_EyePoint = eyePoint; }
	D3DXVECTOR3		GetLookAtPoint() const { return m_LookAtPoint; }
	void			SetLookAtPoint( D3DXVECTOR3 lookAtPoint ) { m_LookAtPoint = lookAtPoint; }
	D3DXVECTOR3		GetScale() const { return m_Scale; }
	void			SetScale( D3DXVECTOR3 scale ) { m_Scale = scale; }
	
	D3DXVECTOR3		GetTargetPosition() const { return m_TargetPoint; }
	void			SetTargetPosition( D3DXVECTOR3 targetPosition ) { m_TargetPoint = targetPosition; }
	D3DXVECTOR3		GetLookAtVector() const { return ( m_LookAtPoint - m_EyePoint ); }
	void			SetLookAtVector( D3DXVECTOR3 lookAtVector ) { m_LookAtPoint = m_EyePoint + lookAtVector; }
	float			GetSpeed() const { return m_Speed; }
	void			SetSpeed( float speed ) { m_Speed = speed; }

protected:
	D3DXVECTOR3		m_EyePoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_LookAtPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_UpVector = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3		m_TargetPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_Scale = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };

	float	m_Speed = 0.0f;
	bool	m_Visible = false;
	Collision*		m_Collision = nullptr;
};

