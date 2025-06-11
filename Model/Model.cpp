#include <assert.h>
#include <iostream>
#include <fstream>
#include "Model.h"

#include "../d3dUtil.h"
#include "../GraphicsBackend.h"
#include "ModelTools.h"
#include "AzulFileHdr.h"
#include "MeshSeparator.h"

Model::Model(StandardVertex *inVerts, int inVertCount, TriangleByIndex *inTriList, int inTriCount)
{
	assert(inVertCount > 0);
	assert(inTriCount > 0);

	// Copy Data
	numVerts = inVertCount;
	pStdVerts = new StandardVertex[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		pStdVerts[i] = inVerts[i];
	}
	numTris = inTriCount;
	pTriList = new TriangleByIndex[numTris];
	for (int i = 0; i < numTris; i++)
	{
		pTriList[i] = inTriList[i];
	}

	meshes = new MeshSeparator(pStdVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(const char * const inModelName, bool inFlipU, bool inFlipV, float inScale)
{
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	privLoadDataFromFile(inModelName, pStdVerts, numVerts, pTriList, numTris, inFlipU, inFlipV, inScale );
	meshes = new MeshSeparator(pStdVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(FbxMeshInfo& fbxMeshInfo)
{
	numVerts = fbxMeshInfo.nVerts;
	numTris = fbxMeshInfo.nTris;

	pStdVerts = fbxMeshInfo.pVertices;
	pTriList = fbxMeshInfo.pTris;
	
	meshes = new MeshSeparator(pStdVerts, pTriList, numTris);
	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::Model(Model::PreMadeModels inPm, float inScale)
{
	pStdVerts = nullptr;
	pTriList = nullptr;
	numVerts = 0;
	numTris = 0;

	switch (inPm)
	{
	case UnitBox:
		ModelTools::CreateUnitBox(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitBoxRepeatedTexture:
		ModelTools::CreateUnitBoxRepTexture(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitPyramid:
		ModelTools::CreateUnitPyramid(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitSphere:
		ModelTools::CreateUnitSphere(12, 12, pStdVerts, numVerts, pTriList, numTris, inScale);
		break;
	case UnitPlaneXY:
		ModelTools::CreateUnitPlaneXY(pStdVerts, numVerts, pTriList, numTris);
		break;
	case UnitPlaneXZ:
		ModelTools::CreateUnitPlaneXZ(pStdVerts, numVerts, pTriList, numTris);
		break;
	default:
		assert(false && "Invalid option");
	}

	meshes = new MeshSeparator(pStdVerts, pTriList, numTris);

	privPopulateHelperData();
	privLoadDataToGPU();
}

Model::~Model()
{
	delete[] pTriList;
	delete[] pStdVerts;
	delete[] pVectList;

	delete mpVertexBufferObject;
	delete mpIndexBufferObject;
}

void Model::privLoadDataFromFile(const char* const inModelName, StandardVertex*& outVerts, int& outVertCount, TriangleByIndex*& outTriList, int& outTriCount, bool inFlipU, bool inFlipV, float inScale)
{
	std::ifstream file(inModelName, std::ios::binary);
	assert(file);

	// Get the file format for the data
	AzulFileHdr  azulFileHdr;

	struct VertexStride_VUN
	{
		float x;    // Vert - V
		float y;
		float z;

		float u;    // UV tex coor - U
		float v;

		float txt;	// texture trickery 

		float nx;   // Norm verts - N
		float ny;
		float nz;

		float r;
		float g;
		float b;

		void set(float _x, float _y, float _z,
			float _u, float _v,
			float _nx, float _ny, float _nz,
			float _txt = 0,
			float _r = 1, float _g = 1, float _b = 1)
		{
			x = _x;
			y = _y;
			z = _z;
			u = _u;
			v = _v;
			nx = _nx;
			ny = _ny;
			nz = _nz;

			txt = _txt;
			r = _r;
			g = _g;
			b = _b;
		}

	};

	// ******************


	// Read the header
	assert(file.read((char*)&azulFileHdr, sizeof(AzulFileHdr)));

	// create the vertex buffer
	outVertCount = azulFileHdr.numVerts;
	VertexStride_VUN* pTmpVerts = new VertexStride_VUN[outVertCount];

	// load the verts
	// seek to the location
	assert(file.seekg(azulFileHdr.vertBufferOffset));

	// read it
	assert(file.read((char*)pTmpVerts, outVertCount * sizeof(VertexStride_VUN)));

	// create the triLists buffer
	outTriCount = azulFileHdr.numTriangles;
	pTriList = new TriangleByIndex[outTriCount];

	// load the triList
	// seek to the location
	assert(file.seekg(azulFileHdr.triangleListBufferOffset));

	// read it
	assert(file.read((char*)outTriList, outTriCount * sizeof(TriangleByIndex)));

	// close
	file.close();
	assert(!file.is_open());

	// **** Overly complicated detour Part 2: now we copy the vertex data into our StandardVertex
	outVerts = new StandardVertex[outVertCount];
	StandardVertex* v;
	VertexStride_VUN* tmp;
	for (int i = 0; i < outVertCount; i++)
	{
		tmp = &(pTmpVerts[i]);
		v = &(outVerts[i]);
		v->set(tmp->x, tmp->y, tmp->z, tmp->u, tmp->v, tmp->nx, tmp->ny, tmp->nz, tmp->r, tmp->g, tmp->b, (int) tmp->txt);
		if (inFlipU)
			v->u = -v->u;
		if (inFlipV)
			v->v = -v->v;
		v->Pos = v->Pos * inScale;
	}

	delete[] pTmpVerts;
}

void Model::privPopulateHelperData()
{
	pVectList = new Vect[numVerts];
	for (int i = 0; i < numVerts; i++)
	{
		pVectList[i] = pStdVerts[i].Pos;
	}
}

void  Model::privLoadDataToGPU()
{
	mpVertexBufferObject = new VertexBufferObject();
	mpVertexBufferObject->num = numVerts;
	mpVertexBufferObject->pVerts = pStdVerts;
	mpIndexBufferObject = new IndexBufferObject();
	mpIndexBufferObject->num = numTris;
	mpIndexBufferObject->pTris = pTriList;

	mpVertexBufferObject->LoadToGPU();
	mpIndexBufferObject->LoadToGPU();
}

bool Model::ValidMeshNum(int inMeshNum)
{
	return (inMeshNum >= 0) && (inMeshNum < GetMeshCount());
}

int Model::GetMeshCount()
{
	return meshes->GetMeshCount();
}

void  Model::BindVertexIndexBuffers()
{
	mpVertexBufferObject->Bind();
	mpIndexBufferObject->Bind();
	GraphicsBackend::SetPrimitiveTopologyAsTriList();
}

void Model::Render()
{
	BindVertexIndexBuffers();
	GraphicsBackend::DrawIndexed(numTris * 3, 0, 0);
}

void Model::RenderMesh(int inMeshNum)
{
	assert(ValidMeshNum(inMeshNum));

	int tricount, trioffset;
	meshes->GetMeshTriCountAndOffset(inMeshNum, tricount, trioffset);

	GraphicsBackend::DrawIndexed(tricount * 3, trioffset * 3, 0);
}





