/*==================================================================*\
  Tool.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/Tool.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	Tool::Tool( Allocator& allocator ) : _inputFiles( { allocator, UTF8L("Tool Input File Name Allocator") } ) {}

}	// namespace Detail
}	// namespace Tools
}	// namespace Eldritch2

