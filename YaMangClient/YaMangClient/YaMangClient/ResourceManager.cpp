#include "stdafx.h"
#include "ResourceManager.h"
#include "MainWindow.h"
#include "YaMangDxDll.h"
#include "MouseManager.h"
#include "ResourceDef.h"
#include "MapManager.h"

ResourceManager::ResourceManager()
{
	m_MeshArray.fill( nullptr );
	m_HeightMapArray.fill( nullptr );
}


ResourceManager::~ResourceManager()
{
}

void ResourceManager::Init()
{
	AddMesh( MODEL_UNIT_SWORD, MESH_KEY_UNIT_SWORD );
	AddMesh( MODEL_UNIT_PIKE, MESH_KEY_UNIT_PIKE );
	AddMesh( MODEL_UNIT_ARROW, MESH_KEY_UNIT_ARROW );
	AddMesh( MODEL_UNIT_KNIGHT, MESH_KEY_UNIT_KNIGHT );
	AddMesh( MODEL_UNIT_GUARD, MESH_KEY_UNIT_GUARD );
	AddMesh( MODEL_UNIT_KING, MESH_KEY_UNIT_KING );

	AddMesh( MODEL_CORPS_DESTROY_A, MESH_KEY_CORPS_DESTROY_ENEMY );
	AddMesh( MODEL_CORPS_DEFENSE_A, MESH_KEY_CORPS_DEFENSE_ENEMY );
	AddMesh( MODEL_CORPS_RUSH_A, MESH_KEY_CORPS_RUSH_ENEMY );
	AddMesh( MODEL_CORPS_DESTROY_B, MESH_KEY_CORPS_DESTROY_MINE );
	AddMesh( MODEL_CORPS_DEFENSE_B, MESH_KEY_CORPS_DEFENSE_MINE );
	AddMesh( MODEL_CORPS_RUSH_B, MESH_KEY_CORPS_RUSH_MINE );

	AddMap( HEIGHT_MAP, HEIGHT_TEX, MAP_KEY_TEST );
	CreateMap( MAP_KEY_TEST );

	//////////////////////////////////////////////////////////////////////////
	// 쿼드트리를 적용하기 위해 하이트 맵은 2^n 형태의 크기로 만들어야 합니다.
	//
	// 주의! 실제 하이트맵 크기는 한 변이 2^n + 1 사이즈여야 합니다!
	//////////////////////////////////////////////////////////////////////////
	InitGroundMesh( 128, 128 );
	CreateRawGround( 128, 128, 5.0f );

	if ( FAILED( InitHeightMap( HEIGHT_MAP, 5.0f ) ) )
	{
		m_IsMapForQuadTreeReady = false;
	}
	else
	{
		m_IsMapForQuadTreeReady = true;
	}

	MapManager::GetInstance()->SetPixelSize( 5.0f );

	m_MapSize = 640;
	m_IsMapReady = true;

	InitSkyBoxMesh( 640 );

	SetSkyBoxTexture( SPRITE_SKYBOX_BACK, SKY_BOX_BACK );
	SetSkyBoxTexture( SPRITE_SKYBOX_FRONT, SKY_BOX_FRONT );
	SetSkyBoxTexture( SPRITE_SKYBOX_LEFT, SKY_BOX_LEFT );
	SetSkyBoxTexture( SPRITE_SKYBOX_RIGHT, SKY_BOX_RIGHT );
	SetSkyBoxTexture( SPRITE_SKYBOX_TOP, SKY_BOX_TOP );
	SetSkyBoxTexture( SPRITE_SKYBOX_BOTTOM, SKY_BOX_BOTTOM );
	
	InitCursor( CURSOR_MAX, MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	CreateCursorImage( SPRITE_CURSOR_DEFAULT, CURSOR_DEFAULT );
	CreateCursorImage( SPRITE_CURSOR_ATTACK, CURSOR_ATTACK );
	CreateCursorImage( SPRITE_CURSOR_CLICK, CURSOR_CLICK );
	CreateCursorImage( SPRITE_CURSOR_OVER_CORPS, CURSOR_OVER_CORPS );
	CreateCursorImage( SPRITE_CURSOR_UNRECHEABLE, CURSOR_UNRECHEABLE );
	CreateCursorImage( SPRITE_CURSOR_CAMERA_ROTATING, CURSOR_CAMERA_ROTATING );
	CreateCursorImage( SPRITE_CURSOR_CORPS_MOVABLE, CURSOR_CORPS_MOVABLE );
	CreateCursorImage( SPRITE_CURSOR_CORPS_MOVABLE_CLICK, CURSOR_CORPS_MOVABLE_CLICK );
	CreateCursorImage( SPRITE_CURSOR_OVER_PICKED_CORPS, CURSOR_OVER_PICKED_CORPS );

	m_ISCursorReady = true;

	CreateSprite();

	ShaderCreate( SHADER_TYPE_MAX );
	ShaderImport( SELECT_SHADER, SHADER_SELECT );
	ShaderImport( FIGHT_SHADER, SHADER_FIGHT );
	ShaderImport( MAP_SHADER, SHADER_MAP );

	MeshTextureCreateBySize( CORPS_TEXTURE_MAX );
	MeshTextureImport( SPRITE_CORPS_TYPE_ARROW, CORPS_TEXTURE_ARROW );
	MeshTextureImport( SPRITE_CORPS_TYPE_GUARD, CORPS_TEXTURE_GUARD );
	MeshTextureImport( SPRITE_CORPS_TYPE_KING, CORPS_TEXTURE_KING );
	MeshTextureImport( SPRITE_CORPS_TYPE_KNIGHT, CORPS_TEXTURE_KNIGHT );
	MeshTextureImport( SPRITE_CORPS_TYPE_PIKE, CORPS_TEXTURE_PIKE );
	MeshTextureImport( SPRITE_CORPS_TYPE_SWORD, CORPS_TEXTURE_SWORD );
}

void ResourceManager::Destroy()
{
	DeleteSprite();
	CursorCleanUp();

	SkyBoxCleanUp();
	DeleteMap();

	for ( auto& toBeDelete : m_MeshArray )
	{
		if ( toBeDelete )
		{
			DeleteMesh( toBeDelete->m_MeshObject );
			SafeDelete( toBeDelete->m_MeshObject );
			delete toBeDelete;
		}
	}
	for ( auto& toBeDelete : m_HeightMapArray )
	{
		SafeDelete( toBeDelete );
	}
}

void ResourceManager::AddMap( LPCTSTR heightMapFileName, LPCTSTR textureFileName, MapKeyType key )
{
	SafeDelete( m_HeightMapArray[key] );
		
	ResourceMap* map = new ResourceMap();
	m_HeightMapArray[key] = map;

	map->m_HeightMap.append( heightMapFileName );
	map->m_TextureMap.append( textureFileName );

	m_IsMapReady = CreateMap( key );
}

bool ResourceManager::CreateMap( MapKeyType key )
{
	if ( m_HeightMapArray[key] )
	{
		return ( S_OK == HeightMapTextureImport(
			MainWindow::GetInstance()->Window(), m_HeightMapArray[key]->m_HeightMap.c_str(), m_HeightMapArray[key]->m_TextureMap.c_str() ) );
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// 우선은 이렇게 해 두지만,
// 나중에 맵이 여러 종류가 될 경우
// 씬 쪽에서 필요한 맵들을 불러오고 제거할 수 있게 되어야 한다.
//////////////////////////////////////////////////////////////////////////
void ResourceManager::DeleteMap()
{
	if ( m_IsMapReady )
	{
		HeightMapCleanup();
	}
	m_IsMapReady = false;

	MapManager::Release();
}

bool ResourceManager::AddMesh( LPCTSTR fileName, MeshKeyType key )
{
	ResourceMesh* mesh = new ResourceMesh();
	mesh->m_MeshObject = new MESHOBJECT();

	CreateMesh( fileName, mesh->m_MeshObject );

	if ( mesh->m_MeshObject )
	{
		SafeDelete( m_MeshArray[key] );
		m_MeshArray[key] = mesh;

		return true;
	}
	
	SafeDelete( mesh->m_MeshObject );
	SafeDelete( mesh );
	return false;
}

void ResourceManager::CreateMesh( LPCTSTR fileName, MESHOBJECT* mesh )
{
	if ( S_OK == InitCharacterModel( MainWindow::GetInstance( )->Window( ), fileName, mesh ) )
	{
		return;
	}
	else mesh = nullptr;
}

void ResourceManager::DeleteMesh( MESHOBJECT* mesh )
{
	if ( mesh )
	{
		MeshObjectCleanUp( mesh );
	}
}

void ResourceManager::CreateSprite()
{
	m_IsUISpriteReady = true;
	InitUISprite( SPRITE_MAX );

	CreateSpriteImage( SPRITE_IMAGE_LOADING, SPRITE_LOADING );
	CreateSpriteImage( SPRITE_IMAGE_RESULT_WIN, SPRITE_RESULT_WIN );
	CreateSpriteImage( SPRITE_IMAGE_RESULT_LOSE, SPRITE_RESULT_LOSE );

	//CreateUIImage
	CreateSpriteImage(SPRITE_UI_MINIMAP, SPRITE_UI_MAP);
	CreateSpriteImage( SPRITE_UI_REGENBAR, SPRITE_UI_REGEN_BAR );
	CreateSpriteImage( SPRITE_UI_REGENFLAG, SPRITE_UI_REGEN_FLAG );
	CreateSpriteImage( SPRITE_UI_CORPS_MARK_BLUE, SPRITE_UI_CORPSMARK_BLUE );
	CreateSpriteImage( SPRITE_UI_CORPS_MARK_RED, SPRITE_UI_CORPSMARK_RED );
	CreateSpriteImage( SPRITE_UI_CORPS_FLAG_GRAY, SPRITE_UI_CORPSFLAG_GRAY );

}

void ResourceManager::DeleteSprite()
{
	UICleanUp();
}

