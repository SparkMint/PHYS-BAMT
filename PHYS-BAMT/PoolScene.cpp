#include "PoolScene.h"

#include <GL/glut.h>

#include "PBEngine.h"

using namespace PhysBamt::Physics;

namespace PhysBamt
{
	void PoolScene::SetVisualisation() const
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}

	void PoolScene::Start()
	{
		eventCallback = new PoolSceneEventCallback();
		px_scene->setSimulationEventCallback(eventCallback);
		
		SetVisualisation();

		// This scene is our main gameplay scene, give it a high update rate!
		SetFixedDeltaTime(1.f / 144.f);

		// Set default material stuff.
		GetMaterial()->setDynamicFriction(.2f);
		
		cueMaterial = CreateMaterial(.6f, .6f, .81f);

		plane = new Plane();
		plane->Color(PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
		Add(plane);

		table = new PoolTable(PxVec3(0.f, 0.f, 0.f));
		table->AddToScene(this);

		balls = new PoolBalls(PxVec3(1.25f, 2.f, 0.f));
		balls->AddToScene(this);

		cue = new Capsule(PxVec3(-2.f, 5.f, 0.f), PxIdentity, PxVec2(0.02f, 1.44f));
		cue->Material(cueMaterial, 0);
		// Enable Continuous Collision Detection for the cue.
		((PxRigidDynamic*)cue->Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		Add(cue);

		const PxVec3 cueOffset = PxVec3(0.f, -.25f, 0.f);
		
		cueJoint = new ConfigurableJoint(nullptr, PxTransform(cueOffset, PxIdentity), cue, PxTransform(PxVec3(1.44f / 2.f, 0.f, 0.f)));

		cueJoint->SetLinearMotions(PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE);
		cueJoint->SetRotationalMotions(PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE);
		cueJoint->SetAngularXLimit(Deg2Rad(15.f), Deg2Rad(15.f), PxSpring(1000.f, 100.f));
		cueJoint->SetAngularYZLimit(Deg2Rad(15.f), Deg2Rad(15.f), PxSpring(1000.f, 100.f));

		cueJoint->SetSlerpDrive(50.f, 1.f, .5f);
		cueJoint->SetLinearDrives(50.f, 1.f, .1f);
	}
	
	void PoolScene::Update(PxReal dt)
	{

	}

	// The current offset the cue is at.
	PxReal cueCurrentOffset = 0.f;

	// The speed at which the cue gets pulled back.
	PxReal cuePullbackSpeed = .5f;

	// The distance the cue can be pulled back.
	PxReal cuePullbackLimit = -.25f;

	// How far the cue is pushed forward when the mouse is released.
	PxReal cuePushForwardLimit = .1f;

	// How long should the cue be launched forward.
	PxReal cuePushForwardTime = .5f;
	PxReal cuePushForwardTimer = 0.f;

	void PoolScene::FixedUpdate(PxReal fdt)
	{ 
		((PxRigidDynamic*)cue->Get())->wakeUp();

		if(Engine::camera != nullptr)
		{
			PxVec3 posePosition = Engine::camera->getTransform().p;
			posePosition -= Engine::camera->getDir() * 1.f;
			const PxQuat poseRotation = Engine::camera->getTransform().q * PxQuat(Deg2Rad(-90.f),  PxVec3(0.f, 1.f, 0.f));

			// Cue Shot Logic

			// If the mouse is pressed and we can accept that input, start moving the cue back.

			// Once the mouse is released. Set the cue target to be slightly ahead of the cue's starting position.

			// wait a bit, this gives the cue time to get to the offset position.

			// Move the cue back to the start and accept input again.

			switch (cueState)
			{
			case READY:
				cue->SetupFiltering(PoolFilterGroup::CUEINACTIVE, PoolFilterGroup::CUEINACTIVE);
				if (Engine::GetMouseDown(GLUT_LEFT_BUTTON))
					cueState = CHARGING;
				break;
			case CHARGING:
				if (!Engine::GetMouseDown(GLUT_LEFT_BUTTON))
				{
					cuePushForwardTimer = cuePushForwardTime;
					cueState = SHOOTING;
					break;
				}
				
				if (cueCurrentOffset > cuePullbackLimit)
					cueCurrentOffset -= cuePullbackSpeed * fdt;
				break;
				
			case SHOOTING:
				cue->SetupFiltering(PoolFilterGroup::CUEACTIVE, PoolFilterGroup::CUEACTIVE);
				cueCurrentOffset = cuePushForwardLimit;

				if(cuePushForwardTimer > 0.f)
					cuePushForwardTimer -= fdt;
				else
				{
					cueCurrentOffset = 0.f;
					cueState = READY;
				}
				break;
			}

			posePosition += Engine::camera->getDir() * cueCurrentOffset;
			cueJoint->UpdateTargetPose(posePosition, poseRotation);
		}
	}
}


