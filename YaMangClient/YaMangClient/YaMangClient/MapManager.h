#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

struct CUSTOMVERTEX;

class MapManager: public Singleton<MapManager>
{
public:
	MapManager();
	~MapManager();

	float	GetHeightByPosition( D3DXVECTOR3 pos ) { return GetHeightByPosition( pos.x, pos.z ); }
	float	GetHeightByPosition( float x, float z );

	void	SetHeightMap( CUSTOMVERTEX* heightMap ) { m_HeightMap = heightMap; }
	void	SetHeightMapSize( int width, int height ) { m_HeightMapWidth = width; m_HeightMapHeight = height; };
	void	SetPixelSize( float size ) { m_PixelSize = size; }

private:
	float	GetHeightInMap( int row, int col );
	float	Lerp( float a, float b, float t );

	int		m_HeightMapWidth = 0;
	int		m_HeightMapHeight = 0;
	float	m_PixelSize = 0.0f;

	CUSTOMVERTEX*	m_HeightMap = nullptr;
};