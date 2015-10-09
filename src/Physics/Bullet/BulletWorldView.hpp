/*==================================================================*\
  BulletWorldView.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/Bullet/btDiscreteDynamicsMixinWrapper.hpp>
#include <Physics/Bullet/btHeightfieldTerrainShapeEx.hpp>
#include <Physics/Bullet/btSoftBodyWorldMixin.hpp>
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
		class	BulletEngine;
	}
}

namespace Eldritch2 {
namespace Physics {

	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Physics		= ::Eldritch2::Physics;

// ---------------------------------------------------

	class ET16ByteAligned BulletWorldView : public Foundation::WorldView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		typedef ::btSoftBodyWorldMixin<::btDiscreteDynamicsMixinWrapper<>>	BulletWorld;

		enum FilterBehaviors : short {
			DEFAULT_FILTER			= 1,
			STATIC_FILTER			= 2,
			KINEMATIC_FILTER		= 4,
			DEBRIS_FILTER			= 8,
			SENSOR_TRIGGER_FILTER	= 16,
			CHARACTER_FILTER		= 32,
			VISIBILITY_FILTER		= 64,
			PROJECTILE_FILTER		= 128,
			ALL						= -1,
			NONE					= 0
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this BulletWorldView instance.
		BulletWorldView( Foundation::World& owningWorld, const Physics::BulletEngine& hostingEngine );

		// Destroys this BulletWorldView instance.
		~BulletWorldView();

	// ---------------------------------------------------

		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor );

	// ---------------------------------------------------

	protected:
		static BulletWorldView&	GetActiveWorldView();

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
				COLLISION_FILTER_GROUP	= FilterBehaviors::CHARACTER_FILTER,
				COLLISION_FILTER_MASK	= (FilterBehaviors::STATIC_FILTER | FilterBehaviors::DEFAULT_FILTER | FilterBehaviors::SENSOR_TRIGGER_FILTER)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this CharacterArmatureComponent instance.
			CharacterArmatureComponent( Physics::BulletWorldView& owningWorldView = GetActiveWorldView() );

			// Destroys this CharacterArmatureComponent instance.
			~CharacterArmatureComponent();

		// - SCRIPT API REFERENCE ----------------------------

			// Registers all script-callable methods for the CharacterArmatureComponent type with the specified script type registrar.
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

			// Constructs this TerrainColliderComponent.
			TerrainColliderComponent( Physics::BulletWorldView& owningView = GetActiveWorldView() );

			// Constructs this TerrainColliderComponent.
			~TerrainColliderComponent();

		// - SCRIPT API REFERENCE ----------------------------

		public:
			// Registers all script-callable methods for the SceneCameraComponent type with the specified script type registrar.
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

			// Constructs this TriggerVolumeComponent.
			TriggerVolumeComponent( Physics::BulletWorldView& owningView = GetActiveWorldView() );

			// Constructs this TriggerVolumeComponent.
			~TriggerVolumeComponent();

		// - SCRIPT API REFERENCE ----------------------------

		public:
			// Registers all script-callable methods for the SceneCameraComponent type with the specified script type registrar.
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

			// Constructs this PhysicalSoftBodyComponent.
			PhysicalSoftBodyComponent( Physics::BulletWorldView& owningView = GetActiveWorldView() );

			// Constructs this PhysicalSoftBodyComponent.
			~PhysicalSoftBodyComponent();

		// - SCRIPT API REFERENCE ----------------------------

		public:
			// Registers all script-callable methods for the SceneCameraComponent type with the specified script type registrar.
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
		::btPoolAllocator					_persistentManifoldPool;
		::btPoolAllocator					_collisionAlgorithmPool;
		::btDefaultSoftBodySolver			_softBodySolver;
		BulletWorld::CollisionConfiguration	_collisionConfiguration;
		::btCollisionDispatcher				_dispatcher;
		::btHashedOverlappingPairCache		_pairCache;
		::bt32BitAxisSweep3					_broadphaseInterface;
		BulletWorld::ConstraintSolver		_constraintSolver;
		BulletWorld							_dynamicsWorld;
		::btGhostPairCallback				_ghostPairCallback;
	};

}	// namespace Physics
}	// namespace Eldritch2