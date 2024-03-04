#pragma once

#include "MyPhysicsEngine.h"
#include "BamtingScene.h"

namespace PhysBamt
{
	namespace Engine
	{
		using namespace physx;

		static Camera* camera;

		///Init visualisation
		void Init(const char* window_name, int width = 512, int height = 512);

		///Start visualisation
		void Start();
	}
}

