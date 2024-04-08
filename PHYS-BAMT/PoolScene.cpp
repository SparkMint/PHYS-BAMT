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
		SetFixedDeltaTime(1.f / 60.f);

		// Set default material stuff.
		GetMaterial()->setDynamicFriction(.2f);

		plane = new Plane();
		plane->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
		Add(plane);

		table = new PoolTable(PxVec3(0.f, 0.f, 0.f));
		table->AddToScene(this);

		balls = new PoolBalls(PxVec3(1.15f, 1.6f, 0.f));
		balls->AddToScene(this);

		cueMaterial = CreateMaterial(.6f, .6f, .81f);
		cue = new Capsule(PxVec3(-2.f, 5.f, 0.f), PxIdentity, PxVec2(0.02f, 1.44f));
		cue->Material(cueMaterial, 0);
		cue->CreateShape(PxBoxGeometry(PxVec3(.05f, .02f, .02f)), 1.f);
		cue->GetShape(1)->setLocalPose(PxTransform(PxVec3(-1.44f - .05f, 0.f, 0.f)));
		cue->Color(PxVec3(84.f / 255, 43.f / 255.f, 16.f / 255.f));
		// Enable Continuous Collision Detection for the cue.
		((PxRigidDynamic*)cue->Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		Add(cue);

		const PxVec3 cueOffset = PxVec3(0.f, -.1f, 0.f);
		
		cueJoint = new ConfigurableJoint(nullptr, PxTransform(cueOffset, PxIdentity), cue, PxTransform(PxVec3(1.44f / 2.f, 0.f, 0.f)));

		cueJoint->SetLinearMotions(PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE);
		cueJoint->SetRotationalMotions(PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE, PxD6Motion::Enum::eFREE);
		cueJoint->SetAngularXLimit(Deg2Rad(15.f), Deg2Rad(15.f), PxSpring(1000.f, 100.f));
		cueJoint->SetAngularYZLimit(Deg2Rad(15.f), Deg2Rad(15.f), PxSpring(1000.f, 100.f));

		cueJoint->SetSlerpDrive(25.f, 2.f, .5f);
		cueJoint->SetLinearDrives(25.f, 2.5f, .25f);

		hugeCueBall = new Sphere(PxVec3(-50.f, 10.f, 0.f), PxIdentity, .061f * 100);
		Add(hugeCueBall);
	}

	PxReal maxCameraSpeed = 1.f;
	PxReal cameraLookSensitivity = 5.f;
	PxReal acceleration = 2.f;
	PxReal currentCameraSpeed = 0.f;
	
	void PoolScene::Update(PxReal dt)
	{
		Engine::hud.Clear();

		// EMPTY SCREEN
		Engine::hud.AddLine(Engine::EMPTY, "");

		// HELP SCREEN
					// EMPTY SCREEN
		Engine::hud.AddLine(Engine::EMPTY, "");

		// HELP SCREEN
		Engine::hud.AddLine(Engine::HELP, "\n"
			"SIMULATION \n"
			"F9 - Select next actor \n"
			"F10 - Pause \n"
			"F12 - Reset Everything\n"
			"\n"
			"DISPLAY \n"
			"F5 - Toggle Debug HUD \n"
			"F6 - Render Shadows \n"
			"F7 - Debug/Normal/Both Render Modes \n"
			"\n"
			"CAMERA \n"
			"W,S,A,D,E,Q - Forward, Backward, Left, Right, Up, Down \n"
			"RMB + Mouse - Look Around \n"
			"F8 - Reset View \n"
			"SCORE \n"
			+ std::to_string(eventCallback->score));

		// PAUSE SCREEN
		Engine::hud.AddLine(Engine::PAUSE, "\n"
			"SIMULATION PAUSED. Press F10 to continue.");

		// FONT SETTINGS
		Engine::hud.FontSize(0.018f);
		Engine::hud.Color(PxVec3(0.f, 0.f, 0.f));


		if(Engine::GetKeyDown('w') || Engine::GetKeyDown('a') || Engine::GetKeyDown('s') || Engine::GetKeyDown('d') || Engine::GetKeyDown('q') || Engine::GetKeyDown('e'))
			currentCameraSpeed += dt * acceleration;
		else
			currentCameraSpeed = 0.f;
		
		if(Engine::GetKeyDown('w'))
			Engine::camera->MoveForward(currentCameraSpeed, dt);
		if(Engine::GetKeyDown('a'))
			Engine::camera->MoveLeft(currentCameraSpeed, dt);
		if(Engine::GetKeyDown('s'))
			Engine::camera->MoveBackward(currentCameraSpeed, dt);
		if(Engine::GetKeyDown('d'))
			Engine::camera->MoveRight(currentCameraSpeed, dt);
		if(Engine::GetKeyDown('q'))
			Engine::camera->MoveDown(currentCameraSpeed, dt);
		if(Engine::GetKeyDown('e'))
			Engine::camera->MoveUp(currentCameraSpeed, dt);
		
		if(Engine::GetMouseDown(GLUT_RIGHT_BUTTON))
			Engine::camera->Look(Engine::GetMouseDeltaX(), Engine::GetMouseDeltaY(), cameraLookSensitivity, dt);
	}

	
	void PoolScene::FixedUpdate(PxReal fdt)
	{ 
		((PxRigidDynamic*)cue->Get())->wakeUp();

		if(Engine::camera != nullptr)
		{
			//AutoCueBehaviour(fdt);
			MouseCueBehaviour(fdt);
		}
	}

	// The current offset the cue is at.
	PxReal cueCurrentOffset = 0.f;

	// The speed at which the cue gets pulled back.
	PxReal cuePullbackSpeed = .2f;

	// Limits for how far the cue can be pulled/pushed.
	PxReal cuePullbackLimit = -.25f;
	PxReal cuePushForwardLimit = .1f;

	// How long should the cue be launched forward.
	PxReal cuePushForwardTime = .1f;
	PxReal cuePushForwardTimer = 0.f;

	void PoolScene::AutoCueBehaviour(PxReal fdt)
	{
		PxVec3 posePosition = Engine::camera->getTransform().p;
		posePosition -= Engine::camera->getDir() * 1.f;
		const PxQuat poseRotation = Engine::camera->getTransform().q * PxQuat(Deg2Rad(-90.f),  PxVec3(0.f, 1.f, 0.f));
		
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

	void PoolScene::MouseCueBehaviour(PxReal fdt)
	{
		PxVec3 posePosition = Engine::camera->getTransform().p;
		posePosition -= Engine::camera->getDir() * 1.f;
		const PxQuat poseRotation = Engine::camera->getTransform().q * PxQuat(Deg2Rad(-90.f),  PxVec3(0.f, 1.f, 0.f));

		if (Engine::GetMouseDown(GLUT_LEFT_BUTTON))
		{
			cue->SetupFiltering(PoolFilterGroup::CUEACTIVE, PoolFilterGroup::CUEACTIVE);
			cueCurrentOffset += (PxReal)Engine::GetMouseDeltaY() * .005f;

			if(cueCurrentOffset > 1.f)
				cueCurrentOffset = 1.f;
			if(cueCurrentOffset < -.3f)
				cueCurrentOffset = -.3f;
		}
		else
		{
			cue->SetupFiltering(PoolFilterGroup::CUEINACTIVE, PoolFilterGroup::CUEINACTIVE);
			cueCurrentOffset = 0.f;
		}

		posePosition += Engine::camera->getDir() * cueCurrentOffset;
		cueJoint->UpdateTargetPose(posePosition, poseRotation);
	}
}


