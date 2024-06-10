#ifndef _ModelTools
#define _ModelTools

#include "../Math/Vect.h"

struct StandardVertex;
struct TriangleByIndex;

class ModelTools
{

public:
	static void CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	//static void CreateSkyBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	//static void CreateTerrain(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
	static void CreateUnitPlaneXY(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri);
	static void CreateUnitPlaneXZ(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTriList, int& ntri);
	static void CreateUnitBoxRepTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);

	static void CreateUnitPyramid(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);

	static void CreateUnitPyramidRepTexture(StandardVertex *& pVerts, int & nverts, TriangleByIndex *& pTriList, int & ntri);

	static void CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri, float scale);

	//static void CreateUnitBoxSixFacesTexture(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&pTriList, int& ntri);
};



#endif _ModelTools