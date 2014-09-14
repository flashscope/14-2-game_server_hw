#include <d3dx9.h>

struct CUSTOMVERTEX
{
	D3DXVECTOR3	m_VertexPoint = { 0, 0, 0 };
	DWORD		m_Diffuse = 0;
	D3DXVECTOR2	m_VertexTexturePoint = { 0, 0 };
};

//마지막에는 각 texturepoint의 차원과 index를 의미
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )

struct BILLBOARDVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	float px, py, pz;
	float tu, tv;
};

struct SKYBOXVERTEX
{
	SKYBOXVERTEX() {}
	SKYBOXVERTEX( float x, float y, float z, float u, float v )
	{
		m_X = x;
		m_Y = y;
		m_Z = z;
		m_U = u;
		m_V = v;
	}

	float	m_X = 0;
	float	m_Y = 0;
	float	m_Z = 0;
	float	m_U = 0;
	float	m_V = 0;
};
#define D3DFVF_SKYBOXVERTEX (D3DFVF_XYZ | D3DFVF_TEX1)

struct MYINDEX
{
	UINT _0, _1, _2;
};

#define MAP_TEXTURE_COUNT 5

//////////////////////////////////////////////////////////////////////////
// Global val 선언
//////////////////////////////////////////////////////////////////////////
// 부모 Process 공용 자원
extern LPDIRECT3D9 g_D3D;
extern LPDIRECT3DDEVICE9 g_D3dDevice;

//////////////////////////////////////////////////////////////////////////
// Height Map 생성용 공용 자원
//////////////////////////////////////////////////////////////////////////
extern LPD3DXMESH g_Mesh;

// 클라이언트용 textureload 함수
extern LPDIRECT3DTEXTURE9	g_MapHeightInfo;
extern LPDIRECT3DTEXTURE9	g_MapTexture;

extern LPDIRECT3DTEXTURE9*	g_MeshTextures;
extern INT					g_MeshTextureSize;

//////////////////////////////////////////////////////////////////////////
// 쿼드트리용
// 하이트맵 버텍스 배열
extern CUSTOMVERTEX* g_HeightMap;
extern DWORD g_HeightMapWidth;
extern DWORD g_HeightMapHeight;

// 버텍스 버퍼
extern LPDIRECT3DVERTEXBUFFER9	g_HeightMapVertexBuffer;
// 인덱스 버퍼
extern LPDIRECT3DINDEXBUFFER9	g_HeightMapIndexBuffer;
extern bool g_HeightMapWithQuadTreeIsReady;

// 맵툴용 Texture array
extern LPDIRECT3DTEXTURE9 g_MapTextureArray[MAP_TEXTURE_COUNT];

extern DWORD g_XHeight;
extern DWORD g_ZHeight;

//////////////////////////////////////////////////////////////////////////
// 텍스트 출력에 필요한 자원
//////////////////////////////////////////////////////////////////////////
extern ID3DXFont*	g_Font;		// 글자를 그릴 폰트 스타일 객체
extern ID3DXSprite*	g_Sprite;		// 폰트를 그릴 스프라이트 객체 

//////////////////////////////////////////////////////////////////////////
// D3D cursor Set에 필요한 자원
//////////////////////////////////////////////////////////////////////////
// extern IDirect3DSurface9* g_surfcursor; //테스트 중
// extern IDirect3DTexture9* g_cursortex; //테스트 중

extern LPDIRECT3DTEXTURE9*	g_CursorTex;
extern LPD3DXSPRITE			g_CursorSprite;
extern INT					g_CursorType;
extern INT					g_CursorMaxSize;
extern D3DXVECTOR3			g_CursorPos;

//////////////////////////////////////////////////////////////////////////
// tool Camera 관련 벡터
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_EyePoint;
extern D3DXVECTOR3		g_LookAtPoint;
extern D3DXVECTOR3		g_UpVector;

//////////////////////////////////////////////////////////////////////////
// peeking 관련 변수
//////////////////////////////////////////////////////////////////////////
extern D3DXVECTOR3		g_RayOrigin;
extern D3DXVECTOR3		g_RayDirection;

//////////////////////////////////////////////////////////////////////////
// 화면 종횡비
//////////////////////////////////////////////////////////////////////////
extern float			g_StartWidth;
extern float			g_StartHeight;
extern float			g_NowWidth;
extern float			g_NowHeight;

//////////////////////////////////////////////////////////////////////////
// SKYBOX
//////////////////////////////////////////////////////////////////////////
extern LPDIRECT3DTEXTURE9	g_SkyBoxTextures[6];
extern LPD3DXMESH			g_SkyBoxMesh;

//////////////////////////////////////////////////////////////////////////
// UI SPRITE
//////////////////////////////////////////////////////////////////////////
extern LPDIRECT3DTEXTURE9*	g_UITexures;
extern LPD3DXSPRITE			g_UISprite;
extern INT					g_UIMaxSize;

//////////////////////////////////////////////////////////////////////////
// 셰이더
//////////////////////////////////////////////////////////////////////////
extern LPD3DXEFFECT*		g_Effects;
extern int					g_EffectSize;
extern bool					g_IsEffectReady;