#pragma once
#include "EnumSet.h"
#include "SharedDefine.h"

#define MAX_CHAT_LEN	1024

#define MAX_NAME_LEN	30
#define MAX_COMMENT_LEN	40


enum PacketTypes: short
{
	PKT_NONE = 0,

	PKT_CS_LOGIN = 1,
	PKT_SC_LOGIN = 2, 

	PKT_CS_CHAT = 3,
	PKT_SC_CHAT = 4,

	PKT_CS_GAMEOVER = 5,
	PKT_SC_GAMEOVER = 6,

	PKT_CS_ROOM_CREATE = 7,
	PKT_SC_ROOM_CREATE = 8,

	PKT_CS_ENTER_ROOM = 9,
	PKT_SC_ENTER_ROOM = 10,

	PKT_CS_LEAVE_ROOM = 11,
	PKT_SC_LEAVE_ROOM = 12,

	PKT_SC_GAME_START = 50,

	PKT_SC_KING_INDEX = 60,

	PKT_CS_GENERATE_CORPS = 100,
	PKT_SC_GENERATE_CORPS = 101,

	PKT_CS_MOVE_CORPS = 102,
	PKT_SC_MOVE_CORPS = 103,
	PKT_CS_STOP_CORPS = 104,
	PKT_SC_STOP_CORPS = 105,

	PKT_CS_CORPS_CHANGE_FORMATION = 200,
	PKT_SC_CORPS_CHANGE_FORMATION = 201,

	PKT_CS_CORPS_ATTACK = 300,
	PKT_SC_CORPS_ATTACK = 400,

	PKT_SC_REFRESH_UI = 500,
	PKT_SC_REFRESH_BASE = 501,

	PKT_CS_SYNC_ALL = 600,

	PKT_SC_SYNC_ONE_CORP = 601,

	PKT_MAX = 1024
};

#pragma pack(push, 1)

struct PacketHeader
{
	PacketHeader(): m_Size( 0 ), m_Type( PKT_NONE ) {}
	short m_Size;
	short m_Type;
};



struct LoginRequest: public PacketHeader
{
	LoginRequest()
	{
		m_Size = sizeof( LoginRequest );
		m_Type = PKT_CS_LOGIN;
		m_PlayerId = -1;
	}

	int	m_PlayerId;
};

struct LoginResult: public PacketHeader
{
	LoginResult()
	{
		m_Size = sizeof( LoginResult );
		m_Type = PKT_SC_LOGIN;
		m_PlayerId = -1;
		memset( m_Name, 0, MAX_NAME_LEN );
	}

	int		m_PlayerId;
	char	m_Name[MAX_NAME_LEN];

};

struct ChatBroadcastRequest: public PacketHeader
{
	ChatBroadcastRequest()
	{
		m_Size = sizeof( ChatBroadcastRequest );
		m_Type = PKT_CS_CHAT;
		m_PlayerId = -1;

		memset( m_Chat, 0, MAX_CHAT_LEN );
	}

	int	m_PlayerId;
	char m_Chat[MAX_CHAT_LEN];
};

struct ChatBroadcastResult: public PacketHeader
{
	ChatBroadcastResult()
	{
		m_Size = sizeof( ChatBroadcastResult );
		m_Type = PKT_SC_CHAT;
		m_PlayerId = -1;

		memset( m_Name, 0, MAX_NAME_LEN );
		memset( m_Chat, 0, MAX_CHAT_LEN );
	}

	int	m_PlayerId;
	char m_Name[MAX_NAME_LEN];
	char m_Chat[MAX_CHAT_LEN];
};

// test Request packet
struct GameOverRequest: public PacketHeader
{
	GameOverRequest( )
	{
		m_Size = sizeof( GameOverRequest );
		m_Type = PKT_CS_GAMEOVER;
		m_PlayerId = -1;
		m_IsWon = false;
	}

	int			m_PlayerId;
	bool		m_IsWon;
};

