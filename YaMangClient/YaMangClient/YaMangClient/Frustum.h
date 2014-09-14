#pragma once

// 프러스텀에 정확하게 포함되지 않더라도, 약간의 여분을 주어서 프러스텀에 포함시키기 위한 값
#define PLANE_EPSILON	5.0f

class Frustum
{
public:
	Frustum();
	~Frustum();

	BOOL			Make( D3DXMATRIXA16* viewProjMatrix );

	BOOL			IsIn( D3DXVECTOR3* point );
	BOOL			IsInSphere( D3DXVECTOR3* point, float radius );

	D3DXVECTOR3*	GetPos() { return &m_ViewerPosition; }

private:
	D3DXVECTOR3		m_Vertex[8];		// 프러스텀을 구성할 정점 8개
	D3DXVECTOR3		m_ViewerPosition;	// 현재 카메라의 월드좌표
	D3DXPLANE		m_Plane[6];			// 프러스텀을 구성하는 6개의 평면
};