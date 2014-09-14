#include "stdafx.h"
#include "NetworkManager.h"

#include "PacketType.h"
#include "MainWindow.h"
#include "tinyxml.h"
#include "xpath_static.h"
#include "PacketType.h"
#include "Timer.h"

#include "ResourceDef.h"
#include "Logger.h"

#pragma comment(lib,"ws2_32.lib")



#include "mypacket.pb.h"

#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/text_format.h>


using namespace google;



const int BUFSIZE = 1024 * 10;

NetworkManager::NetworkManager()
: m_RecvBuffer( BUFSIZE ), m_SendBuffer( BUFSIZE ), m_PrevTime( Timer::GetInstance()->GetNowTime() )
{
}

NetworkManager::~NetworkManager()
{
}

bool NetworkManager::Init()
{
	WSADATA WsaDat;

	int nResult = WSAStartup( MAKEWORD( 2, 2 ), &WsaDat );
	if ( nResult != 0 )
	{
		return false;
	}

	m_Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_Socket == INVALID_SOCKET )
	{
		return false;
	}

	// 소켓에 오는 것들은 WM_SOCKET 이벤트로 처리하겠음
	nResult = WSAAsyncSelect( m_Socket, MainWindow::GetInstance()->Window(), WM_SOCKET, ( FD_CLOSE | FD_CONNECT ) );
	
	if ( nResult )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"WSAAsyncSelect failed", L"Critical Error", MB_ICONERROR );
		SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
		return false;
	}

	return true;
}

void NetworkManager::Destroy()
{
	MessageBox( MainWindow::GetInstance()->Window(), L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
	closesocket( m_Socket );
	SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
}

bool NetworkManager::Connect()
{

	std::string serverAddr = "127.0.0.1";
	int port;

	// xml 로드 테스트
	TiXmlDocument document = TiXmlDocument( CLIENT_CONFIG );

	if ( document.LoadFile() )
	{
		std::string ipLoad = TinyXPath::S_xpath_string( document.RootElement(), "/client/ip/text()" ).c_str();
		std::string portLoad = TinyXPath::S_xpath_string( document.RootElement( ), "/client/port/text()" ).c_str( );
		
		serverAddr = ipLoad;
		port = std::stoi( portLoad );
		Log( "Loaded ip :%s Port Number :%s \n", serverAddr.c_str(), portLoad.c_str() );
	}
	else
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"ClientConfig.xml Load Fail", L"Connection Load Fail!", MB_ICONINFORMATION | MB_OK );
		return false;
	}

	struct hostent* host;

	if ( NULL == ( host = gethostbyname( serverAddr.c_str() ) ) )
	{
		return false;
	}

	// Set up our socket address structure
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons( port );
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *( (unsigned long*)host->h_addr );

	if ( SOCKET_ERROR == connect( m_Socket, (LPSOCKADDR)( &SockAddr ), sizeof( SockAddr ) ) )
	{
		if ( GetLastError() != WSAEWOULDBLOCK )
		{
			return false;
		}
	}

	return true;
}

void NetworkManager::ProcessPacket()
{
	PacketHeader header;

	if ( false == m_RecvBuffer.Peek( (char*)&header, sizeof( PacketHeader ) ) )
	{
		return;
	}

	if ( header.m_Size > static_cast<short> ( m_RecvBuffer.GetCurrentSize() ) )
	{
		return;
	}

	if ( header.m_Type < 0 )
	{
		m_RecvBuffer.Reset();
		return;
	}

	HandlerTable[header.m_Type]( header );
}

void NetworkManager::ProcessProto()
{
	MyPacket::PacketHeader packetHeader;
	int* asd = new int[1];
	m_RecvBuffer.Read( (char*)asd, 4 );
	
	printf_s( "[type:%d][size:%d] \n", packetHeader.messagetype(), packetHeader.messagesize() );
	protobuf::io::ArrayInputStream is( &m_RecvBuffer, 4 );

	packetHeader.ParseFromZeroCopyStream( &is );

	printf_s( "[type:%d][size:%d] \n", packetHeader.messagetype(), packetHeader.messagesize() );
	protobuf::io::ArrayInputStream input_array_stream( &m_RecvBuffer, packetHeader.ByteSize() );
	protobuf::io::CodedInputStream input_coded_stream( &input_array_stream );

	//while ( )
	{
		input_coded_stream.ReadRaw( &packetHeader, packetHeader.ByteSize() );
		printf_s("[type:%d][size:%d] \n", packetHeader.messagetype(), packetHeader.messagesize() );
		// 직접 억세스 할수 있는 버퍼 포인터와 남은 길이를 알아냄

		const void* payload_ptr = NULL;
		int remainSize = 0;
		input_coded_stream.GetDirectBufferPointer( &payload_ptr, &remainSize );
		
		if ( remainSize < (signed)packetHeader.ByteSize())
		{
			//break;
		}
		

	}
}