struct GameOverResult: public PacketHeader
{
	GameOverResult( )
	{
		m_Size = sizeof( GameOverResult );
		m_Type = PKT_SC_GAMEOVER;
		m_PlayerId = -1;
		m_IsWon = false;

		m_KilledCorpsNum = -1;
		m_DeathCorpsNum = -1;
		m_TotalMyCorpsNum = -1;
	}

	int			m_PlayerId;
	bool		m_IsWon;

	int			m_KilledCorpsNum;
	int			m_DeathCorpsNum;
	int			m_TotalMyCorpsNum;
};


struct RoomCreateRequest: public PacketHeader
{
	RoomCreateRequest( )
	{
		m_Size = sizeof( RoomCreateRequest );
		m_Type = PKT_CS_ROOM_CREATE;
		m_PlayerId = -1;
	}

	int	m_PlayerId;
};

struct RoomCreateResult: public PacketHeader
{
	RoomCreateResult( )
	{
		m_Size = sizeof( RoomCreateResult );
		m_Type = PKT_SC_ROOM_CREATE;
		m_RoomNumber = -1;
	}

	int	m_RoomNumber;
};



struct EnterRoomRequest: public PacketHeader
{
	EnterRoomRequest( )
	{
		m_Size = sizeof( EnterRoomRequest );
		m_Type = PKT_CS_ENTER_ROOM;
		m_RoomNumber = -1;
	}

	int m_RoomNumber;
};

struct EnterRoomResult: public PacketHeader
{
	EnterRoomResult( )
	{
		m_Size = sizeof( EnterRoomResult );
		m_Type = PKT_SC_ENTER_ROOM;
		m_RoomNumber = -1;
	}

	int	m_RoomNumber;
};

struct LeaveRoomRequest: public PacketHeader
{
	LeaveRoomRequest( )
	{
		m_Size = sizeof( LeaveRoomRequest );
		m_Type = PKT_CS_LEAVE_ROOM;
		m_RoomNumber = -1;
	}

	int m_RoomNumber;
};

struct LeaveRoomResult: public PacketHeader
{
	LeaveRoomResult( )
	{
		m_Size = sizeof( LeaveRoomResult );
		m_Type = PKT_SC_LEAVE_ROOM;
		m_RoomNumber = -1;
	}

	int	m_RoomNumber;
};

struct GameStartResult: public PacketHeader
{
	GameStartResult( )
	{
		m_Size = sizeof( GameStartResult );
		m_Type = PKT_SC_GAME_START;
	}
};

struct KingIndexResult: public PacketHeader
{
	KingIndexResult( )
	{
		m_Size = sizeof( KingIndexResult );
		m_Type = PKT_SC_KING_INDEX;
		m_KingIndex = -1;
		m_StartPositionX = NOT_A_NUMBER_FLOAT;
		m_StartPositionZ = NOT_A_NUMBER_FLOAT;
	}

	int		m_KingIndex;
	float	m_StartPositionX;
	float	m_StartPositionZ;
};

// 사실 request는 테스트용
struct GenerateCorpsRequest: public PacketHeader
{
	GenerateCorpsRequest()
	{
		m_Size = sizeof( GenerateCorpsRequest );
		m_Type = PKT_CS_GENERATE_CORPS;
		m_UnitType = UnitType::UNIT_NONE;
		m_NowX = NOT_A_NUMBER_FLOAT;
		m_NowZ = NOT_A_NUMBER_FLOAT;
		m_LookX = NOT_A_NUMBER_FLOAT;
		m_LookZ = NOT_A_NUMBER_FLOAT;
		m_PlayerId = -1;
	}

	UnitType m_UnitType;
	float	m_NowX;
	float	m_NowZ;
	float	m_LookX;
	float	m_LookZ;
	int		m_PlayerId;
};

