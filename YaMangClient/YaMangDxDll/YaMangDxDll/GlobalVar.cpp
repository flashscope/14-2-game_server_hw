#include "stdafx.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
// 공용 변수 초기화 영역
//////////////////////////////////////////////////////////////////////////
// 부모 Process 공용 자원
LPDIRECT3D9 g_D3D = nullptr;
LPDIRECT3DDEVICE9 g_D3dDevice = nullptr;

// Height Map 생성용 공용 자원
LPD3DXMESH g_Mesh = nullptr;

// 쿼드트리용
CUSTOMVERTEX* g_HeightMap = nullptr;
DWORD g_HeightMapWidth = 0;
DWORD g_HeightMapHeight = 0;
LPDIRECT3DVERTEXBUFFER9	g_HeightMapVertexBuffer = nullptr;
LPDIRECT3DINDEXBUFFER9	g_HeightMapIndexBuffer = nullptr;
bool g_HeightMapWithQuadTreeIsReady = false;

LPDIRECT3DTEXTURE9	g_MapHeightInfo = nullptr;
LPDIRECT3DTEXTURE9	g_MapTexture = nullptr;
LPDIRECT3DTEXTURE9	g_MapTextureArray[MAP_TEXTURE_COUNT] = { 0, };

LPDIRECT3DTEXTURE9*	g_MeshTextures = nullptr;
INT					g_MeshTextureSize = 0;

DWORD g_XHeight = 0;
DWORD g_ZHeight = 0;

// 텍스트 출력에 필요한 자원
ID3DXFont*		g_Font = nullptr;		// 글자를 그릴 폰트 스타일 객체
ID3DXSprite*	g_Sprite = nullptr;		// 폰트를 그릴 스프라이트 객체 

// D3D cursor Set에 필요한 자원
LPDIRECT3DTEXTURE9*		g_CursorTex = nullptr;
LPD3DXSPRITE			g_CursorSprite = nullptr;
INT						g_CursorType = 0;
INT						g_CursorMaxSize = 0;
D3DXVECTOR3				g_CursorPos = { 0, 0, 0 };

// tool Camera 관련 벡터
D3DXVECTOR3		g_EyePoint = { 0, 0, 0 };
D3DXVECTOR3		g_LookAtPoint = { 0, 0, 1.0f };
D3DXVECTOR3		g_UpVector = { 0, 1, 0 };

// Picking 관련 변수
D3DXVECTOR3		g_RayOrigin = { 0, 0, 0 };
D3DXVECTOR3		g_RayDirection = { 0, 0, 0 };

// 화면 종횡비 관련 변수
float			g_StartWidth = 1280.0f;
float			g_StartHeight = 720.0f;
float			g_NowWidth = 1280.0f;
float			g_NowHeight = 720.0f;

// 스카이 박스
LPDIRECT3DTEXTURE9	g_SkyBoxTextures[6] = { 0, };
LPD3DXMESH          g_SkyBoxMesh = nullptr;

// UI Sprite
LPDIRECT3DTEXTURE9*		g_UITexures = nullptr;
LPD3DXSPRITE			g_UISprite = nullptr;
INT						g_UIMaxSize = 0;

// 셰이더
// fx파일을 사용하기 위한 인터페이스
LPD3DXEFFECT*		g_Effects = nullptr;
int					g_EffectSize = 0;
bool				g_IsEffectReady = false;