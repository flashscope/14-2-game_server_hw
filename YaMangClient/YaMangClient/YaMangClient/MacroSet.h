#pragma  once

#include <cassert>
#define ALIGNMENT_SIZE	16

/**
*@brief		싱글톤 템플릿
*@details	모든 싱글톤은 이 템플릿을 상속 받아 사용합니다.
*			D3DXMATRIXA16 객체 사용 때문에 16바이트로 align하고 있습니다.
*@author	zrma
*@date		2014-04-25
*
*@todo		16바이트 align이 필요한 클래스에 대해 #ifdef ... #endif 등을 활용 할 수 있다.
*/
template <class T>
class Singleton
{
protected:
	Singleton( void ) {}
	virtual ~Singleton( void ) {}
public:
	static T* GetInstance( void )
	{
		if ( !m_Instance )
		{
			m_Instance = (T*) _aligned_malloc(sizeof(T), ALIGNMENT_SIZE);
			new ( m_Instance ) T();
		}
		return m_Instance;
	}
	
	static void Release( void )
	{
		if ( m_Instance )
		{
			m_Instance->~T();
			_aligned_free( m_Instance );
		}
	}

private:
	static T*	m_Instance;
};
template <typename T>
T* Singleton<T>::m_Instance = nullptr;


template <typename T>
inline void SafeRelease( T* &p )
{
	if ( p != nullptr )
	{
		p->Release();
		p = nullptr;
	}
}

template <typename T>
inline void SafeDelete( T* &p )
{
	if ( p != nullptr )
	{
		delete p;
		p = nullptr;
	}
}