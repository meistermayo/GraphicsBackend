#ifndef MODEL_H
#define MODEL_H

#include "Math/Vect.h"
#include "Align16.h"
#include "Math/Matrix.h"
#include "FbxModelLoader.h"

class MeshSeparator;
struct VertexBufferObject;
struct IndexBufferObject;

struct StandardVertex : public Align16
{
	Vect Pos;
	float u;    
	float v;
	Vect normal;
	Vect Color;
	int meshNum;		// new property

	void set(float _x, float _y, float _z,
		float _u = 0, float _v = 0,
		float _nx = 0, float _ny = 0, float _nz = 0,
		float _r = 1, float _g = 1, float _b = 1, int num = 0 )
	{
		Pos = Vect(_x, _y, _z);
		u = _u;
		v = _v;
		normal = Vect(_nx, _ny, _nz, 0);
		Color = Vect(_r, _g, _b);
		meshNum = num;
	}

	void set(float _x, float _y, float _z, float _u, float _v, const Vect& c, int num = 0)
	{
		Pos = Vect(_x, _y, _z);
		u = _u;
		v = _v;
		Color = c;
		meshNum = num;
	}

	void set(float _x, float _y, float _z, float _u, float _v, const Vect& n, const Vect& c, int num = 0)
	{
		Pos = Vect(_x, _y, _z);
		u = _u;
		v = _v;
		normal = n;
		Color = c;
		meshNum = num;
	}

	void set(float _x, float _y, float _z, const Vect& c, int num = 0)
	{
		Pos = Vect(_x, _y, _z);
		u = 0;
		v = 0;
		Color = c;
		meshNum = num;
	}

	void set(float _x, float _y, float _z, const Vect& n, const Vect& c, int num = 0)
	{
		Pos = Vect(_x, _y, _z);
		u = 0;
		v = 0;
		normal = n;
		Color = c;
		meshNum = num;
	}

	void set(const Vect& p, const Vect& c)
	{
		Pos = p;
		Color = c;
	}
};

struct TriangleByIndex
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;

	void set(int _v0, int _v1, int _v2)
	{
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
	}
};

class Model : public Align16
{
protected:
	// Model information
	TriangleByIndex *pTriList;
	StandardVertex* pStdVerts;
	Vect* pVectList;
	int numVerts;
	int numTris;

	MeshSeparator* meshes;

	// GPU connections
	VertexBufferObject*		mpVertexBufferObject;
	IndexBufferObject*		mpIndexBufferObject;

	void privLoadDataToGPU();
	void privLoadDataFromFile(const char * const _modelName, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& ptlist, int& ntri, bool flipU, bool flipV, float scale);
	void privPopulateHelperData();

public:
	Model(const Model&) = delete;				// Copy constructor
	Model(Model&&) = delete;                    // Move constructor
	Model& operator=(const Model&) & = delete;  // Copy assignment operator
	Model& operator=(Model&&) & = delete;       // Move assignment operator

	enum PreMadeModels {
		UnitBox,
		UnitPlaneXY,
		UnitPlaneXZ,
		UnitBoxRepeatedTexture,
		UnitPyramid,
		UnitBoxSixFacesTexture,
		UnitSphere
	};

protected:
	Model() {}

public:
	Model(StandardVertex *pVerts, int nverts, TriangleByIndex *ptlist, int ntri);
	Model(PreMadeModels pm, float scale = 1);
	Model(const char * const _modelName, bool flipU = false, bool flipV = false, float scale = 1);
	Model(FbxMeshInfo& fbxMeshInfo);

	virtual ~Model();

	void BindVertexIndexBuffers();
	void Render();
	void RenderMesh(int meshnum);

	// Get access
	int GetMeshCount();
	bool ValidMeshNum(int i);

	Vect GetCenter() const { return Vect(1, 1, 1); }
	float GetRadius() const { return 10.0f; } // TODO

	Vect* GetVectList() { return pVectList; }
	int GetVectNum() { return numVerts; }
};



#endif