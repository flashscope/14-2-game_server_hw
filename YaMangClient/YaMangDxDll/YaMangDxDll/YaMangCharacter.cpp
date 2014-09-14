#include "stdafx.h"
#include "yaMangDxDll.h"
#include "GlobalVar.h"

//////////////////////////////////////////////////////////////////////////
//캐릭터 mesh를 가져오는 함수
//x file read
//normal vector 생성 등 포함
//////////////////////////////////////////////////////////////////////////
YAMANGDXDLL_API HRESULT InitCharacterModel( HWND hWnd, LPCTSTR fileName, MESHOBJECT* inputVal )
{
	LPD3DXBUFFER D3dxMtrialBuffer;

	//x file import
	if ( FAILED( D3DXLoadMeshFromX( fileName, D3DXMESH_SYSTEMMEM, g_D3dDevice, NULL,
		&D3dxMtrialBuffer, NULL, &( inputVal->NumMaterials ), &inputVal->importedMesh ) ) )
	{
		std::wstring errorFileName = L"Could not find x file:";
		errorFileName += fileName;
		MessageBox( NULL, errorFileName.c_str(), L"YaMang.DLL", MB_OK );
		return E_FAIL;
	}


	//import한 mesh에 normal 값이 없는 경우 normal 계산
	if ( !( ( inputVal->importedMesh )->GetFVF()&D3DFVF_NORMAL ) )
	{
		ID3DXMesh* pTempMesh = 0;
		( inputVal->importedMesh )->CloneMeshFVF( D3DXMESH_MANAGED, ( inputVal->importedMesh )->GetFVF() | D3DFVF_NORMAL, g_D3dDevice, &pTempMesh );

		D3DXComputeNormals( pTempMesh, 0 );

		( inputVal->importedMesh )->Release();
		( inputVal->importedMesh ) = pTempMesh;
	}

	D3DXMATERIAL* d3dxMarteials = (D3DXMATERIAL*)D3dxMtrialBuffer->GetBufferPointer();
	inputVal->MeshMarterials = new D3DMATERIAL9[inputVal->NumMaterials];
	if ( nullptr == inputVal->MeshMarterials )
	{
		return E_OUTOFMEMORY;
	}

	inputVal->MeshTexture = new LPDIRECT3DTEXTURE9[inputVal->NumMaterials];
	if ( nullptr == inputVal->MeshTexture )
	{
		return E_OUTOFMEMORY;
	}

	USES_CONVERSION;
	WCHAR szPath[MAX_PATH] = { 0, };
	swprintf_s( szPath, L"%s", fileName );
	PathRemoveFileSpec( szPath );

	for ( DWORD i = 0; i < inputVal->NumMaterials; ++i )
	{
		inputVal->MeshMarterials[i] = d3dxMarteials[i].MatD3D;

		inputVal->MeshMarterials[i].Ambient = inputVal->MeshMarterials[i].Diffuse;

		inputVal->MeshTexture[i] = NULL;
		if ( d3dxMarteials[i].pTextureFilename && lstrlenA( d3dxMarteials[i].pTextureFilename ) > 0 )
		{
			std::wstring texName = szPath;
			texName += L"\\";
			texName += A2W( d3dxMarteials[i].pTextureFilename );

			if ( FAILED( D3DXCreateTextureFromFile( g_D3dDevice, texName.c_str(), &( inputVal->MeshTexture[i] ) ) ) )
			{
				texName = L"Could not find texture map:" + texName;
				MessageBox( NULL, texName.c_str(), L"YaMang.DLL", MB_OK );
			}
		}
	}

	D3dxMtrialBuffer->Release();

	return S_OK;
}