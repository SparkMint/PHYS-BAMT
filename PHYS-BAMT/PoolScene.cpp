#include "PoolScene.h"

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
		};
	};

	void PoolScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}

	void PoolScene::Start()
	{
		SetVisualisation();

		// 10 Hz Stress Test
		SetFixedDeltaTime(.1f);

		GetMaterial()->setDynamicFriction(.2f);

		plane = new Plane();
		plane->Color(PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
		Add(plane);

		table = new PoolTable(PxVec3(0.f, 0.f, 0.f));
		table->AddToScene(this);
		
		for (int i = 0; i < 100; ++i)
		{
			PxVec3 ballPos = PxVec3(-1.f + i * .01f, 2.f + i * .068f, 0.f);
			
			if ((i & 1) == 0)
			{
				// Offset the bricks by its size to prevent it from sinking into the floor.
				ballPos.z += .025f;
			}

			Sphere* sphere = new Sphere(ballPos, PxIdentity, .068f, 1.f);
			Add(sphere);
		}
	}

	void PoolScene::Update(PxReal dt)
	{
		
	}

	void PoolScene::FixedUpdate(PxReal fdt)
	{
		
	}
}


