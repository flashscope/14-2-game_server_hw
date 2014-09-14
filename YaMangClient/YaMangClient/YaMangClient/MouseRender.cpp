#include "stdafx.h"
#include "MouseRender.h"
#include "YaMangDxDll.h"
#include "MouseManager.h"
#include "ResourceManager.h"


MouseRender::MouseRender()
{
	Init();
}


MouseRender::~MouseRender()
{
}

void MouseRender::Init()
{
	if ( ResourceManager::GetInstance()->IsCursorReady() )
	{
		ChangeCursorImage( CURSOR_DEFAULT );
		MouseManager::GetInstance()->SetGameCursorMode( true );

		m_IsCursorReady = true;
	}
}

void MouseRender::Update()
{
	SetGameCursorPos( MouseManager::GetInstance()->GetMousePositionX(), MouseManager::GetInstance()->GetMousePositionY() );
	ChangeCursorImage( MouseManager::GetInstance()->GetCursorRenderType() );
}

void MouseRender::Render() const
{
	if ( m_IsCursorReady )
	{
		RenderCursor();
	}
}

void MouseRender::SetGameCursorPos(int PosX, int PosY)
{
	SetCursorPosition( PosX - m_CursorHotspotX, PosY - m_CursorHotspotY );
}