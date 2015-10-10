/*==================================================================*\
  OrderedSet.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/RDESTLAllocatorAdapterMixin.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
//------------------------------------------------------------------//
#include <rdestl/set.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename StoredObject, class Allocator = ::Eldritch2::ChildAllocator>
	class OrderedSet {
	// - TYPE PUBLISHING ---------------------------------

	protected:
		using PrivateAllocator		= ::Eldritch2Detail::RDESTLAllocatorAdapterMixin<Allocator>;
		using UnderlyingContainer	= ::rde::set<StoredObject, PrivateAllocator>;

	public:
		using ValueType				= typename UnderlyingContainer::value_type;
		using AllocatorType			= Allocator;
		using Iterator				= typename UnderlyingContainer::iterator;
		using ConstIterator			= typename UnderlyingContainer::const_iterator;
		using InsertResult			= ::rde::pair<Iterator, bool>;
		using SizeType				= typename AllocatorType::SizeType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref OrderedSet instance.
		template <typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//!	Constructs this @ref OrderedSet instance.
		template <typename InputIterator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( InputIterator begin, InputIterator end, AllocatorConstructorArguments&&... allocatorConstructorArguments );
		//!	Constructs this @ref OrderedSet instance.
		template <class AlternateAllocator, typename... AllocatorConstructorArguments>
		ETInlineHint OrderedSet( const ::Eldritch2::OrderedSet<StoredObject, AlternateAllocator>& containerTemplate, AllocatorConstructorArguments&&... allocatorConstructorArguments );

		//!	Destroys this @ref OrderedSet instance.
		ETInlineHint ~OrderedSet();

	// - ALGORITHMS --------------------------------------

		ETInlineHint Iterator		Find( const ValueType& itemTemplate );
		ETInlineHint ConstIterator	Find( const ValueType& itemTemplate ) const;

		template <typename Predicate>
		ETInlineHint void			RemoveIf( Predicate predicate );
		template <typename ExtraArgumentType, typename Predicate>
		ETInlineHint void			RemoveIf( ExtraArgumentType extraArgument, Predicate predicate );

	// - ELEMENT ITERATION -------------------------------

		ETInlineHint Iterator		Begin();
		ETInlineHint ConstIterator	Begin() const;

		ETInlineHint ConstIterator	ConstBegin() const;

		ETInlineHint Iterator		End();
		ETInlineHint ConstIterator	End() const;

		ETInlineHint ConstIterator	ConstEnd() const;

	// - CONTAINER DUPLICATION ---------------------------

		ETInlineHint void	CloneFrom( const ::Eldritch2::OrderedSet<StoredObject, Allocator>& containerTemplate );

		ETInlineHint void	Swap( ::Eldritch2::OrderedSet<StoredObject, Allocator>& other );

	// - CONTAINER MANIPULATION --------------------------

		ETInlineHint InsertResult	Insert( const ValueType& v );

		ETInlineHint void	Erase( Iterator position );
		ETInlineHint void	Erase( Iterator begin, Iterator end );

		ETInlineHint void	Clear();

	// - CONTENT QUERY -----------------------------------

		ETInlineHint SizeType	Size() const;

		ETInlineHint bool	Empty() const;

		ETInlineHint	operator bool() const;

	// - ALLOCATOR ACCESS --------------------------------

		ETInlineHint const AllocatorType&	GetAllocator() const;

	// - DATA MEMBERS ------------------------------------

	private:
		UnderlyingContainer	_underlyingContainer;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Containers/OrderedSet.inl>
//------------------------------------------------------------------//