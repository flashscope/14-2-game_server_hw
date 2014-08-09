#pragma once
#include "Exception.h"
#include "TypeTraits.h"
#include "XTL.h"
#include "ThreadLocal.h"

class GrandCentralExecuter
{
public:
	typedef std::function<void()> GCETask;

	GrandCentralExecuter(): mRemainTaskCount(0)
	{}

	void DoDispatch(const GCETask& task)
	{
		CRASH_ASSERT(LThreadType == THREAD_IO_WORKER); ///< �ϴ� IO thread ����

		
		if (InterlockedIncrement64(&mRemainTaskCount) > 1)
		{
			//TODO: �̹� ������ �۾����̸� ���?
			
		}
		else
		{
			/// ó�� ������ ���� å������ �������� -.-;

			mCentralTaskQueue.push(task);
			
			while (true)
			{
				GCETask task;
				if (mCentralTaskQueue.try_pop(task))
				{
					//TODO: task�� �����ϰ� mRemainTaskCount�� �ϳ� ���� 
					// mRemainTaskCount�� 0�̸� break;
					if ( InterlockedDecrement64( &mRemainTaskCount ) == 0)
					{
						break;
					}
				}
			}
		}

	}


private:
	typedef concurrency::concurrent_queue<GCETask, STLAllocator<GCETask>> CentralTaskQueue;
	CentralTaskQueue mCentralTaskQueue;
	int64_t mRemainTaskCount;
};

extern GrandCentralExecuter* GGrandCentralExecuter;



template <class T, class F, class... Args>
void GCEDispatch(T instance, F memfunc, Args&&... args)
{
	/// shared_ptr�� �ƴ� �༮�� ������ �ȵȴ�. �۾�ť�� ����ִ��߿� ������ �� ������..
	static_assert(true == is_shared_ptr<T>::value, "T should be shared_ptr");
	
	//TODO: intance�� memfunc�� std::bind�� ��� ����

	auto bind = std::bind( memfunc, instance, std::forward<Args>( args )... ) ;
	
	
	GGrandCentralExecuter->DoDispatch(bind);
}
/*



195 template < class T, class F, class... Args>
196 void DoAsync( T instance, F memfunc, Args&&... args )
197 {
	198 	static_assert( true == is_shared_ptr<T>::value, "T should be shared_ptr" );
	199 	static_assert( true == std::is_convertible<T, std::shared_ptr<AsyncExecutable>>::value, "T should be shared_ptr AsyncExecutable" );
	200
		201 	JobEntry* job = new JobEntry( std::bind( memfunc, instance, std::forward<Args>( args )... ) );
	202

		203 	std::static_pointer_cast<AsyncExecutable>( instance )->DoTask( job );
	204 }
205

206 template < class T, class F, class... Args>
207 void DoAsyncAfter( uint32_t after, T instance, F memfunc, Args&&... args )
208 {
	209 	static_assert( true == is_shared_ptr<T>::value, "T should be shared_ptr" );
	210 	static_assert( true == std::is_convertible<T, std::shared_ptr<AsyncExecutable>>::value, "T should be shared_ptr AsyncExecutable" );
	211

		212 	JobEntry* job = new JobEntry( std::bind( memfunc, instance, std::forward<Args>( args )... ) );
	213 	LTimer->PushTimerJob( std::static_pointer_cast<AsyncExecutable>( instance ), after, job );
	214 }
	*/