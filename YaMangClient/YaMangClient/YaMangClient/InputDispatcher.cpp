/**
*@file		InputDispatcher.cpp
*@brief		키와 마우스 이벤트를 지속성 있게 관리해줍니다.
*@details	MainWindow의 객체의 윈도우 메시지 핸들링에서 키와 마우스 관련 메시지를 받아 이벤트 분석합니다.
*			키 입력이 초기 한 번 들어오면 대략 1초의 딜레이 후 연속 발생하는데,
			해당 부분을 해결하기 위해 처음 한 번 눌렸을 때와 손을 뗄 때의 사이에 눌리고 있음을
			가상으로 만들어서 지속적으로 핸들링 함으로써 부드러운 키 입력을 구현합니다.
*/

#include "stdafx.h"
#include "InputDispatcher.h"
#include "KeyInput.h"
#include "GameManager.h"
#include "CameraController.h"
#include "NetworkManager.h"
#include "PacketType.h"
#include "MainWindow.h"
#include "Timer.h"
#include "PlayerManager.h"
#include "yaMangDxDll.h"
#include "SceneManager.h"
#include "SceneResult.h"
#include "ScenePlay.h"
#include "Logger.h"
#include "Renderer.h"

typedef void( *KeyEventHandler )( KeyInput inputKey );
static KeyEventHandler KeyHandlerTable[MAX_KEY];

static void DefaultKeyHandler( KeyInput inputKey )
{
}

struct InitializeKeyHandlers
{
	InitializeKeyHandlers()
	{
		for ( int i = 0; i < MAX_KEY; ++i )
		{
			KeyHandlerTable[i] = DefaultKeyHandler;
		}
	}
} _init_key_handlers_;

struct RegisterKeyHandler
{
	RegisterKeyHandler( unsigned char keyType, KeyEventHandler keyHandler )
	{
		KeyHandlerTable[keyType] = keyHandler;
	}
};

#define REGISTER_KEY_HANDLER( KEY_TYPE ) \
	static void Handler_##KEY_TYPE( KeyInput inputKey ); \
	static RegisterKeyHandler _register_##KEY_TYPE(KEY_TYPE, Handler_##KEY_TYPE); \
	static void Handler_##KEY_TYPE( KeyInput inputKey )

InputDispatcher::InputDispatcher()
{
	m_IsKeyPressed.fill( false );
}


InputDispatcher::~InputDispatcher()
{
}

void InputDispatcher::EventKeyInput( KeyInput key )
{
	switch ( key.GetKeyStatus() )
	{
		case KeyStatusType::KEY_DOWN:
		{
			m_KeyInputList.push_back( key );
			m_IsKeyPressed[key.GetKeyValue()] = true;
		}
			break;
		case KeyStatusType::KEY_PRESSED:
			break;
		case KeyStatusType::KEY_UP:
		{
			m_IsKeyPressed[key.GetKeyValue()] = false;
		}
			break;
		default:
		{
			assert( false );
		}
	}
}

void InputDispatcher::DispatchKeyInput()
{
	// Log("현재 키 이벤트 발생 개수 : %d \n", m_KeyInputList.size());

	auto iter = m_KeyInputList.begin();
	while ( iter != m_KeyInputList.end() )
	{
		if ( IsPressed(*(iter)) )
		{
			KeyHandlerTable[iter->GetKeyValue()]( *(iter) );
			( iter++ )->SetKeyStatus( KeyStatusType::KEY_PRESSED );
		}
		else
		{
			( iter )->SetKeyStatus( KeyStatusType::KEY_UP );
			KeyHandlerTable[iter->GetKeyValue()]( *(iter) );

			m_KeyInputList.erase(iter);
			break;
		}
	}
}

void InputDispatcher::ClearList()
{
	m_IsKeyPressed.fill( false );
	m_KeyInputList.clear();
}

REGISTER_KEY_HANDLER( VK_SPACE )
{
	switch ( inputKey.GetKeyStatus() )
	{
		case KeyStatusType::KEY_DOWN:
			break;
		case KeyStatusType::KEY_PRESSED:
			// Log( "** 키 누르고 있다! ** \n" );
			break;
		case KeyStatusType::KEY_UP:
		{
			if ( SCENE_RESULT == SceneManager::GetInstance()->GetNowSceneType() )
			{
				GameManager::GetInstance()->Stop();
			}
		}
			break;
		default:
			break;
	}
}

REGISTER_KEY_HANDLER( VK_ESCAPE )
{
	GameManager::GetInstance()->Stop();
}

REGISTER_KEY_HANDLER( VK_W )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 10.0f;
	CameraController::GetInstance()->MoveForward( delta );

//	Log( "Time : %f \n", delta );
}

REGISTER_KEY_HANDLER( VK_S )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -10.0f;
	CameraController::GetInstance()->MoveForward( delta );
}

REGISTER_KEY_HANDLER( VK_A )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -10.0f;
	CameraController::GetInstance()->MoveSide( delta );
}

REGISTER_KEY_HANDLER( VK_D )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 10.0f;
	CameraController::GetInstance()->MoveSide( delta );
}

REGISTER_KEY_HANDLER( VK_Q )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * -1.0f;
	CameraController::GetInstance()->RotateSide( delta );
}

REGISTER_KEY_HANDLER( VK_E )
{
	float time = static_cast<float>( Timer::GetInstance()->GetElapsedTime() ) / 1000;
	float delta = time * 1.0f;
	CameraController::GetInstance()->RotateSide( delta );
}

REGISTER_KEY_HANDLER( VK_1 )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		PlayerManager::GetInstance()->ChangeCorpsFormation( FormationType::FORMATION_RUSH );
	}
}

REGISTER_KEY_HANDLER( VK_2 )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		PlayerManager::GetInstance()->ChangeCorpsFormation( FormationType::FORMATION_DESTROY );
	}
}

REGISTER_KEY_HANDLER( VK_3 )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		PlayerManager::GetInstance()->ChangeCorpsFormation( FormationType::FORMATION_DEFENSE );
	}
}

REGISTER_KEY_HANDLER( VK_4 )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		TakeScreenShot();
	}
}

REGISTER_KEY_HANDLER( VK_Z )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		Renderer::GetInstance()->ToggleHeightMapWire();
	}
}

// 콘솔 로그
REGISTER_KEY_HANDLER( VK_I )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		Logger::GetInstance()->SetLogStatus( LOG_CONSOLE );
	}
}

// 파일 로그
REGISTER_KEY_HANDLER( VK_O )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		Logger::GetInstance()->SetLogStatus( LOG_FILE );
	}
}

// 로그 중지
REGISTER_KEY_HANDLER( VK_P )
{
	if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
	{
		Logger::GetInstance()->SetLogStatus( LOG_NONE );
	}
}

REGISTER_KEY_HANDLER( VK_TAB )
{
	// 씬 플레이의 맵 보이기
	if ( SCENE_PLAY == SceneManager::GetInstance()->GetNowSceneType() )
	{
		if ( KeyStatusType::KEY_DOWN == inputKey.GetKeyStatus() )
		{
			Scene* scene = SceneManager::GetInstance()->GetNowScene();
			ScenePlay* thisScene = static_cast<ScenePlay*>( scene );

			thisScene->SetMapVisible( true );
		}

		if ( KeyStatusType::KEY_UP == inputKey.GetKeyStatus() )
		{
			Scene* scene = SceneManager::GetInstance()->GetNowScene();
			ScenePlay* thisScene = static_cast<ScenePlay*>( scene );

			thisScene->SetMapVisible( false );
		}
	}
}