struct GenerateCorpsResult: public PacketHeader
{
	GenerateCorpsResult()
	{
		m_Size = sizeof( GenerateCorpsResult );
		m_Type = PKT_SC_GENERATE_CORPS;
		m_UnitType = UnitType::UNIT_NONE;
		m_NowX = NOT_A_NUMBER_FLOAT;
		m_NowZ = NOT_A_NUMBER_FLOAT;
		m_LookX = NOT_A_NUMBER_FLOAT;
		m_LookZ = NOT_A_NUMBER_FLOAT;
		m_CorpsID = -1;
		m_PlayerId = -1;
		m_UnitNum = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	UnitType m_UnitType;
	float	m_NowX;
	float	m_NowZ;
	float	m_LookX;
	float	m_LookZ;
	int		m_CorpsID;
	int		m_PlayerId;

	int		m_UnitNum;
	FormationType	m_FormationType;
};


struct MoveCorpsRequest: public PacketHeader
{
	MoveCorpsRequest( )
	{
		m_Size = sizeof( MoveCorpsRequest );
		m_Type = PKT_CS_MOVE_CORPS;
		m_CorpsID = -1;
		m_NowX = NOT_A_NUMBER_FLOAT;
		m_NowZ = NOT_A_NUMBER_FLOAT;
		m_TargetX = NOT_A_NUMBER_FLOAT;
		m_TargetZ = NOT_A_NUMBER_FLOAT;
	}
	int		m_CorpsID;
	float	m_NowX;
	float	m_NowZ;
	float	m_TargetX;
	float	m_TargetZ;
};

struct MoveCorpsResult: public PacketHeader
{
	MoveCorpsResult( )
	{
		m_Size = sizeof( MoveCorpsResult );
		m_Type = PKT_SC_MOVE_CORPS;
		m_CorpsID = -1;
		m_Speed = 0;
		m_TargetX = NOT_A_NUMBER_FLOAT;
		m_TargetZ = NOT_A_NUMBER_FLOAT;
		m_LookX = NOT_A_NUMBER_FLOAT;
		m_LookZ = NOT_A_NUMBER_FLOAT;
	}

	int		 m_CorpsID;
	float	 m_Speed;

	float	m_TargetX;
	float	m_TargetZ;
	float	m_LookX;
	float	m_LookZ;
};


struct StopCorpsRequest: public PacketHeader
{
	StopCorpsRequest( )
	{
		m_Size = sizeof( StopCorpsRequest );
		m_Type = PKT_CS_STOP_CORPS;
		m_CorpsID = -1;
	}
	int		 m_CorpsID;

};

struct StopCorpsResult: public PacketHeader
{
	StopCorpsResult( )
	{
		m_Size = sizeof( StopCorpsResult );
		m_Type = PKT_SC_STOP_CORPS;
		m_CorpsID = -1;
		m_NowX = NOT_A_NUMBER_FLOAT;
		m_NowZ = NOT_A_NUMBER_FLOAT;
		m_LookX = NOT_A_NUMBER_FLOAT;
		m_LookZ = NOT_A_NUMBER_FLOAT;
	}

	int		m_CorpsID;
	float	m_NowX;
	float	m_NowZ;
	float	m_LookX;
	float	m_LookZ;
};


struct ChangeCorpsFormationRequest: public PacketHeader
{
	ChangeCorpsFormationRequest( )
	{
		m_Size = sizeof( ChangeCorpsFormationRequest );
		m_Type = PKT_CS_CORPS_CHANGE_FORMATION;
		m_CorpsID = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	int				m_CorpsID;
	FormationType	m_FormationType;
};

struct ChangeCorpsFormationResult: public PacketHeader
{
	ChangeCorpsFormationResult( )
	{
		m_Size = sizeof( ChangeCorpsFormationResult );
		m_Type = PKT_SC_CORPS_CHANGE_FORMATION;
		m_CorpsID = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	int				m_CorpsID;
	FormationType	m_FormationType;
};


struct AttackCorpsRequest: public PacketHeader
{
	AttackCorpsRequest( )
	{
		m_Size = sizeof( AttackCorpsRequest );
		m_Type = PKT_CS_CORPS_ATTACK;
		m_MyCorpsID = -1;
		m_TargetCorpsID = -1;
	}
	int		m_MyCorpsID;
	int		m_TargetCorpsID;
};


struct AttackCorpsResult: public PacketHeader
{
	AttackCorpsResult( )
	{
		m_Size = sizeof( AttackCorpsResult );
		m_Type = PKT_SC_CORPS_ATTACK;
		m_AttackerCorpsID = -1;
		m_TargetCorpsID = -1;
		m_TargetUnitNum = -1;
		m_AttackerNowX = NOT_A_NUMBER_FLOAT;
		m_AttackerNowZ = NOT_A_NUMBER_FLOAT;
		m_AttackerLookX = NOT_A_NUMBER_FLOAT;
		m_AttackerLookZ = NOT_A_NUMBER_FLOAT;
		m_TargetNowX = NOT_A_NUMBER_FLOAT;
		m_TargetNowZ = NOT_A_NUMBER_FLOAT;
		m_TargetLookX = NOT_A_NUMBER_FLOAT;
		m_TargetLookZ = NOT_A_NUMBER_FLOAT;
	}

