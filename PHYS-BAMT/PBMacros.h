#pragma once
#include <PxPhysicsAPI.h>

namespace PhysBamt
{
    using namespace physx;

    #define Deg2Rad(deg) ((deg) * PxPi / 180.0f)
    #define Rad2Deg(rad) ((rad) * 180.0f / PxPi)

    #define PxVec3Zero PxVec3(0.f, 0.f, 0.f) 
    #define PxVec3Up PxVec3(0.f, 1.f, 0.f)
    #define PxVec3Right PxVec3(1.f, 0.f, 0.f)
    #define PxVec3Forward PxVec3(0.f, 0.f, 1.f)

    #define ColourRed PxVec3(1.f, 0.f, 0.f)
    #define ColourGreen PxVec3(0.f, 1.f, 0.f)
    #define ColourBlue PxVec3(0.f, 0.f, 1.f)
}
