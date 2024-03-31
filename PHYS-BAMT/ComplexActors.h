#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

#include "PBMacros.h"


namespace PhysBamt
{
	namespace Physics
	{
		// Lists a load of colours for the pool table
		const vector<PxVec3> poolTablePallete = { PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f), PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f), PxVec3(255.f / 255.f, 0.f / 255.f, 0.f / 255.f) };
		
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

		class PoolTableCushion : public DynamicActor
		{	
			PxMaterial* cushionMaterial = nullptr;
			
		public:
			PoolTableCushion(const PxVec3& position, const PxQuat& rotation, PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				constexpr PxReal cushionThickness = .075f;
				constexpr PxReal cushionToFrameHeight = .01f;

				cushionMaterial = CreateMaterial(.015f, .2f, .8f);
				
				// Frame
				DynamicActor::CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 2, dimensions.y, dimensions.z)), density);
				
				GetShape(0)->setLocalPose(PxTransform(PxVec3(0, -dimensions.y + cushionThickness * 2 + cushionToFrameHeight, 0), PxIdentity));
				
				// Cushion
				DynamicActor::CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 2, cushionThickness, dimensions.z)), density);

				// This lays the cushion just above the position provided. It also moves it forward a bit as to not overlap with the frame.
				GetShape(1)->setLocalPose(PxTransform(PxVec3(dimensions.x, cushionThickness, 0), PxIdentity));

				// Apply Cushion Material
				this->Material(cushionMaterial, 1);

				this->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f), 1);
			}
		};
		
		class PoolTable
		{
			Box* tableTop;

			PoolTableCushion* bottomCushion;
			PoolTableCushion* topCushion;
			PoolTableCushion* topLeftCushion;
			PoolTableCushion* bottomLeftCushion;
			PoolTableCushion* topRightCushion;
			PoolTableCushion* bottomRightCushion;

			PxMaterial* clothMaterial = nullptr;
			
		public:
			PoolTable(const PxVec3& position, const PxQuat& rotation = PxIdentity)
			{
				const PxVec3 tableTopSize = PxVec3(2.54f, .1f, 1.27f);
				constexpr PxReal frameAndCushionThickness = .1f;
				constexpr PxReal pocketGapSize = .2f;
				constexpr PxReal tableSurfaceHeight = 1.5f;

				clothMaterial = CreateMaterial(.015f, .2f, .5f);

				// Set our tabletop in such a way that the top of the table is at the origin.
				tableTop = new Box(position + PxVec3(0, tableSurfaceHeight - tableTopSize.y, 0), rotation, tableTopSize);
				tableTop->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				tableTop->Material(clothMaterial, 0);
				tableTop->SetKinematic(true);

				bottomCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x - frameAndCushionThickness / 2, 1.5f, 0.f), PxIdentity, PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.z - pocketGapSize));
				bottomCushion->SetKinematic(true);

				topCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x + frameAndCushionThickness / 2, 1.5f, 0.f), PxQuat(Deg2Rad(180.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.z - pocketGapSize));
				topCushion->SetKinematic(true);

				topLeftCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x / 2, 1.5f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.x / 2 - pocketGapSize));
				topLeftCushion->SetKinematic(true);

				bottomLeftCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x / 2, 1.5f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.x / 2 - pocketGapSize));
				bottomLeftCushion->SetKinematic(true);
				
				topRightCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x / 2, 1.5f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.x / 2 - pocketGapSize));
				topRightCushion->SetKinematic(true);
				
				bottomRightCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x / 2, 1.5f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f, tableTopSize.x / 2 - pocketGapSize));
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

		class PoolTableSurface : public DynamicActor
		{
			
		};

		class PoolBalls
		{
			vector<Sphere*> balls;
			Sphere* cueBall;

			PxReal ballDensity = .17f;
			PxReal ballDrag = 4.f;
			
			PxVec3 ballYellow = PxVec3(255.f / 255.f, 255.f / 255.f, 0.f / 255.f);
			PxVec3 ballRed = PxVec3(255.f / 255.f, 0.f / 255.f, 0.f / 255.f);
			PxVec3 ballBlack = PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f);

			// 1 is Yellow, 2 is Red, 3 is Black.
			vector<PxU8> ballColourIndices = { 1, 2, 1, 1, 3, 2, 2, 1, 2, 1, 1, 2, 2, 1, 2 };

			PxMaterial* ballMaterial = nullptr;

		public:
			PoolBalls(const PxVec3& position, PxReal ballRadius = 0.068f)
			{	
				PxU8 ballsToPlace = 1;
				PxU8 ballsPlaced = 0;

				ballMaterial = CreateMaterial(.1f, .2f, .92f);

				cueBall = new Sphere(position + PxVec3(-3.f, 0.f, 0.f), PxIdentity, ballRadius, ballDensity);
				cueBall->Material(ballMaterial, 0);
				cueBall->SetupFiltering(PoolFilterGroup::CUEBALL, PoolFilterGroup::CUEBALL);
				((PxRigidDynamic*)cueBall->Get())->setAngularDamping(ballDrag);
				((PxRigidDynamic*)cueBall->Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
				for (int i = 0; i < 5; ++i)
				{
					for (int j = 0; j < ballsToPlace; ++j)
					{
						PxVec3 ballPosition = position;
				
						// Move balls back per row.
						ballPosition.x += i * ballRadius * 1.75f;

						// Move the balls to the furthest right.
						ballPosition.z += i * ballRadius;

						// Move the balls back by their current place in the row.
						ballPosition.z -= j * ballRadius * 2.f;
				
						Sphere* ball = new Sphere(ballPosition, PxIdentity, ballRadius, ballDensity);
						ball->SetupFiltering(PoolFilterGroup::BALLS, PoolFilterGroup::BALLS);
						((PxRigidDynamic*)ball->Get())->setAngularDamping(ballDrag);
						((PxRigidDynamic*)ball->Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
						ball->Material(ballMaterial, 0);

						if(ballColourIndices[ballsPlaced] == 1)
						{
							ball->Color(ballYellow);
						}
						else if (ballColourIndices[ballsPlaced] == 2)
						{
							ball->Color(ballRed);
						}
						else if (ballColourIndices[ballsPlaced] == 3)
						{
							ball->Color(ballBlack);
						}
						
						balls.push_back(ball);
						ballsPlaced++;
					}
					ballsToPlace++;
				}
			}

			void AddToScene(Scene* scene)
			{
				for (auto ball : balls)
				{
					scene->Add(ball);
				}
				scene->Add(cueBall);
			}
			
		};
	}
}
