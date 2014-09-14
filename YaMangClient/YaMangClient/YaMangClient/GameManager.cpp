#include "stdafx.h"
#include "GameManager.h"
#include "YaMangDxDll.h"
#include "Renderer.h"
#include "NetworkManager.h"
#include "DummyRender.h"
#include "InputDispatcher.h"
#include "CameraController.h"
#include "Timer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "MouseRender.h"
#include "TextManager.h"

#include "CollisionManager.h"
#include "PlayerManager.h"
#include "MouseManager.h"
#include "SoundManager.h"
#include "UIManager.h"

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

//////////////////////////////////////////////////////////////////////////
// 게임의 자원 초기화는 이곳에서
//////////////////////////////////////////////////////////////////////////
bool GameManager::Init() const
{
	//////////////////////////////////////////////////////////////////////////
	// 하단의 코드들은 방어코드가 필요합니다.
	//////////////////////////////////////////////////////////////////////////

	// 사운드 초기화
	SoundManager::GetInstance();

	// 렌더러 생성 및 초기화
	Renderer::GetInstance()->Init();

	// 리소스 로딩 및 초기화
	// 리소스는 씬보다 먼저 생성해야 함
	ResourceManager::GetInstance()->Init();
	
	// UI 매니져 생성 및 초기화
	UIManager::GetInstance()->Init();

	// 씬 생성 및 초기화
	SceneManager::GetInstance()->Init();
	SceneManager::GetInstance()->ChangeScene( SCENE_LOAD );

	// 네트워크 생성 및 초기화
	NetworkManager::GetInstance()->Init();
	NetworkManager::GetInstance()->Connect();

	// 카메라 초기화
	// 카메라 초기화는 게임 시작 패킷을 받는 부분으로 보냅니다.
	
	// 텍스트 매니저
	// 타이머
	// 플레이어

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 업데이트 부분
//////////////////////////////////////////////////////////////////////////
bool GameManager::Process() const
{
	if ( !m_Continue )
	{
		return false;
	}
	// 업데이트

	Timer::GetInstance()->Tick();
	InputDispatcher::GetInstance()->DispatchKeyInput();
	NetworkManager::GetInstance()->ProcessPacket();

	CameraController::GetInstance()->Update();
	SceneManager::GetInstance()->Update();

	UIManager::GetInstance()->Update();

	{
		// 더미 렌더
		DummyRender dRender;

		SceneManager::GetInstance()->Render();
		UIManager::GetInstance()->Render();
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 자원 해제는 이곳에서 합니다.
//////////////////////////////////////////////////////////////////////////
void GameManager::Destroy() const
{
	// 텍스트 매니저 해제
	TextManager::Release();

	// 네트워크에서 씬을 참조하는 부분이 있으므로 씬을 먼저 해제하면 안 됨
	NetworkManager::GetInstance()->Destroy();
	NetworkManager::Release();

	// UI 매니져 해제
	UIManager::GetInstance()->Destroy();
	UIManager::Release();

	// 씬 해제
	SceneManager::GetInstance()->Destroy();
	SceneManager::Release();

	// 충돌 체크 매니저는 씬매니저보다 나중에 제거
	CollisionManager::GetInstance()->ClearList();
	CollisionManager::Release();

	// 리소스 해제
	ResourceManager::GetInstance()->Destroy();
	ResourceManager::Release();

	// 렌더러 해제
	Renderer::GetInstance()->Destroy();
	Renderer::Release();

	CameraController::Release();
	InputDispatcher::Release();

	Timer::Release();
	PlayerManager::Release();
	MouseManager::Release();

	SoundManager::Release();
}
