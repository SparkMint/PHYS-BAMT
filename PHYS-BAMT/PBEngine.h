#pragma once

#include <chrono>

#include "PoolScene.h"
#include "Extras/Camera.h"
#include "Extras/HUD.h"

namespace PhysBamt
{
	namespace Engine
	{
		using namespace physx;
		using namespace PhysBamt::Physics;
		using namespace PhysBamt::Renderer;
		
		enum RenderMode
		{
			DEBUG,
			NORMAL,
			BOTH
		};
		enum HUDState
		{
			EMPTY = 0,
			HELP = 1,
			PAUSE = 2
		};

		static HUD hud;
		static RenderMode renderMode = NORMAL;
		static HUDState hudState = HELP;
		
		static Scene* scene;
		static Camera* camera;

		static PxReal deltaTime;
		
		void InitializeEngine(const char* window_name, int width = 512, int height = 512);
		void StartEngine();
		void EngineLoop();
		
		void HandleKeyboardInputs();
		void HandleDebugInputs(int key, int x, int y);
		void OnKeyReleased(unsigned char key, int x, int y);
		void OnKeyPressed(unsigned char key, int x, int y);

		void motionCallback(int x, int y);
		void mouseCallback(int button, int state, int x, int y);
		void ExitCallback(void);


		void ToggleRenderMode();
		void ToggleHUDMode();
		void InitDebugHUD();
	}
}

