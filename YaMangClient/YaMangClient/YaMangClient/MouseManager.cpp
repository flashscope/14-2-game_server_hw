#include "stdafx.h"
#include "MainWindow.h"
#include "YaMangDxDll.h"
#include "SceneManager.h"
#include "ScenePlay.h"
#include "PlayerManager.h"
#include "Corps.h"
#include "MouseManager.h"
#include "NetworkManager.h"
#include "CameraController.h"
#include "Timer.h"
#include "Logger.h"
#include "ResourceManager.h"

MouseManager::MouseManager()
{
	RECT rect;
	GetWindowRect( MainWindow::GetInstance()->Window(), &rect );
	m_Boundary.X = static_cast<SHORT>( rect.right - rect.left );
	m_Boundary.Y = static_cast<SHORT>( rect.bottom - rect.top );

	m_WndLocationX = static_cast<int>( rect.left );
	m_WndLocationY = static_cast<int>( rect.top );

	m_WndXPos = m_PressedMousePosition.X = m_MousePosition.X = m_Boundary.X / 2;
	m_WndYPos = m_PressedMousePosition.Y = m_MousePosition.Y = m_Boundary.Y / 2;
}

MouseManager::~MouseManager()
{
}

void MouseManager::MoveMousePosition( int x, int y )
{
	if ( !m_GameCursorMod )
	{
		return;
	}

	m_CursorType = CURSOR_DEFAULT;

	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;

	//이젠 드래그 시작 하면 드래그 시작 좌표와 비교해서 회전하도록 한다
	if ( m_IsRightDragging )
	{
		m_CursorType = CURSOR_CAMERA_ROTATING;
		if ( m_PressedMousePosition.X > x )
		{
			CameraController::GetInstance()->RotateSide( -0.1f );
			SetDragStartPoint( x, y );
			//SetMousePosition(m_PressedMousePosition.X, m_PressedMousePosition.Y);
		}

		if ( m_PressedMousePosition.X < x )
		{
			CameraController::GetInstance()->RotateSide( 0.1f );
			SetDragStartPoint( x, y );
			//SetMousePosition(m_PressedMousePosition.X, m_PressedMousePosition.Y);
		}

		return;
	}
	
	//윈도 커서 절대좌표로 커서 위치를 지정
	m_MousePosition.X = x;
	m_MousePosition.Y = y;

	//일단 창밖으로 나가는거 값 때려박음
	if ( m_MousePosition.X > m_Boundary.X - 35 )
	{
		m_MousePosition.X = m_Boundary.X - 35;
		SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
	}
	if ( m_MousePosition.Y > m_Boundary.Y - 35 )
	{
		m_MousePosition.Y = m_Boundary.Y - 35;
		SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
	}
	if ( m_MousePosition.X < 3 )
	{
		m_MousePosition.X = 3;
		SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
	}
	if ( m_MousePosition.Y < 5 )
	{
		m_MousePosition.Y = 5;
		SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
	}

	// 처음 클릭 시작된 점과 일정 거리 이상 떨어졌을 경우 드래그 상태로 전환
	if ( m_IsLeftClicked )
	{
		m_CursorType = CURSOR_CLICK;

		if ( GetDistanceBetweenCOORD( m_MousePosition, m_PressedMousePosition ) > 3.f )
		{
			m_IsLeftDragging = true;
		}
		return;
	}

	if ( m_IsRightClicked )
	{
		if ( GetDistanceBetweenCOORD( m_MousePosition, m_PressedMousePosition ) > 3.f )
		{
			m_IsRightDragging = true;
		}
	}

	if ( PlayerManager::GetInstance()->IsSelectedCorps() )
	{
		float pickedX = 0;
		float pickedZ = 0;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();

		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );

		HRESULT hr = S_OK;

		if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
		{
			hr = TransPickedTriangleQuadTree( &pickedX, &pickedZ );
		}
		else
		{
			hr = TransPickedTriangle( &pickedX, &pickedZ );
		}
		if ( S_FALSE != hr )
		{
			Corps* pickedCorps = static_cast<ScenePlay*>( scene )->SearchCorpsByPosition( pickedX, pickedZ, false );

			if ( pickedCorps )
			{
				if ( PlayerManager::GetInstance()->IsCorpsInIdList( pickedCorps->GetCorpsID() ) )
				{
					// Log( "현재 선택 중인 부대를 가리킴 \n" );
					m_CursorType = CURSOR_OVER_PICKED_CORPS;
				}
				// 여기에 플레이어의 유닛이 아닌가 확인하는 코드 넣어야 됨
				else if ( pickedCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
				{
					// Log( "결과 - 부대 번호 : %d, 부대 타입 : %d - 공격 가능! \n", pickedCorps->GetCorpsID(), static_cast<int>( pickedCorps->GetUnitType() ) );
					m_CursorType = CURSOR_ATTACK;
				}
				else
				{
					// Log( "내 소속 부대를 가리킴 \n" );
					m_CursorType = CURSOR_OVER_CORPS;
				}
			}
			else if ( !m_IsRightDragging )
			{
				// Log( "결과 - 부대 없음! 해당 좌표로 이동 가능\n" );
				m_CursorType = CURSOR_CORPS_MOVABLE;

				if ( m_IsRightClicked )
				{
					// Log( "이동한다!\n" );
					m_CursorType = CURSOR_CORPS_MOVABLE_CLICK;
				}
			}
		}
	}
	else
	{
		float pickedX = 0;
		float pickedZ = 0;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();

		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );

		HRESULT hr = S_OK;

		if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
		{
			hr = TransPickedTriangleQuadTree( &pickedX, &pickedZ );
		}
		else
		{
			hr = TransPickedTriangle( &pickedX, &pickedZ );
		}
		if ( S_FALSE != hr )
		{
			Corps* pickedCorps = static_cast<ScenePlay*>( scene )->SearchCorpsByPosition( pickedX, pickedZ, false );

			if ( pickedCorps )
			{
				if ( UnitType::UNIT_GUARD == pickedCorps->GetUnitType( ) || UnitType::UNIT_KING == pickedCorps->GetUnitType( ) ||
					 pickedCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
				{
					// Log( "클릭 선택 할 수 없음! \n" );
					m_CursorType = CURSOR_UNRECHEABLE;
				}
				else
				{
					// Log( "선택 가능한 내 부대!\n" );
					m_CursorType = CURSOR_OVER_CORPS;
				}
			}
			else
			{
				// Log( "결과 - 부대 없음! 기본 커서!\n" );
				m_CursorType = CURSOR_DEFAULT;
			}
		}
	}
}

