/*==================================================================*\
  World.cpp
  ------------------------------------------------------------------
  Purpose:
  Encapsulates all functionality and bookkeeping associated with
  running a game 'world'-- a global environment that contains a
  number of entities and their respective collections of constituent
  components.

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduler/CRTPTransientTask.hpp>
#include <Utility/Memory/InstanceNew.hpp>
#include <Packages/ContentPackage.hpp>
#include <Foundation/GameEngine.hpp>
#include <Utility/ErrorCode.hpp>
#include <Foundation/World.hpp>
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Scheduler;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Foundation {

	World::World( ObjectHandle<ContentPackage>&& package, GameEngine& owningEngine ) : _allocator( owningEngine._allocator, UTF8L( "World Allocator" ) ),
																					   _viewAllocator( _allocator, owningEngine._worldViewAllocationHintInBytes, Allocator::AllocationOption::PERMANENT_ALLOCATION, UTF8L("World View Allocator") ),
																					   _owningEngine( owningEngine ),
																					   _package( ::std::move( package ) ),
																					   _isPaused( 1u ),
																					   _isLoaded( 0u ),
																					   _lastError( Errors::NONE ) {
		using LogMessageType = GameEngine::LogMessageType;

	// ---

		UTF8Char	logString[128u];

		owningEngine._attachedWorlds.PushFront( *this );

		if( const auto instantiateViewsResult = owningEngine.InstantiateViewsForWorld( _viewAllocator, *this ) ) {
			owningEngine.GetLoggerForMessageType( LogMessageType::MESSAGE ).WriteString( PrintFormatted( logString, UTF8L("Loading world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) ) );
		} else {
			owningEngine.GetLoggerForMessageType( LogMessageType::ERROR ).WriteString( PrintFormatted( logString, UTF8L("Error instantiating view objects for world '%p': %s.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this), instantiateViewsResult.ToUTF8String() ) );
			_lastError = instantiateViewsResult;
		}
	}

// ---------------------------------------------------

	World::~World() {
		UTF8Char	logString[128u];

		_owningEngine->GetLoggerForMessageType( GameEngine::LogMessageType::MESSAGE ).WriteString( PrintFormatted( logString, UTF8L("Destroying world '%p'.") ET_UTF8_NEWLINE_LITERAL, static_cast<void*>(this) ) );
		_owningEngine->_attachedWorlds.Erase( _owningEngine->_attachedWorlds.IteratorTo( *this ) );
		DeleteViews();
	}

// ---------------------------------------------------

	void World::QueueUpdateTasks( Allocator& frameTaskAllocator, WorkerContext& executingContext, Task& frameWorldUpdatesCompleteTask ) {
		class TickWorldTask : public CRTPTransientTask<TickWorldTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			// Constructs this TickWorldTask instance.
			ETInlineHint TickWorldTask( World& world, WorkerContext& executingContext, Task& updatesCompleteTask, Allocator& frameTaskAllocator ) : CRTPTransientTask<TickWorldTask>( updatesCompleteTask, Scheduler::CodependentTaskSemantics ),
																																					_worldReference( world ),
																																					_frameTaskAllocator( frameTaskAllocator ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Tick World Task");
			}

			Task* Execute( WorkerContext& executingContext ) override sealed {
				WorldView::FrameTickTaskVisitor	visitor;

				for( WorldView& currentView : _worldReference->_attachedViews ) {
					currentView.AcceptTaskVisitor( _frameTaskAllocator, executingContext, *this, visitor );
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	_worldReference;
			Allocator&			_frameTaskAllocator;
		};

	// ---

		class TryFinalizeLoadTask : public CRTPTransientTask<TryFinalizeLoadTask> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
			// Constructs this FinalizeLoadTask instance.
			ETInlineHint TryFinalizeLoadTask( World& world, WorkerContext& executingContext, Task& updatesCompleteTask ) : CRTPTransientTask<TryFinalizeLoadTask>( updatesCompleteTask, Scheduler::CodependentTaskSemantics ),
																														   _worldReference( world ) {
				TrySchedulingOnContext( executingContext );
			}

		// ---------------------------------------------------

			const UTF8Char* const GetHumanReadableName() const override sealed {
				return UTF8L("Try Finalize World Load Task");
			}

			Task* Execute( WorkerContext& /*executingContext*/ ) override sealed {
				const bool	canFinalizeLoad( _worldReference->GetRootPackage().GetResidencyState() == ContentPackage::ResidencyState::PUBLISHED );

				if( canFinalizeLoad ) {
					_worldReference->_isLoaded = 1u;
				}

				return nullptr;
			}

		// - DATA MEMBERS ------------------------------------

		private:
			// This handle ensures the world will not be deleted until the task has finished executing.
			ObjectHandle<World>	_worldReference;
		};

	// ---

		if( IsLoaded() ) {
			new(frameTaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TickWorldTask( *this, executingContext, frameWorldUpdatesCompleteTask, frameTaskAllocator );
		} else {
			new(frameTaskAllocator, Allocator::AllocationOption::TEMPORARY_ALLOCATION) TryFinalizeLoadTask( *this, executingContext, frameWorldUpdatesCompleteTask );
		}
	}

// ---------------------------------------------------

	bool World::IsLoaded() const {
		return 0u != _isLoaded;
	}

// ---------------------------------------------------

	bool World::IsPaused() const {
		return 0u != _isPaused;
	}

// ---------------------------------------------------

	void World::Dispose() {
		_owningEngine->_allocator.Delete( *this );
	}

// ---------------------------------------------------

	void World::DeleteViews() {
		{	// Broadcast a delete message to all the views so they can clean any shared resources.
			const WorldView::DeletionPreparationVisitor	visitor;

			for( WorldView& view : _attachedViews ) {
				view.AcceptViewVisitor( visitor );
			}
		}
		

		{	// Invoke the destructor and deallocate all currently-attached views.
			Allocator&	viewAllocator( _viewAllocator );

			_attachedViews.ClearAndDispose( [&viewAllocator] ( WorldView& view ) {
				viewAllocator.Delete( view );
			} );
		}
	}

}	// namespace Foundation
}	// namespace Eldritch2