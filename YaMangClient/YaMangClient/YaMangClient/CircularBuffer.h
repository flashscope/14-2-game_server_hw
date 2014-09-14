#pragma once

//////////////////////////////////////////////////////////////////////////
// 서버쪽 서큘러 버퍼와 클라쪽 서큘러 버퍼가 다름
//////////////////////////////////////////////////////////////////////////
class CircularBuffer
{
public:
  CircularBuffer(size_t capacity) ;
  ~CircularBuffer() ;

  size_t GetCurrentSize() const
  { 
	  return m_CurrentSize ; 
  }

  size_t GetCapacity() const
  { 
	  return m_Capacity ;
  }

  bool Write(const char* data, size_t bytes) ;
  
  bool Read(char* data, size_t bytes) ;

  /// 데이터 훔쳐보기 (제거하지 않고)
  void Peek(char* data) const;
  bool Peek(char* data, size_t bytes) ;

  /// 데이터 제거
  bool Consume(size_t bytes) ;

  void Reset() { m_CurrentSize = 0; m_BeginIndex = 0; m_EndIndex = 0; }

private:
  size_t m_BeginIndex ;
  size_t m_EndIndex ;
  size_t m_CurrentSize ;
  size_t m_Capacity ;

  char* m_Data ;
} ;
