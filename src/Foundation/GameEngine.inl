/*==================================================================*\
  GameEngine.inl
  ------------------------------------------------------------------
  Purpose:
  Implements an 'engine' object that binds together most subsystems
  into a coherent whole. Also serves as an intermediate layer
  between higher-level OS-dependent application systems and
  low-level generic simulation engines.

  ------------------------------------------------------------------
  �2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <istream>
#include <ostream>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace FileSystem	= ::Eldritch2::FileSystem;

// ---------------------------------------------------

	ETInlineHint const Scheduler::TaskScheduler& GameEngine::GetTaskScheduler() const {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint Scheduler::TaskScheduler& GameEngine::GetTaskScheduler() {
		return _scheduler;
	}

// ---------------------------------------------------

	ETInlineHint const Foundation::SystemInterface& GameEngine::GetSystemInterface() const {
		return _systemInterface;
	}

}	// namespace Foundation
}	// namespace Eldritch2

template <typename Traits>
::std::basic_istream<char, Traits>&	operator >>( ::std::basic_istream<char, Traits>& stream, ::Eldritch2::Foundation::LogMessageType& threshold ) {
	static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };
	::Eldritch2::uint32			result { 0 };
	char						value[16u];

	stream.get( value, _countof(value) );

	for( const char* const name : nameTable ) {
		if( ::Eldritch2::EqualityCompareStringCaseInsensitive( name, value ) ) {
			threshold = static_cast<::Eldritch2::Foundation::LogMessageType>(result);
			break;
		}

		++result;
	}

	return stream;
}

// ---------------------------------------------------

template <typename Traits>
::std::basic_ostream<char, Traits>&	operator <<( ::std::basic_ostream<char, Traits>& stream, const ::Eldritch2::Foundation::LogMessageType threshold ) {
	static const char* const	nameTable[] = { "VerboseWarning", "Warning", "Error", "Never" };

	return stream << nameTable[static_cast<size_t>(threshold)];
}