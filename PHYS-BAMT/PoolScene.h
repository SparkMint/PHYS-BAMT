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
		vector<Sphere*> balls;

	public:
		//specify your custom filter shader here
		//PxDefaultSimulationFilterShader by default
		PoolScene() = default;

		///A custom scene class
		void SetVisualisation();

		//Custom scene initialisation
		void Start() override;

		//Custom udpate function
		void Update(PxReal dt) override;

		void FixedUpdate(PxReal fdt) override;

	};
}