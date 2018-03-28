/*==================================================================*\
  GreaterThan.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T>
	class GreaterThan {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GreaterThan instance.
		GreaterThan() = default;

		~GreaterThan() = default;

	// ---------------------------------------------------

	public:
		bool	operator()( const T& object0, const T& object1 ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/GreaterThan.inl>
//------------------------------------------------------------------//