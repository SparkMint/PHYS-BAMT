#pragma once
#include <vector>
#include <foundation/PxVec3.h>

static std::vector<physx::PxVec3> cornerPocketVerts1 = {
    physx::PxVec3(-0.200000, -1.500000, 0.000000),
    physx::PxVec3(-0.200000, 0.160000, 0.000000),
    physx::PxVec3(-0.200000, -1.500000, -0.200000),
    physx::PxVec3(-0.200000, 0.160000, -0.200000),
    physx::PxVec3(-0.100000, -1.500000, -0.200000),
    physx::PxVec3(-0.100000, 0.160000, -0.200000),
    physx::PxVec3(-0.100000, -1.500000, -0.100000),
    physx::PxVec3(-0.100000, 0.160000, -0.100000)
};

static std::vector<physx::PxVec3> cornerPocketVerts2 = {
    physx::PxVec3(-0.100000, -1.500000, -0.100000),
    physx::PxVec3(-0.100000, 0.160000, -0.100000),
    physx::PxVec3(-0.100000, -1.500000, -0.200000),
    physx::PxVec3(-0.100000, 0.160000, -0.200000),
    physx::PxVec3(0.000000, -1.500000, -0.200000),
    physx::PxVec3(0.000000, 0.160000, -0.200000),
    physx::PxVec3(0.000000, -1.500000, -0.100000),
    physx::PxVec3(0.000000, 0.160000, -0.100000)
};
        
static std::vector<physx::PxVec3> cornerPocketVerts3 = {
    physx::PxVec3(0.075000, -1.500000, -0.075000),
    physx::PxVec3(0.075000, 0.160000, -0.075000),
    physx::PxVec3(-0.000000, -1.500000, -0.200000),
    physx::PxVec3(-0.000000, 0.160000, -0.200000),
    physx::PxVec3(0.175000, -1.500000, -0.175000),
    physx::PxVec3(0.175000, 0.160000, -0.175000),
    physx::PxVec3(0.200000, -1.500000, 0.000000),
    physx::PxVec3(0.200000, 0.160000, 0.000000),
    physx::PxVec3(0.000000,-1.500000, -0.100000),
    physx::PxVec3(0.000000, 0.160000, -0.100000),
    physx::PxVec3(0.100000, 0.160000, 0.000000),
    physx::PxVec3(0.100000, -1.500000, 0.000000)
};

static std::vector<physx::PxVec3> cornerPocketVerts4 = {
    physx::PxVec3(0.100000, -1.500000, 0.100000),
    physx::PxVec3(0.100000, 0.160000, 0.100000),
    physx::PxVec3(0.100000, -1.500000, 0.000000),
    physx::PxVec3(0.100000, 0.160000, 0.000000),
    physx::PxVec3(0.200000, -1.500000, 0.000000),
    physx::PxVec3(0.200000, 0.160000, 0.000000),
    physx::PxVec3(0.200000, -1.500000, 0.100000),
    physx::PxVec3(0.200000, 0.160000, 0.100000)
};

static std::vector<physx::PxVec3> cornerPocketVerts5 = {
    physx::PxVec3(0.100000, -1.500000, 0.100000),
    physx::PxVec3(0.100000, 0.160000, 0.100000),
    physx::PxVec3(0.200000, -1.500000, 0.100000),
    physx::PxVec3(0.200000, 0.160000, 0.100000),
    physx::PxVec3(0.200000, -1.500000, 0.200000),
    physx::PxVec3(0.200000, 0.160000, 0.200000),
    physx::PxVec3(-0.000000, -1.500000, 0.200000),
    physx::PxVec3(-0.000000, 0.160000, 0.200000)
};

static std::vector<std::vector<physx::PxVec3>> cornerPocketVertices = {cornerPocketVerts1, cornerPocketVerts2, cornerPocketVerts3,cornerPocketVerts4, cornerPocketVerts5 };