bool NetworkManager::HandleMessage( WPARAM wParam, LPARAM lParam )
{
	// lParam 이 에러인지 검출 해보기
	if ( WSAGETSELECTERROR( lParam ) )
	{
		MessageBox( MainWindow::GetInstance()->Window(), L"WSAGETSELECTERROR", L"Error", MB_OK | MB_ICONERROR );
		SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );

		return false;
	}

	// 에러 아니면 이벤트 검출해서 switch
	switch ( WSAGETSELECTEVENT( lParam ) )
	{
		// 연결이 되었다
		case FD_CONNECT:
		{
			/// NAGLE 끈다
			int opt = 1;
			setsockopt( m_Socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof( int ) );

			srand( static_cast<unsigned int> ( time( NULL ) ) );
			/// 대략 1000~1100 의 ID로 로그인 해보자

			//int playerID = 1000 + rand() % 101;
			LoginRequest sendData;
			sendData.m_PlayerId = m_MyPlayerId;

			SendPacket( &sendData );

			int nResult = WSAAsyncSelect( m_Socket, MainWindow::GetInstance()->Window(), WM_SOCKET, ( FD_CLOSE | FD_READ | FD_WRITE ) );
			if ( nResult )
			{
				assert( false );
				break;
			}
		}
			break;

		case FD_READ:
		{
			char inBuf[4096] = { 0, };

			// send() 함수와 반대
			int recvLen = recv( m_Socket, inBuf, 4096, 0 );
			
			// 소켓에서 읽어온 데이터를 일단 버퍼에 쓰자
			if ( !m_RecvBuffer.Write( inBuf, recvLen ) )
			{
				/// 버퍼 꽉찼다.
				assert( false );
			}

			//////////////////////////////////////////////////////////////////////////
			// 패킷 핸들링!
			//////////////////////////////////////////////////////////////////////////
			//ProcessPacket();
			ProcessProto();
		}
			break;
			
			//////////////////////////////////////////////////////////////////////////
			// 데이터를 받으면 -> 버퍼에 쓴 후에, 핸들링 하는 쪽에서 버퍼 데이터 뽑아서 처리
			//
			// 데이터를 보낼때 -> 버퍼에 쓴 후에, FD_WRITE 쪽에서 버퍼 데이터 뽑아서 send()
			//////////////////////////////////////////////////////////////////////////

		case FD_WRITE:
		{
			/// 실제로 버퍼에 있는것들 꺼내서 보내기
			int size = m_SendBuffer.GetCurrentSize();
			if ( size > 0 )
			{
				char* data = new char[size];
				m_SendBuffer.Peek( data );

				int sent = send( m_Socket, data, size, 0 );

				/// 다를수 있다
				if ( sent != size )
				{
					OutputDebugStringA( "sent != request\n" );
				}

				// 보낸 데이터는 지우자
				m_SendBuffer.Consume( sent );

				delete[] data;
			}

		}
			break;

		case FD_CLOSE:
		{
			MessageBox( MainWindow::GetInstance()->Window(), L"Server closed connection", L"Connection closed!", MB_ICONINFORMATION | MB_OK );
			closesocket( m_Socket );
			SendMessage( MainWindow::GetInstance()->Window(), WM_DESTROY, NULL, NULL );
		}
			break;
	}

	return true;
}


void NetworkManager::SendPacket( PacketHeader* pkt )
{
	UINT nowTime = Timer::GetInstance()->GetNowTime();
	if ( nowTime - m_PrevTime < 1 )
	{
		// 비정상적으로 과도한 요청이다 (0.001초 이내에 반복 요청 되고 있음)
		// return;
	}

	if ( m_SendBuffer.Write( (const char*)pkt, pkt->m_Size ) )
	{
		// @see http://blog.naver.com/gkqxhq324456/110177315036 참조
		// 채팅을 날리려고 버퍼에 데이터도 넣어 두었으니, WM_SOCKET 이벤트를 발생시키자
		PostMessage( MainWindow::GetInstance()->Window(), WM_SOCKET, NULL, FD_WRITE );

		m_PrevTime = nowTime;
	}
}

void NetworkManager::SendPlayPacket( PacketHeader* pkt )
{
	if ( m_IsPlaying )
	{
		SendPacket( pkt );
	}
}
