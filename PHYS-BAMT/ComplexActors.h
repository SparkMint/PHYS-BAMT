#pragma once

#include "PBPhysics.h"
#include "BasicActors.h"
#include <iostream>
#include <iomanip>



namespace PhysBamt
{
	namespace Physics
	{

		///An example class showing the use of springs (distance joints).
		class Trampoline
		{
			vector<DistanceJoint*> springs;
			Box* bottom, * top;

		public:
			Trampoline(const PxVec3& dimensions = PxVec3(1.f, 1.f, 1.f), PxReal stiffness = 1.f, PxReal damping = 1.f)
			{
				PxReal thickness = .1f;
				bottom = new Box(PxTransform(PxVec3(0.f, thickness, 0.f)), PxVec3(dimensions.x, thickness, dimensions.z));
				top = new Box(PxTransform(PxVec3(0.f, dimensions.y + thickness, 0.f)), PxVec3(dimensions.x, thickness, dimensions.z));
				springs.resize(4);
				springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
				springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
				springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
				springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));

				for (unsigned int i = 0; i < springs.size(); i++)
				{
					springs[i]->Stiffness(stiffness);
					springs[i]->Damping(damping);
				}
			}

			void AddToScene(Scene* scene)
			{
				scene->Add(bottom);
				scene->Add(top);
			}

			~Trampoline()
			{
				for (unsigned int i = 0; i < springs.size(); i++)
					delete springs[i];
			}
		};


		class PoolTable
		{
			Box* tableTop;

			Box* topCushion;
			Box* bottomCushion;
			Box* topLeftCushion;
			Box* bottomLeftCushion;
			Box* topRightCushion;
			Box* bottomRightCushion;


		public:
			PoolTable(const PxVec3& position, const PxQuat& rotation = PxIdentity)
			{
				auto tableTopSize = PxVec3(2.54f, .1f, 1.27f);
				PxReal cushionThickness = .1f;
				PxReal pocketGapSize = .2f;
				tableTop = new Box(position + PxVec3(0, 1.5f, 0), rotation, tableTopSize);
				tableTop->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				tableTop->SetKinematic(true);

				topCushion = new Box(position + PxVec3(tableTopSize.x + cushionThickness, 1.5f, 0.f), PxIdentity, PxVec3(cushionThickness, .25f, tableTopSize.z - pocketGapSize));
				//topCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				topCushion->SetKinematic(true);

				bottomCushion = new Box(position + PxVec3(-tableTopSize.x - cushionThickness, 1.5f, 0.f), PxIdentity, PxVec3(cushionThickness, .25f, tableTopSize.z - pocketGapSize));
				//bottomCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				bottomCushion->SetKinematic(true);

				topLeftCushion = new Box(position + PxVec3(tableTopSize.x / 2, 1.5f, tableTopSize.z + cushionThickness), PxIdentity, PxVec3(tableTopSize.x / 2 - pocketGapSize, .25f, cushionThickness));
				//topLeftCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				topLeftCushion->SetKinematic(true);

				topRightCushion = new Box(position + PxVec3(-tableTopSize.x / 2, 1.5f, tableTopSize.z + cushionThickness), PxIdentity, PxVec3(tableTopSize.x / 2 - pocketGapSize, .25f, cushionThickness));
				//topRightCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				topRightCushion->SetKinematic(true);

				bottomLeftCushion = new Box(position + PxVec3(tableTopSize.x / 2, 1.5f, -tableTopSize.z - cushionThickness), PxIdentity, PxVec3(tableTopSize.x / 2 - pocketGapSize, .25f, cushionThickness));
				//bottomLeftCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				bottomLeftCushion->SetKinematic(true);

				bottomRightCushion = new Box(position + PxVec3(-tableTopSize.x / 2, 1.5f, -tableTopSize.z - cushionThickness), PxIdentity, PxVec3(tableTopSize.x / 2 - pocketGapSize, .25f, cushionThickness));
				//bottomRightCushion->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				bottomRightCushion->SetKinematic(true);
			}

			void AddToScene(Scene* scene)
			{
				scene->Add(tableTop);
				scene->Add(topCushion);
				scene->Add(bottomCushion);
				scene->Add(topLeftCushion);
				scene->Add(topRightCushion);
				scene->Add(bottomLeftCushion);
				scene->Add(bottomRightCushion);
			}
		};
	}
}
