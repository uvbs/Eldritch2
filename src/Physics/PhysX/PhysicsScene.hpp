/*==================================================================*\
  PhysicsScene.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysXPointer.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does.
 *		We can't fix this, but we can at least disable the warning. */
	ET_SET_MSVC_WARNING_STATE( disable : 6326 )
#	include <characterkinematic/PxControllerManager.h>
#	include <PxSimulationEventCallback.h>
#	include <PxContactModifyCallback.h>
#	include <PxQueryReport.h>
#	include <PxFiltering.h>
#	include <PxMaterial.h>
#	include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}
}

namespace Eldritch2 {
namespace Physics {
namespace PhysX {

	class PhysicsScene : public physx::PxSimulationEventCallback, public physx::PxContactModifyCallback {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			ScratchMemorySize	= 16384u
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref PhysicsScene instance.
		PhysicsScene(
			PhysxPointer<physx::PxScene> scene,
			PhysxPointer<physx::PxControllerManager> controllerManager,
			PhysxPointer<physx::PxMaterial> triggerMaterial
		);
	//! Disable copy construction.
		PhysicsScene( const PhysicsScene& ) = delete;

		~PhysicsScene() = default;

	// ---------------------------------------------------

	public:
		const physx::PxMaterial&	GetTriggerMaterial() const;

	// ---------------------------------------------------

	public:
		bool	SweepGeometry(
			const physx::PxGeometry& geometry,
			const Transformation& pose,
			const Vector& direction,
			float32 distance,
			physx::PxSweepCallback& callback,
			physx::PxHitFlags hitFlags = physx::PxHitFlags( physx::PxHitFlag::eDEFAULT ),
			const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
			physx::PxQueryFilterCallback* filter = nullptr,
			const physx::PxQueryCache* cache = nullptr,
			float32 inflation = 0.0f
		) const;

		bool	CheckOverlap(
			const physx::PxGeometry& geometry,
			const Transformation& pose,
			physx::PxOverlapCallback& callback,
			const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
			physx::PxQueryFilterCallback* filter = nullptr
		) const;

		bool	SweepRay(
			const Vector& origin,
			const Vector& direction,
			float32 distance,
			physx::PxRaycastCallback& callback,
			physx::PxHitFlags hitFlags = physx::PxHitFlags( physx::PxHitFlag::eDEFAULT ),
			const physx::PxQueryFilterData& filterData = physx::PxQueryFilterData(),
			physx::PxQueryFilterCallback* filter = nullptr,
			const physx::PxQueryCache* cache = nullptr
		) const;

	// ---------------------------------------------------

	public:
		bool	IsSimulationComplete() const;

	// ---------------------------------------------------

	public:
		ErrorCode	JoinSimulation( Scheduling::JobExecutor& executor );

		void		BeginSimulation( Scheduling::JobExecutor& executor, physx::PxReal duration );

		void		AbortSimulation( Scheduling::JobExecutor& executor );

	// - PXSIMULATIONEVENTCALLBACK METHODS ---------------

	public:
		void	onAdvance( const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count ) override;

		void	onConstraintBreak( physx::PxConstraintInfo* constraints, physx::PxU32 count ) override;

		void	onWake( physx::PxActor** actors, physx::PxU32 count ) override;

		void	onSleep( physx::PxActor** actors, physx::PxU32 count ) override;

		void	onContact( const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs ) override;

		void	onTrigger( physx::PxTriggerPair* pairs, physx::PxU32 count ) override;

	// - PXCONTACTMODIFYCALLBACK METHODS -----------------

	public:
		void	onContactModify( physx::PxContactModifyPair* const pairs, physx::PxU32 count ) override;

	// ---------------------------------------------------

	public:
		static physx::PxFilterFlags	FilterShader(
			physx::PxFilterObjectAttributes attributes0,
			physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1,
			physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags,
			const void* constantBlock,
			physx::PxU32 constantBlockSize
		);

	// - DATA MEMBERS ------------------------------------

	private:
		const PhysxPointer<physx::PxScene>				_scene;
		const PhysxPointer<physx::PxControllerManager>	_controllerManager;
		const PhysxPointer<physx::PxMaterial>			_triggerMaterial;
	//!	Scratch buffer used for PhysX simulation.
		ET16ByteAligned char							_scratchMemory[ScratchMemorySize];
	};

	PhysxPointer<physx::PxMaterial>	MakeTriggerMaterial();

}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/PhysicsScene.inl>
//------------------------------------------------------------------//