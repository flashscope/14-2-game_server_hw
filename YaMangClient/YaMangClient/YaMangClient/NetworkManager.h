#pragma once
#include "MacroSet.h"
#include "CircularBuffer.h"

struct PacketHeader;

struct LoginResult;
struct GameOverResult;
struct ChatBroadcastResult;
struct GenerateCorpsResult;
struct ChangeCorpsFormationResult;
struct MoveCorpsResult;
struct StopCorpsResult;
struct RefreshUIResult;
struct AttackCorpsResult;
struct SyncOneCorpResult;
struct RefreshBaseResult;
struct GameStartResult;
struct EnterRoomResult;
struct KingIndexResult;

class NetworkManager:public Singleton<NetworkManager>
{
public:
	NetworkManager();
	~NetworkManager();

	bool	Init();
	void	Destroy();
	void	ProcessPacket();
	void	ProcessProto();
	bool	Connect();
	bool	HandleMessage( WPARAM wParam, LPARAM lParam );

	void	SetMyPlayerID( int playerID ) { m_MyPlayerId = playerID; };
	int		GetMyPlayerID() const { return m_MyPlayerId; }
	
	void	SetRoomNumber( int roomNumber ) { m_RoomNumber = roomNumber; }

	//////////////////////////////////////////////////////////////////////////
	// 패킷 핸들러
	//////////////////////////////////////////////////////////////////////////
	void	HandleLoginResult( LoginResult& inPacket );
	void	HandleGameOverResult( GameOverResult& inPacket );
	void	HandleChatResult( ChatBroadcastResult& inPacket );
	void	HandleGenerateCorpsResult( GenerateCorpsResult& inPacket );
	void	HandleChangeCorpsFormationResult( ChangeCorpsFormationResult& inPacket );
	void	HandleMoveCorpsResult( MoveCorpsResult& inPacket );
	void	HandleStopCorpsResult( StopCorpsResult& inPacket );
	void	HandleRefreshUIResult( RefreshUIResult& inPacket );
	void	HandleAttackCorpsResult( AttackCorpsResult& inPacket );
	void	HandleSyncOneCorpResult( SyncOneCorpResult& inPacket );
	void	HandleBaseResult( RefreshBaseResult& inPacket );
	void	HandleGameStartResult( GameStartResult& inPacket );
	void	HandleEnterRoomResult( EnterRoomResult& inPacket );
	void	HandleKingIndexResult( KingIndexResult& inPacket );

	//////////////////////////////////////////////////////////////////////////
	// 리퀘스트 패킷 보내기
	//////////////////////////////////////////////////////////////////////////
	void	SendPacket( PacketHeader* pkt );
	void	SendPlayPacket( PacketHeader* pkt );

private:
	SOCKET			m_Socket = NULL;
	CircularBuffer	m_RecvBuffer;
	CircularBuffer	m_SendBuffer;
	int				m_MyPlayerId = -1;

	UINT			m_PrevTime = 0;

	int				m_RoomNumber = 0;
	bool			m_IsPlaying = false;
};

typedef void( *HandlerFunc )( PacketHeader& pktBase );
extern HandlerFunc HandlerTable[];