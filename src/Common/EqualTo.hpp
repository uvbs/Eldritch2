/*==================================================================*\
  EqualTo.hpp
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
	class EqualTo {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EqualTo instance.
		EqualTo() = default;

		~EqualTo() = default;

	// ---------------------------------------------------

	public:
		bool	operator()( const T& object0, const T& object1 ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/EqualTo.inl>
//------------------------------------------------------------------//