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
		
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			if (filterData0.word0 & PoolFilterGroup::BALLS)
			{
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
				return PxFilterFlags();
			}
			if (filterData0.word0 & PoolFilterGroup::CUEBALL)
			{
				pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
				return PxFilterFlags();
			}
			return PxFilterFlag::eSUPPRESS;
		}

		pairFlags = PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;
		return PxFilterFlags();
	}
	
	class PoolSceneEventCallback : public PxSimulationEventCallback
	{
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count)
		{
			if(tableUnplayable)
				return;
			
			for (size_t i = 0; i < count; i++)
			{
				if (pairs[i].otherShape->getSimulationFilterData().word0 & PoolFilterGroup::BALLS && pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND && pairs[i].triggerShape->getSimulationFilterData().word0 != PoolFilterGroup::CUERESET)
				{
					score++;
					cout << "Ball Potted! Score: " << score << endl;
				}
					
				if (pairs[i].otherShape->getSimulationFilterData().word0 & PoolFilterGroup::CUEBALL && pairs[i].triggerShape->getSimulationFilterData().word0 & PoolFilterGroup::CUERESET && pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					score--;
					cout << "Cue ball out of bounds! Score: " << score << endl;
					pairs[i].otherShape->getActor()->setGlobalPose(PxTransform(PxVec3(0.f, 5.f, 0.f), PxIdentity));
					((PxRigidDynamic*)pairs[i].otherShape->getActor())->setLinearVelocity(PxVec3Zero);
				}
			}
		}
		virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {}
		virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count)
		{
			cout << "Table is broken! GAME OVER!" << endl;
			tableUnplayable = true;
		}
		virtual void onWake(PxActor** actors, PxU32 count) {}
		virtual void onSleep(PxActor** actors, PxU32 count) {}
		virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) {}


		
		public:
			// Determines if the table is broken beyond repair. At this point the player cant play anymore.
			bool tableUnplayable = false;
		
			int score;
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
		Plane* cueTriggerPlane;
		PoolTable* table;
		PoolBalls* balls;

		PoolCue* cue;
		ConfigurableJoint* cueJoint;

		PoolBalls* hugePoolBalls;
		PoolCue* hugeCue;
		LinearJoint* hugeCueJoint;
		
		PoolSceneEventCallback* eventCallback;

		// Determines how long until the catastrophic event occurs.
		PxReal timeLimit = 45.f;

		PxReal currentTimeLeft;
		
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
		
		void HandleHUDLogic() const;
		void HandleInput(PxReal dt);
	};
}