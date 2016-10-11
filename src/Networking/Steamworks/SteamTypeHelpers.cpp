/*==================================================================*\
  SteamTypeHelpers.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamTypeHelpers.hpp>
//------------------------------------------------------------------//
#include <steamclientpublic.h>
//------------------------------------------------------------------//

size_t GetHashCode( const CSteamID& steamId, size_t seed ) {
	return steamId.ConvertToUint64() + seed;
}