#pragma once

#include "PBPhysics.h" 
#include "BasicActors.h"
#include "ComplexActors.h"

namespace PhysBamt
{
	using namespace PhysBamt::Physics;

	class PoolScene : public Scene
	{
		Plane* plane;
		PoolTable* table;

	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		PoolScene() : Scene() {};

		///A custom scene class
		void SetVisualisation();

		//Custom scene initialisation
		virtual void CustomInit();

		//Custom udpate function
		virtual void CustomUpdate();
	};
}