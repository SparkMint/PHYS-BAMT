#pragma once

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
		
		void InitializeEngine(const char* window_name, int width = 512, int height = 512);
		void StartEngine();
		void EngineLoop();
		
		void HandleEngineInputs();
		void HandleDebugInputs(int key, int x, int y);
		void OnKeyReleased(unsigned char key, int x, int y);
		void OnKeyPressed(unsigned char key, int x, int y);
		
		bool GetKeyPressed(unsigned char key);
		bool GetKeyDown(unsigned char key);

		int GetMousePosX();
		int GetMousePosY();
		int GetMouseDeltaX();
		int GetMouseDeltaY();
	
		bool GetMousePressed(int button);
		bool GetMouseDown(int button);

		void motionCallback(int x, int y);
		void mouseCallback(int button, int state, int x, int y);
		void ExitCallback(void);

		void ToggleRenderMode();
		void ToggleHUDMode();
		void InitDebugHUD();

		extern PxReal deltaTime;
		extern HUDState hudState;
		extern RenderMode renderMode;

		extern HUD hud;
		extern Scene* scene;
		extern Camera* camera;
	}
}

