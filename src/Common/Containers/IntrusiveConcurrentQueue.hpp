/*==================================================================*\
  IntrusiveConcurrentQueue.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/IntrusiveForwardListHook.hpp>
#include <Common/Atomic.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	class QueueStorage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Disable copy construction.
		QueueStorage( const QueueStorage& ) = delete;
	//!	Constructs this @ref QueueStorage instance.
		QueueStorage();

		~QueueStorage() = default;

	// - DATA MEMBERS ------------------------------------

	protected:
		IntrusiveForwardListBaseHook								_stub;
	//	These are aligned to cache boundaries to avoid contention.
		ETCacheLineAligned Atomic<IntrusiveForwardListBaseHook*>	_head;
		ETCacheLineAligned Atomic<IntrusiveForwardListBaseHook*>	_tail;
	};

// ---------------------------------------------------

	class MpscQueuePolicy : public QueueStorage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Disable copy construction.
		MpscQueuePolicy( const MpscQueuePolicy& ) = delete;
	//!	Constructs this @ref MpscQueuePolicy instance.
		MpscQueuePolicy() = default;

		~MpscQueuePolicy() = default;

	// ---------------------------------------------------

	protected:
		IntrusiveForwardListBaseHook*	Pop();

		void							Push( IntrusiveForwardListBaseHook& object );
	};

// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	class IntrusiveConcurrentQueue : private ElementPolicy {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ValueType	= Value;
		using Reference = ValueType&;
		using Pointer	= ValueType*;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref IntrusiveConcurrentQueue instance.
		IntrusiveConcurrentQueue( const IntrusiveConcurrentQueue& ) = delete;
	//!	Constructs this @ref IntrusiveConcurrentQueue instance.
		IntrusiveConcurrentQueue( IntrusiveConcurrentQueue&& );
	//!	Constructs this @ref IntrusiveConcurrentQueue instance.
		IntrusiveConcurrentQueue() = default;

		~IntrusiveConcurrentQueue() = default;

	// ---------------------------------------------------

	public:
	//!	Removes the head element from the queue.
	/*!	@returns A pointer to the removed element if the queue is not empty, or a null pointer if the queue was empty.
		@remarks Thread-safe. */
		ValueType*	Pop();

		template <typename Disposer>
		bool		PopAndDispose( Disposer disposer );

		void		Push( ValueType& object );

	// ---------------------------------------------------

		template <typename Disposer>
		void	ClearAndDispose( Disposer disposer );

		void	Clear();
	};

}	// namespace Detail

	template <typename Value>
	using IntrusiveMpscQueue = Detail::IntrusiveConcurrentQueue<Value, Detail::MpscQueuePolicy>;

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/IntrusiveConcurrentQueue.inl>
//------------------------------------------------------------------//