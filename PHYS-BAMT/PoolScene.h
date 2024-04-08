#pragma once

#include "ComplexActors.h"

namespace PhysBamt
{
	using namespace PhysBamt::Physics;

	static PxFilterFlags poolFilterShader(
	PxFilterObjectAttributes attributes0,
	PxFilterData filterData0,
	PxFilterObjectAttributes attributes1,
	PxFilterData filterData1,
	PxPairFlags& pairFlags,
	const void* constantBlock,
	PxU32 constantBlockSize)
	{
		if ((filterData0.word0 == PoolFilterGroup::CUEINACTIVE && 
		(filterData1.word0 == PoolFilterGroup::CUEBALL || 
		filterData1.word0 == PoolFilterGroup::BALLS)) ||
		(filterData1.word0 == PoolFilterGroup::CUEINACTIVE && 
		(filterData0.word0 == PoolFilterGroup::CUEBALL || 
		filterData0.word0 == PoolFilterGroup::BALLS)))
		{
			return PxFilterFlag::eSUPPRESS;
		}

		if(filterData0.word0 == PoolFilterGroup::CUEACTIVE && 
		(filterData1.word0 == PoolFilterGroup::BALLS ||
		(filterData1.word0 == PoolFilterGroup::CUEACTIVE &&
		filterData0.word0 == PoolFilterGroup::BALLS)))
		{
			return PxFilterFlag::eSUPPRESS;
		}
		
		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
		
		return PxFilterFlags();
	}
	
	class PoolSceneEventCallback : public PxSimulationEventCallback
	{
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) {}
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs){}
		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
		virtual void onAdvance(const PxRigidBody *const *bodyBuffer, const PxTransform *poseBuffer, const PxU32 count) {}
	};

	class PoolScene : public Scene
	{
		enum PoolCueState
		{
			READY,
			CHARGING,
			SHOOTING
		};
		PoolCueState cueState = READY;
		
		Plane* plane;
		PoolTable* table;
		PoolBalls* balls;

		Capsule* cue;
		ConfigurableJoint* cueJoint;
		
		PxMaterial* cueMaterial;
		PoolSceneEventCallback* eventCallback;
		
		void AutoCueBehaviour(PxReal fdt);
		void MouseCueBehaviour(PxReal fdt);
		
	public:
		PoolScene() : Scene(poolFilterShader) {}
		
		void SetVisualisation() const;

		// Start function. Called once at the start
		void Start() override;

		// Update function. Called every frame
		void Update(PxReal dt) override;

		// Fixed update function. Called a fixed number of times per second
		void FixedUpdate(PxReal fdt) override;
	};
}