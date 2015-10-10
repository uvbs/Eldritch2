/*==================================================================*\
  SynchronousFileReader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint SynchronousFileReader : public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct BlockingResult {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this BlockingResult instance.
			ETInlineHint BlockingResult( const BlockingResult& itemTemplate );
			// Constructs this BlockingResult instance.
			ETInlineHint BlockingResult( const ::Eldritch2::ErrorCode readResult, const size_t successfullyReadAmountInBytes );

		// - DATA MEMBERS ------------------------------------

			// The final state of the read operation.
			::Eldritch2::ErrorCode	result;

			// Amount of data successfully read before a failure case, or the original
			// size of the write request in the event no errors were encountered.
			size_t					readAmountInBytes;
		};

	// ---------------------------------------------------

		// Performs a blocking read operation to the output device this SynchronousFileReader uses as its backing.
		virtual BlockingResult	Read( void* destinationBuffer, size_t lengthToReadInBytes ) abstract;
		// Performs a blocking read operation to the output device this SynchronousFileReader uses as its backing.
		virtual BlockingResult	Read( void* destinationBuffer, size_t lengthToReadInBytes, ::Eldritch2::uint64 fileOffsetInBytes ) abstract;

	// ---------------------------------------------------

		// Retrieves the total on-disk size of the file/pseudofile being accessed.
		// Thread-safe.
		virtual ::Eldritch2::uint64			GetSizeInBytes() const abstract;

		// Retrieves the total on-disk size of the file/pseudofile being accessed, rounded up to the nearest multiple of the
		// media sector size.
		// Thread-safe.
		ETInlineHint ::Eldritch2::uint64	GetRoundedSizeInBytes() const;

		// Retrieves the actual/'physical' read size that should be passed to BeginRead() and similar given a 'logical' read size specified in bytes.
		// Thread-safe.
		ETInlineHint size_t					GetRoundedReadSizeInBytes( const size_t logicalReadSizeInBytes ) const;

		// Given a desired offset into the file, calculates the actual offset that should be passed to BeginRead() in such a way that access boundaries
		// will be correctly preserved.
		// Thread-safe.
		ETInlineHint ::Eldritch2::uint64	GetOffsetForRead( const ::Eldritch2::uint64 logicalOffsetInBytes ) const;

		// Retrieves the physical media sector size for this file/pseudofile. All destination buffers must be minimally aligned to
		// this value, and all reads must be an even multiple of this.
		// Thread-safe.
		virtual size_t						GetPhysicalMediaSectorSizeInBytes() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this SynchronousFileReader instance.
		SynchronousFileReader();

	public:
		// Destroys this SynchronousFileReader instance.
		virtual ~SynchronousFileReader();
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/SynchronousFileReader.inl>
//------------------------------------------------------------------//