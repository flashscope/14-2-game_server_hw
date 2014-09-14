#include "stdafx.h"
#include "QuadTree.h"
#include "MacroSet.h"
#include "Frustum.h"
#include "CameraController.h"


QuadTree::QuadTree( int width, int height )
{
	m_Parent = nullptr;

	for ( int i = 0; i < 4; ++i )
	{
		m_Child[i] = nullptr;
		m_Neighbor[i] = nullptr;
	}

	m_Corner[CORNER_TL] = 0;
	m_Corner[CORNER_TR] = width - 1;
	m_Corner[CORNER_BL] = width * ( height - 1 );
	m_Corner[CORNER_BR] = width * height - 1;
	m_Center = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;

	m_Culled = false;
	m_Radius = 0.0f;
}

QuadTree::QuadTree( QuadTree* parent )
{
	m_Parent = parent;
	m_Center = 0;
	for ( int i = 0; i < 4; ++i )
	{
		m_Child[i] = nullptr;
		m_Neighbor[i] = nullptr;
		m_Corner[i] = 0;
	}

	m_Culled = false;
	m_Radius = 0.0f;
}

QuadTree::~QuadTree()
{
	Destroy();
}

BOOL QuadTree::Build( CUSTOMVERTEX* heightMap )
{
	// 쿼드트리 구축
	BuildQuadTree( heightMap );
	// 이웃노드 구축
	BuildNeighborNode( this, heightMap, m_Corner[CORNER_TR] + 1 );

	return TRUE;
}

//	삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다.
int QuadTree::GenerateIndex( LPVOID index, CUSTOMVERTEX* heightMap, Frustum* frustum, float ratioOfLOD )
{
	// 프러스텀 컬링
	FrustumCull( heightMap, frustum );
	// 출력할 인덱스 생성, 폴리곤의 개수 리턴
	return GenTriIndex( 0, index, heightMap, frustum, ratioOfLOD );
}

QuadTree* QuadTree::AddChild( int topLeft, int topRight, int bottomLeft, int bottomRight )
{
	QuadTree* child = new QuadTree( this );
	child->SetCorners( topLeft, topRight, bottomLeft, bottomRight );

	return child;
}

BOOL QuadTree::SetCorners( int topLeft, int topRight, int bottomLeft, int bottomRight )
{
	m_Corner[CORNER_TL] = topLeft;
	m_Corner[CORNER_TR] = topRight;
	m_Corner[CORNER_BL] = bottomLeft;
	m_Corner[CORNER_BR] = bottomRight;
	m_Center = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;
	
	return TRUE;
}

BOOL QuadTree::SubDivide()
{
	int	topEdgeCenter = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] ) / 2;
	int bottomEdgeCenter = ( m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 2;
	int leftEdgeCenter = ( m_Corner[CORNER_TL] + m_Corner[CORNER_BL] ) / 2;
	int rightEdgeCenter = ( m_Corner[CORNER_TR] + m_Corner[CORNER_BR] ) / 2;
	int centralPoint = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] + m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 4;

	// 더이상 분할이 불가능하면 SubDivide() 종료
	if ( ( m_Corner[CORNER_TR] - m_Corner[CORNER_TL] ) <= 1 )
	{
		return FALSE;
	}

	// 4개의 자식노드 추가
	m_Child[CORNER_TL] = AddChild( m_Corner[CORNER_TL], topEdgeCenter, leftEdgeCenter, centralPoint );
	m_Child[CORNER_TR] = AddChild( topEdgeCenter, m_Corner[CORNER_TR], centralPoint, rightEdgeCenter );
	m_Child[CORNER_BL] = AddChild( leftEdgeCenter, centralPoint, m_Corner[CORNER_BL], bottomEdgeCenter );
	m_Child[CORNER_BR] = AddChild( centralPoint, rightEdgeCenter, bottomEdgeCenter, m_Corner[CORNER_BR] );

	return TRUE;
}

