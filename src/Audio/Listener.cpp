/*==================================================================*\
  Listener.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Audio/Listener.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Audio {

	Voice::Voice( float32 loudnessDb ) : loudnessDb( loudnessDb ) {}

// ---------------------------------------------------

	float32	Voice::GetHdrAmplitudeScalar( float32 windowTopDb ) {
		return Pow( 10.0f, ( loudnessDb - windowTopDb ) / 20.0f );
	}

}	// namespace Audio
}	// namespace Eldritch2