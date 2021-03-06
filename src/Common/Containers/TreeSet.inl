/*==================================================================*\
  TreeSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
  //------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet( const TreeSet<Value, SortPredicate, Allocator>& set ) : _container( set._container ) {}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet( TreeSet<Value, SortPredicate, Allocator>&& set ) : _container( eastl::move( set._container ) ) {}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>::TreeSet( const AllocatorType& allocator ) : _container( allocator ) {}

// ---------------------------------------------------
	
	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Find( const ValueType& itemTemplate ) {
		return _container.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Find( const ValueType& itemTemplate ) const {
		return _container.find( itemTemplate );
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	template <typename Predicate>
	ETInlineHint void TreeSet<Value, SortPredicate, Allocator>::RemoveIf( Predicate predicate ) {
		for (Iterator element( _container.begin() ), end( _container.end() ); element != end;) {
			if (predicate( *element )) {
				element = _container.erase( element );
			} else {
				++element;
			}
		}
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Begin() {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstBegin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::End() {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::ConstIterator TreeSet<Value, SortPredicate, Allocator>::ConstEnd() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>& TreeSet<Value, SortPredicate, Allocator>::operator=( const TreeSet<Value, SortPredicate, Allocator>& set ) {
		_container = set._container;

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>& TreeSet<Value, SortPredicate, Allocator>::operator=( TreeSet<Value, SortPredicate, Allocator>&& set ) {
		_container = eastl::move( set._container );

		return *this;
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint Pair<typename TreeSet<Value, SortPredicate, Allocator>::Iterator, bool> TreeSet<Value, SortPredicate, Allocator>::Insert( const ValueType& value ) {
		return _container.insert( value );
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase( Iterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::Iterator TreeSet<Value, SortPredicate, Allocator>::Erase( Iterator begin, Iterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint void TreeSet<Value, SortPredicate, Allocator>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename TreeSet<Value, SortPredicate, Allocator>::SizeType TreeSet<Value, SortPredicate, Allocator>::GetSize() const {
		return _container.size();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint bool TreeSet<Value, SortPredicate, Allocator>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint TreeSet<Value, SortPredicate, Allocator>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint typename const TreeSet<Value, SortPredicate, Allocator>::AllocatorType& TreeSet<Value, SortPredicate, Allocator>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template <typename Value, class SortPredicate, class Allocator>
	ETInlineHint void Swap( TreeSet<Value, SortPredicate, Allocator>& map0, TreeSet<Value, SortPredicate, Allocator>& map1 ) {
		eastl::swap( map0._container, map1._container );
	}

}	// namespace Eldritch2