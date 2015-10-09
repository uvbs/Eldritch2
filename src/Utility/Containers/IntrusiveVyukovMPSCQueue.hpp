/*==================================================================*\
  IntrusiveVyukovMPSCQueue.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#include <rdestl/intrusive_slist.h>
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2Detail {

	class IntrusiveVyukovMPSCQueueBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------
	
	protected:
		//!	Constructs this @ref IntrusiveVyukovMPSCQueueBase instance.
		/*!	Designed to be called from within subclasses.
			*/
		IntrusiveVyukovMPSCQueueBase();

		//!	Destroys this @ref IntrusiveVyukovMPSCQueueBase instance.
		/*!	Designed to be called from within subclasses.
			*/
		~IntrusiveVyukovMPSCQueueBase() = default;

	// ---------------------------------------------------

		::rde::intrusive_slist_node*	PopFront();

		void							PushBack( ::rde::intrusive_slist_node& object );

		void							PushStub();

	// - DATA MEMBERS ------------------------------------

	private:
		::rde::intrusive_slist_node									_stub;

		ET16ByteAligned ::std::atomic<::rde::intrusive_slist_node*>	_head;
		ET16ByteAligned ::std::atomic<::rde::intrusive_slist_node*>	_tail;
	};

}	// namespace Eldritch2Detail

namespace Eldritch2 {

	template <class StoredObject>
	class IntrusiveVyukovMPSCQueue : private ::Eldritch2Detail::IntrusiveVyukovMPSCQueueBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		typedef StoredObject	ValueType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref IntrusiveVyukovMPSCQueue instance.
		IntrusiveVyukovMPSCQueue() = default;

		//! Destroys this @ref IntrusiveVyukovMPSCQueue instance.
		~IntrusiveVyukovMPSCQueue() = default;

	// ---------------------------------------------------

		//!	Removes the head element from the queue.
		/*!	@returns A pointer to the removed element if the queue is not empty, or a null pointer if the queue was empty.
			@remarks Thread-safe.
			*/
		ValueType*	PopFront();

		void		PushBack( ValueType& object );
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/IntrusiveVyukovMPSCQueue.inl>
//------------------------------------------------------------------//