// WSAAsyncSelectEcho.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")


#define DATA_BUFSIZE 8192

// typedef definition
typedef struct _SOCKET_INFORMATION
{
	BOOL RecvPosted;
	CHAR Buffer[DATA_BUFSIZE];
	WSABUF DataBuf;
	SOCKET Socket;
	DWORD BytesSEND;
	DWORD BytesRECV;
	struct _SOCKET_INFORMATION *Next;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;

LPSOCKET_INFORMATION SocketInfoList;

#define WM_SOCKET (WM_USER + 1)

SOCKET listenSocket;
int LISTEN_PORT = 9001;
bool serverRunning = false;





HWND InitWindow();

LRESULT CALLBACK WindowProc
(
HWND   hWnd   // handle to window
, UINT   uMsg   // message identifier
, WPARAM wParam // first message parameter
, LPARAM lParam // second message parameter
);


HRESULT ServerInit( HWND hWnd );
LRESULT CALLBACK ServerMessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void CreateSocketInformation( SOCKET s );
LPSOCKET_INFORMATION GetSocketInformation( SOCKET s );
void FreeSocketInformation( SOCKET s );

int _tmain(int argc, _TCHAR* argv[])
{

	HWND hwnd;
	if ( ( hwnd = InitWindow() ) == NULL )
	{
		return 0;
	}

	MSG msg = { 0, };
	DWORD Ret;
	while ( Ret = GetMessage( &msg, NULL, 0, 0 ) )
	{
		if ( Ret == -1 )
		{
			printf( "GetMessage() failed with error %d\n", GetLastError() );
			return 0;
		}
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return 0;
}

HRESULT ServerInit( HWND hWnd )
{

	/// 윈속 초기화
	// 윈도우 소켓 사용시 WSAStartup() 함수를 호출해야 winsock.dll을 이용 가능하다
	// 사용 시 유의점 : WSAStartup() / WSACleanup() 함수를 짝으로 맞춰 사
	// ws2_32.dll 파일 필요
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsa ) != 0 )
	{
		return S_FALSE;
	}

	// AF_INET : The Internet Protocol version 4 (IPv4) address family.
	// SOCK_STREAM : TCP
	// socket protocol = 0
	// LAW SOCKET을 사용할 때 사용한다. 주로 사용 되지 않아 0이나 IPPROTO_TCP / IPPROTO_UPT로 사용 된다.
	listenSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( listenSocket == INVALID_SOCKET )
	{
		return S_FALSE;
	}

	DWORD retValue;
	retValue = WSAAsyncSelect( listenSocket, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE );
	if ( retValue == SOCKET_ERROR )
	{
		return S_FALSE;
	}


	// 소켓의 세부 사항 조절 - getsockopt / setsockopt
	// http://vinchi.tistory.com/246 참조
	// SOL_SOCKET 레벨에서 SO_REUSEADDR 사용 - 이미 사용 된 주소를 재사용(bind) 하도록 한다.
	// -> Address already in use 방지
	int opt = 1;
	setsockopt( listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof( int ) );



	/// bind
	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof( serveraddr ) );
	serveraddr.sin_family = AF_INET; // IPv4
	serveraddr.sin_port = htons( LISTEN_PORT );
	serveraddr.sin_addr.s_addr = htonl( INADDR_ANY ); // 0.0.0.0

	// socket() 함수로 소켓을 생성 이후에 소켓에 주소 값을 설정하는 함수가 bind()
	retValue = bind( listenSocket, (SOCKADDR*)&serveraddr, sizeof( serveraddr ) );
	if ( retValue == SOCKET_ERROR )
	{
		return S_FALSE;
	}


	/// listen
	// SOMAXCONN = BackLog size : 연결 요청 대기 큐의 사이즈
	retValue = listen( listenSocket, SOMAXCONN );
	if ( retValue == SOCKET_ERROR )
	{
		return S_FALSE;
	}



	serverRunning = true;

	return S_OK;
}


