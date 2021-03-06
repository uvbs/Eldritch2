/*==================================================================*\
  AbstractString.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/AbstractString.hpp>
#include <Common/Mpl/CharTypes.hpp>
//------------------------------------------------------------------//

template class Eldritch2::AbstractString<Eldritch2::PlatformChar>;
template class Eldritch2::AbstractString<Eldritch2::Utf8Char>;