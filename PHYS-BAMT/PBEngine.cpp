#include "PBEngine.h"

#include <chrono>
#include <GL/glut.h>

#include "MyPhysicsEngine.h"

namespace PhysBamt
{
	namespace Engine
	{
		PxReal deltaTime;
		RenderMode renderMode = NORMAL;
		HUDState hudState = HELP;
		HUD hud;
		Scene* scene;
		Camera* camera;
		
		// Time points which are used to calculate deltaTime
		chrono::steady_clock::time_point currentTimePoint;
		chrono::steady_clock::time_point lastTimePoint;

		// Key Handling
		bool keyStates[256];
		bool keyStatesPreviousTick[256];

		bool mouseStates[3];
		bool mouseStatesPreviousTick[3];

		/// Mouse Position
		int mMouseX = 0;
		int mMouseY = 0;
		
		PxReal gForceStrength = .1f;
		
		void InitializeEngine(const char* window_name, int width, int height)
		{
			// Initialize the physics engine
			InitPhysX();

			// Initialise our scene
			scene = new PoolScene();
			scene->Init();

			///Init renderer
			BackgroundColor(PxVec3(161.f / 255.f, 195.f / 255.f, 255.f / 255.f));
			SetRenderDetail(40);
			InitWindow(window_name, width, height);
			InitializeRenderer();
			camera = new Camera(PxVec3(-5.0f, 3.0f, 0.0f), PxVec3(1.f, -.5f, 0.f), 5.f);

			//initialise HUD
			InitDebugHUD();

			///Assign callbacks
			// Engine Loop
			glutDisplayFunc(EngineLoop);

			// Keyboard
			glutKeyboardFunc(OnKeyPressed);
			glutSpecialFunc(HandleDebugInputs);
			glutKeyboardUpFunc(OnKeyReleased);

			// Mouse
			glutMouseFunc(mouseCallback);
			glutMotionFunc(motionCallback);

			// Exit
			atexit(ExitCallback);

			//init motion callback
			motionCallback(0, 0);
		}

		void InitDebugHUD()
		{
			// EMPTY SCREEN
			hud.AddLine(EMPTY, "");

			// HELP SCREEN
			hud.AddLine(HELP, "\n"
					 "SIMULATION \n"
					 "F9 - Select next actor \n"
					 "F10 - Pause \n"
					 "F12 - Reset \n"
					 "\n"
					 "DISPLAY \n"
					 "F5 - Toggle Debug HUD \n"
					 "F6 - Render Shadows \n"
					 "F7 - Debug/Normal/Both Render Modes \n"
					 "\n"
					 "CAMERA \n"
					 "W,S,A,D,E,Q - Forward, Backward, Left, Right, Up, Down \n"
					 "RMB + Mouse - Look Around \n"
					 "F8 - Reset View \n");
			
			// PAUSE SCREEN
			hud.AddLine(PAUSE, "\n"
					  "SIMULATION PAUSED. Press F10 to continue.");

			// FONT SETTINGS
			hud.FontSize(0.018f);
			hud.Color(PxVec3(0.f, 0.f, 0.f));
		}
		
		void StartEngine()
		{
			glutMainLoop();
		}
		
		void EngineLoop()
		{
			HandleEngineInputs();

			// Calculate DeltaTime using chrono.
			currentTimePoint = std::chrono::steady_clock::now();
			deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTimePoint - lastTimePoint).count() / 1000000.0f;
			lastTimePoint = currentTimePoint;

			StartRenderer(camera->getEye(), camera->getDir());
			
			scene->SceneLoop(deltaTime);
			
			// Scene handles rendering

			// Store previous tick's key states
			for (int i = 0; i < 256; ++i)
			{
				keyStatesPreviousTick[i] = keyStates[i];	
			}
			for (int i = 0; i < 3; ++i)
			{
				mouseStatesPreviousTick[i] = mouseStates[i];
			}

			hud.ActiveScreen(hudState);
			
			hud.Render();
			FinishRendering();
		}
		
		//handle camera control keys
		void CameraInput(int key)
		{
			switch (toupper(key))
			{
			case 'W':
				camera->MoveForward(deltaTime);
				break;
			case 'S':
				camera->MoveBackward(deltaTime);
				break;
			case 'A':
				camera->MoveLeft(deltaTime);
				break;
			case 'D':
				camera->MoveRight(deltaTime);
				break;
			case 'E':
				camera->MoveUp(deltaTime);
				break;
			case 'Q':
				camera->MoveDown(deltaTime);
				break;
			default:
				break;
			}
		}

