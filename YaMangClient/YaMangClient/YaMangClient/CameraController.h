#pragma once

#include "MacroSet.h"

const FLOAT NEAREST_Y = 10.0f;
const FLOAT DEGREE_1_Y = 40.0f;
const FLOAT DEGREE_2_Y = 100.0f;
const FLOAT DEGREE_3_Y = 170.0f;
const FLOAT FARTHEST_Y = 280.0f;

const FLOAT NEAREST_VIEW_Y = 0.01f;
const FLOAT DEGREE_1_VIEW_Y = 0.10f;
const FLOAT DEGREE_2_VIEW_Y = 0.30f;
const FLOAT DEGREE_3_VIEW_Y = 0.50f;
const FLOAT FARTHEST_VIEW_Y = 0.80f;

enum ZoomStatusType
{
	ZOOM_STATUS_NONE,
	ZOOM_STATUS_NEAREST,
	
	ZOOM_STATUS_DEGREE_1,
	ZOOM_STATUS_DEGREE_2,
	ZOOM_STATUS_DEGREE_3,
	
	ZOOM_STATUS_FARTHEST,
	ZOOM_STATUS_MAX
};

enum ZoomDirectionType
{
	ZOOM_DIRECTION_NONE,
	
	ZOOM_DIRECTION_FOWARD,
	ZOOM_DIRECTION_BACK,

	ZOOM_DIRECTION_MAX
};

class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init( float x, float z );

	void	MoveForward( float speed, bool zoom = false );
	void	MoveSide( float speed );
	void	MoveElevate( float speed );
	void	RotateUp( float angle );
	void	RotateSide( float angle );

	float	GetHeight() const { return m_EyePoint.y; }
	int		GetHeightGrade() const { return m_ZoomStatus; }
	void	Update();

	void	ChangeMouseZoomStatus( short zoom );
	D3DXMATRIXA16	GetViewMatrix() { return m_ViewMatrix; }
	D3DXMATRIXA16	GetProjMatrix() { return m_ProjMatrix; }

	D3DXVECTOR3		GetEyePoint() { return m_EyePoint; }

private:
	void	InterpolateCameraRadius();

	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, -0.01f, 1.0f };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };
	
	D3DXVECTOR3		m_NowZoomPoint = { 0, 0, 0 };
	D3DXVECTOR3		m_PrevZoomPoint = { 0, 0, 0 };

	ZoomStatusType	m_ZoomStatus = ZoomStatusType::ZOOM_STATUS_DEGREE_1;
	INT				m_ZoomDegree = 0;
	FLOAT			m_ZoomPointX = 0;
	FLOAT			m_ZoomPointY = 0;
	
	ZoomDirectionType	m_ZoomDirection = ZoomDirectionType::ZOOM_DIRECTION_NONE;
	
	FLOAT			m_Radius = 0;
	D3DXMATRIXA16	m_ViewMatrix;
	D3DXMATRIXA16	m_ProjMatrix;
};