int QuadTree::GenTriIndex( int tris, LPVOID index, CUSTOMVERTEX* heightMap, Frustum* frustum, float ratioOfLOD )
{
	if ( m_Culled )
	{
		m_Culled = FALSE;
		return tris;
	}

	LPDWORD p = ( (LPDWORD)index ) + tris * 3;

	if ( IsVisible( heightMap, &(CameraController::GetInstance()->GetEyePoint()), ratioOfLOD ) )
	{
		// 만약 최하위 노드라면 부분 분할(SubDivide)이 불가능하므로 그냥 출력하고 리턴
		if ( m_Corner[CORNER_TR] - m_Corner[CORNER_TL] <= 1 )
		{
			*p++ = m_Corner[0];
			*p++ = m_Corner[1];
			*p++ = m_Corner[2];
			tris++;
			*p++ = m_Corner[2];
			*p++ = m_Corner[1];
			*p++ = m_Corner[3];
			tris++;
			return tris;
		}

		BOOL	b[4] = { 0, 0, 0, 0 };
		// 상단 이웃노드(neightbor node)가 출력가능한가?
		if ( m_Neighbor[EDGE_UP] )
		{
			b[EDGE_UP] = m_Neighbor[EDGE_UP]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// 하단 이웃노드(neightbor node)가 출력가능한가?
		if ( m_Neighbor[EDGE_DN] )
		{
			b[EDGE_DN] = m_Neighbor[EDGE_DN]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// 좌측 이웃노드(neightbor node)가 출력가능한가?
		if ( m_Neighbor[EDGE_LT] )
		{
			b[EDGE_LT] = m_Neighbor[EDGE_LT]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}
		// 우측 이웃노드(neightbor node)가 출력가능한가?
		if ( m_Neighbor[EDGE_RT] )
		{
			b[EDGE_RT] = m_Neighbor[EDGE_RT]->IsVisible( heightMap, frustum->GetPos(), ratioOfLOD );
		}

		// 이웃노드들이 모두다 출력가능하다면 현재노드와 이웃노드들이 같은 LOD이므로 
		// 부분분할이 필요없다.
		if ( b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT] )
		{
			*p++ = m_Corner[0];
			*p++ = m_Corner[1];
			*p++ = m_Corner[2];
			tris++;
			*p++ = m_Corner[2];
			*p++ = m_Corner[1];
			*p++ = m_Corner[3];
			tris++;
			return tris;
		}

		// 상단 부분분할이 필요한가?
		if ( !b[EDGE_UP] )
		{
			int n = ( m_Corner[CORNER_TL] + m_Corner[CORNER_TR] ) / 2;

			*p++ = m_Center;
			*p++ = m_Corner[CORNER_TL];
			*p++ = n; 
			tris++;

			*p++ = m_Center;
			*p++ = n;
			*p++ = m_Corner[CORNER_TR];
			tris++;
		}
		else
		{
			*p++ = m_Center;
			*p++ = m_Corner[CORNER_TL];
			*p++ = m_Corner[CORNER_TR];
			tris++;
		}

		// 하단 부분분할이 필요한가?
		if ( !b[EDGE_DN] )
		{
			int n = ( m_Corner[CORNER_BL] + m_Corner[CORNER_BR] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BR]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_BL]; 
			tris++;
		}
		else
		{
			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BR]; 
			*p++ = m_Corner[CORNER_BL]; 
			tris++;
		}

		if ( !b[EDGE_LT] ) // 좌측 부분분할이 필요한가?
		{
			int n = ( m_Corner[CORNER_TL] + m_Corner[CORNER_BL] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BL]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_TL]; 
			tris++;
		}
		else	// 좌측 부분분할이 필요없을 경우
		{
			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_BL]; 
			*p++ = m_Corner[CORNER_TL]; 
			tris++;
		}

		if ( !b[EDGE_RT] ) // 우측 부분분할이 필요한가?
		{
			int n = ( m_Corner[CORNER_TR] + m_Corner[CORNER_BR] ) / 2;

			*p++ = m_Center; 
			*p++ = m_Corner[CORNER_TR]; 
			*p++ = n; 
			tris++;

			*p++ = m_Center; 
			*p++ = n; 
			*p++ = m_Corner[CORNER_BR]; 
			tris++;
		}
		else	// 우측 부분분할이 필요없을 경우
		{
			*p++ = m_Center;
			*p++ = m_Corner[CORNER_TR];
			*p++ = m_Corner[CORNER_BR];
			tris++;
		}

		return tris;	// 이 노드 아래의 자식노드는 탐색할 필요없으므로 리턴!
	}

	// 자식 노드들 검색
	if ( m_Child[CORNER_TL] )
	{
		tris = m_Child[CORNER_TL]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_TR] )
	{
		tris = m_Child[CORNER_TR]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_BL] )
	{
		tris = m_Child[CORNER_BL]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}
	if ( m_Child[CORNER_BR] )
	{
		tris = m_Child[CORNER_BR]->GenTriIndex( tris, index, heightMap, frustum, ratioOfLOD );
	}

	return tris;
}

