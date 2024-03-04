#include "BamtingScene.h"

using namespace PhysBamt::Physics;

namespace PhysBamt
{
	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = { PxVec3(46.f / 255.f,9.f / 255.f,39.f / 255.f),PxVec3(217.f / 255.f,0.f / 255.f,0.f / 255.f),
		PxVec3(255.f / 255.f,45.f / 255.f,0.f / 255.f),PxVec3(255.f / 255.f,140.f / 255.f,54.f / 255.f),PxVec3(4.f / 255.f,117.f / 255.f,111.f / 255.f) };

	struct FilterGroup
	{
		enum Enum
		{
			ACTOR0 = (1 << 0),
			ACTOR1 = (1 << 1),
			ACTOR2 = (1 << 2)
			//add more if you need
		};
	};

	void BamtingScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}

	void BamtingScene::CustomInit()
	{
		SetVisualisation();

		GetMaterial()->setDynamicFriction(.2f);

		plane = new Plane();
		plane->Color(PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
		Add(plane);

		box = new Box(PxTransform(PxVec3(.0f, 1.5f, .0f)));
		box->Color(color_palette[0]);
		//set collision filter flags
		box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1);
		//use | operator to combine more actors e.g.
		box->SetupFiltering(FilterGroup::ACTOR0, FilterGroup::ACTOR1 | FilterGroup::ACTOR2);

		player = new Capsule(PxTransform(PxVec3(.0f, .5f, .0f)));
		player->Name("Player");
		PxRigidDynamic* playerActor = (PxRigidDynamic*)player->Get();
		playerActor->setGlobalPose(PxTransform(PxVec3(0.f, 2.0f, 0.f), PxQuat(PxPi / 2, PxVec3(0.f, 0.f, 1.f))));

		//Lock the motion
		playerActor->setRigidDynamicLockFlags(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z | PxRigidDynamicLockFlag::eLOCK_ANGULAR_X);


		Add(player);

		box->Name("Box1");
		Add(box);
	}

	void BamtingScene::CustomUpdate()
	{
	}
}


