/*==================================================================*\
  Storyboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Storyboard.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	Storyboard::Storyboard() : _events( MallocAllocator( "Storyboard Event Allocator" ) ) {}

}	// namespace Animation
}	// namespace Eldritch2