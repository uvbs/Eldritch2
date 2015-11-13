/*==================================================================*\
  World.hpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Utility/StringOperators.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Foundation/WorldView.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	GameEngine;
	}

	namespace FileSystem {
		class	ContentPackage;
	}
}

namespace Eldritch2 {
namespace Foundation {

	class World : public Scripting::ReferenceTypeBase, public ::Eldritch2::IntrusiveForwardListBaseHook {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using PropertyKey	= ::Eldritch2::UTF8String<>;
		using Property		= ::Eldritch2::UTF8String<>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref World instance.
		World( Scripting::ObjectHandle<FileSystem::ContentPackage>&& package, Foundation::GameEngine& owningEngine );
		//! Constructs this @ref World instance.
		World( const Foundation::World& ) = delete;

		~World();

	// ---------------------------------------------------

		Property	GetPropertyByKey( ::Eldritch2::Allocator& resultAllocator, const ::Eldritch2::UTF8Char* const key, const ::Eldritch2::UTF8Char* defaultValue = UTF8L("") ) const;

		void		SetProperty( const ::Eldritch2::UTF8Char* const key, const ::Eldritch2::UTF8Char* const value );

	// - WORLD SIMULATION --------------------------------

		void	QueueUpdateTasks( ::Eldritch2::Allocator& frameTaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& worldUpdatesCompleteTask );

	// - STATE INSPECTION --------------------------------

		ETInlineHint const FileSystem::ContentPackage&	GetRootPackage() const;

		ETInlineHint ::Eldritch2::ErrorCode				GetLastError() const;

	// ---------------------------------------------------

		bool	IsLoaded() const;

		bool	IsPaused() const;

	// ---------------------------------------------------

	protected:
		void	Dispose() override sealed;

		void	DeleteViews();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator														_allocator;
		::Eldritch2::ArenaChildAllocator												_viewAllocator;
		Scripting::ObjectHandle<Foundation::GameEngine>									_owningEngine;
		Scripting::ObjectHandle<FileSystem::ContentPackage>								_package;
		::Eldritch2::UnorderedMap<::Eldritch2::UTF8String<>, ::Eldritch2::UTF8String<>>	_keyValuePairs;

		::Eldritch2::uint32																_isPaused : 1;
		::Eldritch2::uint32																_isLoaded : 1;

		::Eldritch2::IntrusiveForwardList<Foundation::WorldView>						_attachedViews;
		::Eldritch2::ErrorCode															_lastError;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::Foundation::WorldView;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/World.inl>
//------------------------------------------------------------------//