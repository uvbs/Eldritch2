/*==================================================================*\
  IniParser.hpp
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
namespace Core {

	template <typename TrinaryConsumer, typename AltTrinaryConsumer>
	void	ApplyIni( Range<const Utf8Char*> source, TrinaryConsumer keyHandler, AltTrinaryConsumer unknownKeyHandler );

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/IniParser.inl>
//------------------------------------------------------------------//