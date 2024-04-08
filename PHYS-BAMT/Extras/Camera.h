#pragma once

#include "foundation/PxTransform.h"
#include "foundation/PxMat33.h"

namespace PhysBamt
{
	namespace Renderer
	{
		using namespace physx;

		///Camera class
		class Camera
		{
			PxVec3	eye, eye_init;
			PxVec3	dir, dir_init;

		public:
			/// Camera Constructor
			Camera(const PxVec3& _eye, const PxVec3& _dir);

			///reset view
			void Reset();

			///get camera position
			PxVec3 getEye() const;

			void UpdatePosition(PxVec3 newPos, PxVec3 newDir);

			///get camera direction
			PxVec3 getDir() const;

			///get camera transformation
			PxTransform	getTransform() const;
			
			///handle analog move
			void AnalogMove(float x, float y);

			///handle camera motion
			void Look(int dx, int dy, PxReal speed, PxReal delta_time);

			///move camera forward
			void MoveForward(PxReal speed, PxReal delta_time);

			///move camera backward
			void MoveBackward(PxReal speed, PxReal delta_time);

			///move camera left
			void MoveLeft(PxReal speed, PxReal delta_time);

			///move camera right
			void MoveRight(PxReal speed, PxReal delta_time);

			///move camera up
			void MoveUp(PxReal speed, PxReal delta_time);

			///move camera down
			void MoveDown(PxReal speed, PxReal delta_time);
		};
	}
}