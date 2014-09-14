#pragma once
#include "MacroSet.h"
#include "EnumSet.h"

struct MESHOBJECT;

enum SkyBoxType
{
	// 순서 중요!
	SKY_BOX_BACK	= 0,
	SKY_BOX_FRONT	= 1,
	SKY_BOX_LEFT	= 2,
	SKY_BOX_RIGHT	= 3,
	SKY_BOX_TOP		= 4,
	SKY_BOX_BOTTOM	= 5
};

enum ShaderType
{
	// 순서 중요!
	SHADER_SELECT = 0,
	SHADER_FIGHT = 1,
	SHADER_MAP = 2,

	SHADER_TYPE_MAX
};

struct Resource
{
	Resource(): m_Type( RESOURCE_NONE ) {}
	ResourceType m_Type;
};

struct ResourceMesh: public Resource
{
	ResourceMesh()
	{
		m_Type = RESOURCE_MESH;
	}
	MESHOBJECT*	m_MeshObject = nullptr;
};

struct ResourceMap: public Resource
{
	ResourceMap()
	{
		m_Type = RESOURCE_MAP;
	}
	std::wstring	m_HeightMap;
	std::wstring	m_TextureMap;
};

class ResourceManager: public Singleton<ResourceManager>
{
public:
	ResourceManager();
	~ResourceManager();

	void	Init();
	void	Destroy();
	bool	IsMapReady() const { return m_IsMapReady; }
	bool	IsMapForQuadTreeReady() const { return m_IsMapForQuadTreeReady; }

	int		GetMapSize() const { return m_MapSize; }
	bool	IsCursorReady() const { return m_ISCursorReady; }
	
	ResourceMesh*	GetMeshByKey( MeshKeyType key ) const { return m_MeshArray[key]; }

private:
	void	AddMap( LPCTSTR heightMapFileName, LPCTSTR textureFileName, MapKeyType key );
	bool	CreateMap( MapKeyType key );
	void	DeleteMap();

	bool	AddMesh( LPCTSTR fileName, MeshKeyType key );
	void	CreateMesh( LPCTSTR fileName, MESHOBJECT* mesh );
	void	DeleteMesh( MESHOBJECT* mesh );

	void	CreateSprite();
	void	DeleteSprite();

	std::array<ResourceMesh*, MESH_KEY_MAX>	m_MeshArray;
	std::array<ResourceMap*, MAP_KEY_MAX> m_HeightMapArray;

	bool	m_IsMapReady = false;
	bool	m_IsMapForQuadTreeReady = false;

	int		m_MapSize = 0;

	bool	m_ISCursorReady = false;
	bool	m_IsUISpriteReady = false;
};