	int		m_AttackerCorpsID;
	int		m_TargetCorpsID;
	int		m_TargetUnitNum;
	float	m_AttackerNowX;
	float	m_AttackerNowZ;
	float	m_AttackerLookX;
	float	m_AttackerLookZ;
	float	m_TargetNowX;
	float	m_TargetNowZ;
	float	m_TargetLookX;
	float	m_TargetLookZ;
};



struct RefreshUIResult: public PacketHeader
{
	RefreshUIResult( )
	{
		m_Size = sizeof( RefreshUIResult );
		m_Type = PKT_SC_REFRESH_UI;
		m_NowCorpsNum = -1;
		m_MaxCorpsNum = -1;
		m_BaseNum = -1;
		m_RegenTime = INT_MAX;

		m_KilledCorpsNum = -1;
		m_DeathCorpsNum = -1;
		m_TotalMyCorpsNum = -1;
	}

	int			m_NowCorpsNum;
	int			m_MaxCorpsNum;
	int			m_BaseNum;
	ULONGLONG	m_RegenTime;


	// 사실 게임 엔드에 나와도 되지만 잘 카운팅 되나 디버깅용 데이터
	int			m_KilledCorpsNum;
	int			m_DeathCorpsNum;
	int			m_TotalMyCorpsNum;
};

struct RefreshBaseResult: public PacketHeader
{
	RefreshBaseResult( )
	{
		m_Size = sizeof( RefreshBaseResult );
		m_Type = PKT_SC_REFRESH_BASE;
		m_BaseID = -1;
		m_PlayerID = -1;
	}

	int	m_BaseID;
	int	m_PlayerID;
};

// 클라 접속전의 게임 룸 상태를 전송
// 게임에 재접등의 이유나 관람에 사용 가능할듯 (지금은 클라가 게임 시작 동기화가 없이 막 들어가서...)
struct SyncAllRequest: public PacketHeader
{
	SyncAllRequest( )
	{
		m_Size = sizeof( SyncAllRequest );
		m_Type = PKT_CS_SYNC_ALL;
		m_RoomNumber = -1;
		m_PlayerId = -1;
	}

	
	int		m_RoomNumber;
	int		m_PlayerId;
};


// 콥스에 변경사항이 발생하면 간단히 갱신, 가끔 서버의 손이남으면 갱신들 시켜줘도 좋을듯
struct SyncOneCorpResult: public PacketHeader
{
	SyncOneCorpResult( )
	{
		m_Size = sizeof( SyncOneCorpResult );
		m_Type = PKT_SC_SYNC_ONE_CORP;
		m_CorpsID = -1;
		m_NowX = NOT_A_NUMBER_FLOAT;
		m_NowZ = NOT_A_NUMBER_FLOAT;
		m_LookX = NOT_A_NUMBER_FLOAT;
		m_LookZ = NOT_A_NUMBER_FLOAT;
		m_UnitNum = -1;
		m_FormationType = FormationType::FORMATION_NONE;
	}

	int		m_CorpsID;
	float	m_NowX;
	float	m_NowZ;
	float	m_LookX;
	float	m_LookZ;
	int		m_UnitNum;
	FormationType	m_FormationType;
};

#pragma pack(pop)