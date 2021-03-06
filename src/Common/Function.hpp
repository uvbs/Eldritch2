/*==================================================================*\
  Function.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/internal/function.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Signature>
	using Function = eastl::function<Signature>;

}	// namespace Eldritch2