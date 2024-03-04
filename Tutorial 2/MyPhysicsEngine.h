#pragma once

#include "BasicActors.h"
#include <iostream>
#include <iomanip>

namespace Physics
{
	using namespace std;

	//a list of colours: Circus Palette
	static const PxVec3 color_palette[] = {PxVec3(46.f/255.f,9.f/255.f,39.f/255.f),PxVec3(217.f/255.f,0.f/255.f,0.f/255.f),
		PxVec3(255.f/255.f,45.f/255.f,0.f/255.f),PxVec3(255.f/255.f,140.f/255.f,54.f/255.f),PxVec3(4.f/255.f,117.f/255.f,111.f/255.f)};

	//pyramid vertices
	static PxVec3 pyramid_verts[] = {PxVec3(0,1,0), PxVec3(1,0,0), PxVec3(-1,0,0), PxVec3(0,0,1), PxVec3(0,0,-1)};
	//pyramid triangles: a list of three vertices for each triangle e.g. the first triangle consists of vertices 1, 4 and 0
	//vertices have to be specified in a counter-clockwise order to assure the correct shading in rendering
	static PxU32 pyramid_trigs[] = {1, 4, 0, 3, 1, 0, 2, 3, 0, 4, 2, 0, 3, 2, 1, 2, 4, 1};

	class Pyramid : public ConvexMesh
	{
	public:
		Pyramid(PxTransform pose=PxTransform(PxIdentity), PxReal density=1.f) :
			ConvexMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), pose, density)
		{
		}
	};

	class PyramidStatic : public TriangleMesh
	{
	public:
		PyramidStatic(PxTransform pose=PxTransform(PxIdentity)) :
			TriangleMesh(vector<PxVec3>(begin(pyramid_verts),end(pyramid_verts)), vector<PxU32>(begin(pyramid_trigs),end(pyramid_trigs)), pose)
		{
		}
	};

	///Custom scene class
	class MyScene : public Scene
	{
		Plane* plane;
		Sphere* box;

		PxVec3 brickSize = { 1.f, .5f, .5f };

		int wallSize = 10;
		float brickGap = .1f;

	public:
		///A custom scene class
		void SetVisualisation()
		{
			px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
			px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		}

		//Custom scene initialisation
		virtual void CustomInit() 
		{
			srand(time(NULL));
			SetVisualisation();			

			GetMaterial()->setDynamicFriction(.2f);

			plane = new Plane();
			plane->Color(PxVec3(210.f/255.f,210.f/255.f,210.f/255.f));
			Add(plane);

			box = new Sphere(PxTransform(PxVec3(0.f, .5f, 10.f)));
			Add(box);

			CreateWall(PxVec3(0.f, 0.5f, 0.f), brickSize, wallSize, wallSize);
			CreateWall(PxVec3(0.f, 0.5f, 5.f), brickSize, wallSize, wallSize);
			CreateWall(PxVec3(0.f, 0.5f, 5.f), brickSize, wallSize, wallSize);

		}

		void CreateWall(const PxVec3& origin, const PxVec3& brickSize, int width, int height)
		{
			PxMaterial* brickMat = GetPhysics()->createMaterial(0.f, 1.f, 0.f);

			for (size_t x = 0; x < width; x++)
			{
				for (size_t y = 0; y < height; y++)
				{
					//Create new brick at specified center.
					PxVec3 brickPosition = origin; 

					// Offset our brick by the current loop iterations.
					brickPosition.x += brickSize.x * x * 2;
					brickPosition.y += brickSize.y * y * 2;

					if ((y & 1) == 0)
					{
						// Offset the bricks by its size to prevent it from sinking into the floor.
						brickPosition.x += brickSize.x;
					}

					Box* box = new Box(PxTransform(brickPosition), brickSize);
					box->Material(brickMat);
					box->Color(PxVec3(rand() & 256, rand() & 256, rand() & 256));
					Add(box);
				}
			}
		}

		//Custom udpate function
		virtual void CustomUpdate() 
		{
		}
	};
}
