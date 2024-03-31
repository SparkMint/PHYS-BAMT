#pragma once

#include <vector>
#include "PxPhysicsAPI.h"
#include <string>

namespace PhysBamt
{
	namespace Physics
	{
		using namespace physx;
		using namespace std;

		///Initialise PhysX framework
		void InitPhysX();

		///Release PhysX resources
		void PxRelease();

		///Get the PxPhysics object
		PxPhysics* GetPhysics();

		///Get the cooking object
		PxCooking* GetCooking();

		///Get the specified material
		PxMaterial* GetMaterial(PxU32 index = 0);

		///Create a new material
		PxMaterial* CreateMaterial(PxReal sf = .0f, PxReal df = .0f, PxReal cr = .0f);

		static const PxVec3 default_color(.8f, .8f, .8f);

		///Abstract Actor class
		///Inherit from this class to create your own actors
		class Actor
		{
		protected:
			PxActor* actor;
			std::vector<PxVec3> colors;
			std::string name;

		public:
			Actor() : actor(nullptr){}
			~Actor();

			PxActor* Get();

			void Color(PxVec3 new_color, PxU32 shape_index = -1);

			const PxVec3* Color(PxU32 shape_indx = 0);

			void Actor::Name(const string& name);

			string Actor::Name();

			void Material(PxMaterial* new_material, PxU32 shape_index = -1);

			PxShape* GetShape(PxU32 index = 0);

			std::vector<PxShape*> Actor::GetShapes(PxU32 index = -1);

			virtual void CreateShape(const PxGeometry& geometry, PxReal density) {}

			void SetTrigger(bool value, PxU32 index = -1);

			void SetFilter(PxU32 filter);

			void SetupFiltering(PxU32 filterGroup, PxU32 filterMask, PxU32 shape_index = -1);
		};

		class DynamicActor : public Actor
		{
		public:
			DynamicActor(const PxTransform& pose);

			DynamicActor(const PxVec3& position, const PxQuat& rotation);

			~DynamicActor();

			void CreateShape(const PxGeometry& geometry, PxReal density);

			void SetKinematic(bool value, PxU32 index = -1);
		};

		class StaticActor : public Actor
		{
		public:
			StaticActor(const PxTransform& pose);

			~StaticActor();

			void CreateShape(const PxGeometry& geometry, PxReal density = 0.f);
		};

		///Generic scene class
		class Scene
		{
		protected:
			//a PhysX scene object
			PxScene* px_scene;
			//pause simulation
			bool pause;
			//selected dynamic actor on the scene
			PxRigidDynamic* selected_actor;
			//original and modified colour of the selected actor
			std::vector<PxVec3> sactor_color_orig;
			//custom filter shader
			PxSimulationFilterShader filter_shader;

			// Fixed Simulation Timestep.
			PxReal fixedDeltaTime = .02f;

			// Accumulator responsible for determining when to run simulation updates.
			PxReal accumulator = 0.0f;

			void HighlightOn(PxRigidDynamic* actor);

			void HighlightOff(PxRigidDynamic* actor);

		public:
			virtual ~Scene() = default;
			Scene(PxSimulationFilterShader custom_filter_shader = PxDefaultSimulationFilterShader) : filter_shader(custom_filter_shader) {}

			///Init the scene
			void Init();

			///User defined initialisation
			virtual void Start() {}

			///Perform a single simulation step
			void SceneLoop(PxReal dt);

			///User defined update step
			virtual void Update(PxReal dt) {}

			virtual void FixedUpdate(PxReal fdt) {}

			void SetFixedDeltaTime(PxReal newFixedDeltaTime);

			///Add actors
			void Add(Actor* actor);

			///Get the PxScene object
			PxScene* Get();

			///Reset the scene
			void Reset();

			///Set pause
			void Pause(bool value);

			///Get pause
			bool Pause();

			///Get the selected dynamic actor on the scene
			PxRigidDynamic* GetSelectedActor();

			///Switch to the next dynamic actor
			void SelectNextActor();

			///a list with all actors
			std::vector<PxActor*> GetAllActors();
		};

		///Generic Joint class
		class Joint
		{
		protected:
			PxJoint* joint;

		public:
			Joint() : joint(0) {}

			PxJoint* Get() const { return joint; }
		};
	}
}
