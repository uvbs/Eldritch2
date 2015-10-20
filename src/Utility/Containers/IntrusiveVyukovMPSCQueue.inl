/*==================================================================*\
  IntrusiveVyukovMPSCQueue.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//


namespace Eldritch2 {

	template <class StoredObject>
	typename IntrusiveVyukovMPSCQueue<StoredObject>::ValueType* IntrusiveVyukovMPSCQueue<StoredObject>::PopFront() {
		return static_cast<ValueType*>(IntrusiveVyukovMPSCQueueBase::PopFront());
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	bool IntrusiveVyukovMPSCQueue<StoredObject>::PopFrontAndDispose( Disposer disposer ) {
		if( auto* const element = PopFront() ) {
			disposer( *element );
			return true;
		}

		return false;
	}

// ---------------------------------------------------

	template <class StoredObject>
	void IntrusiveVyukovMPSCQueue<StoredObject>::PushBack( ValueType& object ) {
		IntrusiveVyukovMPSCQueueBase::PushBack( object );
	}

// ---------------------------------------------------

	template <class StoredObject>
	template <typename Disposer>
	void IntrusiveVyukovMPSCQueue<StoredObject>::ClearAndDispose( Disposer disposer ) {
		while( PopFrontAndDispose( disposer ) );
	}

}	// namespace Eldritch2