#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace PhysicsEngine
{
	using namespace std;

	///A custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Box* box;
		int steps;

	public:
		//Custom scene initialisation
		virtual void CustomInit() 
		{
			GetMaterial()->setDynamicFriction(.2f);

			plane = new Plane();
			Add(plane);

			box = new Box(PxTransform(PxVec3(0.f,10.f,0.f)));
			Add(box);
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
			//get position and velocity of the px_actor
			
			//note that the Get() function returns PxActor* only and therefore
			//to be able to access methods from the inherited classess (e.g. PxRigidDynamic)
			//we need to convert the pointer from PxActor* to PxRigidDynamic*
			
			PxRigidDynamic* px_actor = (PxRigidDynamic*)box->Get();
			PxVec3 position = px_actor->getGlobalPose().p;
			PxVec3 velocity = px_actor->getLinearVelocity();
			
			//'visualise' position and velocity of the box
			cout << setiosflags(ios::fixed) << setprecision(2) << "x=" << position.x << 
				", y=" << position.y << ", z=" << position.z << ",  ";
			cout << setiosflags(ios::fixed) << setprecision(2) << "vx=" << velocity.x << 
				", vy=" << velocity.y << ", vz=" << velocity.z << endl;

			steps++;
			if (steps == 10)
			{
				PxTransform trans = px_actor->getGlobalPose();
				trans.p += PxVec3(10.f, 0, 0);
				px_actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
				px_actor->setKinematicTarget(trans);
				//px_actor->setGlobalPose(trans);
			}
		}
	};
}
