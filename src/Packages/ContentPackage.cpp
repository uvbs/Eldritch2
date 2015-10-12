/*==================================================================*\
  ContentPackage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Packages/ContentProvider.hpp>
#include <Packages/ContentPackage.hpp>
#include <Packages/ContentLibrary.hpp>
#include <Utility/ResultPair.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace FileSystem {

	ContentPackage::ContentPackage( const UTF8Char* const name, ContentLibrary& owningLibrary, Allocator& allocator ) : _allocator( allocator, UTF8L("Package Allocator") ),
																														_name( name, StringLength( name ), allocator, UTF8L("Package Name Allocator") ),
																														_owningLibrary( owningLibrary ),
																														_residencyState( ResidencyState::LOADING ),
																														_referencedPackages( 0u, allocator, UTF8L("Package Dependency Collection Allocator") ) {}

// ---------------------------------------------------

	ContentPackage::~ContentPackage() {
		auto&	allocator( GetAllocator() );

		// It's not necessary to maintain a lock on the content table while we delete resources
		// (they've already been unpublished, we're the only thing that can see them)
		_exportedResources.ClearAndDispose( [&allocator] ( ResourceView& currentResource ) {
			allocator.Delete( currentResource );
		} );
	}

// ---------------------------------------------------

	ResultPair<ReadableMemoryMappedFile> ContentPackage::CreateBackingFile( Allocator& allocator, const UTF8Char* const suffix ) const {
		using KnownContentLocation	= ContentProvider::KnownContentLocation;

	// ---

		UTF8String<FixedStackAllocator<128u>>	fileName( ::Eldritch2::EmptyStringSemantics, UTF8L("ContentPackage::CreateBackingFile() Temporary Allocator") );

		fileName.Reserve( _name.Length() + StringLength( suffix ) );
		fileName.Append( _name ).Append( suffix ? suffix : UTF8L("") );

		return GetLibrary().GetContentProvider().CreateReadableMemoryMappedFile( allocator, KnownContentLocation::PACKAGE_DIRECTORY, fileName.GetCharacterArray() );
	}

}	// namespace FileSystem
}	// namespace Eldritch2