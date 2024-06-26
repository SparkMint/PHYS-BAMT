#pragma once

#include "PBPhysics.h"
#include <iostream>
#include <iomanip>

#include "Exception.h"
#include "Extras/UserData.h"

namespace PhysBamt
{
	namespace Physics
	{
		struct PoolFilterGroup
		{
			enum Filters
			{
				CUEACTIVE   = (1 << 0),
				CUEINACTIVE = (1 << 1),
				CUEBALL     = (1 << 2),
				BALLS       = (1 << 3),
				CUERESET    = (1 << 4)
			};
		};

		
		///Plane class
		class Plane : public StaticActor
		{
		public:
			//A plane with default paramters: XZ plane centred at (0,0,0)
			Plane(PxVec3 normal = PxVec3(0.f, 1.f, 0.f), PxReal distance = 0.f)
				: StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance)))
			{
				CreateShape(PxPlaneGeometry());
			}
		};

		///Sphere class
		class Sphere : public DynamicActor
		{
		public:
			//a sphere with default parameters:
			// - pose in 0,0,0
			// - dimensions: 1m
			// - denisty: 1kg/m^3
			Sphere(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 1.f, PxReal density = 1.f)
				: DynamicActor(pose)
			{
				CreateShape(PxSphereGeometry(radius), density);
			}

			Sphere(const PxVec3& position, const PxQuat& rotation = PxIdentity, PxReal radius = 1.f, PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				CreateShape(PxSphereGeometry(radius), density);
			}
		};

		///Box class
		class Box : public DynamicActor
		{
		public:
			//a Box with default parameters:
			// - pose in 0,0,0
			// - dimensions: 1m x 1m x 1m
			// - denisty: 1kg/m^3
			Box(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f)
				: DynamicActor(pose)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}

			Box(const PxVec3& position, const PxQuat& rotation, PxVec3 dimensions = PxVec3(.5f, .5f, .5f), PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				CreateShape(PxBoxGeometry(dimensions), density);
			}
		};

		class Capsule : public DynamicActor
		{
		public:
			Capsule(const PxTransform& pose = PxTransform(PxIdentity), PxVec2 dimensions = PxVec2(1.f, 1.f), PxReal density = 1.f)
				: DynamicActor(pose)
			{
				CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
			}

			Capsule(const PxVec3& position, const PxQuat& rotation, PxVec2 dimensions = PxVec2(1.f, 1.f), PxReal density = 1.f) : DynamicActor(position, rotation)
			{
				CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
			}

	
		};

		///The ConvexMesh class
		class ConvexMesh : public DynamicActor
		{
		public:
			//constructor
			ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.f)
				: DynamicActor(pose)
			{
				PxConvexMeshDesc mesh_desc;
				mesh_desc.points.count = (PxU32)verts.size();
				mesh_desc.points.stride = sizeof(PxVec3);
				mesh_desc.points.data = &verts.front();
				mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
				mesh_desc.vertexLimit = 256;

				CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
			}

			//mesh cooking (preparation)
			static PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc)
			{
				PxDefaultMemoryOutputStream stream;

				if (!GetCooking()->cookConvexMesh(mesh_desc, stream))
					throw new Exception("ConvexMesh::CookMesh, cooking failed.");

				PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

				return GetPhysics()->createConvexMesh(input);
			}
		};

		///The TriangleMesh class
		class TriangleMesh : public StaticActor
		{
		public:
			//constructor
			TriangleMesh(const std::vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose = PxTransform(PxIdentity))
				: StaticActor(pose)
			{
				PxTriangleMeshDesc mesh_desc;
				mesh_desc.points.count = (PxU32)verts.size();
				mesh_desc.points.stride = sizeof(PxVec3);
				mesh_desc.points.data = &verts.front();
				mesh_desc.triangles.count = (PxU32)trigs.size() / 3;
				mesh_desc.triangles.stride = 3 * sizeof(PxU32);
				mesh_desc.triangles.data = &trigs.front();

				CreateShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
			}

			//mesh cooking (preparation)
			PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc)
			{
				PxDefaultMemoryOutputStream stream;

				if (!GetCooking()->cookTriangleMesh(mesh_desc, stream))
					throw new Exception("TriangleMesh::CookMesh, cooking failed.");

				PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

				return GetPhysics()->createTriangleMesh(input);
			}
		};

			class Cloth : public Actor
	{
		PxClothMeshDesc mesh_desc;

	public:
		//constructor
		Cloth(PxTransform pose=PxTransform(PxIdentity), const PxVec2& size=PxVec2(1.f,1.f), PxU32 width=1, PxU32 height=1, bool fix_top = true)
		{
			//prepare vertices
			PxReal w_step = size.x/width;
			PxReal h_step = size.y/height;

			PxClothParticle* vertices = new PxClothParticle[(width+1)*(height+1)*4];
			PxU32* quads = new PxU32[width*height*4];

			for (PxU32 j = 0; j < (height+1); j++)
			{
				for (PxU32 i = 0; i < (width+1); i++)
				{
					PxU32 offset = i + j*(width+1);
					vertices[offset].pos = PxVec3(w_step*i,0.f,h_step*j);
					if (fix_top && (j == 0)) //fix the top row of vertices
						vertices[offset].invWeight = 0.f;
					else
						vertices[offset].invWeight = 1.f;
				}

				for (PxU32 j = 0; j < height; j++)
				{
					for (PxU32 i = 0; i < width; i++)
					{
						PxU32 offset = (i + j*width)*4;
						quads[offset + 0] = (i+0) + (j+0)*(width+1);
						quads[offset + 1] = (i+1) + (j+0)*(width+1);
						quads[offset + 2] = (i+1) + (j+1)*(width+1);
						quads[offset + 3] = (i+0) + (j+1)*(width+1);
					}
				}
			}

			//init cloth mesh description
			mesh_desc.points.data = vertices;
			mesh_desc.points.count = (width+1)*(height+1);
			mesh_desc.points.stride = sizeof(PxClothParticle);

			mesh_desc.invMasses.data = &vertices->invWeight;
			mesh_desc.invMasses.count = (width+1)*(height+1);
			mesh_desc.invMasses.stride = sizeof(PxClothParticle);

			mesh_desc.quads.data = quads;
			mesh_desc.quads.count = width*height;
			mesh_desc.quads.stride = sizeof(PxU32) * 4;

			//create cloth fabric (cooking)
			PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics(), mesh_desc, PxVec3(0, -1, 0));

			//create cloth
			actor = (PxActor*)GetPhysics()->createCloth(pose, *fabric, vertices, PxClothFlags());
			//collisions with the scene objects
			((PxCloth*)actor)->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);

			colors.push_back(default_color);
			actor->userData = new UserData(&colors.back(), &mesh_desc);
		}

		void SetSolverFrequency(PxReal value)
		{
			((PxCloth*)actor)->setSolverFrequency(value);
		}		

		~Cloth()
		{
			delete (UserData*)actor->userData;		
		}
	};

		//pyramid vertices
		static PxVec3 pyramid_verts[] = { PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1) };
		//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
		//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
		static PxU32 pyramid_trigs[] = { 1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1 };

		class Pyramid : public ConvexMesh
		{
		public:
			Pyramid(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.f) :
				ConvexMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), pose, density)
			{
			}
		};
		
		class PyramidStatic : public TriangleMesh
		{
		public:
			PyramidStatic(PxTransform pose = PxTransform(PxIdentity)) :
				TriangleMesh(vector<PxVec3>(begin(pyramid_verts), end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs), end(pyramid_trigs)), pose)
			{
			}
		};

		// Distance joint
		class DistanceJoint : public Joint
		{
		public:
			DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
			{
				PxRigidActor* px_actor0 = nullptr;
				if (actor0)
					px_actor0 = (PxRigidActor*)actor0->Get();

				joint = (PxJoint*)PxDistanceJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
				joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
				((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
				Damping(1.f);
				Stiffness(1.f);
			}

			void Stiffness(PxReal value) const
			{
				((PxDistanceJoint*)joint)->setStiffness(value);
			}

			PxReal Stiffness() const
			{
				return ((PxDistanceJoint*)joint)->getStiffness();
			}

			void Damping(PxReal value) const
			{
				((PxDistanceJoint*)joint)->setDamping(value);
			}

			PxReal Damping() const
			{
				return ((PxDistanceJoint*)joint)->getDamping();
			}
		};

		// AKA Revolute Joint
		class HingeJoint : public Joint
		{
		public:
			HingeJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
			{
				PxRigidActor* px_actor0 = 0;
				if (actor0)
					px_actor0 = (PxRigidActor*)actor0->Get();

				joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
				joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			}

			void DriveVelocity(PxReal value)
			{
				//wake up the attached actors
				PxRigidDynamic* actor_0, * actor_1;
				((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
				((PxRevoluteJoint*)joint)->setDriveVelocity(value);
				((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
			}

			PxReal DriveVelocity()
			{
				return ((PxRevoluteJoint*)joint)->getDriveVelocity();
			}

			void SetLimits(PxReal lower, PxReal upper)
			{
				((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
				((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
			}
		};

		// Spherical Joint
		class SphericalJoint : public Joint
		{
		public:
			SphericalJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
			{
				PxRigidActor* px_actor0 = 0;
				if (actor0)
					px_actor0 = (PxRigidActor*)actor0->Get();

				joint = PxSphericalJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
				joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			}

			void SetLimits(PxReal yLimit, PxReal zLimit)
			{
				((PxSphericalJoint*)joint)->setLimitCone(PxJointLimitCone(yLimit, zLimit, 0.01f));
				((PxSphericalJoint*)joint)->setSphericalJointFlag(PxSphericalJointFlag::eLIMIT_ENABLED, true);
			}
		};

		// AKA D6 Joint. By default this will be completely locked. So it can also be considered a fixed joint.
		class ConfigurableJoint : public Joint
		{
		public:
			ConfigurableJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
			{
				PxRigidActor* px_actor0 = 0;
				if (actor0)
					px_actor0 = (PxRigidActor*)actor0->Get();

				joint = PxD6JointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
				joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			}

			void SetLinearMotions(PxD6Motion::Enum xMotion, PxD6Motion::Enum yMotion, PxD6Motion::Enum zMotion) const
			{
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eX, xMotion);
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eY, yMotion);
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eZ, zMotion);
			}

			void SetRotationalMotions(PxD6Motion::Enum xMotion, PxD6Motion::Enum yMotion, PxD6Motion::Enum zMotion) const
			{
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eTWIST, xMotion);
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eSWING1, yMotion);
				((PxD6Joint*)joint)->setMotion(PxD6Axis::eSWING2, zMotion);
			}

			void SetLinearLimits(PxReal limit, PxSpring limitSpring) const
			{
				((PxD6Joint*)joint)->setLinearLimit(PxJointLinearLimit(limit, limitSpring));
			}

			void SetAngularXLimit(const PxReal& lowerLimit, const PxReal& upperLimit, PxSpring limitSpring) const
			{
				((PxD6Joint*)joint)->setTwistLimit(PxJointAngularLimitPair(lowerLimit, upperLimit, limitSpring));
			}

			void SetAngularYZLimit(const PxReal& yLimit, const PxReal& zLimit, PxSpring limitSpring) const
			{
				((PxD6Joint*)joint)->setSwingLimit(PxJointLimitCone(yLimit, zLimit, limitSpring));
			}

			void SetSlerpDrive(PxReal spring, PxReal damping, PxReal maxStrength = PX_MAX_F32, bool isAccelerationDrive = false) const
			{
				((PxD6Joint*)joint)->setDrive(PxD6Drive::Enum::eSLERP, PxD6JointDrive(spring, damping, maxStrength, isAccelerationDrive));
			}

			void SetLinearDrives(PxReal spring, PxReal damping, PxReal maxStrength = PX_MAX_F32, bool isAccelerationDrive = false) const
			{
				SetXDrive(spring, damping, maxStrength, isAccelerationDrive);
				SetYDrive(spring, damping, maxStrength, isAccelerationDrive);
				SetZDrive(spring, damping, maxStrength, isAccelerationDrive);
			}

			void SetXDrive(PxReal spring, PxReal damping, PxReal maxStrength = PX_MAX_F32, bool isAccelerationDrive = false) const
			{
				((PxD6Joint*)joint)->setDrive(PxD6Drive::Enum::eX, PxD6JointDrive(spring, damping, maxStrength, isAccelerationDrive));
			}

			void SetYDrive(PxReal spring, PxReal damping, PxReal maxStrength = PX_MAX_F32, bool isAccelerationDrive = false) const
			{
				((PxD6Joint*)joint)->setDrive(PxD6Drive::Enum::eY, PxD6JointDrive(spring, damping, maxStrength, isAccelerationDrive));
			}
			
			void SetZDrive(PxReal spring, PxReal damping, PxReal maxStrength = PX_MAX_F32, bool isAccelerationDrive = false) const
			{
				((PxD6Joint*)joint)->setDrive(PxD6Drive::Enum::eZ, PxD6JointDrive(spring, damping, maxStrength, isAccelerationDrive));
			}

			void UpdateTargetPose(const PxTransform& pose) const
			{
				((PxD6Joint*)joint)->setDrivePosition(pose);
			}

			void UpdateTargetPose(const PxVec3& position, const PxQuat& rotation) const
			{
				((PxD6Joint*)joint)->setDrivePosition(PxTransform(position, rotation));
			}

			void UpdateJointAnchor(PxJointActorIndex::Enum actorIndex, PxVec3 position, PxQuat rotation) const
			{
				joint->setLocalPose(actorIndex, PxTransform(position, rotation));
			}

			void SetBreakForce(PxReal linear, PxReal angular) const
			{
				joint->setBreakForce(linear, angular);
			}
		};

		/// AKA Prismatic Joint
		class LinearJoint : public Joint
		{
		public:
			LinearJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
			{
				PxRigidActor* px_actor0 = 0;
				if (actor0)
					px_actor0 = (PxRigidActor*)actor0->Get();

				joint = PxPrismaticJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
				joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
			}

			void SetLimits(PxReal lower, PxReal upper, PxSpring spring) const
			{
				((PxPrismaticJoint*)joint)->setLimit(PxJointLinearLimitPair(lower, upper, spring));
				((PxPrismaticJoint*)joint)->setPrismaticJointFlag(PxPrismaticJointFlag::eLIMIT_ENABLED, true);
			}
		};
	}
}
