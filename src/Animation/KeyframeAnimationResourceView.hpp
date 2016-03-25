/*==================================================================*\
  KeyframeAnimationResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Animation {

	class KeyframeAnimationResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		KeyframeAnimationResourceView( const ::Eldritch2::UTF8Char* const name );

		~KeyframeAnimationResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

		void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const override;
	};

}	// namespace Animation
}	// namespace Eldritch2