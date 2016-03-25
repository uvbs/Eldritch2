/*==================================================================*\
  Logger.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

#ifdef ERROR
#	if( ET_COMPILER_IS_MSVC )
	// Good old fashioned poor decisions from our friends in Redmond. Somebody thought defining ERROR as a macro was a great idea!
#		pragma push_macro( "ERROR" )
#	endif
#	undef ERROR
#	define CLEANUP_ERROR
#endif

	enum class LogMessageType {
		VerboseWarning = 0,
		Warning,
		Error,
		Message
	};

#ifdef CLEANUP_ERROR
#	if( ET_COMPILER_IS_MSVC )
#		pragma pop_macro( "ERROR" )
#	endif
#	undef CLEANUP_ERROR
#endif

// ---

	class ETPureAbstractHint Logger {
	// ---------------------------------------------------

	public:
		virtual void	Write( const ::Eldritch2::UTF8Char* const string, const size_t lengthInOctets ) abstract;

	// ---------------------------------------------------

		template <typename... Arguments>
		void	operator()( ETFormatStringHint( const ::Eldritch2::UTF8Char* str ), Arguments&&... arguments );

	// ---------------------------------------------------

	protected:
		//! Constructs this @ref Logger instance.
		/*!	Designed to be called from subclasses.
			*/
		ETInlineHint Logger() = default;

		//! Destroys this @ref Logger instance.
		ETInlineHint ~Logger() = default;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/Logger.inl>
//------------------------------------------------------------------//