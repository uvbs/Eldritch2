/*==================================================================*\
  FiberJobSystem.JobThread.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/Win32/FiberJobSystem.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
namespace Win32 {

	FiberJobSystem::JobThread::JobThread(
		FiberJobSystem& owner
	) : JobExecutor(),
		_owner( &owner ),
		_thief( this ),
		_victimSeed( static_cast<size_t>(__rdtsc()) ),
		_runBehavior( RunBehavior::Continue ),
		_transferCell( TransferState::Complete ),
		_victim( nullptr ) {}

// ---------------------------------------------------

	FiberJobSystem::JobThread::JobThread(
		const JobThread& thread
	) : JobExecutor(),
		_owner( thread._owner ),
		_thief( this ),
		_victimSeed( static_cast<size_t>(__rdtsc()) ),
		_runBehavior( RunBehavior::Continue ),
		_transferCell( TransferState::Complete ),
		_victim( nullptr ){}

// ---------------------------------------------------

	FiberJobSystem::JobThread::~JobThread() {
		DeleteFiber( _waitFiber );
		DeleteFiber( _switchFiber );
	}

// ---------------------------------------------------

	Utf8Literal FiberJobSystem::JobThread::GetName() const {
		return "Worker Thread";
	}

// ---------------------------------------------------

	void FiberJobSystem::JobThread::Run() {
		BootSupportFibers();

		_bootFiber	= ConvertThreadToFiberEx( this, FIBER_FLAG_FLOAT_SWITCH );

	//	Client fibers have relinqished execution; time to shut down.
		while (_pooledFibers) {
			const PlatformFiber	fiber( _pooledFibers.Back() );

			_pooledFibers.Pop();

			SwitchToFiber( fiber );
			DeleteFiber( _transitionSource.fiber );

		//	We will resume here once one of the work fibers receives the quit signal.
		}

		ConvertFiberToThread();
	}

// ---------------------------------------------------

	void FiberJobSystem::JobThread::SetShouldShutDown() {
		_runBehavior.store( RunBehavior::Terminate, std::memory_order_release );
	}

// ---------------------------------------------------

	void FiberJobSystem::JobThread::FiberEntryPoint() {
	//	Push this context out to the external object model.
		SetActive();

		_thief.store( nullptr, std::memory_order_release );

		while (ET_LIKELY( _runBehavior.load( std::memory_order_consume ) != RunBehavior::Terminate )) {
		/*	Distribute any work (if it remains) to other worker threads as requested. Prefer the expanded load/store
		 *	over exchange here to avoid false sharing/contention. */
			if (JobThread* const thief = _thief.load( std::memory_order_consume )) {
				auto begin( _jobs.Begin() );
				auto end( _jobs.End() );

				begin += (end - begin) / 2;

				thief->_jobs.Insert( thief->_jobs.Begin(), eastl::make_move_iterator( begin ), eastl::make_move_iterator( end ) );

			//	Ensure all our writes complete before signaling a finished state.
				thief->_transferCell.store( TransferState::Complete, std::memory_order_release );

				_thief.store( nullptr, std::memory_order_relaxed );
				_jobs.Erase( begin, end );
			}

		/*	Execute one of the following code paths:
		 *	1) Complete an outstanding work item.
		 *	2) Resume a suspended work item. 
		 *	3) Steal work. */

			if (_jobs) {
			/*	Work items may suspend and re-enter this function in a different fiber, so it is important that the element be
			 *	destroyed *before* execution to avoid races. */
				const JobClosure job( eastl::move( _jobs.Back() ) );
				_jobs.Pop();
			
				job.work( *this );
				job.completedFence->fetch_sub( 1, std::memory_order_release );

				continue;
			}

			if (_suspendedJobs) {
			/*	Locate a closure capable of resuming by evaluating its condition function. Empirically, most stalled work items
			 *	are jobs waiting for their dependencies to complete in LIFO order. We can exploit this to reduce the number of tests
			 *	by searching the list of suspended fibers in reverse order. */
				const auto	candidate( FindIf( _suspendedJobs.ReverseBegin(), _suspendedJobs.ReverseEnd(), [] ( const SuspendedJob& job ) {
					return job.shouldResume();
				} ) );

				if (candidate != _suspendedJobs.ReverseEnd()) {
					const Detail::PlatformFiber	fiber( candidate->fiber );

					_suspendedJobs.Erase( candidate, UnorderedSemantics() );

					SwitchFibers( fiber );
				}

				continue;
			}

			{
			/*	At this point we have no previously queued work items to complete and no suspended tasks are ready to resume. In the interest of
			 *	Pareto optimality, find a victim thread and attempt to load balance work across the two threads by way of a steal-half sharing
			 *	policy. */

			//	Write is not explicitly ordered as transfer cell is only visible to us until we find a victim.
				_transferCell.store( TransferState::AwaitingTransfer, std::memory_order_relaxed );

			//	Mark as unable to share.
				if (JobThread* const thief = _thief.exchange( this, std::memory_order_consume )) {
					thief->_transferCell.store( TransferState::Complete, std::memory_order_relaxed );
				}

				JobThread& victim( _owner->FindVictim( _victimSeed ) );
				if (victim.BeginShareWith( *this )) {
		#if ET_ENABLE_JOB_DEBUGGING
				//	Update the victim value for easier inspection in a debugger.
					_victim = &victim;
		#endif	// ET_ENABLE_JOB_DEBUGGING
				//	Enter idle loop waiting for completion.
					while (ShouldAwaitTransfer() && victim.IsRunning()) {
						_mm_pause();
					}
		#if ET_ENABLE_JOB_DEBUGGING
					_victim = nullptr;
		#endif	// ET_ENABLE_JOB_DEBUGGING
				}

			//	Mark as being able to share again.
				_thief.store( nullptr, std::memory_order_release );
			}
		}

	//	Mark ourselves as unable to be stolen from on death-- we're not producing work items any more!
		if (JobThread* const thief = _thief.exchange( this, std::memory_order_consume )) {
		//	Complete transfer without sending any work items.
			thief->_transferCell.store( TransferState::Complete, std::memory_order_release );
		}

	//	Switch back to the main fiber.
		_transitionSource.fiber = GetCurrentFiber();
		SwitchToFiber( _bootFiber );
	}

}	// namespace Win32
}	// namespace Scheduling
}	// namespace Eldritch2