#include "stdafx.h"
#include "CircularBuffer.h"


CircularBuffer::CircularBuffer( size_t capacity )
: m_BeginIndex( 0 ), m_EndIndex( 0 ), m_CurrentSize( 0 ), m_Capacity( capacity )
{
	m_Data = new char[capacity];
}

CircularBuffer::~CircularBuffer()
{
	delete[] m_Data;
}

bool CircularBuffer::Write( const char* data, size_t bytes )
{
	if ( bytes == 0 )
	{
		return false;
	}

	/// 용량 부족
	if ( bytes > m_Capacity - m_CurrentSize )
	{
		return false;
	}

	// 바로 쓰기 가능한 경우
	//////////////////////////////////////////////////////////////////////////
	// 끝부분 = mCapacity
	// 현재 데이터가 차 있는 끝 부분 = mEndIndex
	//
	// 현재 데이터 꽉 차 있는 끝부분부터 버퍼 끝까지 남은 공간 사이 - 1
	// = mCapacity - mEndIndex
	//////////////////////////////////////////////////////////////////////////
	if ( bytes <= m_Capacity - m_EndIndex )
	{
		memcpy( m_Data + m_EndIndex, data, bytes );
		m_EndIndex += bytes;

		if ( m_EndIndex == m_Capacity )
		{
			// 데이터가 꽉 차면 mEndIndex를 0으로 보내버림
			// 서큘러 큐 형태
			m_EndIndex = 0;
		}
	}
	// 쪼개서 써야 될 경우
	else
	{
		// 뒷부분 남은 공간 - 위의 주석 - 1 참조
		size_t size1 = m_Capacity - m_EndIndex;
		memcpy( m_Data + m_EndIndex, data, size1 );

		// 뒷부분 꽉 채웠으니 나머지 데이터는 시작 부분부터 앞부터 채워나감
		size_t size2 = bytes - size1;
		memcpy( m_Data, data + size1, size2 );
		m_EndIndex = size2;
	}

	m_CurrentSize += bytes;

	return true;
}

bool CircularBuffer::Read( char* data, size_t bytes )
{
	if ( bytes == 0 )
	{
		return false;
	}

	if ( m_CurrentSize < bytes )
	{
		return false;
	}

	/// 바로 한번에 읽어 올 수 있는 경우
	//////////////////////////////////////////////////////////////////////////
	// 버퍼의 끝부분 = mCapacity 
	// 현재 데이터가 차 있는 시작부분 = mBeginIndex
	//
	// 위에서 mCurrentSize < bytes 이니까
	// bytes <= mCapacity - mBeginIndex 라면 데이터 차 있는 시작부분부터 버퍼 끝까지
	// 꽉 차 있는 데이터 안에서 한 번에 bytes를 긁어 올 수 있겠지
	//////////////////////////////////////////////////////////////////////////
	if ( bytes <= m_Capacity - m_BeginIndex )
	{
		memcpy( data, m_Data + m_BeginIndex, bytes );
		m_BeginIndex += bytes;

		if ( m_BeginIndex == m_Capacity )
		{
			// 서큘러 큐 형태
			m_BeginIndex = 0;
		}
	}
	/// 읽어올 데이터가 쪼개져 있는 경우
	else
	{
		// 이건 시작부분부터 버퍼 메모리의 맨 끝까지
		size_t size1 = m_Capacity - m_BeginIndex;
		memcpy( data, m_Data + m_BeginIndex, size1 );

		// 이건 버퍼 메모리 시작위치부터 나머지 읽을 사이즈까지
		size_t size2 = bytes - size1;
		memcpy( data + size1, m_Data, size2 );
		m_BeginIndex = size2;
	}

	m_CurrentSize -= bytes;

	return true;
}

//////////////////////////////////////////////////////////////////////////
// 데이터를 읽기만 하므로 mBeginIndex, mEndIndex, mCurrentSize 등
// 내부 변수 변경이 없다.
//////////////////////////////////////////////////////////////////////////
// 데이터 들어 있는 만큼 통째로 Peek
void CircularBuffer::Peek( char* data ) const
{
	/// 바로 한번에 읽어 올 수 있는 경우
	if ( m_CurrentSize <= m_Capacity - m_BeginIndex )
	{
		memcpy( data, m_Data + m_BeginIndex, m_CurrentSize );
	}
	/// 읽어올 데이터가 쪼개져 있는 경우
	else
	{
		size_t size1 = m_Capacity - m_BeginIndex;
		memcpy( data, m_Data + m_BeginIndex, size1 );

		size_t size2 = m_CurrentSize - size1;
		memcpy( data + size1, m_Data, size2 );
	}
}

// 지정한 크기 만큼 Peek()
bool CircularBuffer::Peek( char* data, size_t bytes )
{
	if ( bytes == 0 )
	{
		return false;
	}

	if ( m_CurrentSize < bytes )
	{
		return false;
	}

	/// 바로 한번에 읽어 올 수 있는 경우
	if ( bytes <= m_Capacity - m_BeginIndex )
	{
		memcpy( data, m_Data + m_BeginIndex, bytes );
	}
	/// 읽어올 데이터가 쪼개져 있는 경우
	else
	{
		size_t size1 = m_Capacity - m_BeginIndex;
		memcpy( data, m_Data + m_BeginIndex, size1 );

		size_t size2 = bytes - size1;
		memcpy( data + size1, m_Data, size2 );
	}

	return true;
}


bool CircularBuffer::Consume( size_t bytes )
{
	if ( bytes == 0 )
	{
		return false;
	}

	if ( m_CurrentSize < bytes )
	{
		return false;
	}

	/// 바로 한번에 제거할 수 있는 경우
	if ( bytes <= m_Capacity - m_BeginIndex )
	{
		m_BeginIndex += bytes;

		if ( m_BeginIndex == m_Capacity )
		{
			m_BeginIndex = 0;
		}
	}
	/// 제거할 데이터가 쪼개져 있는 경우
	else
	{
		//////////////////////////////////////////////////////////////////////////
		// 제거할 데이터가 쪼개져 있다는 이야기는
		// mBeginIndex + bytes 하면 서큘러 연산처럼 넘어간단 뜻이므로
		// 모듈러 효과를 주기 위해 -mCapacity 해줘야 됨
		//////////////////////////////////////////////////////////////////////////
		size_t size2 = bytes + m_BeginIndex - m_Capacity;
		m_BeginIndex = size2;
	}

	m_CurrentSize -= bytes;

	return true;
}