#pragma once

#include "TypeTraits.h"
#include "FastSpinlock.h"
#include "Timer.h"


class SyncExecutable : public std::enable_shared_from_this<SyncExecutable>
{
public:
	SyncExecutable() : mLock(LO_BUSINESS_CLASS)
	{}
	virtual ~SyncExecutable() {}

	template <class R, class T, class... Args>
	R DoSync(R (T::*memfunc)(Args...), Args... args)
	{
		static_assert(true == std::is_convertible<T, SyncExecutable>::value, "T should be derived from SyncExecutable");

		//TODO: mLock으로 보호한 상태에서, memfunc를 실행하고 결과값 R을 리턴
		FastSpinlockGuard guard( mLock );

		//std::function<R( Args... )> test = std::bind( memfunc,  args ... );

		//auto test = std::bind( memfunc, std::forward<Args>( args )... );
		//std::function<R( Args... )>* asd2 = test;
		//std::function<R( Args... )> ads = asd2;
		//auto test = memfunc( args... );
		//mPlayer->DoSync( &Player::PlayerReset );
		//

		//std::function<void( int )> f_add_display2 = std::bind( &Foo::print_add, foo, _1 );
		//std::function<void() > asd = memfunc;
		//memfunc( memfunc(args...));
		//return ((memfunc) Args( args... ));

		return std::bind( memfunc, static_cast<T*>( this ), std::forward<Args>( args )... )( );
	}
	

	void EnterLock() { mLock.EnterWriteLock(); }
	void LeaveLock() { mLock.LeaveWriteLock(); }
	
 	template <class T>
	std::shared_ptr<T> GetSharedFromThis()
 	{
		static_assert(true == std::is_convertible<T, SyncExecutable>::value, "T should be derived from SyncExecutable");
 		
		//TODO-: this 포인터를 std::shared_ptr<T>형태로 반환.
		//(HINT: 이 클래스는 std::enable_shared_from_this에서 상속받았다. 그리고 static_pointer_cast 사용)
		//return std::shared_ptr<T>((Player*)this); ///< 이렇게 하면 안될걸???

		return std::static_pointer_cast<T>( shared_from_this() );
 	}

private:

	FastSpinlock mLock;
};


template <class T, class F, class... Args>
void DoSyncAfter(uint32_t after, T instance, F memfunc, Args&&... args)
{
	static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
	static_assert(true == std::is_convertible<T, std::shared_ptr<SyncExecutable>>::value, "T should be shared_ptr SyncExecutable");

	//TODO-: instance의 memfunc를 bind로 묶어서 LTimer->PushTimerJob() 수행

	//auto bind = std::bind( memfunc, instance, std::forward<Args>( args )... );
	//LTimer->PushTimerJob( instance, bind, after );

	LTimer->PushTimerJob( instance, std::bind( memfunc, instance, std::forward<Args>( args )... ), after );
	
}