void MouseManager::MoveHiddenCursorToCenter()
{
	if ( !m_GameCursorMod )
	{
		return;
	}
	SetCursorPos( m_WndXPos, m_WndYPos );
}

void MouseManager::MoveHiddenCursorToMousePos()
{
	SetCursorPos( m_MousePosition.X, m_MousePosition.Y );
}

void MouseManager::SetLeftClick( bool isclicked )
{
	m_IsLeftClicked = isclicked;

	if ( isclicked ) //버튼 다운 시
	{
		float pickedX = 0;
		float pickedZ = 0;

		Scene* scene = SceneManager::GetInstance()->GetNowScene();

		if ( typeid( ScenePlay ) != typeid( *scene ) )
		{
			return;
		}

		PlayerManager::GetInstance()->ClearSelectedCorps();
		CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );

		HRESULT hr = S_OK;

		if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
		{
			hr = TransPickedTriangleQuadTree( &pickedX, &pickedZ );
		}
		else
		{
			hr = TransPickedTriangle( &pickedX, &pickedZ );
		}
		if ( S_FALSE != hr )
		{
			Corps* pickedCorps = static_cast<ScenePlay*>( scene )->SearchCorpsByPosition( pickedX, pickedZ );

			Log( "[%d %d] -> [%f, %f] 으로 피킹! \n", m_MousePosition.X, m_MousePosition.Y, pickedX, pickedZ );

			if ( pickedCorps )
			{
				// 여기에 플레이어의 유닛이 아닌가 확인하는 코드 넣어야 됨
				if ( UnitType::UNIT_GUARD == pickedCorps->GetUnitType( ) || UnitType::UNIT_KING == pickedCorps->GetUnitType( ) ||
					 pickedCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
				{
					// Log( "피킹한 놈 %d, 내 아이디 %d \n",
					//	 pickedCorps->GetOwnPlayerID(), NetworkManager::GetInstance()->GetMyPlayerID() );
					Log( "클릭 선택 할 수 없음! \n" );
					pickedCorps->SetSelected( false );
				}
				else
				{
					Log( "결과 - 부대 번호 : %d, 부대 타입 : %d \n", pickedCorps->GetCorpsID(), static_cast<int>( pickedCorps->GetUnitType() ) );
					PlayerManager::GetInstance()->AddToSelectedCorps( pickedCorps->GetCorpsID() );
				}
			}
			else
			{
				Log( "결과 - 부대 없음! \n" );
			}
		}
		//드래그 시작 포인트 저장 ; 첫 DOWN시 한번만 실행되므로
		SetDragStartPoint( m_MousePosition.X, m_MousePosition.Y );
	}
	else //버튼 업시
	{
		if ( m_IsLeftDragging )
		{
			SetLeftDrag();
		}
	}
}

