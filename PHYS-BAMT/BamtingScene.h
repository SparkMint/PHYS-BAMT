#pragma once

#include "PBPhysics.h"
#include "BasicActors.h"

namespace PhysBamt
{
	using namespace PhysBamt::Physics;

	class BamtingScene : public Scene
	{
		Plane* plane;
		Box* box, * box2;
		Capsule* player;

	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		BamtingScene() : Scene() {};

		///A custom scene class
		void SetVisualisation();

		//Custom scene initialisation
		virtual void CustomInit();

		//Custom udpate function
		virtual void CustomUpdate();
	};
}