LRESULT CALLBACK ServerMessageProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if ( !serverRunning )
	{
		return 0;
	}

	
	DWORD retValue;

	// 오류 발생 여부 확인
	if ( WSAGETSELECTERROR( lParam ) )
	{
		printf_s("WSAGETSELECTERROR1 : %s \n", WSAGETSELECTERROR( wParam ) );
		FreeSocketInformation( wParam );
		return 0;
	}

	// 각 상황별 메세지 처리
	switch ( WSAGETSELECTEVENT( lParam ) )
	{
		case FD_ACCEPT: // 클라이언트가 접속 되었을 시
		{
			SOCKADDR_IN clientAddr;
			int addrLength = sizeof( clientAddr );
			SOCKET clientSocket = accept( listenSocket, (SOCKADDR*)&clientAddr, &addrLength );
			if ( clientSocket == INVALID_SOCKET )
			{
				if ( WSAGetLastError() != WSAEWOULDBLOCK )
				{
					printf_s( "accept error : %s \n", "accept()" );
					break;
				}
				return 0;
			}

			printf_s( "[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트번호 = %d\r\n",
						 inet_ntoa( clientAddr.sin_addr ), ntohs( clientAddr.sin_port ) );

			CreateSocketInformation( clientSocket );

			// 접속된 클라이언트도 WSAAsyncSelect를 통해 설정해 준다.
			retValue = WSAAsyncSelect( clientSocket, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE );
			if ( retValue == SOCKET_ERROR )
			{
				printf_s( "WSAAsyncSelect error : %s \n", "WSAAsyncSelect()" );
				shutdown( clientSocket, SD_BOTH );
				closesocket( clientSocket );
				return 0;
			}
		}
			break;
			
		case FD_READ: // 패킷 도착
			LPSOCKET_INFORMATION SocketInfo;
			SocketInfo = GetSocketInformation( wParam );
			// Read data only if the receive buffer is empty
			if ( SocketInfo->BytesRECV != 0 )
			{
				SocketInfo->RecvPosted = TRUE;
				return 0;
			}
			else
			{
				SocketInfo->DataBuf.buf = SocketInfo->Buffer;
				SocketInfo->DataBuf.len = DATA_BUFSIZE;

				DWORD RecvBytes;
				DWORD Flags = 0;
				if ( WSARecv( SocketInfo->Socket, &( SocketInfo->DataBuf ), 1, &RecvBytes,
					&Flags, NULL, NULL ) == SOCKET_ERROR )
				{
					if ( WSAGetLastError() != WSAEWOULDBLOCK )
					{
						printf( "WSARecv() failed with error %d\n", WSAGetLastError() );
						FreeSocketInformation( wParam );
						return 0;
					}
				}
				else // No error so update the byte count
				{
					//printf( "WSARecv() is OK!\n" );
					SocketInfo->BytesRECV = RecvBytes;



					// 쓰기 버퍼에 값이 있으므로, 보내기를 수행하도록 메세지를 전달한다.
					// 에코를 할려면 그냥 break 빼버리면 성능은 좋아짐
					//PostMessage( hWnd, WM_SOCKET, wParam, FD_WRITE );
					//break;
				}
			}

			//break;
		case FD_WRITE: // 패킷 보내기
			SocketInfo = GetSocketInformation( wParam );
			if ( SocketInfo->BytesRECV > SocketInfo->BytesSEND )
			{
				SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
				SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;


				DWORD SendBytes;
				if ( WSASend( SocketInfo->Socket, &( SocketInfo->DataBuf ), 1, &SendBytes, 0, NULL, NULL ) == SOCKET_ERROR )
				{
					if ( WSAGetLastError() != WSAEWOULDBLOCK )
					{
						printf( "WSASend() failed with error %d\n", WSAGetLastError() );
						FreeSocketInformation( wParam );
						return 0;
					}
				}
				else // No error so update the byte count
				{
					//printf( "WSASend() is OK!\n" );
					SocketInfo->BytesSEND += SendBytes;
				}
			}

			if ( SocketInfo->BytesSEND == SocketInfo->BytesRECV )
			{
				SocketInfo->BytesSEND = 0;
				SocketInfo->BytesRECV = 0;
				// If a RECV occurred during our SENDs then we need to post an FD_READ notification on the socket
				if ( SocketInfo->RecvPosted == TRUE )
				{
					SocketInfo->RecvPosted = FALSE;
					PostMessage( hWnd, WM_SOCKET, wParam, FD_READ );
				}
			}
			
			break;
		case FD_CLOSE: // 접속 해제
			printf( "Closing socket %d\n", wParam );
			FreeSocketInformation( wParam );
			return 0;
			
	}

	
	return 0;
}

