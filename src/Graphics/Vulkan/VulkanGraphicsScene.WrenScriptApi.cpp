/*==================================================================*\
  VulkanGraphicsScene.WrenScriptApi.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/VulkanGraphicsScene.hpp>
#include <Scripting/Wren/ApiBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	using namespace ::Eldritch2::Scripting::Wren;

	ET_IMPLEMENT_WREN_CLASS( VulkanGraphicsScene ) {
		api.CreateClass<VulkanGraphicsScene>( ET_BUILTIN_WREN_MODULE_NAME( Graphics ), "_GraphicsScene",
			{/* Constructors */},
			{/*	Properties */},
			{/*	Methods */},
			{/*	Static methods */},
			{/*	Operators */}
		);
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2