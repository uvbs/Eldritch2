/*==================================================================*\
  IoBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/Vulkan/GpuResources.hpp>
#include <Graphics/Vulkan/CommandList.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class IoBuilder {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum Phase : uint32 {
			StandardDownload,
			SparseBind,
			StandardUpload,
			SparseUpload,

			COUNT
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		IoBuilder( const IoBuilder& ) = delete;
	//!	Constructs this @ref IoBuilder instance.
		IoBuilder();

		~IoBuilder() = default;

	// ---------------------------------------------------

	public:
		void	TransferToGpu( VkImage target, VkImageSubresourceLayers subresource, VkOffset3D targetOffset, VkExtent3D targetExtent );
		void	TransferToGpu( VkBuffer target, VkDeviceSize targetOffset, VkDeviceSize targetExtent );

		void	TransferToHost( VkImage source, VkImageSubresourceLayers subresource, VkOffset3D sourceOffset, VkExtent3D sourceExtent );
		void	TransferToHost( VkBuffer source, VkDeviceSize sourceOffset, VkDeviceSize sourceExtent );

	// ---------------------------------------------------

	public:
		VkResult	SubmitCommands( Gpu& gpu );

		bool		CheckCommandsConsumed( Gpu& gpu ) const;

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, GpuHeap& heap, VkDeviceSize transferBufferSize );

		void		FreeResources( Gpu& gpu, GpuHeap& heap );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		IoBuilder&	operator=( const IoBuilder& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<VkSparseImageOpaqueMemoryBindInfo>	_opaqueImageBinds;
		ArrayList<VkSparseImageMemoryBindInfo>			_imageBinds;

		TransferBuffer									_stagingBuffer;
		Atomic<VkDeviceSize>							_readOffset;
		Atomic<VkDeviceSize>							_writeOffset;

		VkSemaphore										_phaseCompleted[Phase::COUNT];
		VkFence											_commandsConsumed;
		VkEvent											_ioCompleted;

		CommandList										_commandLists[Phase::COUNT];
		CommandList										_finalizeCommandList;

	// ---------------------------------------------------

		friend void Swap( IoBuilder&, IoBuilder& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2