		//handle force control keys
		void ForceInput(int key)
		{
			if (!scene->GetSelectedActor())
				return;

			switch (toupper(key))
			{
				// Force controls on the selected actor
			case 'I': //forward
				scene->GetSelectedActor()->addForce(PxVec3(0, 0, -1) * gForceStrength);
				break;
			case 'K': //backward
				scene->GetSelectedActor()->addForce(PxVec3(0, 0, 1) * gForceStrength);
				break;
			case 'J': //left
				scene->GetSelectedActor()->addForce(PxVec3(-1, 0, 0) * gForceStrength);
				break;
			case 'L': //right
				scene->GetSelectedActor()->addForce(PxVec3(1, 0, 0) * gForceStrength);
				break;
			case 'U': //up
				scene->GetSelectedActor()->addForce(PxVec3(0, 1, 0) * gForceStrength);
				break;
			case 'M': //down
				scene->GetSelectedActor()->addForce(PxVec3(0, -1, 0) * gForceStrength);
				break;
			default:
				break;
			}
		}

		///handle special keys
		void HandleDebugInputs(int key, int x, int y)
		{
			//simulation control
			switch (key)
			{
				//display control
			case GLUT_KEY_F5:
				//hud on/off
				ToggleHUDMode();
				break;
			case GLUT_KEY_F6:
				//shadows on/off
				Renderer::ShowShadows(!Renderer::ShowShadows());
				break;
			case GLUT_KEY_F7:
				//toggle render mode
				ToggleRenderMode();
				break;
			case GLUT_KEY_F8:
				//reset camera view
				camera->Reset();
				break;

				//simulation control
			case GLUT_KEY_F9:
				//select next actor
				scene->SelectNextActor();
				break;
			case GLUT_KEY_F10:
				//toggle scene pause
				scene->Pause(!scene->Pause());
				ToggleHUDMode();
				break;
			case GLUT_KEY_F12:
				//resect scene
				scene->Reset();
				break;
			default:
				break;
			}
		}

		void OnKeyPressed(unsigned char key, int x, int y)
		{
			if (keyStates[key] == true)
				return;

			keyStates[key] = true;

			// I think this is meant to be the escape key?
			if (key == 27)
				exit(0);
			
		}

		void OnKeyReleased(unsigned char key, int x, int y)
		{
			keyStates[key] = false;
		}
		
		void HandleEngineInputs()
		{
			for (int i = 0; i < 256; i++)
			{
				// If the key is down.
				if (keyStates[i])
				{
					CameraInput(i);
					ForceInput(i);
				}
			}
		}

		bool GetKeyPressed(unsigned char key)
		{
			return !keyStatesPreviousTick[key] && keyStates[key];
		}

		bool GetKeyDown(unsigned char key)
		{
			return keyStates[key];
		}

		bool GetMousePressed(int button)
		{
			return !mouseStatesPreviousTick[button] && mouseStates[button];
		}

		bool GetMouseDown(int button)
		{
			return mouseStates[button];
		}

		void motionCallback(int x, int y)
		{
			int deltaX = mMouseX - x;
			int deltaY = mMouseY - y;

			camera->Motion(deltaX, deltaY, deltaTime);

			mMouseX = x;
			mMouseY = y;
		}

		void mouseCallback(int button, int state, int x, int y)
		{
			mouseStates[button] = state == GLUT_DOWN;
			
			mMouseX = x;
			mMouseY = y;
		}

		void ToggleHUDMode()
		{
			if(scene->Pause())
			{
				hudState = PAUSE;
			}
			else
			{
				hudState = hudState == HELP ? EMPTY : HELP;
			}
		}


		void ToggleRenderMode()
		{
			if (renderMode == NORMAL)
				renderMode = DEBUG;
			else if (renderMode == DEBUG)
				renderMode = BOTH;
			else if (renderMode == BOTH)
				renderMode = NORMAL;
		}
		
		void ExitCallback(void)
		{
			delete camera;
			delete scene;
			PxRelease();
		}
	}
}

