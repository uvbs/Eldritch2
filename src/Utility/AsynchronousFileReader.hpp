/*==================================================================*\
  AsynchronousFileReader.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
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

	class ETPureAbstractHint AsynchronousFileReader : public Utility::Noncopyable {
	public:
		struct BlockingResult {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this BlockingResult instance.
			ETInlineHint BlockingResult( const BlockingResult& itemTemplate );
			// Constructs this BlockingResult instance.
			ETInlineHint BlockingResult( ::Eldritch2::ErrorCode	readResult, size_t successfullyReadAmountInBytes );

			ETForceInlineHint operator bool() const;

		// - DATA MEMBERS ------------------------------------

			// The final state of the read operation.
			::Eldritch2::ErrorCode	result;

			// Amount of data successfully read before a failure case, or the original
			// size of the write request in the event no errors were encountered.
			size_t					readAmountInBytes;
		};

	// ---------------------------------------------------

		// Begins an (optionally) asynchronous read of a block of data starting at the file offset passed in through the third parameter and continuing for
		// (at most) the specified number of bytes. The buffer itself must persist at least until the next call to FinishRead() returns.
		// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
		virtual ::Eldritch2::ErrorCode	BeginRead( void* destinationBuffer, ::Eldritch2::uint64 fileOffsetInBytes, size_t lengthToReadInBytes ) abstract;

		// Tests to see whether the previous asynchronous read started by a call to BeginRead() has completed.
		// Thread-safe.
		virtual bool					IsReadComplete() abstract;

		// Blocks until the read begun by a previous call to BeginRead() completes, returning the number of bytes successfully copied.
		// Not thread-safe, though it is likely that independent read strategies can safely access the same file.
		virtual BlockingResult			BlockUntilReadComplete() abstract;

		virtual void					CancelRead() abstract;

	// ---------------------------------------------------

		ETForceInlineHint const void*		FixupReadPointer( const void* const rawReadPointer, ::Eldritch2::uint64 desiredFileOffsetInBytes );

		// Retrieves the total on-disk size of the file/pseudofile being accessed.
		// Thread-safe.
		virtual ::Eldritch2::uint64			GetSizeInBytes() const abstract;

		// Calculates the total on-disk size of the file/pseudofile being accessed, rounded up to the nearest multiple of the
		// media sector size.
		// Thread-safe.
		ETInlineHint ::Eldritch2::uint64	GetRoundedSizeInBytes() const;

		// Calculates the actual/'physical' read size that should be passed to BeginRead() and similar given a 'logical' read size specified in bytes.
		// Thread-safe.
		ETInlineHint size_t					GetRoundedReadSizeInBytes( size_t logicalReadSizeInBytes ) const;

		// Given a desired offset into the file, calculates the actual offset that should be passed to BeginRead() in such a way that access boundaries
		// will be correctly preserved.
		// Thread-safe.
		ETInlineHint ::Eldritch2::uint64	GetOffsetForRead( ::Eldritch2::uint64 logicalOffsetInBytes ) const;

		// Retrieves the physical media sector size for this file/pseudofile. All destination buffers must be minimally aligned to
		// this value, and all reads must be an even multiple of this.
		// Thread-safe.
		virtual size_t						GetMediaSectorSizeInBytes() const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this AsynchronousFileReader instance.
		AsynchronousFileReader();

	public:
		// Destroys this AsynchronousFileReader instance.
		virtual ~AsynchronousFileReader();
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/AsynchronousFileReader.inl>
//------------------------------------------------------------------//