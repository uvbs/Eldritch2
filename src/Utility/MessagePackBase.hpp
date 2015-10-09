/*==================================================================*\
  MessagePackBase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <cmp/cmp.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint MessagePackBase : protected ::cmp_ctx_t {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum KnownExtendedDataMarkers : ::Eldritch2::int8 {
			OBJECT_REFERENCE	= 0,
			TYPE_INDEX,
			TYPE_STRING,
			FLOAT4,
			INT4,

			CUSTOM_TYPE_BEGIN	= 100
		};

	// ---

		template <class Container>
		struct DefaultKeyExtractor {
			auto operator()( typename Container::ValueType& value ) -> decltype((value.first)) {
				return value.first;
			}

			auto operator()( typename const Container::ValueType& value ) -> decltype((value.first)) {
				return value.first;
			}
		};

	// ---

		template <class Container>
		struct DefaultValueExtractor {
			auto operator()( typename Container::ValueType& value ) -> decltype((value.second)) {
				return value.second;
			}

			auto operator()( typename const Container::ValueType& value ) -> decltype((value.second)) {
				return value.second;
			}
		};

	// ---

		struct Nil {};

	// ---

		struct ArrayHeader {
			::Eldritch2::uint32 arraySizeInElements;
		};

	// ---

		struct MapHeader {
			::Eldritch2::uint32 mapSizeInPairs;
		};

	// ---

		struct ObjectReference {
			::Eldritch2::uint32	identifier;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this MessagePackBase instance.
		MessagePackBase( const ::cmp_reader readFunction, const void* const buffer = nullptr );
		//! Constructs this MessagePackBase instance.
		MessagePackBase( const ::cmp_writer writeFunction, void* const buffer = nullptr );
	};

}	// namespace Utility
}	// namespace Eldritch2
