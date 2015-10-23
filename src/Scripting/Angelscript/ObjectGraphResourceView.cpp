/*==================================================================*\
  AngelscriptEngine.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/ObjectGraphResourceView.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ObjectGraphResourceView::ObjectGraphResourceView( const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ) {}

// ---------------------------------------------------

	ErrorCode ObjectGraphResourceView::DeserializeIntoWorldView( WorldView& /*worldView*/ ) const {
		return Error::NONE;
	}

// ---------------------------------------------------

	ETNoAliasHint const UTF8Char* const ObjectGraphResourceView::GetSerializedDataTag() {
		return UTF8L("AngelscriptObjectGraph");
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2