void QuadTree::Destroy()
{
	for ( int i = 0; i < 4; ++i )
	{
		SafeDelete( m_Child[i] );
	}
}

void QuadTree::AllIsInFrustum()
{
	m_Culled = FALSE;
	if ( !m_Child[0] )
	{
		return;
	}
	m_Child[0]->AllIsInFrustum();
	m_Child[1]->AllIsInFrustum();
	m_Child[2]->AllIsInFrustum();
	m_Child[3]->AllIsInFrustum();
}

int QuadTree::IsInFrustum( CUSTOMVERTEX* heightMap, Frustum* frustum )
{
	BOOL b[4] = { 0, };
	BOOL isInSphere = frustum->IsInSphere( (D3DXVECTOR3*)( heightMap + m_Center ), m_Radius );

	// 경계구 안에 없으면 점 단위의 프러스텀 테스트가 필요없으므로 리턴
	if ( !isInSphere )
	{
		return FRUSTUM_OUT;
	}

	// 쿼드트리의 4군데 경계 프러스텀 테스트
	b[0] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[0] ) );
	b[1] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[1] ) );
	b[2] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[2] ) );
	b[3] = frustum->IsIn( (D3DXVECTOR3*)( heightMap + m_Corner[3] ) );

	// 4개모두 프러스텀 안에 있음
	if ( ( b[0] + b[1] + b[2] + b[3] ) == 4 )
	{
		return FRUSTUM_COMPLETELY_IN;
	}

	// 일부분이 프러스텀에 있는 경우
	return FRUSTUM_PARTIALLY_IN;
}

void QuadTree::FrustumCull( CUSTOMVERTEX* heightMap, Frustum* frustum )
{
	m_Culled = FALSE;

	int ret = IsInFrustum( heightMap, frustum );
	switch ( ret )
	{
		case FRUSTUM_COMPLETELY_IN:		// 프러스텀에 완전포함, 하위노드 검색 필요 없음
			AllIsInFrustum();
			return;
		case FRUSTUM_PARTIALLY_IN:		// 프러스텀에 일부포함, 하위노드 검색 필요함
			m_Culled = FALSE;
			break;
		case FRUSTUM_OUT:				// 프러스텀에서 완전벗어남, 하위노드 검색 필요 없음
			m_Culled = TRUE;
			return;
	}

	if ( m_Child[0] )
	{
		m_Child[0]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[1] )
	{
		m_Child[1]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[2] )
	{
		m_Child[2]->FrustumCull( heightMap, frustum );
	}
	if ( m_Child[3] )
	{
		m_Child[3]->FrustumCull( heightMap, frustum );
	}
}

// 이웃노드를 만든다.(삼각형 찢어짐 방지용)
void QuadTree::BuildNeighborNode( QuadTree* rootNode, CUSTOMVERTEX* heightMap, int cx )
{
	int	n;
	int	_0, _1, _2, _3;

	for ( int i = 0; i < 4; ++i )
	{
		_0 = m_Corner[0];
		_1 = m_Corner[1];
		_2 = m_Corner[2];
		_3 = m_Corner[3];

		// 이웃노드의 4개 코너값을 얻는다.
		n = GetNodeIndex( i, cx, _0, _1, _2, _3 );
		// 코너값으로 이웃노드의 포인터를 얻어온다.
		if ( n >= 0 )
		{
			m_Neighbor[i] = rootNode->FindNode( heightMap, _0, _1, _2, _3 );
		}
	}

	// 재귀 호출
	if ( m_Child[0] )
	{
		m_Child[0]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[1]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[2]->BuildNeighborNode( rootNode, heightMap, cx );
		m_Child[3]->BuildNeighborNode( rootNode, heightMap, cx );
	}
}

