/*==================================================================*\
  EngineService.PipelineViewFactory.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Direct3D11/HLSLPipelineDefinitionView.hpp>
#include <Renderer/Direct3D11/EngineService.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Renderer;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	EngineService::PipelineViewFactory::PipelineViewFactory() : _device( nullptr ) {}

// ---------------------------------------------------

	void EngineService::PipelineViewFactory::AcceptInitializationVisitor( ConfigurationPublishingInitializationVisitor& /*visitor*/ ) {
		// Nothing here for now.
	}

// ---------------------------------------------------

	ErrorCode EngineService::PipelineViewFactory::AllocateResourceView( Allocator& allocator, ContentLibrary& contentLibrary, ContentPackage& package, const UTF8Char* const name, const Range<const char*> /*sourceAsset*/ ) {
		ETRuntimeAssert( _device );

	// ---

		MICROPROFILE_SCOPEI( "Direct3D11 Renderer", "Create mesh resource view", 0xACBACC );

		return new(allocator, Allocator::AllocationDuration::Normal) HLSLPipelineDefinitionView( _device, contentLibrary, package, name, allocator ) ? Error::None : Error::OutOfMemory;
	}

// ---------------------------------------------------

	void EngineService::PipelineViewFactory::SetDevice( const COMPointer<::ID3D11Device>& device ) {
		_device = device;
	}

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2