static std::vector<physx::PxVec3> sidePocketVerts1 = {
    physx::PxVec3(-0.200000, -0.200000, 0.000000),
    physx::PxVec3(-0.200000, 0.160000, 0.000000),
    physx::PxVec3(-0.200000, -1.500000, -0.200000),
    physx::PxVec3(-0.200000, 0.160000, -0.200000),
    physx::PxVec3(-0.100000, -1.500000, -0.200000),
    physx::PxVec3(-0.100000, 0.160000, -0.200000),
    physx::PxVec3(-0.100000, -1.500000, -0.100000),
    physx::PxVec3(-0.100000, 0.160000, -0.100000),
    physx::PxVec3(-0.160000, 0.160000, 0.000000),
    physx::PxVec3(-0.150000, -1.500000, 0.000000)
};
static std::vector<physx::PxVec3> sidePocketVerts2 = {
    physx::PxVec3(-0.100000, -1.500000, -0.100000),
    physx::PxVec3(-0.100000, 0.160000, -0.100000),
    physx::PxVec3(-0.100000, -1.500000, -0.200000),
    physx::PxVec3(-0.100000, 0.160000, -0.200000),
    physx::PxVec3(0.000000, -1.500000, -0.200000),
    physx::PxVec3(0.000000, 0.160000, -0.200000),
    physx::PxVec3(0.000000, -1.500000, -0.150000),
    physx::PxVec3(0.000000, 0.160000, -0.150000)
};
static std::vector<physx::PxVec3> sidePocketVerts3 = {
    physx::PxVec3(0.100000, -1.500000, -0.100000),
    physx::PxVec3(0.100000, 0.160000, -0.100000),
    physx::PxVec3(0.100000, -1.500000, -0.200000),
    physx::PxVec3(0.100000, 0.160000, -0.200000),
    physx::PxVec3(0.200000, -1.500000, -0.200000),
    physx::PxVec3(0.200000, 0.160000, -0.200000),
    physx::PxVec3(0.200000, -1.500000, 0.000000),
    physx::PxVec3(0.200000, 0.160000, 0.000000),
    physx::PxVec3(0.150000, 0.160000, -0.000000),
    physx::PxVec3(0.150000, -1.500000, -0.000000)
}; 
static std::vector<physx::PxVec3> sidePocketVerts4 = {
    physx::PxVec3(0.000000, -1.500000, -0.150000),
    physx::PxVec3(0.000000, 0.160000, -0.150000),
    physx::PxVec3(0.000000, -1.500000, -0.200000),
    physx::PxVec3(0.000000, 0.160000, -0.200000),
    physx::PxVec3(0.100000, -1.500000, -0.200000),
    physx::PxVec3(0.100000, 0.160000, -0.200000),
    physx::PxVec3(0.100000, -1.500000, -0.100000),
    physx::PxVec3(0.100000, 0.160000, -0.100000)
}; 

static std::vector<std::vector<physx::PxVec3>> sidePocketVertices = {sidePocketVerts1,sidePocketVerts2,sidePocketVerts3,sidePocketVerts4};

static std::vector<physx::PxVec3> tableTopVertices = {
    physx::PxVec3(-2.240000, -0.100000, 1.170000),
      physx::PxVec3(-2.240000, 0.100000, 1.170000),
      physx::PxVec3(2.240000, 0.100000, 1.170000),
      physx::PxVec3(2.240000, -0.100000, 1.170000),
      physx::PxVec3(-2.240000, 0.100000, -1.170000),
      physx::PxVec3(-2.240000, -0.100000, -1.170000),
      physx::PxVec3(2.240000, -0.100000, -1.170000),
      physx::PxVec3(2.240000, 0.100000, -1.170000),
      physx::PxVec3(2.440000, 0.100000, -0.970000),
      physx::PxVec3(2.440000, -0.100000, -0.970000),
      physx::PxVec3(2.440000, -0.100000, 0.970000),
      physx::PxVec3(2.440000, 0.100000, 0.970000),
      physx::PxVec3(-2.440000, -0.100000, -0.970000),
      physx::PxVec3(-2.440000, -0.100000, 0.970000),
      physx::PxVec3(-2.440000, 0.100000, 0.970000),
      physx::PxVec3(-2.440000, 0.100000, -0.970000)
};

  