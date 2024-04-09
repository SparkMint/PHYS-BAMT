#pragma once

#include "BasicActors.h"
#include "PoolTableConvexMeshes.h"
#include <iostream>
#include <iomanip>

#include "PBMacros.h"



namespace PhysBamt
{
	namespace Physics
	{
		// Lists a load of colours for the pool table
		const vector<PxVec3> poolTablePallete = { PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f), PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f), PxVec3(255.f / 255.f, 0.f / 255.f, 0.f / 255.f) };

		class PoolTableTop : public DynamicActor
		{
		public:
			PoolTableTop(const PxVec3& position, const PxQuat& rotation, PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				PxConvexMeshDesc meshDesc;
				meshDesc.points.count = (PxU32)tableTopVertices.size();
				meshDesc.points.stride = sizeof(PxVec3);
				meshDesc.points.data = &tableTopVertices.front();
				meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
				meshDesc.vertexLimit = 256;
				DynamicActor::CreateShape(PxConvexMeshGeometry(ConvexMesh::CookMesh(meshDesc)), density);
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

				cushionMaterial = CreateMaterial(.015f, .3f, .8f);
				
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

		class PoolTableSidePocket : public DynamicActor
		{
		public:
			PoolTableSidePocket(const PxVec3& position, const PxQuat& rotation, PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				for (auto vertexList : sidePocketVertices)
				{
					PxConvexMeshDesc meshDesc;
					meshDesc.points.count = (PxU32)vertexList.size();
					meshDesc.points.stride = sizeof(PxVec3);
					meshDesc.points.data = &vertexList.front();
					meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
					meshDesc.vertexLimit = 256;
								
					DynamicActor::CreateShape(PxConvexMeshGeometry(ConvexMesh::CookMesh(meshDesc)), density);
				}
			}
		};

		class PoolTableCornerPocket : public DynamicActor
		{
		public:
			PoolTableCornerPocket(const PxVec3& position, const PxQuat& rotation, PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				for (auto vertexList : cornerPocketVertices)
				{
					PxConvexMeshDesc meshDesc;
					meshDesc.points.count = (PxU32)vertexList.size();
					meshDesc.points.stride = sizeof(PxVec3);
					meshDesc.points.data = &vertexList.front();
					meshDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
					meshDesc.vertexLimit = 256;
								
					DynamicActor::CreateShape(PxConvexMeshGeometry(ConvexMesh::CookMesh(meshDesc)), density);
				}
			}
		};
		
		class PoolTable
		{
			PxMaterial* clothMaterial = nullptr;
			PxMaterial* pocketMaterial = nullptr;
			
			PoolTableTop* tableTop;

			PoolTableCushion* bottomCushion;
			PoolTableCushion* topCushion;
			PoolTableCushion* topLeftCushion;
			PoolTableCushion* bottomLeftCushion;
			PoolTableCushion* topRightCushion;
			PoolTableCushion* bottomRightCushion;

			PoolTableCornerPocket* topLeftPocket;
			PoolTableCornerPocket* topRightPocket;
			PoolTableCornerPocket* bottomLeftPocket;
			PoolTableCornerPocket* bottomRightPocket;

			PoolTableSidePocket* leftPocket;
			PoolTableSidePocket* rightPocket;

			ConfigurableJoint* topCushionJoint;
			ConfigurableJoint* bottomCushionJoint;
			ConfigurableJoint* topLeftCushionJoint;
			ConfigurableJoint* bottomLeftCushionJoint;
			ConfigurableJoint* topRightCushionJoint;
			ConfigurableJoint* bottomRightCushionJoint;

			ConfigurableJoint* topLeftPocketJoint;
			ConfigurableJoint* topRightPocketJoint;
			ConfigurableJoint* bottomLeftPocketJoint;
			ConfigurableJoint* bottomRightPocketJoint;
			ConfigurableJoint* rightPocketJoint;
			ConfigurableJoint* leftPocketJoint;

			Box* ballPotTrigger;

			const PxReal breakForce = 50.f;
			const PxReal breakTorque = 100.f;
			const PxVec3 tableTopSize = PxVec3(2.54f, .1f, 1.27f);
			const PxReal frameAndCushionThickness = .1f;
			const PxReal pocketGapSize = .3f;
			const PxReal tableSurfaceHeight = 1.5f;
			
		public:
			PoolTable(const PxVec3& position, const PxQuat& rotation = PxIdentity)
			{
				clothMaterial = CreateMaterial(.015f, .2f, .5f);
				pocketMaterial = CreateMaterial(.015f, .2f, .0f);

				ballPotTrigger = new Box(position + PxVec3(0, 1.25f, 0), PxIdentity, tableTopSize);
				ballPotTrigger->SetKinematic(true);
				ballPotTrigger->SetTrigger(true);

				// Set our tabletop in such a way that the top of the table is at the origin.
				tableTop = new PoolTableTop(position + PxVec3(0, tableSurfaceHeight - tableTopSize.y, 0), rotation);
				tableTop->Color(PxVec3(0.f / 255.f, 200.f / 255.f, 0.f / 255.f));
				tableTop->Material(clothMaterial, -1);

				bottomCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x - frameAndCushionThickness / 2, 1.5f, 0.f), PxIdentity, PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.z - pocketGapSize));
				bottomCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(-tableTopSize.x - frameAndCushionThickness / 2, 0.1f, 0.f), PxIdentity), bottomCushion, PxTransform(PxVec3Zero, PxIdentity));
				bottomCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				topCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x + frameAndCushionThickness / 2, 1.5f, 0.f), PxQuat(Deg2Rad(180.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.z - pocketGapSize));
				topCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(tableTopSize.x + frameAndCushionThickness / 2, 0.1f, 0.f), PxQuat(Deg2Rad(180.f), PxVec3Up)), topCushion, PxTransform(PxVec3Zero, PxIdentity));
				topCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				topLeftCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x / 2 - frameAndCushionThickness / 2, 1.5f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.x / 2 - .25f));
				topLeftCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(tableTopSize.x / 2 - frameAndCushionThickness / 2, 0.1f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up)), topLeftCushion, PxTransform(PxVec3Zero, PxIdentity));
				topLeftCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				bottomLeftCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x / 2 + frameAndCushionThickness / 2, 1.5f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.x / 2 - .25f));
				bottomLeftCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(-tableTopSize.x / 2 + frameAndCushionThickness / 2, 0.1f, -tableTopSize.z - frameAndCushionThickness / 2), PxQuat(Deg2Rad(-90.f), PxVec3Up)), bottomLeftCushion, PxTransform(PxVec3Zero, PxIdentity));
				bottomLeftCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				topRightCushion = new PoolTableCushion(position + PxVec3(tableTopSize.x / 2 - frameAndCushionThickness / 2, 1.5f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.x / 2 - .25f));
				topRightCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(tableTopSize.x / 2 - frameAndCushionThickness / 2, 0.1f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up)), topRightCushion, PxTransform(PxVec3Zero, PxIdentity));
				topRightCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				bottomRightCushion = new PoolTableCushion(position + PxVec3(-tableTopSize.x / 2 + frameAndCushionThickness / 2, 1.5f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up), PxVec3(frameAndCushionThickness, 1.5f / 2, tableTopSize.x / 2 - .25f));
				bottomRightCushionJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(-tableTopSize.x / 2 + frameAndCushionThickness / 2, 0.1f, tableTopSize.z + frameAndCushionThickness / 2), PxQuat(Deg2Rad(90.f), PxVec3Up)), bottomRightCushion, PxTransform(PxVec3Zero, PxIdentity));
				bottomRightCushionJoint->SetBreakForce(breakForce, breakTorque);
				
				topLeftPocket = new PoolTableCornerPocket(position + PxVec3(tableTopSize.x - frameAndCushionThickness, 1.5f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(0.f), PxVec3Up));
				topLeftPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(tableTopSize.x - frameAndCushionThickness, 0.1f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(0.f), PxVec3Up)), topLeftPocket, PxTransform(PxVec3Zero, PxIdentity));
				topLeftPocketJoint->SetBreakForce(breakForce, breakTorque);
				
				topRightPocket = new PoolTableCornerPocket(position + PxVec3(tableTopSize.x - frameAndCushionThickness, 1.5f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(270.f), PxVec3Up));
				topRightPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(tableTopSize.x - frameAndCushionThickness, 0.1f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(270.f), PxVec3Up)), topRightPocket, PxTransform(PxVec3Zero, PxIdentity));
				topRightPocketJoint->SetBreakForce(breakForce, breakTorque);
				
				bottomLeftPocket = new PoolTableCornerPocket(position + PxVec3(-tableTopSize.x + frameAndCushionThickness, 1.5f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(90.f), PxVec3Up));
				bottomLeftPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(-tableTopSize.x + frameAndCushionThickness, 0.1f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(90.f), PxVec3Up)), bottomLeftPocket, PxTransform(PxVec3Zero, PxIdentity));
				bottomLeftPocketJoint->SetBreakForce(breakForce, breakTorque);
				
				bottomRightPocket = new PoolTableCornerPocket(position + PxVec3(-tableTopSize.x + frameAndCushionThickness, 1.5f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(180.f), PxVec3Up));
				bottomRightPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(-tableTopSize.x + frameAndCushionThickness, 0.1f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(180.f), PxVec3Up)), bottomRightPocket, PxTransform(PxVec3Zero, PxIdentity));
				bottomRightPocketJoint->SetBreakForce(breakForce, breakTorque);
				
				leftPocket = new PoolTableSidePocket(position + PxVec3(0.f, 1.5f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(0.f), PxVec3Up));
				leftPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(0.f, 0.1f, -tableTopSize.z + frameAndCushionThickness), PxQuat(Deg2Rad(0.f), PxVec3Up)), leftPocket, PxTransform(PxVec3Zero, PxIdentity));
				leftPocketJoint->SetBreakForce(breakForce, breakTorque);
				
				rightPocket = new PoolTableSidePocket(position + PxVec3(0.f, 1.5f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(180.f), PxVec3Up));
				rightPocketJoint = new ConfigurableJoint(tableTop, PxTransform(PxVec3(0.f, 0.1f, tableTopSize.z - frameAndCushionThickness), PxQuat(Deg2Rad(180.f), PxVec3Up)), rightPocket, PxTransform(PxVec3Zero, PxIdentity));
				rightPocketJoint->SetBreakForce(breakForce, breakTorque);
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

				scene->Add(topLeftPocket);
				scene->Add(topRightPocket);
				scene->Add(bottomLeftPocket);
				scene->Add(bottomRightPocket);

				scene->Add(leftPocket);
				scene->Add(rightPocket);
				scene->Add(ballPotTrigger);
			}
		};

		class PoolCue : public DynamicActor
		{
			PxMaterial* cueMaterial = nullptr;
			PxMaterial* cueTipMaterial = nullptr;

		public:
			PoolCue(const PxVec3& position, const PxQuat& rotation, PxVec2 dimensions = PxVec2(0.02f, 1.44f), PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				cueMaterial = CreateMaterial(.6f, .6f, .81f);
				cueTipMaterial = CreateMaterial(.6f, .6f, .81f);
				
				DynamicActor::CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
				GetShape(0)->setLocalPose(PxTransform(PxVec3(-dimensions.y / 2, 0, 0), PxIdentity));

				DynamicActor::CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.x, dimensions.x)), density);
				GetShape(1)->setLocalPose(PxTransform(PxVec3(-dimensions.y * 1.5, 0.f, 0.f)));
				Color(PxVec3(84.f / 255, 43.f / 255.f, 16.f / 255.f));

				((PxRigidDynamic*)Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);

				this->Material(cueMaterial, 0);
				this->Material(cueTipMaterial, 1);
			}
		};

		class PoolBalls
		{
			vector<Sphere*> balls;

			PxReal ballDensity = 1.69f;
			PxReal ballDrag = 3.f;
			
			PxVec3 ballYellow = PxVec3(255.f / 255.f, 255.f / 255.f, 0.f / 255.f);
			PxVec3 ballRed = PxVec3(255.f / 255.f, 0.f / 255.f, 0.f / 255.f);
			PxVec3 ballBlack = PxVec3(0.f / 255.f, 0.f / 255.f, 0.f / 255.f);
			PxVec3 ballWhite = PxVec3(255.f / 255.f, 255.f / 255.f, 255.f / 255.f);

			// 1 is Yellow, 2 is Red, 3 is Black.
			vector<PxU8> ballColourIndices = { 1, 2, 1, 1, 3, 2, 2, 1, 2, 1, 1, 2, 2, 1, 2 };

			PxMaterial* ballMaterial = nullptr;

		public:
			Sphere* cueBall;
			
			PoolBalls(const PxVec3& position, PxReal ballRadius = 0.068f)
			{	
				PxU8 ballsToPlace = 1;
				PxU8 ballsPlaced = 0;

				ballMaterial = CreateMaterial(.1f, .2f, .82f);

				cueBall = new Sphere(position + PxVec3(-ballRadius * 40, 0.f, 0.f), PxIdentity, ballRadius, ballDensity);
				cueBall->Material(ballMaterial, -1);
				cueBall->Color(ballWhite);
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
						ball->Material(ballMaterial, -1);

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

		class SpinningFlag
		{
			Capsule* flagBase;
			Box* spinnyBit;
			Cloth* flagCloth;
			Cloth* flagCloth2;
			HingeJoint* spinnyBitJoint;

			PxVec2 flagBaseDimensions = PxVec2(.25f, 4.f);
			PxVec3 spinnyBitDimensions = PxVec3(.2f, 3.f, .2f);

		public:
			SpinningFlag(PxVec3& position, PxQuat rotation)
			{
				flagBase = new Capsule(position + PxVec3(0.f, flagBaseDimensions.y, 0.f), rotation, flagBaseDimensions, 1.f);
				flagBase->SetKinematic(true);

				spinnyBit = new Box(position + PxVec3(0.f, flagBaseDimensions.y * 2, spinnyBitDimensions.z * 2), rotation, spinnyBitDimensions);
			
				spinnyBitJoint = new HingeJoint(flagBase, PxTransform(PxVec3(flagBaseDimensions.y, 0.f, 0.f), PxQuat(Deg2Rad(90.f), PxVec3(1.f,0.f,0.f))), spinnyBit, PxTransform(PxVec3(0.f, 0.f, 0.f), PxIdentity));
				spinnyBitJoint->DriveVelocity(1.f);

				flagCloth = new Cloth(PxTransform(position + PxVec3(-spinnyBitDimensions.y * .75f, flagBaseDimensions.y * 2.5, 2.5f), PxQuat(Deg2Rad(90.f), PxVec3(0.f,1.f,0.f))), PxVec2(5.f,5.f), 10, 10);
				flagCloth2 = new Cloth(PxTransform(position + PxVec3(spinnyBitDimensions.y * .75f, flagBaseDimensions.y * 2.5, 2.5f), PxQuat(Deg2Rad(90.f), PxVec3(0.f,1.f,0.f))), PxVec2(5.f,5.f), 10, 10);
				flagCloth->SetSolverFrequency(240.f);
				flagCloth2->SetSolverFrequency(240.f);
			}


			void AddToScene(Scene* scene)
			{
				scene->Add(flagBase);
				scene->Add(spinnyBit);
				scene->Add(flagCloth);
				scene->Add(flagCloth2);
			}
		};
	}
}