BOOL QuadTree::BuildQuadTree( CUSTOMVERTEX* heightMap )
{
	if ( SubDivide() )
	{
		// 좌측상단과, 우측 하단의 거리를 구한다.
		D3DXVECTOR3 v = *( (D3DXVECTOR3*)( heightMap + m_Corner[CORNER_TL] ) ) -
			*( (D3DXVECTOR3*)( heightMap + m_Corner[CORNER_BR] ) );
		// v의 거리값이 이 노드를 감싸는 경계구의 지름이므로, 
		// 2로 나누어 반지름을 구한다.
		m_Radius = D3DXVec3Length( &v ) / 2.0f;
		m_Child[CORNER_TL]->BuildQuadTree( heightMap );
		m_Child[CORNER_TR]->BuildQuadTree( heightMap );
		m_Child[CORNER_BL]->BuildQuadTree( heightMap );
		m_Child[CORNER_BR]->BuildQuadTree( heightMap );
	}
	return TRUE;
}

QuadTree* QuadTree::FindNode( CUSTOMVERTEX* heightMap, int _0, int _1, int _2, int _3 )
{
	QuadTree* p = nullptr;
	
	if ( ( m_Corner[0] == _0 ) && ( m_Corner[1] == _1 ) && ( m_Corner[2] == _2 ) && ( m_Corner[3] == _3 ) )
	{
		return this;
	}

	if ( m_Child[0] )
	{
		RECT	rc;
		POINT	pt;

		ZeroMemory( &rc, sizeof( RECT ) );
		ZeroMemory( &pt, sizeof( POINT ) );

		int n = ( _0 + _1 + _2 + _3 ) / 4;

		pt.x = (int)heightMap[n].m_VertexPoint.x;
		pt.y = (int)heightMap[n].m_VertexPoint.z;

		// 4개의 코너값을 기준으로 자식노드의 맵 점유범위를 얻는다.
		SetRect( &rc, (int)heightMap[m_Child[0]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[0]->m_Corner[CORNER_BR]].m_VertexPoint.z );

		// pt값이 점유범위안에 있다면 자식노드로 들어간다.
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[0]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[1]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[1]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[1]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[2]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[2]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[2]->FindNode( heightMap, _0, _1, _2, _3 );
		}

		SetRect( &rc, (int)heightMap[m_Child[3]->m_Corner[CORNER_TL]].m_VertexPoint.x,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_TL]].m_VertexPoint.z,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_BR]].m_VertexPoint.x,
				 (int)heightMap[m_Child[3]->m_Corner[CORNER_BR]].m_VertexPoint.z );
		if ( IsInRect( &rc, pt ) )
		{
			return m_Child[3]->FindNode( heightMap, _0, _1, _2, _3 );
		}
	}

	return NULL;
}

int QuadTree::GetNodeIndex( int ed, int cx, int& _0, int& _1, int& _2, int& _3 )
{
	int	n = 0;
	int _a = _0;
	int _b = _1; 
	int _c = _2;
	int _d = _3;
	int gap = _b - _a;	// 현재 노드의 좌우폭값

	switch ( ed )
	{
		case EDGE_UP:	// 위쪽 방향 이웃노드의 인덱스
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// 아래 방향 이웃노드의 인덱스
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// 좌측 방향 이웃노드의 인덱스
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// 우측 방향 이웃노드의 인덱스
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if ( ( _0 / cx ) != ( _a / cx ) )
			{
				return -1;
			}
			if ( ( _1 / cx ) != ( _b / cx ) )
			{
				return -1;
			}
			break;
	}

	n = ( _0 + _1 + _2 + _3 ) / 4;	// 가운데 인덱스
	if ( !IS_IN_RANGE( n, 0, cx * cx - 1 ) )
	{
		return -1;
	}

	return n;
}

// 2를 밑으로 하는 숫자 n의 로그값을 구한다.
int	Log2( int n )
{
	for ( int i = 1; i < 64; ++i )
	{
		n = n >> 1;
		if ( n == 1 )
		{
			return i;
		}
	}

	return 1;
}

// 2^n값을 구한다
int	Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
BOOL IsInRect( RECT* rc, POINT pt )
{
	if ( ( rc->left <= pt.x ) && ( pt.x <= rc->right ) &&
		 ( rc->bottom <= pt.y ) && ( pt.y <= rc->top ) )
	{
		return TRUE;
	}

	return FALSE;
}
