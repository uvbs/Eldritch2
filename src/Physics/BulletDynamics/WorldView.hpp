/*==================================================================*\
  WorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/BulletDynamics/btDiscreteDynamicsMixinWrapper.hpp>
#include <Physics/BulletDynamics/btHeightfieldTerrainShapeEx.hpp>
#include <Physics/BulletDynamics/btSoftBodyWorldMixin.hpp>
#include <Utility/Containers/UnorderedMap.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Foundation/WorldView.hpp>
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseProxy.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletSoftBody/btDefaultSoftBodySolver.h>
#include <LinearMath/btPoolAllocator.h>
#include <btBulletDynamicsCommon.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
		class	ReferenceTypeBase;
	}

	namespace Physics {
		namespace BulletDynamics {
			class	EngineService;
		}
	}
}

namespace Eldritch2 {
namespace Physics {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Physics		= ::Eldritch2::Physics;

// ---------------------------------------------------

namespace BulletDynamics {

	class ET16ByteAligned WorldView : public Foundation::WorldView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using BulletWorld				= ::btSoftBodyWorldMixin<::btDiscreteDynamicsMixinWrapper<>>;
		using CollisionConfiguration	= BulletWorld::CollisionConfiguration;
		using CollisionDispatcher		= ::btCollisionDispatcher;
		using OverlappingPairCache		= ::btHashedOverlappingPairCache;
		using BroadphaseInterface		= ::bt32BitAxisSweep3;
		using ConstraintSolver			= BulletWorld::ConstraintSolver;
		using SoftBodySolver			= ::btDefaultSoftBodySolver;

	// ---

		enum FilterBehaviors : short {
			DEFAULT_FILTER			= 1,
			STATIC_FILTER			= 2,
			KINEMATIC_FILTER		= 4,
			DEBRIS_FILTER			= 8,
			SENSOR_TRIGGER_FILTER	= 16,
			CHARACTER_FILTER		= 32,
			VISIBILITY_FILTER		= 64,
			PROJECTILE_FILTER		= 128,
			CUSTOM_GROUP_BEGIN		= 256,
			NONE					= 0,
			ALL						= ~NONE,
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldView instance.
		WorldView( Foundation::World& owningWorld, const BulletDynamics::EngineService& hostingEngine );

		//!	Destroys this @ref WorldView instance.
		~WorldView() = default;

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		static WorldView&	GetActiveWorldView();

	// ---------------------------------------------------

		void	AcceptViewVisitor( const ScriptExecutionPreparationVisitor ) override sealed;
		void	AcceptViewVisitor( Scripting::ScriptMessageSink& messageSink ) override sealed;

	// ---------------------------------------------------

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PreScriptTickTaskVisitor ) override sealed;
		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::WorkerContext& executingContext, Scheduler::Task& visitingTask, const PostScriptTickTaskVisitor ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

		class CharacterArmatureComponent : public Animation::Armature {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				COLLISION_FILTER_GROUP	= (FilterBehaviors::CHARACTER_FILTER),
				COLLISION_FILTER_MASK	= (FilterBehaviors::STATIC_FILTER | FilterBehaviors::DEFAULT_FILTER | FilterBehaviors::SENSOR_TRIGGER_FILTER)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref CharacterArmatureComponent instance.
			CharacterArmatureComponent( WorldView& owningWorldView = GetActiveWorldView() );

			//!	Destroys this @ref CharacterArmatureComponent instance.
			~CharacterArmatureComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

			//! Registers all script-callable methods for the @ref CharacterArmatureComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

			void						Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btPairCachingGhostObject			_ghostObject;
			::btKinematicCharacterController	_controller;
		};

	// ---

		class TerrainColliderComponent : public Scripting::ReferenceTypeBase {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				COLLISION_FILTER_GROUP	= FilterBehaviors::STATIC_FILTER,
				COLLISION_FILTER_MASK	= FilterBehaviors::NONE
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref TerrainColliderComponent.
			TerrainColliderComponent( WorldView& owningView = GetActiveWorldView() );

			//! Constructs this @ref TerrainColliderComponent.
			~TerrainColliderComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//!	Registers all script-callable methods for the @ref TerrainColliderComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const		scriptTypeName;

		private:
			::btHeightfieldTerrainShapeEx	_collisionShape;
			::btRigidBody					_body;
		};

	// ---

		class TriggerVolumeComponent : public Scripting::ReferenceTypeBase {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				COLLISION_FILTER_GROUP	= FilterBehaviors::SENSOR_TRIGGER_FILTER,
				COLLISION_FILTER_MASK	= FilterBehaviors::NONE
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref TriggerVolumeComponent.
			TriggerVolumeComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref TriggerVolumeComponent.
			~TriggerVolumeComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//!	Registers all script-callable methods for the @ref TriggerVolumeComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btPairCachingGhostObject	_ghostObject;
		};

	// ---

		class PhysicalSoftBodyComponent : public Scripting::ReferenceTypeBase {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : short {
				COLLISION_FILTER_GROUP	= FilterBehaviors::DEFAULT_FILTER,
				COLLISION_FILTER_MASK	= (FilterBehaviors::STATIC_FILTER | FilterBehaviors::DEFAULT_FILTER)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			PhysicalSoftBodyComponent( WorldView& owningView = GetActiveWorldView() );

			//!	Constructs this @ref PhysicalSoftBodyComponent.
			~PhysicalSoftBodyComponent() = default;

		// - SCRIPT API REFERENCE ----------------------------

		public:
			//!	Registers all script-callable methods for the @ref PhysicalSoftBodyComponent type with the specified script type registrar.
			static ETNoAliasHint void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

		// ---------------------------------------------------

			void	Dispose() override sealed;

		// - DATA MEMBERS ------------------------------------

		public:
			static const char* const	scriptTypeName;

		private:
			::btSoftBody				_body;
		};

	// - DATA MEMBERS ------------------------------------

	private:
		::btPoolAllocator		_persistentManifoldPool;
		::btPoolAllocator		_collisionAlgorithmPool;
		SoftBodySolver			_softBodySolver;
		CollisionConfiguration	_collisionConfiguration;
		CollisionDispatcher		_dispatcher;
		OverlappingPairCache	_pairCache;
		BroadphaseInterface		_broadphaseInterface;
		ConstraintSolver		_constraintSolver;
		BulletWorld				_dynamicsWorld;
		::btGhostPairCallback	_ghostPairCallback;
	};

}	// namespace BulletDynamics
}	// namespace Physics
}	// namespace Eldritch2