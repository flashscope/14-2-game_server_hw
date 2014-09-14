#include "stdafx.h"
#include "Interface3D.h"


Interface3D::Interface3D()
{
}


Interface3D::~Interface3D()
{
}

D3DXMATRIXA16 Interface3D::GetMatrix( bool leftHand /* = true */ ) const
{
	D3DXMATRIXA16 rotateMatrix;
	if ( leftHand )
	{
		D3DXMatrixLookAtLH( &rotateMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	}
	else
	{
		D3DXMatrixLookAtRH( &rotateMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	}
	rotateMatrix._41 = rotateMatrix._42 = rotateMatrix._43 = 0.0f;
	D3DXMatrixTranspose( &rotateMatrix, &rotateMatrix );

	D3DXMATRIXA16 transMatrix;
	D3DXMatrixTranslation( &transMatrix, m_EyePoint.x, m_EyePoint.y, m_EyePoint.z );

	D3DXMATRIXA16 scaleMatrix;
	D3DXMatrixScaling( &scaleMatrix, m_Scale.x, m_Scale.y, m_Scale.z );

	D3DXMATRIXA16 worldMatrix;
	D3DXMatrixIdentity( &worldMatrix );

	worldMatrix = scaleMatrix * rotateMatrix * transMatrix;
	
	return worldMatrix;
}
