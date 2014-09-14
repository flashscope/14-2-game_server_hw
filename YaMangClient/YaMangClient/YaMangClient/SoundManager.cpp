#include "stdafx.h"
#include "SoundManager.h"
#include "fmod_studio.hpp"
#include "MainWindow.h"
#include "ResourceDef.h"
#include "Logger.h"

SoundManager::SoundManager()
{

	m_SoundList.fill( nullptr );
	
	FMOD_RESULT result;
	FMOD::Studio::System::create( &m_System );
	result = m_System->initialize( 32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, NULL );
	if ( result )
	{
		Log( "System Initialize Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"System Initialize Fail", L"Sound Error", MB_ICONERROR );
	}

	USES_CONVERSION;
	LoadBank( W2A(SOUND_FILE_MASTER_BANK) );
	LoadBank( W2A(SOUND_FILE_MASTER_BANK_STRINGS) );

	LoadBank( W2A(SOUND_FILE_LOOP) );
	LoadBank( W2A( SOUND_FILE_SFX_BANK ) );

	//////////////////////////////////////////////////////////////////////////

	m_SoundList[SOUND_SYSTEM_MOUSE_CLICK] = GetEventInstance( "event:/sfx/system/mouse_click" );
	m_SoundList[SOUND_SCENE_PLAY_BGM1] = GetEventInstance( "event:/loop/scene_play_01" );
	m_SoundList[SOUND_SCENE_PLAY_BGM2] = GetEventInstance( "event:/loop/scene_play_02" );
	m_SoundList[SOUND_SCENE_PLAY_BGM3] = GetEventInstance( "event:/loop/archi_4" );
	m_SoundList[SOUND_SCENE_PLAY_BGM4] = GetEventInstance( "event:/loop/archi_5" );
	m_SoundList[SOUND_CORPS_GENERATE] = GetEventInstance( "event:/sfx/play/corps/generate_corps" );
	m_SoundList[SOUND_CORPS_MOVE] = GetEventInstance( "event:/sfx/play/corps/generate_corps" );
	m_SoundList[SOUND_CORPS_ATTACK_ARROW] = GetEventInstance( "event:/sfx/play/corps/attack/attack_arrow" );
	m_SoundList[SOUND_CORPS_ATTACK_GUARD] = GetEventInstance( "event:/sfx/play/corps/attack/attack_guard" );
	m_SoundList[SOUND_CORPS_ATTACK_KNIGHT] = GetEventInstance( "event:/sfx/play/corps/attack/attack_horse" );
	m_SoundList[SOUND_CORPS_ATTACK_PIKE] = GetEventInstance( "event:/sfx/play/corps/attack/attack_pike" );
	m_SoundList[SOUND_CORPS_ATTACK_SWORD] = GetEventInstance( "event:/sfx/play/corps/attack/attack_sword" );

	m_SoundList[SOUND_SCENE_RESULT_BGM_WIN] = GetEventInstance( "event:/loop/result_win" );
	m_SoundList[SOUND_SCENE_RESULT_BGM_LOSE] = GetEventInstance( "event:/loop/result_lose" );

}


SoundManager::~SoundManager()
{
}


void SoundManager::LoadBank( const char* fileName )
{
	FMOD_RESULT result;
	FMOD::Studio::Bank* bank = NULL;
	result = m_System->loadBankFile( fileName, FMOD_STUDIO_LOAD_BANK_NORMAL, &bank );
	if ( result )
	{
		Log( "Bank Load Fail:%s \n", fileName );
		std::wstring message = std::wstring( L"Bank Load Fail:" ) + std::wstring( fileName, fileName + strlen( fileName ) );
		MessageBox( MainWindow::GetInstance( )->Window( ), message.c_str(), L"Sound Error", MB_ICONERROR );
	}
}

FMOD::Studio::EventInstance* SoundManager::GetEventInstance( const char* path )
{
	FMOD_RESULT result;
	FMOD::Studio::ID eventID = { 0 };
	result = m_System->lookupID( path, &eventID );
	if ( result )
	{
		Log( "Event Load Fail:%s \n", path );
		std::wstring message = std::wstring( L"Event Load Fail:" ) + std::wstring( path, path + strlen( path ) );
		MessageBox( MainWindow::GetInstance()->Window(), message.c_str(), L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventDescription* eventDescription = NULL;
	result = m_System->getEvent( &eventID, FMOD_STUDIO_LOAD_BEGIN_NOW, &eventDescription );
	if ( result )
	{
		Log( "GetEvent Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"GetEvent Fail", L"Sound Error", MB_ICONERROR );
	}

	FMOD::Studio::EventInstance* eventInstance = NULL;
	result = eventDescription->createInstance( &eventInstance );
	if ( result )
	{
		Log( "Create Instance Fail \n" );
		MessageBox( MainWindow::GetInstance()->Window(), L"Create Instance Fail", L"Sound Error", MB_ICONERROR );
	}

	
	return eventInstance;
}

bool SoundManager::PlaySound( SoundType soundType )
{
	if ( m_SoundList[soundType] != nullptr)
	{
		m_SoundList[soundType]->start();

		m_System->update();

		return true;
	}
	
	return false;
}

bool SoundManager::StopSound( SoundType soundType )
{
	if ( m_SoundList[soundType] != nullptr )
	{
		m_SoundList[soundType]->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
		return true;
	}

	return false;
}


void SoundManager::StopAllSound()
{
	for ( auto& sound : m_SoundList )
	{
		sound->stop( FMOD_STUDIO_STOP_ALLOWFADEOUT );
	}
}

bool SoundManager::SetVolume( SoundType soundType, float volume )
{
	if ( m_SoundList[soundType] != nullptr )
	{
		m_SoundList[soundType]->setVolume( volume );
		m_System->update( );
		return true;
	}

	return false;
}
