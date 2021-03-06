/*==================================================================*\
  AbstractString.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#include <eastl/algorithm.h>
ET_PUSH_COMPILER_WARNING_STATE()
	ET_SET_MSVC_WARNING_STATE( disable : 4459 4244 )
#	include <fmt/format.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const AlternateCharacter* const begin,
		const AlternateCharacter* const end,
		const AllocatorType& allocator
	) : _container( UnderlyingContainer::CtorConvert(), begin, eastl::distance( begin, end ), allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const CharacterType* const begin,
		const CharacterType* const end,
		const AllocatorType& allocator
	) : _container( begin, end, allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString( 
		const CharacterType* const string,
		const AllocatorType& allocator
	) : _container( string, allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const AlternateCharacter* const string,
		const AllocatorType& allocator
	) : _container( UnderlyingContainer::CtorConvert(), string, StringLength( string ), allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const AbstractString<Character, Allocator>& string,
		const AllocatorType& allocator
	) : _container( string.Begin(), string.End(), allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		SizeType reservedLength,
		const AllocatorType& allocator
	) : _container( UnderlyingContainer::CtorDoNotInitialize(), reservedLength, allocator ) {}

// ---------------------------------------------------
	
	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const AllocatorType& allocator
	) : _container( allocator ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		const AbstractString<Character, Allocator>& string
	) : AbstractString( string, string.GetAllocator() ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::AbstractString(
		AbstractString<Character, Allocator>&& string
	) : _container( eastl::move( string._container ) ) {}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( ConstIterator start, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		const Character* const	needleEnd( FindTerminator( needle ) );
		const Character* const	position( eastl::search( start, _container.cend(), needle, needleEnd ) );

		return eastl::min( _container.cend(), position + ( needleEnd - needle ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( ConstIterator start, const CharacterType* const needle ) const {
		return eastl::search( start, _container.cend(), needle, Eldritch2::FindTerminator( needle ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return this->FindFirstInstance( _container.cbegin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( const CharacterType* const needle ) const {
		return this->FindFirstInstance( _container.cbegin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( ConstIterator start, CharacterType character ) const {
		return eastl::find( start, _container.cend(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindFirstInstance( CharacterType character ) const {
		return this->FindFirstInstance( _container.cbegin(), character );
	}

// ---------------------------------------------------
	

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( const CharacterType* const needle, const ReturnEndOfNeedleSemantics semantics ) const {
		return this->FindLastInstance( Begin(), needle, semantics );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( const CharacterType* const needle ) const {
		return this->FindLastInstance( Begin(), needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( ConstIterator start, const CharacterType* const needle, const ReturnEndOfNeedleSemantics ) const {
		const Character* const	position( ::Eldritch2::FindLastInstance( start, needle ) );

		return position ? position + StringLength( needle ) : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( ConstIterator start, const CharacterType* const needle ) const {
		const Character* const	position( ::Eldritch2::FindLastInstance( start, needle ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( ConstIterator start, CharacterType character ) const {
		const Character* const	position( ::Eldritch2::FindLastInstance( start, character ) );

		return position ? position : End();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::FindLastInstance( CharacterType character ) const {
		return this->FindLastInstance( Begin(), character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::Contains( const CharacterType* const needle ) const {
		return _container.cend() != this->FindFirstInstance( needle );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::Contains( CharacterType character ) const {
		return _container.cend() != this->FindFirstInstance( character );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::MakeLowerCase() {
		_container.make_lower();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::MakeUpperCase() {
		_container.make_upper();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Replace( CharacterType source, CharacterType replacement ) {
		Iterator	position( _container.begin() );
		Iterator	end( _container.end() );

		while (position != end) {
			position = eastl::find( position, end, source );
			if (position != end) {
				*position++ = replacement;
			}
		}
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::HasPrefix( const CharacterType* const needle ) const {
		const SizeType needleLength( StringLength( needle ) );

		return (needleLength <= GetLength()) ? StringsEqual( AsCString(), needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::HasPrefix( CharacterType character ) const {
		return (*this)[0] == character;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::HasSuffix( const CharacterType* const needle ) const {
		const SizeType needleLength( StringLength( needle ) );

		return (needleLength <= GetLength()) ? StringsEqual( End() - needleLength, needle, needleLength ) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::HasSuffix( const CharacterType character ) const {
		return (*this) ? (End()[-1] == character) : false;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Erase( ConstIterator begin, ConstIterator end ) {
		return _container.erase( begin, end );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Erase( ConstIterator position ) {
		return _container.erase( position );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::Begin() const {
		return _container.begin();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::ConstIterator AbstractString<Character, Allocator>::End() const {
		return _container.end();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator=( const AbstractString<Character, Allocator>& string ) {
		_container = string._container;
		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator=( AbstractString<Character, Allocator>&& string ) {
		_container = eastl::move( string._container );
		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator=( const CharacterType* const string ) {
		_container = string;
		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign( const CharacterType* const begin, const CharacterType* const end ) {
		_container.assign( begin, end );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter, typename AlternateAllocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign( const AbstractString<AlternateCharacter, AlternateAllocator>& string ) {
		_container.assign( string._container );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Assign( const CharacterType* const string ) {
		_container.assign( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const AlternateCharacter* const begin, const AlternateCharacter* const end ) {
		_container.append_convert( begin, eastl::distance( begin, end ) );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const CharacterType* const begin, const CharacterType* const end ) {
		_container.append( begin, end );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const AlternateCharacter* const string ) {
		_container.append_convert( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const CharacterType* const string ) {
		_container.append( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter, class AlternateAllocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const AbstractString<AlternateCharacter, AlternateAllocator>& string ) {
		_container.append_convert( string._container );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( const AbstractString<Character, Allocator>& string ) {
		_container.append( string._container );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::Append( CharacterType character ) {
		_container.push_back( character );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix( const CharacterType* const string ) {
		if (!HasSuffix( string )) {
			Append( string );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::EnsureSuffix( CharacterType character ) {
		if (!HasSuffix( character )) {
			Append( character );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter, class AlternateAllocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=( const AbstractString<AlternateCharacter, AlternateAllocator>& string ) {
		_container.append_convert( string.Begin(), string.GetLength() );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=( const AbstractString<Character, Allocator>& string ) {
		_container += string._container;

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <typename AlternateCharacter>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=( const AlternateCharacter* const string ) {
		_container.append_convert( string );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=( const CharacterType* const string ) {
		_container += string;

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>& AbstractString<Character, Allocator>::operator+=( CharacterType character ) {
		_container.push_back( character );

		return *this;
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	template <size_t formatCount, typename... Arguments>
	ETInlineHint void AbstractString<Character, Allocator>::Format( const Character (&formatString)[formatCount], Arguments&&... arguments ) {
		fmt::internal::container_buffer<UnderlyingContainer>	target( _container );

		fmt::vformat_to( target, fmt::basic_string_view<Character>( formatString, formatCount ), fmt::make_args( eastl::forward<Arguments>( arguments )... ) );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Clear() {
		_container.clear();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Trim( SizeType charactersToAdvanceBeginning, SizeType charactersToRemoveFromEnd ) {
		_container.erase( _container.end() - charactersToRemoveFromEnd, _container.end() );
		_container.erase( _container.begin(), _container.begin() + charactersToAdvanceBeginning );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Trim( ConstIterator newBegin, ConstIterator newEnd ) {
		_container.erase( newEnd, _container.end() );
		_container.erase( _container.begin(), newBegin );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Resize( const SizeType size ) {
		_container.resize( size );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int AbstractString<Character, Allocator>::Compare( const CharacterType* const string ) const {
		return OrderStrings( this->AsCString(), string );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint int AbstractString<Character, Allocator>::Compare( const AbstractString<Character, Allocator>& string ) const {
		return OrderStrings( this->AsCString(), string.AsCString() );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::CharacterType AbstractString<Character, Allocator>::operator[]( SizeType indexInCharacters ) const {
		return _container[indexInCharacters];
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::operator typename const AbstractString<Character, Allocator>::CharacterType*() const {
		return _container.c_str();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::AsCString() const {
		return _container.c_str();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename const AbstractString<Character, Allocator>::CharacterType* AbstractString<Character, Allocator>::GetData() {
		return _container.data();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetLength() const {
		return _container.length();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint bool AbstractString<Character, Allocator>::IsEmpty() const {
		return _container.empty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint AbstractString<Character, Allocator>::operator bool() const {
		return !_container.empty();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void AbstractString<Character, Allocator>::Reserve( SizeType capacityInCharacters ) {
		_container.reserve( capacityInCharacters );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint typename AbstractString<Character, Allocator>::SizeType AbstractString<Character, Allocator>::GetCapacityInCharacters() const {
		return _container.capacity();
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint const typename AbstractString<Character, Allocator>::AllocatorType& AbstractString<Character, Allocator>::GetAllocator() const {
		return _container.get_allocator();
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const AbstractString<Character, StringAllocator>& string0, const AbstractString<Character, StringAllocator>& string1 ) {
		return string0.Compare( string1 ) == 0;
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const AbstractString<Character, StringAllocator>& string0, const Character* const string1 ) {
		return string0.Compare( string1 ) == 0;
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator==( const Character* const string0, const AbstractString<Character, StringAllocator>& string1 ) {
		return string1.Compare( string0 ) == 0;
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const AbstractString<Character, StringAllocator>& string0, const AbstractString<Character, StringAllocator>& string1 ) {
		return string0.Compare( string1 ) != 0;
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const AbstractString<Character, StringAllocator>& string0, const Character* const string1 ) {
		return string0.Compare( string1 ) != 0;
	}
	
// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint bool operator!=( const Character* const string0, const AbstractString<Character, StringAllocator>& string1 ) {
		return string1.Compare( string0 ) != 0;
	}

// ---------------------------------------------------

	template <typename Character, class StringAllocator>
	ETInlineHint ETPureFunctionHint size_t GetHashCode( const AbstractString<Character, StringAllocator>& string, size_t hashSeed ) {
		return HashMemory( string.AsCString(), string.GetLength() * sizeof(Character), hashSeed );
	}

// ---------------------------------------------------

	template <typename Character, class Allocator>
	ETInlineHint void Swap( AbstractString<Character, Allocator>& string0, AbstractString<Character, Allocator>& string1 ) {
		eastl::swap( string0._container, string1._container );
	}

}	// namespace Eldritch2