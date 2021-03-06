/*==================================================================*\
  LessThan.hpp
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

	template <typename T>
	class LessThan {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref LessThan instance.
		LessThan( const LessThan& ) = default;
	//!	Constructs this @ref LessThan instance.
		LessThan() = default;

		~LessThan() = default;

	// ---------------------------------------------------

	public:
		bool	operator()( const T& object0, const T& object1 ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/LessThan.inl>
//------------------------------------------------------------------//