void MouseManager::SetRightClick( bool isclicked )
{
	m_IsRightClicked = isclicked;

	//오른쪽 마우스 클릭
	if ( isclicked ) //버튼 다운 시
	{
		//드래그 시작 포인트 저장 ; 첫 DOWN시 한번만 실행되므로
		SetDragStartPoint( m_MousePosition.X, m_MousePosition.Y );
	}
	else //버튼 업시
	{
		if ( m_IsRightDragging )
		{
			SetRightDrag();
			return;
		}

		if ( PlayerManager::GetInstance()->IsSelectedCorps() )
		{
			float pickedX = 0;
			float pickedZ = 0;

			Scene* scene = SceneManager::GetInstance()->GetNowScene();

			if ( typeid( ScenePlay ) != typeid( *scene ) )
			{
				return;
			}

			CalcPickingRay( m_MousePosition.X, m_MousePosition.Y );

			HRESULT hr = S_OK;

			if ( ResourceManager::GetInstance()->IsMapForQuadTreeReady() )
			{
				hr = TransPickedTriangleQuadTree( &pickedX, &pickedZ );
			}
			else
			{
				hr = TransPickedTriangle( &pickedX, &pickedZ );
			}
			if ( S_FALSE != hr )
			{
				Corps* pickedCorps = static_cast<ScenePlay*>( scene )->SearchCorpsByPosition( pickedX, pickedZ, false );

				Log( "[%d %d] -> [%f, %f] 으로 우클릭 피킹! \n", m_MousePosition.X, m_MousePosition.Y, pickedX, pickedZ );

				if ( pickedCorps )
				{
					if ( PlayerManager::GetInstance()->IsCorpsInIdList( pickedCorps->GetCorpsID() ) )
					{
						Log( "자기 자신 클릭! \n" );
					}
					else if ( pickedCorps->GetOwnPlayerID() != NetworkManager::GetInstance()->GetMyPlayerID() )
					{
						Log( "결과 - 부대 번호 : %d, 부대 타입 : %d - 공격! \n", pickedCorps->GetCorpsID(), static_cast<int>( pickedCorps->GetUnitType() ) );

						PlayerManager::GetInstance()->AttackCorpsById( pickedCorps->GetCorpsID() );
					}
				}
				else if ( !m_IsRightDragging )
				{
					Log( "결과 - 부대 없음! 해당 좌표로 이동!\n" );

					PlayerManager::GetInstance()->MoveCorpsToPosition( pickedX, pickedZ );
					m_CursorType = CURSOR_CORPS_MOVABLE_CLICK;
				}
			}
		}
	}
}

void MouseManager::SetLeftDrag()
{


	Log( "드래그 종료 결과 %d, %d \n", m_MousePosition.X, m_MousePosition.Y );
	m_IsLeftDragging = false;
}

void MouseManager::SetRightDrag()
{
	m_IsRightDragging = false;
}

double MouseManager::GetDistanceBetweenCOORD( COORD C1, COORD C2 )
{
	int distanceX = C1.X - C2.X;
	int distanceY = C1.Y - C2.Y;

	double ret = sqrt( distanceX*distanceX + distanceY*distanceY );

	//printf_s("%fl\n", ret);
	return ret;
}