void CreateSocketInformation( SOCKET s )
{
	LPSOCKET_INFORMATION SI;

	// 이걸 미리 만들면 좀더 성능이 나아지겠지...
	if ( ( SI = (LPSOCKET_INFORMATION)GlobalAlloc( GPTR, sizeof( SOCKET_INFORMATION ) ) ) == NULL )
	{
		printf_s( "GlobalAlloc() failed with error %d\n", GetLastError() );
		return;
	}
	else
	{
		printf_s( "GlobalAlloc() for SOCKET_INFORMATION is OK!\n" );
	}
		

	// Prepare SocketInfo structure for use
	SI->Socket = s;
	SI->RecvPosted = FALSE;
	SI->BytesSEND = 0;
	SI->BytesRECV = 0;
	SI->Next = SocketInfoList;
	SocketInfoList = SI;
}

LPSOCKET_INFORMATION GetSocketInformation( SOCKET s )
{
	// 이걸 순환하지 않으면 좀더 성능을...

	SOCKET_INFORMATION *SI = SocketInfoList;

	while ( SI )
	{
		if ( SI->Socket == s )
		{
			return SI;
		}

		SI = SI->Next;
	}

	return NULL;
}

void FreeSocketInformation( SOCKET s )
{
	SOCKET_INFORMATION *SI = SocketInfoList;
	SOCKET_INFORMATION *PrevSI = NULL;

	while ( SI )
	{
		if ( SI->Socket == s )
		{
			if ( PrevSI )
			{
				PrevSI->Next = SI->Next;
			}
			else
			{
				SocketInfoList = SI->Next;
			}
				
			shutdown( s, SD_RECEIVE );
			closesocket( SI->Socket );
			GlobalFree( SI );
			return;
		}

		PrevSI = SI;
		SI = SI->Next;
	}
}


HWND InitWindow()
{
	WNDCLASSEX tWndClass;
	HINSTANCE  hInstance;
	TCHAR*     cpClassName;
	TCHAR*     cpWindowName;
	TCHAR*     cpMenu;
	HWND       hWnd;


	// 어플리케이션 인스턴스
	hInstance = ::GetModuleHandle( NULL );

	// 클래스 이름
	cpClassName = _T( "MainWindowClass" );

	// 메뉴
	cpMenu = MAKEINTRESOURCE( NULL );

	// 윈도우 이름
	cpWindowName = _T( "window in console program" );

	// 윈도우 클래스 파라미터 셋
	tWndClass.cbSize = sizeof( WNDCLASSEX );
	tWndClass.style = CS_HREDRAW | CS_VREDRAW;
	tWndClass.lpfnWndProc = WindowProc;
	tWndClass.cbClsExtra = 0;
	tWndClass.cbWndExtra = 0;
	tWndClass.hInstance = hInstance;
	tWndClass.hIcon = ::LoadIcon( NULL, IDI_APPLICATION );
	tWndClass.hCursor = ::LoadCursor( NULL, IDC_ARROW );
	tWndClass.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	tWndClass.lpszMenuName = cpMenu;
	tWndClass.lpszClassName = cpClassName;
	tWndClass.hIconSm = NULL;

	// 윈도우 클래스 생성
	if ( 0 == ::RegisterClassEx( &tWndClass ) )
	{
		// 실패
		return NULL;
	}

	// 윈도우 생성
	hWnd = ::CreateWindowEx(
		0                       // extended window style
		, tWndClass.lpszClassName // pointer to registered class name
		, cpWindowName            // pointer to window name
		, WS_OVERLAPPEDWINDOW     // window style
		, CW_USEDEFAULT           // horizontal position of window
		, CW_USEDEFAULT           // vertical position of window
		, CW_USEDEFAULT           // window width
		, CW_USEDEFAULT           // window height
		, NULL                    // handle to parent or owner window
		, NULL                    // handle to menu, or child-window identifier
		, hInstance               // handle to application instance
		, (VOID*)0x12345678       // pointer to window-creation data
		);

	return hWnd;

}


LRESULT CALLBACK WindowProc
(
HWND   hWnd   // handle to window
, UINT   uMsg   // message identifier
, WPARAM wParam // first message parameter
, LPARAM lParam // second message parameter
)
{
	switch ( uMsg )
	{
		case WM_CREATE:
			// 윈도우 표시
			//::ShowWindow( hWnd, SW_SHOW );

			ServerInit( hWnd );
			break;


		case WM_DESTROY:
			// 윈속 종료
			WSACleanup();
			::PostQuitMessage( 0 );
			break;

		case WM_SOCKET:
			ServerMessageProc( hWnd, uMsg, wParam, lParam );
			break;
		
		default:
			return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return ::DefWindowProc( hWnd, uMsg, wParam, lParam );
	
}