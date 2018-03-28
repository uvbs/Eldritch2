/*==================================================================*\
  TypeId.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Type.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	Type::Type() : Type( ([] () -> const std::type_info& { struct Dummy {}; return typeid( Dummy ); })() ) {}

}	// namespace Eldritch2
