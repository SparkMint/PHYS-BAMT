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

	void PoolScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	}

	void PoolScene::CustomInit()
	{
		SetVisualisation();

		GetMaterial()->setDynamicFriction(.2f);

		plane = new Plane();
		plane->Color(PxVec3(210.f / 255.f, 210.f / 255.f, 210.f / 255.f));
		Add(plane);

		table = new PoolTable(PxVec3(0.f, 0.f, 0.f));
		table->AddToScene(this);
	}

	void PoolScene::CustomUpdate()
	{
	}
}


